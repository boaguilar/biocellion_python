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

class BioModel {
public:
  BioModel( );
  BOOL getDistanceJunctionsEnabled( ) const;
  void setDistanceJunctionsEnabled( const BOOL& value );
protected:
  BOOL mDistanceJunctionsEnabled;
};

extern BioModel *gBioModel;
extern Simulator *gSimulator;
extern AgentGrid *gAgentGrid;
extern Vector<AgentSpecies *> gAgentSpecies;
extern Vector<Solute *> gSolutes;
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
