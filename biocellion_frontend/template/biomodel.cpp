#include "biomodel.h"

static bool gBioModelInitialized = false;
BioModel *gBioModel = 0;
Simulator *gSimulator = 0;
AgentGrid *gAgentGrid = 0;
Vector<AgentSpecies *> gAgentSpecies;
Vector<Particle *> gParticles;
Vector<MechIntrctSpAgent *> gMechIntrctSpAgent;
Vector< Vector<BOOL> > gMechIntrctShoveEnabled;

BioModel::BioModel( )
  : mDistanceJunctionsEnabled( false )
{
}

BioModel::~BioModel( ) {
  S32 i;
  
  for( i = 0; i < (S32) mSolutes.size(); i++ ) {
    if( mSolutes[i] ) {
      delete mSolutes[i];
      mSolutes[i] = 0;
    }
  }
  mSolutes.clear();

  for( i = 0; i < (S32) mReactions.size(); i++ ) {
    if( mReactions[i] ) {
      delete mReactions[i];
      mReactions[i] = 0;
    }
  }
  mReactions.clear();
}

const Vector< Solute * >& BioModel::getSolutes( ) const {
  return mSolutes;
}

Vector< Solute * >& BioModel::getSolutes( ) {
  return mSolutes;
}

const Vector< Reaction * >& BioModel::getReactions( ) const {
  return mReactions;
}

Vector< Reaction * >& BioModel::getReactions( ) {
  return mReactions;
}

BOOL BioModel::getDistanceJunctionsEnabled( ) const {
  return mDistanceJunctionsEnabled;
}

const World& BioModel::getWorld( ) const {
  return mWorld;
}

World& BioModel::getWorld( ) {
  return mWorld;
}

void BioModel::setDistanceJunctionsEnabled( const BOOL& value ) {
  mDistanceJunctionsEnabled = value;
}

// support for model_routine_config.cpp

void BioModel::updatePhiPDEInfo( Vector<PDEInfo>& v_phiPDEInfo ) const {

  WARNING( "This implementation relies on pdeIdx and elemIdx being 1-to-1 matched." );
  if( mSolutes.size() > 0 ) {
    S32 i;
    v_phiPDEInfo.resize( mSolutes.size( ) );
    for( i = 0; i < (S32) mSolutes.size( ); i++ ) {
      mSolutes[ i ]->setPDEInfo( v_phiPDEInfo[ i ] );
    }
  } else {
    v_phiPDEInfo.clear( );
  }
  
}

void BioModel::updateFileOutputInfo( FileOutputInfo& fileOutputInfo ) const {

  WARNING( "Should configure particle output in BioModel." );
  
  if( mSolutes.size() > 0 ) {
    S32 i;
    fileOutputInfo.v_gridPhiOutput.resize( mSolutes.size( ) );
    fileOutputInfo.v_gridPhiOutputDivideByKappa.resize( mSolutes.size( ) );
    for( i = 0; i < (S32) mSolutes.size( ); i++ ) {
      fileOutputInfo.v_gridPhiOutput[ mSolutes[ i ]->getSoluteIdx( ) ] = mSolutes[ i ]->getParamBool( mSolutes[ i ]->getIdxBool( SOLUTE_writeOutput ) );
      WARNING( "gridPhiOutputDivideByKappa should not be hard-coded." );
      fileOutputInfo.v_gridPhiOutputDivideByKappa[ mSolutes[ i ]->getSoluteIdx( ) ] = false;
    }
  } else {
    fileOutputInfo.v_gridPhiOutput.clear( );
    fileOutputInfo.v_gridPhiOutputDivideByKappa.clear(  );
  }
  
}

// support for model_routine_grid.cpp

void BioModel::initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) const {

  S32 i;
  for( i = 0 ; i < (S32) mSolutes.size( ); i++ ) {
    mSolutes[ i ]->initIfGridVar( vIdx, ubAgentData, ubEnv );
  }
  
  return;
}

void BioModel::initIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {

  mSolutes[ pdeIdx ]->initIfSubgridKappa( vIdx, subgridVOffset, ubAgentData, ubEnv, gridKappa );

  return;
}

void BioModel::updateIfGridVar( const BOOL pre, const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv ) const {

  S32 i;
  for( i = 0 ; i < (S32) mSolutes.size( ); i++ ) {
    if (pre) {
      mSolutes[ i ]->updateIfGridVarPre( iter, vIdx, nbrUBAgentData, nbrUBEnv );
    } else {
      mSolutes[ i ]->updateIfGridVarPost( iter, vIdx, nbrUBAgentData, nbrUBEnv );
    }
  } 
  
  return;
}

void BioModel::updateIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {

  mSolutes[ pdeIdx ]->updateIfSubgridKappa( vIdx, subgridVOffset, ubAgentData, ubEnv, gridKappa );

  return;
}

void BioModel::updateIfSubgridAlpha( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) const {

  mSolutes[ elemIdx ]->updateIfSubgridAlpha( vIdx, subgridVOffset, ubAgentData, ubEnv, gridAlpha );
  
  return;
}

void BioModel::updateIfSubgridBetaInIfRegion( const S32 elemIdx, const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) const {

  mSolutes[ elemIdx ]->updateIfSubgridBetaInIfRegion( dim, vIdx0, subgridVOffset0, ubAgentData0, ubEnv0, vIdx1, subgridVOffset1, ubAgentData1, ubEnv1, gridBeta );

  return;
}

void BioModel::updateIfSubgridBetaPDEBufferBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {

  mSolutes[ elemIdx ]->updateIfSubgridBetaPDEBufferBdry( dim, vIdx, subgridVOffset, ubAgentData, ubEnv, gridBeta );

  return;
}

void BioModel::updateIfSubgridBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {

  mSolutes[ elemIdx ]->updateIfSubgridBetaDomainBdry( dim, vIdx, subgridVOffset, ubAgentData, ubEnv, gridBeta );

  return;
}

void BioModel::updateIfSubgridRHSLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {

  mSolutes[ elemIdx ]->updateIfSubgridRHSLinear( vIdx, subgridVOffset, ubAgentData, ubEnv, gridRHS );

  return;
}

void BioModel::adjustIfSubgridRHSTimeDependentLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) const {

  mSolutes[ elemIdx ]->adjustIfSubgridRHSTimeDependentLinear( vIdx, subgridVOffset, ubAgentData, ubEnvModelVar, gridPhi, gridRHS );

  return;
}

void BioModel::updateIfSubgridRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const {/* for Wnt & SFRP */

  mSolutes[ pdeIdx ]->updateIfSubgridRHSTimeDependentSplitting( vIdx, subgridVOffset, ubAgentData, ubEnvModelVar, v_gridPhi[ pdeIdx ], v_gridRHS[ pdeIdx ] );

  return;
}

void BioModel::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, Vector<S32>& v_finestLevel/* [pdeIdx] */ ) const {

  if( mSolutes.size( ) > 0 ) {
    v_finestLevel.resize( mSolutes.size( ) ); /* coarsest level */
    S32 i;
    for( i = 0 ; i < (S32) mSolutes.size(); i++ ) {
      mSolutes[ i ]->updateIfGridAMRTags( vIdx, nbrUBAgentData, nbrUBEnv, v_finestLevel[ i ] );
    }
  } else {
    v_finestLevel.clear( );
  }

  return;
}

void BioModel::updateIfGridDirichletBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {

  mSolutes[ elemIdx ]->updateIfGridDirichletBCVal( pos, dim, lowSide, a_ubEnvModelVar, av_gridPhi, bcVal );

  return;
}

void BioModel::updateIfGridNeumannBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {

  mSolutes[ elemIdx ]->updateIfGridNeumannBCVal( pos, dim, lowSide, a_ubEnvModelVar, av_gridPhi, bcVal );

  return;
}

void BioModel::initPDEBufferPhi( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) const {

  mSolutes[ pdeIdx ]->initPDEBufferPhi( startVIdx, pdeBufferBoxSize, v_gridPhi );

  return;
}

void BioModel::initPDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {/* relevant only if v_gridPhiOutputDivideByKappa[pdeIdx] is set to true in updateFileOutputInfo() */

  mSolutes[ pdeIdx ]->initPDEBufferKappa( startVIdx, pdeBufferBoxSize, gridKappa );

  return;
}

void BioModel::updatePDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {

  mSolutes[ pdeIdx ]->updatePDEBufferKappa( startVIdx, pdeBufferBoxSize, gridKappa );

  return;
}

void BioModel::updatePDEBufferAlpha( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) const {

  mSolutes[ elemIdx ]->updatePDEBufferAlpha( startVIdx, pdeBufferBoxSize, gridAlpha );

  return;
}

void BioModel::updatePDEBufferBetaInPDEBufferRegion( const S32 elemIdx, const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {

  mSolutes[ elemIdx ]->updatePDEBufferBetaInPDEBufferRegion( dim, startVIdx0, startVIdx1, pdeBufferBoxSize, gridBeta );

  return;
}

void BioModel::updatePDEBufferBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {

  mSolutes[ elemIdx ]->updatePDEBufferBetaDomainBdry( dim, startVIdx, pdeBufferBoxSize, gridBeta );

  return;
}

void BioModel::updatePDEBufferRHSLinear( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {

  mSolutes[ elemIdx ]->updatePDEBufferRHSLinear( startVIdx, pdeBufferBoxSize, gridPhi, gridRHS );

  return;
}

void BioModel::updatePDEBufferRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const {

  mSolutes[ pdeIdx ]->updatePDEBufferRHSTimeDependentSplitting( startVIdx, pdeBufferBoxSize, v_gridPhi, v_gridRHS );

  return;
}

void BioModel::updatePDEBufferDirichletBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {

  mSolutes[ elemIdx ]->updatePDEBufferDirichletBCVal( startPos, pdeBufferFaceSize, dim, lowSide, bcVal );

  return;
}

void BioModel::updatePDEBufferNeumannBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {

  mSolutes[ elemIdx ]->updatePDEBufferNeumannBCVal( startPos, pdeBufferFaceSize, dim, lowSide, bcVal );

  return;
}



void initializeBioModel() {
  if( gBioModelInitialized ) {
    return;
  }
  // AGENT SPECIES AUTO BEGIN
  // AGENT SPECIES AUTO END

  S32 i, j;
  gMechIntrctShoveEnabled.resize( NUM_AGENT_SPECIES ); 
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    gMechIntrctShoveEnabled[ i ].resize( NUM_AGENT_SPECIES );
    for( j = 0 ; j < NUM_AGENT_SPECIES ; j++ ) {
      // FIXME: Should control with a <shoves> field, like tightJunctions and adhesions
      gMechIntrctShoveEnabled[ i ][ j ] = true;
    }
  }
  gMechIntrctSpAgent.push_back( MechIntrctSpAgentShove::create() );
  gMechIntrctSpAgent.push_back( MechIntrctSpAgentAdhesion::create() );
  if( gBioModel->getDistanceJunctionsEnabled( ) ) {
    gMechIntrctSpAgent.push_back( MechIntrctSpAgentDistanceJunction::create() );
  }

  gBioModelInitialized = true;
}

void terminateBioModel() {
  if( !gBioModelInitialized ) {
    return;
  }

  S32 i;

  if( gSimulator ) {
    delete gSimulator;
    gSimulator = 0;
  }

  if( gAgentGrid ) {
    delete gAgentGrid;
    gAgentGrid = 0;
  }

  for( i = 0; i < (S32) gAgentSpecies.size(); i++ ) {
    if( gAgentSpecies[i] ) {
      delete gAgentSpecies[i];
      gAgentSpecies[i] = 0;
    }
  }
  gAgentSpecies.clear();

  for( i = 0; i < (S32) gParticles.size(); i++ ) {
    if( gParticles[i] ) {
      delete gParticles[i];
      gParticles[i] = 0;
    }
  }
  gParticles.clear();
  
  for( i = 0; i < (S32) gMechIntrctSpAgent.size(); i++ ) {
    if( gMechIntrctSpAgent[i] ) {
      delete gMechIntrctSpAgent[i];
      gMechIntrctSpAgent[i] = 0;
    }
  }
  gMechIntrctSpAgent.clear();

  if( gBioModel ) {
    delete gBioModel;
    gBioModel = 0;
  }

  gBioModelInitialized = false;
}

