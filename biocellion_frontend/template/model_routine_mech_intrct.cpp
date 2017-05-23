/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

/* DO NOT USE FUNCTIONS THAT ARE NOT THREAD SAFE (e.g. rand(), use Util::getModelRand() instead) */

#include "biocellion.h"

#include "model_routine.h"

/* MODEL START */

#include "model_define.h"

/* MODEL END */

using namespace std;

#if HAS_SPAGENT
void ModelRoutine::initJunctionSpAgent( const VIdx& vIdx0, const SpAgent& spAgent0, const VIdx& vIdx1, const SpAgent& spAgent1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */
) {
  /* MODEL START */

  link = false;

  /* MODEL END */

  return;
}

#if ADHESION_ON
static inline void adhesion(const SpAgent& spAgent0, const SpAgent& spAgent1, const VReal& dir, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1) {
  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
  
  if( dist > R ) {
    REAL x = dist / R;
    REAL a = ADHESION_SCALE[spAgent0.state.getType()][spAgent1.state.getType()];
    REAL d = (ADHESION_DISTANCE[spAgent0.state.getType()] + ADHESION_DISTANCE[spAgent1.state.getType()])/2.0;
    REAL mag = -a * ( dist - R ) * exp( -1.0 * ( x - 1.0 ) * ( x - 1.0 ) / d );

#if USE_ADHESION_CONTACT_INHIBITION
    if( spAgent0.junctionData.getNumJunctions() < ADHESION_CONTACT_INHIBITION_THRESHOLD[spAgent0.state.getType()] ) {
#endif // USE_ADHESION_CONTACT_INHIBITION
      mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_X, dir[0] * mag );
      mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Y, dir[1] * mag );
#if TWO_DIMENSION
#else // TWO_DIMENSION
      mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Z, dir[2] * mag );
#endif // TWO_DIMENSION
#if USE_ADHESION_CONTACT_INHIBITION
    }
#endif // USE_ADHESION_CONTACT_INHIBITION

#if USE_ADHESION_CONTACT_INHIBITION
    if( spAgent1.junctionData.getNumJunctions() < ADHESION_CONTACT_INHIBITION_THRESHOLD[spAgent1.state.getType()] ) {
#endif // USE_ADHESION_CONTACT_INHIBITION
      mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_X, -dir[0] * mag );
      mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Y, -dir[1] * mag );
#if TWO_DIMENSION
#else // TWO_DIMENSION
      mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Z, -dir[2] * mag );
#endif // TWO_DIMENSION
#if USE_ADHESION_CONTACT_INHIBITION
    }
#endif // USE_ADHESION_CONTACT_INHIBITION
  }
}
#else // ADHESION_ON
static inline void adhesion(const SpAgent& spAgent0, const SpAgent& spAgent1, const VReal& dir, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1) {
}
#endif // ADHESION_ON


#if USE_DISTANCE_JUNCTIONS
static inline void maintain_distance_junctions( const SpAgent& spAgent0, const SpAgent& spAgent1, const REAL& dist, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) {
  
  link = false;
  unlink = false;

#if USE_TIGHT_JUNCTIONS
  if( !ENABLE_TIGHT_JUNCTION[spAgent0.state.getType()][spAgent1.state.getType()] ) {
    return;
  }
#endif // USE_TIGHT_JUNCTIONS

  REAL link_dist = JUNCTION_LINK_DISTANCE[spAgent0.state.getType()] + JUNCTION_LINK_DISTANCE[spAgent1.state.getType()];
  REAL unlink_dist = JUNCTION_UNLINK_DISTANCE[spAgent0.state.getType()] + JUNCTION_UNLINK_DISTANCE[spAgent1.state.getType()];
  if(dist <= link_dist) {
    link = true;
    end0.setType(JUNCTION_TYPE_DISTANCE);
    end1.setType(JUNCTION_TYPE_DISTANCE);
  } else if(dist > unlink_dist) {
    if(spAgent0.junctionData.isLinked( spAgent1.junctionData )) {
      unlink = true;
    }
  }
}
#else
static inline void maintain_distance_junctions( const SpAgent& spAgent0, const SpAgent& spAgent1, const REAL& dist, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) {
}
#endif


#if USE_TIGHT_JUNCTIONS
static inline void tight_junctions(const SpAgent& spAgent0, const SpAgent& spAgent1, const VReal& dir, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1) {
  if( !spAgent0.junctionData.isLinked( spAgent1.junctionData )) {
    return;
  }
  
  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
  REAL difference = dist - R;

  REAL stiffness = TIGHT_JUNCTION_STIFFNESS[spAgent0.state.getType()][spAgent1.state.getType()];
  if( difference > R || stiffness == 0.0 ) {
    return;
  }
  
  REAL mag = -0.5 * fabs(difference) * tanh(difference * stiffness);

  mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_X, dir[0] * mag );
  mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Y, dir[1] * mag );
#if TWO_DIMENSION
#else // TWO_DIMENSION
  mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Z, dir[2] * mag );
#endif // TWO_DIMENSION

  mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_X, -dir[0] * mag );
  mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Y, -dir[1] * mag );
#if TWO_DIMENSION
#else // TWO_DIMENSION
  mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Z, -dir[2] * mag );
#endif // TWO_DIMENSION
}
#else // USE_TIGHT_JUNCTIONS
static inline void tight_junctions(const SpAgent& spAgent0, const SpAgent& spAgent1, const VReal& dir, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1) {
}
#endif // USE_TIGHT_JUNCTIONS

void ModelRoutine::computeMechIntrctSpAgent( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) {
  /* MODEL START */

  S32 i;
  for(i = 0; i < ( S32 )gMechIntrctSpAgent.size(); i++) {
    gMechIntrctSpAgent[i]->compute( iter, vIdx0, spAgent0, ubEnv0, vIdx1, spAgent1, ubEnv1, dir, dist, mechIntrctData0, mechIntrctData1, link, end0, end1, unlink );
  }
  adhesion(spAgent0, spAgent1, dir, dist, mechIntrctData0, mechIntrctData1);
  maintain_distance_junctions(spAgent0, spAgent1, dist, link, end0, end1, unlink);
  tight_junctions(spAgent0, spAgent1, dir, dist, mechIntrctData0, mechIntrctData1);

  /* MODEL END */

  return;
}
#endif

