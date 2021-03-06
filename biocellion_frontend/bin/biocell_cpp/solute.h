#ifndef _SOLUTE_H_
#define _SOLUTE_H_
// forward declarations
class Solute;
class BulkSolute;

#include "biomodel.h"

class Solute : public ParamHolder {
public:
  Solute();
  Solute(const std::string& name, const S32& solute_idx, const S32& domain_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);

  const BioModel* getModel() const;
  const std::string& getName() const;
  S32 getSoluteIdx() const;
  S32 getDomainIdx() const;
  S32 getSolverIdx() const;
  S32 getAMRLevels() const;
  S32 getInterfaceAMRLevel() const;
  S32 getNumTimeSteps() const;

  const Vector< S32 >& getReactions() const;
  Vector< S32 >& getReactions();
  const Vector< S32 >& getMoleculeReactions() const;
  Vector< S32 >& getMoleculeReactions();

  void setModel(const BioModel*& biomodel );
  void setName(const std::string& name);
  void setSoluteIdx(const S32& idx);
  void setDomainIdx(const S32& idx);
  void setSolverIdx(const S32& idx);
  void setAMRLevels( const S32& value );
  void setInterfaceAMRLevel( const S32& value );
  void setNumTimeSteps( const S32& value );

  // general grid support
  idx_t getSubgridDimension( ) const;
  void calcSubgridDimension( );
  void getSubgridOffset( const VReal& vOffset, VIdx& subgridVOffset ) const;
  void getSubgridCenter( const VIdx& subgridVOffset, VReal& vOffset ) const;
  BOOL offsetIsInSubgrid( const VReal& vOffset, const VIdx& vSubgrid ) const;
  REAL getSubgridValue( const NbrUBEnv& nbrUBEnv, const VReal& vOffset ) const;
  REAL getSubgridValue( const UBEnv& ubEnv, const VReal& vOffset ) const;
  REAL getSubgridValue( const UBEnv& ubEnv, const VIdx& subgridVOffset ) const;
  REAL getSubgridVolume( ) const;
  REAL getPDETimeStepDuration( ) const;

  // support for model_routine_config.cpp
  void calculateBoundaryConditions( );
  void updatePhiPDEInfo( PDEInfo& pdeInfo ) const;

  // support for model_routine_grid.cpp
  void initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) const;
  void initIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const;
  void updateIfGridVarPre( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const;
  void updateIfGridVarPost( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const;
  void updateIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const;
  void updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) const;
  void updateIfSubgridBetaInIfRegion( const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) const;
  void updateIfSubgridBetaPDEBufferBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const;
  void updateIfSubgridBetaDomainBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const;
  void updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const;
  void adjustIfSubgridRHSTimeDependentLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) const;
  void updateIfSubgridRHSTimeDependentSplitting( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL& gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const;
  void updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, S32& finestLevel ) const;
  void updateIfGridDirichletBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const;
  void updateIfGridNeumannBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const;
  void initPDEBufferPhi( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) const;
  void initPDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const;
  void updatePDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const;
  void updatePDEBufferAlpha( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) const;
  void updatePDEBufferBetaInPDEBufferRegion( const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const;
  void updatePDEBufferBetaDomainBdry( const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const;
  void updatePDEBufferRHSLinear( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const;
  void updatePDEBufferRHSTimeDependentSplitting( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const;
  void updatePDEBufferDirichletBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const;
  void updatePDEBufferNeumannBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const;
  
protected:
  const BioModel *mModel;
  std::string mName;
  S32         mSoluteIdx;
  S32         mDomainIdx;
  S32         mSolverIdx;
  REAL        mGridAlpha; // decay rate within the domain
  REAL        mGridBeta; // diffusivity within the domain
  bc_type_e   mGridBCType[3][2]; // Boundary type 
  REAL        mGridBCVal[3][2]; // Boundary value
  REAL        mGridBoundaryBeta[3][2]; // mGridBeta[ dim ][ face ] diffusivity at the boundary

  Vector< S32 > mReactions; // Reactions that do not have molecule related kinetic factors
  Vector< S32 > mMoleculeReactions;  // Reactions that do have molecule related kinetic factors
  
  S32         mAMRLevels;         // Total number of AMR Levels
  S32         mInterfaceAMRLevel; // AMR Level of the interface // RANGE 0 <-> mAMRLevels - 1
  S32         mNumTimeSteps;
  S32         mSubgridDimension;  // subgrid dimension
  };

#endif /* _SOLUTE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
