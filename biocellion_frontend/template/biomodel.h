#ifndef _BIOMODEL_H_
#define _BIOMODEL_H_

#include "biocellion.h"
#include "simulator.h"
#include "agent_grid.h"
#include "agent_species.h"
#include "particle.h"
#include "model_mechanisms.h"
#include "adhesion.h"
#include "tight_junction.h"

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

#endif /* _BIOMODEL_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
