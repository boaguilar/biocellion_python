#include "model_define.h"
#include "model_mechanisms.h"
#include <iomanip>
#include <cmath>

MechIntrctSpAgent* MechIntrctSpAgent::create()
{
  return 0;
}

MechIntrctSpAgent::~MechIntrctSpAgent()
{
  // empty
}

// SHOVE

MechIntrctSpAgent* MechIntrctSpAgentShove::create()
{
  MechIntrctSpAgentShove* intrct = new MechIntrctSpAgentShove();

  S32 i;
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    intrct->setScale( i , gAgentSpecies[ i ]->getParamReal( gAgentSpecies[ i ]->getIdxReal( SP_shoveScale ) ) );
    intrct->setFactor( i , gAgentSpecies[ i ]->getParamReal( gAgentSpecies[ i ]->getIdxReal( SP_shoveFactor ) ) );
    intrct->setLimit( i , gAgentSpecies[ i ]->getParamReal( gAgentSpecies[ i ]->getIdxReal( SP_shoveLimit ) ) );
  }
  intrct->setDimensions(3);

  return intrct;
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove()
  : mScales(0), mDimensions(3)
{
  // empty
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove(const Vector<REAL>& scales, const Vector<REAL>& factors, const Vector<REAL>& limits, const S32& dimensions)
  : mScales(scales), mFactors(factors), mLimits(limits), mDimensions(dimensions)
{
  // empty
}

MechIntrctSpAgentShove::~MechIntrctSpAgentShove()
{
  // empty
}

void MechIntrctSpAgentShove::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();

#if 0
  if( !gMechIntrctShoveEnabled[ agentType0 ][ agentType1 ] ) {
    return;
  }
#endif

  REAL R = mFactors[ agentType0 ] * spAgent0.state.getRadius() + mFactors[ agentType1 ] *  spAgent1.state.getRadius() + ( ( mLimits[ agentType0 ] + mLimits[ agentType1 ] ) / 2.0 );

  if( dist <= R ) { /* shoving */
    REAL scale = (mScales[ agentType0 ] + mScales[ agentType1 ] ) / 2.0;
    REAL mag = 0.5 * scale * ( R - dist ); // 0.5 to share between the agents
    
    mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealX(), dir[0] * mag );
    mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealY(), dir[1] * mag );

    mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealX(), -dir[0] * mag );
    mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealY(), -dir[1] * mag );

    if(mDimensions > 2) {
      mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealZ(), dir[2] * mag );
      mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealZ(), -dir[2] * mag );
    } else {
      /* No need to set to 0, since DELTA updates are used */
    }
  }
}

void MechIntrctSpAgentShove::setScale(const S32& agent_type, const REAL& value)
{
  if(( S32 )mScales.size() <= agent_type) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = 0.0;
    }
  }
  mScales[agent_type] = value;
}

void MechIntrctSpAgentShove::setFactor(const S32& agent_type, const REAL& value)
{
  if(( S32 )mFactors.size() <= agent_type) {
    S32 old_size = ( S32 )mFactors.size();
    mFactors.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mFactors.size(); i++) {
      mFactors[i] = 0.0;
    }
  }
  mFactors[agent_type] = value;
}

void MechIntrctSpAgentShove::setLimit(const S32& agent_type, const REAL& value)
{
  if(( S32 )mLimits.size() <= agent_type) {
    S32 old_size = ( S32 )mLimits.size();
    mLimits.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mLimits.size(); i++) {
      mLimits[i] = 0.0;
    }
  }
  mLimits[agent_type] = value;
}

void MechIntrctSpAgentShove::setDimensions(const S32& dimensions)
{
  mDimensions = dimensions;
}

// ADHESION

MechIntrctSpAgent* MechIntrctSpAgentAdhesion::create()
{
  MechIntrctSpAgentAdhesion* intrct = new MechIntrctSpAgentAdhesion();

  S32 i, j;
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    const Vector< Adhesion * >& adhesions = gAgentSpecies[ i ]->getAdhesions( );
    for( j = 0 ; j < (S32)adhesions.size( ) ; j++ ) {
      intrct->setScale( i, adhesions[ j ]->getWithSpecies( ), adhesions[ j ]->getScale( ) );
      intrct->setDistanceScale( i, adhesions[ j ]->getWithSpecies( ), adhesions[ j ]->getStrength( ) );
    }
  }

  return intrct;
}

MechIntrctSpAgentAdhesion::MechIntrctSpAgentAdhesion()
{
  // empty
}

MechIntrctSpAgentAdhesion::~MechIntrctSpAgentAdhesion()
{
  // empty
}

void MechIntrctSpAgentAdhesion::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();

  if( mScales[ agentType0 ][ agentType1 ] <= 0.0 || mDistanceScales[ agentType0 ][ agentType1 ] <= 0.0 ) {
    return;
  }

  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
  if( dist > R ) {
    REAL x = dist / R;
    REAL a = 0.5 * mScales[ agentType0 ][ agentType1 ]; // 0.5 to share between the agents
    REAL d = mDistanceScales[ agentType0 ][ agentType1 ];
    REAL mag = -a * ( dist - R ) * exp( -1.0 * ( x - 1.0 ) * ( x - 1.0 ) / d );
    
    mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealX(), dir[0] * mag );
    mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealY(), dir[1] * mag );
    mechIntrctData0.setModelReal( gAgentSpecies[ agentType0 ]->getIdxMechForceRealZ(), dir[2] * mag );

    mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealX(), -dir[0] * mag );
    mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealY(), -dir[1] * mag );
    mechIntrctData1.setModelReal( gAgentSpecies[ agentType1 ]->getIdxMechForceRealZ(), -dir[2] * mag );
  }
}

void MechIntrctSpAgentAdhesion::setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mScales.size() <= agent_type0) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mScales[ agent_type0 ].size();
    mScales[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type0 ].size(); i++) {
      mScales[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mScales.size() <= agent_type1) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mScales[ agent_type1 ].size();
    mScales[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type1 ].size(); i++) {
      mScales[ agent_type1 ][ i ] = 0.0;
    }
  }

  mScales[ agent_type0 ][ agent_type1 ] = value;
  mScales[ agent_type1 ][ agent_type0 ] = value;
}


void MechIntrctSpAgentAdhesion::setDistanceScale(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mDistanceScales.size() <= agent_type0) {
    S32 old_size = ( S32 )mDistanceScales.size();
    mDistanceScales.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales.size(); i++) {
      mDistanceScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mDistanceScales[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mDistanceScales[ agent_type0 ].size();
    mDistanceScales[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales[ agent_type0 ].size(); i++) {
      mDistanceScales[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mDistanceScales.size() <= agent_type1) {
    S32 old_size = ( S32 )mDistanceScales.size();
    mDistanceScales.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales.size(); i++) {
      mDistanceScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mDistanceScales[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mDistanceScales[ agent_type1 ].size();
    mDistanceScales[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales[ agent_type1 ].size(); i++) {
      mDistanceScales[ agent_type1 ][ i ] = 0.0;
    }
  }

  mDistanceScales[ agent_type0 ][ agent_type1 ] = value;
  mDistanceScales[ agent_type1 ][ agent_type0 ] = value;
}

/////////////////////////////////////////////////////////////////
MolecularSpecies* MolecularSpecies::create(const S32& idx, const std::string& name)
{
  MolecularSpecies*  p = new MolecularSpecies();
  p->setIdx(idx);
  p->setName(name);
  return p;
}

MolecularSpecies::MolecularSpecies()
  : mIdx(-1), mName(""), mInitialValue(0.0),
    mDoFileOutput(true), mDoDivideByKappa(false),
    mDiffusivity(1.0), mDecayRate(0.0),
    mAMRLevels(3), mInterfaceAMRLevel(2),
    mNumTimeSteps(1), mSubgridDimension(-1)
{
}

MolecularSpecies::~MolecularSpecies()
{
}

// support for general attributes
S32 MolecularSpecies::getIdx() const
{
  return mIdx;
}

void MolecularSpecies::setIdx(const S32& idx)
{
  if ( idx >= 0 ) { 
    mIdx = idx;
  }
}

std::string MolecularSpecies::getName() const
{
  return mName;
}

void MolecularSpecies::setName(const std::string& name)
{
  mName = name;
}

REAL MolecularSpecies::getInitialValue() const
{
  return mInitialValue;
}

void MolecularSpecies::setInitialValue(const REAL& value)
{
  mInitialValue = value;
}

bool MolecularSpecies::getDoFileOutput() const
{
  return mDoFileOutput;
}

void MolecularSpecies::setDoFileOutput(const bool& value)
{
  mDoFileOutput = value;
}

bool MolecularSpecies::getDoDivideByKappa() const
{
  return mDoDivideByKappa;
}

void MolecularSpecies::setDoDivideByKappa(const bool& value)
{
  mDoDivideByKappa = value;
}

REAL MolecularSpecies::getDiffusivity() const
{
  return mDiffusivity;
}

void MolecularSpecies::setDiffusivity(const REAL& value)
{
  mDiffusivity = value;
}

REAL MolecularSpecies::getDecayRate() const
{
  return mDecayRate;
}

void MolecularSpecies::setDecayRate(const REAL& value)
{
  mDecayRate = value;
}

S32  MolecularSpecies::getAMRLevels() const
{
  return mAMRLevels;
}

void MolecularSpecies::setAMRLevels(const S32 value)
{
  mAMRLevels = value;
}

S32  MolecularSpecies::getInterfaceAMRLevel() const
{
  return mInterfaceAMRLevel;
}

void MolecularSpecies::setInterfaceAMRLevel(const S32 value)
{
  mInterfaceAMRLevel = value;
}

idx_t MolecularSpecies::getSubgridDimension()
{
  if( mSubgridDimension > 0 ) {
    return mSubgridDimension;
  }
  S32 l = (mAMRLevels-1) - mInterfaceAMRLevel;
  mSubgridDimension = 1;
  for( S32 i = l ; i > 0 ; i-- ) {
    mSubgridDimension *= Info::getAMRRatio();
  }
  return mSubgridDimension;
}

S32  MolecularSpecies::getNumTimeSteps() const
{
  return mNumTimeSteps;
}

void MolecularSpecies::setNumTimeSteps(const S32 value)
{
  mNumTimeSteps = value;
}

void MolecularSpecies::getSubgridOffset(const VReal& vOffset, VIdx& subgridVOffset)
{
  for( S32 dim = 0; dim < DIMENSION; dim++ ) {
    REAL offset  = 0.5 + vOffset[dim] / gAgentGrid->getResolution( );
    subgridVOffset[dim] = offset * getSubgridDimension();
  }
}

bool MolecularSpecies::offsetIsInSubgrid(const VReal& vOffset, const VIdx& vSubgrid) 
{
  VIdx sgridVOffset;
  getSubgridOffset(vOffset, sgridVOffset);
  return sgridVOffset == vSubgrid;
}

REAL MolecularSpecies::getSubgridValue(const NbrUBEnv& nbrUBEnv, const VReal& vOffset)
{
  // Find the subgridphi value in the subgrid at vOffset from the center of nbrUBEnv.
  // If vOffset is not a valid position, return -1.0;
  idx_t subgrid_dimension = getSubgridDimension();
  S32 dim;
  VIdx vidx, vsubidx;
  REAL step = gAgentGrid->getResolution( ) / subgrid_dimension;
  for( dim = 0 ; dim < DIMENSION ; dim++ ) {
    vidx[dim] = 0;
    vsubidx[dim] = (idx_t)(vOffset[dim] + 0.5) / step;
    while( vsubidx[dim] < 0 ) {
      vsubidx[dim] += subgrid_dimension;
      vidx[dim] -= 1;
    }
    while( vsubidx[dim] >= subgrid_dimension ) {
      vsubidx[dim] -= subgrid_dimension;
      vidx[dim] += 1;
    }
  }

  if(! nbrUBEnv.getValidFlag( vidx ) ) {
    return -1.0;
  }

  return nbrUBEnv.getSubgridPhi( vidx, vsubidx, mIdx );
}


// support for model_routine_config.cpp
PDEInfo MolecularSpecies::getPDEInfo() const
{
  PDEInfo pdeInfo;
  GridPhiInfo gridPhiInfo;
  SplittingInfo splittingInfo;

  pdeInfo.pdeIdx = mIdx;
  pdeInfo.pdeType = PDE_TYPE_REACTION_DIFFUSION_TIME_DEPENDENT_LINEAR;
  pdeInfo.numLevels = mAMRLevels; // ???FIXME: Need to configure this correctly
  pdeInfo.ifLevel = mInterfaceAMRLevel; // ???FIXME: Need to configure this correctly
  pdeInfo.v_tagExpansionSize.assign( mAMRLevels, 0 ); // ???FIXME: Need to configure this correctly
  pdeInfo.numTimeSteps = mNumTimeSteps; // ???FIXME: Need to configure this correctly
  pdeInfo.callAdjustRHSTimeDependentLinear = false; // ???FIXME: Need to configure this correctly

  // set mgSolveInfo.* here
  // ???FIXME: Need to configure these correctly
  
  pdeInfo.mgSolveInfo.numPre = 3;/* multigrid parameters */
  pdeInfo.mgSolveInfo.numPost = 3;/* multigrid parameters */
  pdeInfo.mgSolveInfo.numBottom = 3;/* multigrid parameters */
  pdeInfo.mgSolveInfo.vCycle = true;/* multigrid parameters */
  pdeInfo.mgSolveInfo.maxIters = 30;/* multigrid parameters */
  pdeInfo.mgSolveInfo.epsilon = 1e-8;/* multigrid parameters */
  pdeInfo.mgSolveInfo.hang = 1e-6;/* multigrid parameters */
  pdeInfo.mgSolveInfo.normThreshold = 1e-10;/* multigrid parameters */

  // set splittingInfo here
  pdeInfo.splittingInfo = splittingInfo;

  // set gridPhiInfo here
  pdeInfo.v_gridPhiInfo.resize( 1 );

  gridPhiInfo.elemIdx = mIdx;
  gridPhiInfo.name = mName;
  gridPhiInfo.syncMethod = VAR_SYNC_METHOD_DELTA;
  gridPhiInfo.aa_bcType[0][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[0][0] = 0.0;
  gridPhiInfo.aa_bcType[0][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[0][1] = 0.0;
  gridPhiInfo.aa_bcType[1][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[1][0] = 0.0;
  gridPhiInfo.aa_bcType[1][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[1][1] = 0.0;
  gridPhiInfo.aa_bcType[2][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[2][0] = 0.0;
  gridPhiInfo.aa_bcType[2][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[2][1] = 0.0;

  // ???FIXME: Need to configure this correctly
  gridPhiInfo.errorThresholdVal = pdeInfo.mgSolveInfo.normThreshold * -1.0;
  gridPhiInfo.warningThresholdVal = pdeInfo.mgSolveInfo.normThreshold * -1.0;
  gridPhiInfo.setNegToZero = true;

  pdeInfo.v_gridPhiInfo[0] = gridPhiInfo;

  return pdeInfo;
}


// support for model_routine_grid.cpp
void MolecularSpecies::initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv )
{
  ubEnv.setPhi( mIdx, mInitialValue );
}

void MolecularSpecies::initIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa )
{
  gridKappa = 1.0;
}

void MolecularSpecies::updateIfGridVarPre( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ )
{
  /* noop */
}

void MolecularSpecies::updateIfGridVarPost( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ )
{
  /* noop */
}

void MolecularSpecies::updateIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa )
{
  if( mDoDivideByKappa ) {
    gridKappa = 1.0;
    // FIXME: This is not real
    for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
      const SpAgent& spAgent = ubAgentData.v_spAgent[l];
      VIdx agentOffset;
      getSubgridOffset(spAgent.vOffset, agentOffset);
      if( agentOffset == subgridVOffset ) {
        gridKappa *= 0.5;
      }
    }
  } else {
    gridKappa = 1.0;
  }
  
}

void MolecularSpecies::updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ )
{
  gridAlpha = -mDecayRate;
}

void MolecularSpecies::updateIfSubgridBetaInIfRegion( const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta )
{
  /* flux between subgrids */
  gridBeta = mDiffusivity;
}

void MolecularSpecies::updateIfSubgridBetaPDEBufferBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta )
{
  /* flux between grid and buffer */
  gridBeta = mDiffusivity;
}

void MolecularSpecies::updateIfSubgridBetaDomainBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta )
{
  /* flux at domain boundary */
  gridBeta = 0.0;
}

void MolecularSpecies::updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ )
{
  gridRHS = 0.0; // no secretion or uptake
}

void MolecularSpecies::adjustIfSubgridRHSTimeDependentLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updateIfSubgridRHSTimeDependentSplitting( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL& gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, S32& finestLevel )
{
  // FIXME: ?documentation says finestLevel is a vector, but it's not?
  // FIXME: could be dynamic based on the number of particles around, etc.
  finestLevel = mAMRLevels - 1; // FIXME: need to be specific to the model.
}

void MolecularSpecies::updateIfGridDirichletBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updateIfGridNeumannBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::initPDEBufferPhi( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::initPDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferAlpha( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferBetaInPDEBufferRegion( const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferBetaDomainBdry( const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferRHSLinear( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ )
{
  gridRHS = 0.0; // no uptake or secretion
}

void MolecularSpecies::updatePDEBufferRHSTimeDependentSplitting( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferDirichletBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal )
{
  ERROR( "unimplmented." );
}

void MolecularSpecies::updatePDEBufferNeumannBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal )
{
  ERROR( "unimplmented." );
}

MolecularSpeciesGradient* MolecularSpeciesGradient::create(const S32& idx, const std::string& name)
{
  MolecularSpeciesGradient*  p = new MolecularSpeciesGradient();
  p->setIdx(idx);
  p->setName(name);
  p->setDiffusivity(1.0);
  p->setDecayRate(0.000001);
  p->setAMRLevels(3);
  p->setInterfaceAMRLevel(2);
  p->setNumTimeSteps(1);
  return p;
}

MolecularSpeciesGradient::MolecularSpeciesGradient()
  : MolecularSpecies()
{
  /* empty */
}

void MolecularSpeciesGradient::updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ )
{
  // // scale decay with Y dimension for testing purposes.
  // REAL size = Info::getDomainSize(1) - 1;
  // REAL scale = vIdx[1]/size;
	// gridAlpha = -0.025 * scale;  // FIXME:  This is just for testing.
  gridAlpha = -mDecayRate;
}

void MolecularSpeciesGradient::updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ )
{
  // FIXME: Need all secretion and uptake rates for all agent species.  If present, can solve this generically.
  // secrete from agent
  gridRHS = 0.0;
  // for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
  //   const SpAgent& spAgent = ubAgentData.v_spAgent[l];
  //   S32 agentType = spAgent.state.getType();
  //   /* FIXME gridRHS += CHEMO_ATTRACTANT_LONG_SECRETION_RATE[agentType]; */
  // }
  
}



MolecularSpeciesShortAttractant* MolecularSpeciesShortAttractant::create(const S32& idx, const std::string& name)
{
  MolecularSpeciesShortAttractant*  p = new MolecularSpeciesShortAttractant();
  p->setIdx(idx);
  p->setName(name);
  p->setDiffusivity(1.0);
  p->setDecayRate(0.04);
  p->setAMRLevels(4);
  p->setInterfaceAMRLevel(2);
  p->setDoDivideByKappa(false);
  return p;
}

MolecularSpeciesShortAttractant::MolecularSpeciesShortAttractant()
  : MolecularSpecies()
{
  /* empty */
}

void MolecularSpeciesShortAttractant::updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ )
{
  // FIXME: Need all secretion and uptake rates for all agent species.  If present, can solve this generically.
  // secrete from agent
  gridRHS = 0.0;
  // for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
  //   const SpAgent& spAgent = ubAgentData.v_spAgent[l];
  //   VIdx sgridVOffset;
  //   getSubgridOffset( spAgent.vOffset, sgridVOffset );
  //   if( sgridVOffset == subgridVOffset ) {
  //     S32 agentType = spAgent.state.getType();
  //     /* FIXME: gridRHS += CHEMO_ATTRACTANT_SHORT_SECRETION_RATE[agentType]; */
  //   }
  // }
  
}
