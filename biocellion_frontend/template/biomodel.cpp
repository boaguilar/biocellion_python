#include "biomodel.h"

static bool gBioModelInitialized = false;
BioModel *gBioModel = 0;
Simulator *gSimulator = 0;
AgentGrid *gAgentGrid = 0;
Vector<AgentSpecies *> gAgentSpecies;
Vector<Solute *> gSolutes;
Vector<Particle *> gParticles;
Vector<MechIntrctSpAgent *> gMechIntrctSpAgent;
Vector< Vector<BOOL> > gMechIntrctShoveEnabled;

BioModel::BioModel( )
  : mDistanceJunctionsEnabled( false )
{
}

BOOL BioModel::getDistanceJunctionsEnabled( ) const {
  return mDistanceJunctionsEnabled;
}

void BioModel::setDistanceJunctionsEnabled( const BOOL& value ) {
  mDistanceJunctionsEnabled = value;
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

  for( i = 0; i < (S32) gSolutes.size(); i++ ) {
    if( gSolutes[i] ) {
      delete gSolutes[i];
      gSolutes[i] = 0;
    }
  }
  gSolutes.clear();
  
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

