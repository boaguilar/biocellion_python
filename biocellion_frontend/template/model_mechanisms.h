#ifndef _MODEL_MECHANISMS_H_
#define _MODEL_MECHANISMS_H_

#include "biocellion.h"
#include <string>

class MechIntrctSpAgent {
public:
  static MechIntrctSpAgent* create();
  virtual ~MechIntrctSpAgent();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) = 0;
};

class MechIntrctSpAgentShove : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentShove();
  MechIntrctSpAgentShove(const Vector<REAL>& scales, const Vector<REAL>& factors, const Vector<REAL>& limits, const S32& dimensions);
  virtual ~MechIntrctSpAgentShove();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type, const REAL& value);
  void setFactor(const S32& agent_type, const REAL& value);
  void setLimit(const S32& agent_type, const REAL& value);
  void setDimensions(const S32& dimensions);

protected:
  Vector<REAL> mScales;
  Vector<REAL> mFactors;
  Vector<REAL> mLimits;
  S32 mDimensions;
  
};

class MechIntrctSpAgentAdhesion : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentAdhesion();
  virtual ~MechIntrctSpAgentAdhesion();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setDistanceScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);

protected:
  Vector< Vector< REAL > > mScales;
  Vector< Vector< REAL > > mDistanceScales;
};


class MechIntrctSpAgentDistanceJunction : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentDistanceJunction();
  virtual ~MechIntrctSpAgentDistanceJunction();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setEnabled(const S32& agent_type0, const S32& agent_type1, const BOOL& value);
  void setLinkScale(const S32& agent_type, const REAL& value);
  void setUnlinkScale(const S32& agent_type, const REAL& value);
  void setJunctionIdx(const S32& value);
  void setRealDistanceIdx(const S32& value);
  void setIntTouchedIdx(const S32& value);

protected:
  Vector< Vector< BOOL > > mEnabled;
  Vector< REAL > mLinkScales;
  Vector< REAL > mUnlinkScales;
  S32 mJunctionIdx;
  S32 mRealDistanceIdx;
  S32 mIntTouchedIdx;
};

class MechIntrctSpAgentTightJunction : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentTightJunction();
  virtual ~MechIntrctSpAgentTightJunction();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setStiffness(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setIntTouchedIdx(const S32& value);

protected:
  Vector< Vector< REAL > > mScales;
  Vector< Vector< REAL > > mStiffnesses;
  S32 mIntTouchedIdx;
};



class MolecularSpecies {

public:
  static MolecularSpecies* create(const S32& idx, const std::string& name);
  MolecularSpecies();
  virtual ~MolecularSpecies();

  // support for general attributes
  virtual S32 getIdx() const;
  virtual void setIdx(const S32& idx);
  virtual std::string getName() const;
  virtual void setName(const std::string& name);
  virtual REAL getInitialValue() const;
  virtual void setInitialValue(const REAL& value);
  virtual bool getDoFileOutput() const;
  virtual void setDoFileOutput(const bool& value);
  virtual bool getDoDivideByKappa() const;
  virtual void setDoDivideByKappa(const bool& value);
  virtual REAL getDiffusivity() const;
  virtual void setDiffusivity(const REAL& value);
  virtual REAL getDecayRate() const;
  virtual void setDecayRate(const REAL& value);
  virtual S32  getAMRLevels() const;
  virtual void setAMRLevels(const S32 value);
  virtual S32  getInterfaceAMRLevel() const;
  virtual void setInterfaceAMRLevel(const S32 value);
  virtual idx_t getSubgridDimension();
  virtual S32  getNumTimeSteps() const;
  virtual void setNumTimeSteps(const S32 value);
  virtual void getSubgridOffset(const VReal& vOffset, VIdx& subgridVOffset);
  virtual bool offsetIsInSubgrid(const VReal& vOffset, const VIdx& vSubgrid) ;
  virtual REAL getSubgridValue(const NbrUBEnv& nbrUBEnv, const VReal& vOffset);

  // support for model_routine_config.cpp
  virtual PDEInfo getPDEInfo() const;

  // support for model_routine_grid.cpp
  virtual void initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv );
  virtual void initIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa );
  virtual void updateIfGridVarPre( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ );
  virtual void updateIfGridVarPost( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ );
  virtual void updateIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa );
  virtual void updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ );
  virtual void updateIfSubgridBetaInIfRegion( const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta );
  virtual void updateIfSubgridBetaPDEBufferBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta );
  virtual void updateIfSubgridBetaDomainBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta );
  virtual void updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ );
  virtual void adjustIfSubgridRHSTimeDependentLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ );
  virtual void updateIfSubgridRHSTimeDependentSplitting( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL& gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ );
  virtual void updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, S32& finestLevel );
  virtual void updateIfGridDirichletBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal );
  virtual void updateIfGridNeumannBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal );
  virtual void initPDEBufferPhi( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ );
  virtual void initPDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa );
  virtual void updatePDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa );
  virtual void updatePDEBufferAlpha( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ );
  virtual void updatePDEBufferBetaInPDEBufferRegion( const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta );
  virtual void updatePDEBufferBetaDomainBdry( const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta );
  virtual void updatePDEBufferRHSLinear( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ );
  virtual void updatePDEBufferRHSTimeDependentSplitting( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ );
  virtual void updatePDEBufferDirichletBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal );
  virtual void updatePDEBufferNeumannBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal );
  
protected:

  S32         mIdx;             // global index in grid phi vector
  std::string mName;            // text name for model output
  REAL        mInitialValue;    // initial value
  bool        mDoFileOutput;    // write values to file?
  bool        mDoDivideByKappa; // divide by Kappa when writing?

  REAL        mDiffusivity;     // BETA in PDE equations  // UNITS??
  REAL        mDecayRate;       // ALPHA in PDE equations // UNITS = fraction of current value // RANGE = 0.0 <-> 1.0

  S32         mAMRLevels;         // Total number of AMR Levels
  S32         mInterfaceAMRLevel; // AMR Level of the interface // RANGE 0 <-> mAMRLevels - 1
  S32         mNumTimeSteps;
  S32         mSubgridDimension;  // subgrid dimension

private:

};

class MolecularSpeciesGradient : public MolecularSpecies {

public:

  static MolecularSpeciesGradient* create(const S32& idx, const std::string& name);
  MolecularSpeciesGradient();
  
  virtual void updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ );
  virtual void updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ );
  
protected:

private:

};

class MolecularSpeciesShortAttractant : public MolecularSpecies {

public:

  static MolecularSpeciesShortAttractant* create(const S32& idx, const std::string& name);
  MolecularSpeciesShortAttractant();
  
  virtual void updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ );
  
protected:

private:

};

#endif /* _MODEL_MECHANISMS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
