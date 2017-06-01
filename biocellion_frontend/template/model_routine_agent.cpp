/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

/* DO NOT USE FUNCTIONS THAT ARE NOT THREAD SAFE (e.g. rand(), use Util::getModelRand() instead) */

#include "biocellion.h"
#include "biomodel.h"
#include "model_routine.h"

/* MODEL START */

#include "model_define.h"
#include <cmath>

/* MODEL END */

using namespace std;

#if HAS_SPAGENT

void ModelRoutine::addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) {/* initialization */
  /* MODEL START */
  REAL current_time;
  if( init ) {
    current_time = 0;
  } else {
    current_time = gSimulator->getAgentTimeStep() * Info::getCurBaselineTimeStep();
  }
  size_t i, j;
  for( i = 0 ; i < gAgentSpecies.size() ; i++ ) {
    for( j = 0 ; j < gAgentSpecies[ i ]->getInitAreas( ).size() ; j++ ) {
      REAL birth_time = gAgentSpecies[ i ]->getInitAreas( )[ j ]->getBirthday( );
      REAL dt = std::abs( birth_time - current_time );
      if(( init && birth_time < gSimulator->getAgentTimeStep() / 2.0 ) ||
         ( !init && birth_time > gSimulator->getAgentTimeStep() / 2.0 && dt < gSimulator->getAgentTimeStep() / 2.0 ) ) {
        gAgentSpecies[ i ]->getInitAreas( )[ j ]->addSpAgents( init, startVIdx, regionSize, ifGridHabitableBoxData, v_spAgentVIdx, v_spAgentState, v_spAgentOffset );
      }
    }
  }

  /* MODEL END */

  return;
}

void ModelRoutine::spAgentCRNODERHS( const S32 odeNetIdx, const VIdx& vIdx, const SpAgent& spAgent, const NbrUBEnv& nbrUBEnv, const Vector<double>& v_y, Vector<double>& v_f ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */ ) {
  /* MODEL START */

  /* nothing to do */

  /* MODEL END */

  return;
}

void ModelRoutine::spAgentSecretionBySpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentDisp ) {
  /* MODEL START */

  /* nothing to do */

  /* MODEL END */

  return;
}

void ModelRoutine::updateSpAgentBirthDeath( const VIdx& vIdx, const SpAgent& spAgent, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, BOOL& divide, BOOL& disappear ) {
  /* MODEL START */

  divide = false;
  disappear = false;

  /* MODEL END */

  return;
}


static inline void brownianMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) {
  for( S32 dim = 0 ; dim < 3 ; dim++ ) { // FIXME:  Use computation domain grid for dimension
    if( dim == 3-1 ) { 
      continue; 
    }
    disp[dim] += state.getRadius() * ( Util::getModelRand( MODEL_RNG_GAUSSIAN ) ) * gAgentSpecies[ state.getType() ]->getParamReal( gAgentSpecies[ state.getType() ]->getIdxReal( SPECIES_brownianScale ) );
  }
}

static inline void limitMotion(VReal& disp) {
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {/* limit the maximum displacement within a single time step */
#if TWO_DIMENSION
    if(dim == DIMENSION-1) { disp[dim] = 0.0; continue; }
#endif
    if( disp[dim] > gAgentGrid->getResolution( ) ) {
      disp[dim] = gAgentGrid->getResolution( );
    }
    else if( disp[dim] < ( gAgentGrid->getResolution( ) * -1.0 ) ) {
      disp[dim] = gAgentGrid->getResolution( ) * -1.0;
    }
  }
}

#if USE_CHEMOTAXIS

/*
 * Find which interface of the unit box would be reached first,
 * given the starting point and direction.
 */
static inline S32 findNearestInterface(const VReal& vOffset, const VReal& vDir) {
  S32 interface = -1;
  REAL tNearest = 1.0e10;
  for (S32 dim = 0; dim < DIMENSION; dim++) {
    if (fabs(vDir[dim]) > 0.0001) {
      REAL t = ((vDir[dim] > 0 ? 0.5 : -0.5) - vOffset[dim] / IF_GRID_SPACING) / vDir[dim];
      if (t < tNearest) {
        interface = dim;
        tNearest = t;
      }
    }
  }
  return interface;
}

/*
 * replace contents of vUnit with a unit vector
 */
static inline void randomUnitVector( VReal& vUnit ) {
  const REAL epsilon = 0.1;

  vUnit = VReal::ZERO;
  while(vUnit.length() < epsilon) {
    for (S32 dim = 0; dim < DIMENSION; dim++) {
#if TWO_DIMENSION
      if( dim == DIMENSION-1 ) {
        vUnit[dim] = 0.0;
        continue;
      }
#endif
      vUnit[dim] = -0.5 + Util::getModelRand(MODEL_RNG_UNIFORM);
    }
    vUnit = VReal::normalize(epsilon, vUnit);
  }

}


static inline void findChemoTaxisDirectionAndConcentration( const S32 elemIdx,  const VReal& vOffset, const NbrUBEnv& nbrUBEnv, VReal& dir, REAL& delta ) {

  // random direction of motion
  VReal fwdDir, bckDir;
  randomUnitVector( fwdDir );
  bckDir = VReal::ZERO - fwdDir;
  
  S32 fwdInt = findNearestInterface(vOffset, fwdDir); // nearest forward interface
  S32 bckInt = findNearestInterface(vOffset, bckDir); // nearest backward interface
  
  // unit box offset indexes for relevant boxes
  VIdx curIdx; // self
  VIdx fwdIdx; // forward neighbor 
  VIdx bckIdx; // backward neighbor
  for (S32 dim = 0; dim < DIMENSION; dim++) {
    curIdx[dim] = 0;
    fwdIdx[dim] = dim == fwdInt ? (fwdDir[fwdInt] > 0 ? 1 : -1) : 0;
    bckIdx[dim] = dim == bckInt ? (bckDir[bckInt] > 0 ? 1 : -1) : 0;
  }
  
  REAL curVal = nbrUBEnv.getPhi( curIdx, elemIdx );
  REAL fwdVal = curVal;
  REAL bckVal = curVal;

  if (nbrUBEnv.getValidFlag(fwdIdx)) {
    fwdVal = nbrUBEnv.getPhi( fwdIdx, elemIdx );
  }
  /*
  if (nbrUBEnv.getValidFlag(bckIdx)) {
    bckVal = nbrUBEnv.getPhi( bckIdx, elemIdx );
  }
  */

  // this scaling reduces the sharpness of the chemotactic pull
  // makes it about the relative difference, not the absolute difference
  REAL alpha = 0.1; // number borrowed from idynomics implementation
  fwdVal = fwdVal / (1.0 + alpha * fwdVal);
  bckVal = bckVal / (1.0 + alpha * bckVal);

  delta = fwdVal - bckVal;
  dir = fwdDir;
}

static inline void chemoTaxisMotion( const S32 elemIdx, const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) {
  
  VReal dir;
  REAL delta;
  S32 agentType = state.getType();

#if USE_CHEMOTAXIS_CONTACT_INHIBITION
  if(junctionData.getNumJunctions() >= CHEMOTAXIS_CONTACT_INHIBITION_THRESHOLD[elemIdx][agentType]) {
    return;
  }
#endif
  
  findChemoTaxisDirectionAndConcentration( elemIdx,  vOffset, nbrUBEnv, dir, delta );


  VReal chemDisp = dir * (PHI_CELL_ATTRACTION_SCALE[elemIdx][agentType] * delta);
  disp += chemDisp;
}

static inline void allChemoTaxisMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) {
  S32 agentType = state.getType();
  for(S32 elemIdx = 0; elemIdx < NUM_GRID_PHIS; elemIdx++) {
    if( PHI_CELL_ATTRACTION_SCALE[elemIdx][agentType] != 0.0 ) {
      chemoTaxisMotion(elemIdx, vIdx, junctionData, vOffset, nbrUBEnv, state, disp);
    }
  }
}

#else
static inline void allChemoTaxisMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) {
}
#endif

void ModelRoutine::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) {/* if not dividing or disappearing */
  /* MODEL START */
  S32 agentType = state.getType( );
  disp[0] = mechIntrctData.getModelReal( gAgentSpecies[ agentType ]->getIdxMechForceRealX() );
  disp[1] = mechIntrctData.getModelReal( gAgentSpecies[ agentType ]->getIdxMechForceRealY() );
#if TWO_DIMENSION
  disp[2] = 0.0;
#else
  disp[2] = mechIntrctData.getModelReal( gAgentSpecies[ agentType ]->getIdxMechForceRealZ() );
#endif
  
  brownianMotion( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  /*
  allChemoTaxisMotion(vIdx, junctionData, vOffset, nbrUBEnv, state, disp);
  */
  limitMotion(disp);
  
  /* MODEL END */

  return;
}

void ModelRoutine::divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}
#endif

