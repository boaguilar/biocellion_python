#ifndef _BIOMODEL_H_
#define _BIOMODEL_H_

#include "biocellion.h"
#include "agent_species.h"
#include "particle.h"
#include "model_mechanisms.h"

extern Vector<AgentSpecies *> gAgentSpecies;
extern Vector<Particle *> gParticles;
extern Vector<MechIntrctSpAgent *> gMechIntrctSpAgent;

extern void initializeBioModel();
extern void terminateBioModel();

// AGENT SPECIES AUTO BEGIN
// AGENT SPECIES AUTO END

#endif /* _BIOMODEL_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
