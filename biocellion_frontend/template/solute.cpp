#include "solute.h"

Solute::Solute()
  : ParamHolder( ),
    mName( "" ), mSoluteIdx( -1 ), mDomainIdx( -1 ),
    mAMRLevels( 3 ), mInterfaceAMRLevel( 2 ),    // FIXME: AMR related values should not be hard-coded
    mNumTimeSteps( 1 ), mSubgridDimension(-1)
{
  WARNING( "Solute::mAMRLevels should not be hard-coded" );
  calcSubgridDimension( );
}

Solute::Solute(const std::string& name, const S32& solute_idx, const S32& domain_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param) 
  : ParamHolder( num_real_param, num_int_param, num_bool_param, num_string_param ),
    mName( name ), mSoluteIdx( solute_idx ), mDomainIdx( domain_idx ),
    mAMRLevels( 3 ), mInterfaceAMRLevel( 2 ),
    mNumTimeSteps( 1 ), mSubgridDimension(-1)
{
  WARNING( "Solute::mAMRLevels should not be hard-coded" );
  calcSubgridDimension( );
}

const std::string& Solute::getName() const {
  return mName;
}

S32 Solute::getSoluteIdx() const {
  return mSoluteIdx;
}

S32 Solute::getDomainIdx() const {
  return mDomainIdx;
}

S32 Solute::getAMRLevels() const {
  return mAMRLevels;
}

S32 Solute::getInterfaceAMRLevel() const {
  return mInterfaceAMRLevel;
}

S32 Solute::getNumTimeSteps() const {
  return mNumTimeSteps;
}


void Solute::setName(const std::string& name) {
  mName = name;
}

void Solute::setSoluteIdx(const S32& idx) {
  mSoluteIdx = idx;
}

void Solute::setDomainIdx(const S32& idx) {
  mDomainIdx = idx;
}

void Solute::setAMRLevels(const S32& value) {
  mAMRLevels = value;
  calcSubgridDimension( );
}

void Solute::setInterfaceAMRLevel(const S32& value) {
  mInterfaceAMRLevel = value;
  calcSubgridDimension( );
}

void Solute::setNumTimeSteps(const S32& value) {
  mNumTimeSteps = value;
}


idx_t Solute::getSubgridDimension() const {
  CHECK( mSubgridDimension > 0  );
  return mSubgridDimension;
}

void Solute::calcSubgridDimension() {
  S32 l = ( mAMRLevels -1 ) - mInterfaceAMRLevel;
  mSubgridDimension = 1;
  for( S32 i = l ; i > 0 ; i-- ) {
    mSubgridDimension *= Info::getAMRRatio();
  }
  return;
}

void Solute::getSubgridOffset( const VReal& vOffset, VIdx& subgridVOffset ) const {
  for( S32 dim = 0; dim < DIMENSION; dim++ ) {
    REAL offset  = 0.5 + vOffset[ dim ] / gAgentGrid->getResolution( );
    subgridVOffset[ dim ] = offset * getSubgridDimension();
  }
}

BOOL Solute::offsetIsInSubgrid(const VReal& vOffset, const VIdx& vSubgrid) const {
  VIdx sgridVOffset;
  getSubgridOffset( vOffset, sgridVOffset );
  return sgridVOffset == vSubgrid;
}

REAL Solute::getSubgridValue(const NbrUBEnv& nbrUBEnv, const VReal& vOffset) const {
  // Find the subgridphi value in the subgrid at vOffset from the center of nbrUBEnv.
  // If vOffset is not a valid position, return a value < 0
  idx_t subgrid_dimension = getSubgridDimension();
  S32 dim;
  VIdx vidx, vsubidx;
  REAL step = gAgentGrid->getResolution( ) / subgrid_dimension;
  for( dim = 0 ; dim < DIMENSION ; dim++ ) {
    vidx[ dim ] = 0;
    vsubidx[ dim ] = (idx_t)( vOffset[ dim ] + 0.5 ) / step;
    while( vsubidx[ dim ] < 0 ) {
      vsubidx[ dim ] += subgrid_dimension;
      vidx[ dim ] -= 1;
    }
    while( vsubidx[ dim ] >= subgrid_dimension ) {
      vsubidx[ dim ] -= subgrid_dimension;
      vidx[ dim ] += 1;
    }
  }

  if(! nbrUBEnv.getValidFlag( vidx ) ) {
    return -1.0;
  }

  return nbrUBEnv.getSubgridPhi( vidx, vsubidx, mSoluteIdx );
}



// support for model_routine_config.cpp
void Solute::setPDEInfo( PDEInfo& pdeInfo ) const {
  WARNING( "setPDEInfo still has numerous hard-coded values." );
  GridPhiInfo gridPhiInfo;
  SplittingInfo splittingInfo;

  pdeInfo.pdeIdx = mSoluteIdx;
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

  gridPhiInfo.elemIdx = mSoluteIdx;
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
}


// support for model_routine_grid.cpp
void Solute::initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) const {
  WARNING( "Solute:: initialvalue should not be hard coded" );
  //ubEnv.setPhi( mSoluteIdx, mInitialValue );
  ubEnv.setPhi( mSoluteIdx, 0 );
}

void Solute::initIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {
  gridKappa = 1.0;
}

void Solute::updateIfGridVarPre( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const {
  /* noop */
}

void Solute::updateIfGridVarPost( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const {
  /* noop */
}

void Solute::updateIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {
  // if( mDoDivideByKappa ) {
  //   gridKappa = 1.0;
  //   // FIXME: This is not real
  //   for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
  //     const SpAgent& spAgent = ubAgentData.v_spAgent[l];
  //     VIdx agentOffset;
  //     getSubgridOffset(spAgent.vOffset, agentOffset);
  //     if( agentOffset == subgridVOffset ) {
  //       gridKappa *= 0.5;
  //     }
  //   }
  // } else {
  gridKappa = 1.0;
  // }
  
}

void Solute::updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) const {
  //gridAlpha = -mDecayRate;
  gridAlpha = -0.04;
}

void Solute::updateIfSubgridBetaInIfRegion( const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) const {
  /* flux between subgrids */
  // gridBeta = mDiffusivity;
  gridBeta = 1.0;
}

void Solute::updateIfSubgridBetaPDEBufferBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {
  /* flux between grid and buffer */
  //gridBeta = mDiffusivity;
  gridBeta = 1.0;
}

void Solute::updateIfSubgridBetaDomainBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {
  /* flux at domain boundary */
  gridBeta = 0.0;
}

void Solute::updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  
  // for every reaction with a yield that involves this solute, add/subtract gridRHS.
  // amount is controlled by kineticFactors
  // FIXME: Need to apply computationDomains

  gridRHS = 0.0;
  S32 i;
  const Vector< Reaction * >& reactions = gBioModel->getReactions( );
  for( i = 0 ; i < (S32) reactions.size( ) ; i++ ) {
    REAL solute_value = ubEnv.getSubgridPhi( subgridVOffset, getSoluteIdx( ) );
    REAL factor = reactions[ i ]->getKineticFactor( getSoluteIdx( ), solute_value );
    
    // Check for all agents that apply
    for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
      const SpAgent& spAgent = ubAgentData.v_spAgent[ l ];
      VIdx sgridVOffset;
      getSubgridOffset( spAgent.vOffset, sgridVOffset );
      if( sgridVOffset == subgridVOffset ) {
        // agent is in the subgrid
        REAL yield = reactions[ i ]->getYield( getSoluteIdx( ), spAgent );
        gridRHS += yield * factor;
      }
    }
  }
}

void Solute::adjustIfSubgridRHSTimeDependentLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfSubgridRHSTimeDependentSplitting( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL& gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, S32& finestLevel ) const {
  // FIXME: ?documentation says finestLevel is a vector, but it's not?
  // FIXME: could be dynamic based on the number of particles around, etc.
  finestLevel = mAMRLevels - 1; // FIXME: need to be specific to the model.
}

void Solute::updateIfGridDirichletBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfGridNeumannBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::initPDEBufferPhi( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) const {
  ERROR( "unimplmented." );
}

void Solute::initPDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferAlpha( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferBetaInPDEBufferRegion( const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferBetaDomainBdry( const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferRHSLinear( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  gridRHS = 0.0; // no uptake or secretion
}

void Solute::updatePDEBufferRHSTimeDependentSplitting( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferDirichletBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferNeumannBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}



BulkSolute::BulkSolute()
  :mSBulk(0), mSin(0), mIsConstant(false), mSPulse(0), mPulseRate(0)
{
  //empty
}
BulkSolute::BulkSolute(const REAL& SBulk, const REAL& sin, const BOOL& isConstant, const REAL& sPulse, const REAL& pulseRate)
 :mSBulk(SBulk), mSin(sin), mIsConstant(isConstant), mSPulse(sPulse), mPulseRate(pulseRate)
{
  //empty
}
void BulkSolute::setSBulk(const REAL& SBulk)
{
  mSBulk = SBulk;
}
void BulkSolute::setSin(const REAL& sin)
{
  mSin = sin;
}
void BulkSolute::setIsConstant(const BOOL& isConstant)
{
  mIsConstant = isConstant;
}
void BulkSolute::setSPulse(const REAL& sPulse)
{
  mSPulse = sPulse;
}
void BulkSolute::setPulseRate(const REAL& pulseRate)
{
  mPulseRate = pulseRate;
}
