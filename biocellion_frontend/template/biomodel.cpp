#include "biomodel.h"

static bool gBioModelInitialized = false;
Vector<AgentSpecies *> gAgentSpecies;
Vector<MechIntrctSpAgent *> gMechIntrctSpAgent;

void initializeBioModel() {
  if( gBioModelInitialized ) {
    return;
  }
  // AGENT SPECIES AUTO BEGIN
  // AGENT SPECIES AUTO END

  gMechIntrctSpAgent.push_back( MechIntrctSpAgentShove::create() );

  gBioModelInitialized = true;
}

void terminateBioModel() {
  if( !gBioModelInitialized ) {
    return;
  }

  S32 i;
  
  for( i = 0; i < (S32) gAgentSpecies.size(); i++ ) {
    if( gAgentSpecies[i] ) {
      delete gAgentSpecies[i];
      gAgentSpecies[i] = 0;
    }
  }
  gAgentSpecies.clear();
  
  for( i = 0; i < (S32) gMechIntrctSpAgent.size(); i++ ) {
    if( gMechIntrctSpAgent[i] ) {
      delete gMechIntrctSpAgent[i];
      gMechIntrctSpAgent[i] = 0;
    }
  }
  gMechIntrctSpAgent.clear();

  gBioModelInitialized = false;
}

