#ifndef _BIOMODEL_H_
#define _BIOMODEL_H_

#include "biocellion.h"
#include "simulator.h"
#include "agent_grid.h"
#include "agent_species.h"
#include "particle.h"
#include "model_mechanisms.h"
#include "adhesion.h"
#include "distance_junction.h"
#include "tight_junction.h"
#include "solute.h"
#include "reaction.h"

class BioModel {
public:
  BioModel( );
  virtual ~BioModel( );  

  const Vector< Solute * >& getSolutes( ) const;
  Vector< Solute * >& getSolutes( );
  const Vector< Reaction * >& getReactions( ) const;
  Vector< Reaction * >& getReactions( );
  BOOL getDistanceJunctionsEnabled( ) const;

  void setDistanceJunctionsEnabled( const BOOL& value );

  // support for model_routine_config.cpp
  void updatePhiPDEInfo( Vector<PDEInfo>& v_phiPDEInfo ) const;
  void updateFileOutputInfo( FileOutputInfo& fileOutputInfo ) const;
  
  // support for model_routine_grid.cpp
  void initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) const;
  void initIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const;
  void updateIfGridVar( const BOOL pre, const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const;
  void updateIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const;
  void updateIfSubgridAlpha( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) const;
  void updateIfSubgridBetaInIfRegion( const S32 elemIdx, const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) const;
  void updateIfSubgridBetaPDEBufferBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const;
  void updateIfSubgridBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const;
  void updateIfSubgridRHSLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const;
  void adjustIfSubgridRHSTimeDependentLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) const;
  void updateIfSubgridRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const;
  void updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, Vector<S32>& v_finestLevel/* [pdeIdx] */ ) const;
  void updateIfGridDirichletBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const;
  void updateIfGridNeumannBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const;
  void initPDEBufferPhi( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) const;
  void initPDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const;
  void updatePDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const;
  void updatePDEBufferAlpha( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) const;
  void updatePDEBufferBetaInPDEBufferRegion( const S32 elemIdx, const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const;
  void updatePDEBufferBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const;
  void updatePDEBufferRHSLinear( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const;
  void updatePDEBufferRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const;
  void updatePDEBufferDirichletBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const;
  void updatePDEBufferNeumannBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const;

protected:
  Vector<Solute *> mSolutes;
  Vector<Reaction *> mReactions;
  BOOL mDistanceJunctionsEnabled;
};

extern BioModel *gBioModel;
extern Simulator *gSimulator;
extern AgentGrid *gAgentGrid;
extern Vector<AgentSpecies *> gAgentSpecies;
extern Vector<Particle *> gParticles;
extern Vector<MechIntrctSpAgent *> gMechIntrctSpAgent;
extern Vector< Vector<BOOL> > gMechIntrctShoveEnabled;

extern void initializeBioModel();
extern void terminateBioModel();

// AGENT SPECIES AUTO BEGIN
// AGENT SPECIES AUTO END

typedef enum _junction_type_e {
  JUNCTION_TYPE_DISTANCE,
	NUM_JUNCTION_TYPES
} junction_type_e;

typedef enum _junction_real_type_e {
  JUNCTION_REAL_DISTANCE, // last know distance between centers
  NUM_JUNCTION_REAL_TYPES
} junction_real_type_e;

typedef enum _junction_int_type_e {
  JUNCTION_INT_TOUCHED, // 0 == no, 1 == yes
  NUM_JUNCTION_INT_TYPES
} junction_int_type_e;

#endif /* _BIOMODEL_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
