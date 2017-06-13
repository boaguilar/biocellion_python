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
    current_time = gBioModel->getSimulator().getAgentTimeStep() * Info::getCurBaselineTimeStep();
  }
  size_t i, j;
  for( i = 0 ; i < gBioModel->getAgentSpecies().size() ; i++ ) {
    for( j = 0 ; j < gBioModel->getAgentSpecies()[ i ]->getInitAreas( ).size() ; j++ ) {
      REAL birth_time = gBioModel->getAgentSpecies()[ i ]->getInitAreas( )[ j ]->getBirthday( );
      REAL dt = std::abs( birth_time - current_time );
      if(( init && birth_time < gBioModel->getSimulator().getAgentTimeStep() / 2.0 ) ||
         ( !init && birth_time > gBioModel->getSimulator().getAgentTimeStep() / 2.0 && dt < gBioModel->getSimulator().getAgentTimeStep() / 2.0 ) ) {
        gBioModel->getAgentSpecies()[ i ]->getInitAreas( )[ j ]->addSpAgents( init, startVIdx, regionSize, ifGridHabitableBoxData, v_spAgentVIdx, v_spAgentState, v_spAgentOffset );
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

  gBioModel->updateSpAgentState(vIdx, junctionData, vOffset, nbrUBEnv, state);

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

  gBioModelRW->updateSpAgentBirthDeath( vIdx, spAgent, mechIntrctData, nbrUBEnv, divide, disappear);
  /* MODEL END */

  return;
}


static inline void brownianMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) {
  for( S32 dim = 0 ; dim < 3 ; dim++ ) { // FIXME:  Use computation domain grid for dimension
    if( dim == 3-1 ) { 
      continue; 
    }
    disp[dim] += state.getRadius() * ( Util::getModelRand( MODEL_RNG_GAUSSIAN ) ) * gBioModel->getAgentSpecies()[ state.getType() ]->getParamReal( gBioModel->getAgentSpecies()[ state.getType() ]->getIdxReal( SPECIES_brownianScale ) );
  }
}

static inline void limitMotion(VReal& disp) {
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {/* limit the maximum displacement within a single time step */
#if TWO_DIMENSION
    if(dim == DIMENSION-1) { disp[dim] = 0.0; continue; }
#endif
    if( disp[dim] > gBioModel->getAgentGrid().getResolution( ) ) {
      disp[dim] = gBioModel->getAgentGrid().getResolution( );
    }
    else if( disp[dim] < ( gBioModel->getAgentGrid().getResolution( ) * -1.0 ) ) {
      disp[dim] = gBioModel->getAgentGrid().getResolution( ) * -1.0;
    }
  }
}

void ModelRoutine::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) {/* if not dividing or disappearing */
  /* MODEL START */
  S32 agentType = state.getType( );
  disp[0] = mechIntrctData.getModelReal( gBioModel->getAgentSpecies()[ agentType ]->getIdxMechForceRealX() );
  disp[1] = mechIntrctData.getModelReal( gBioModel->getAgentSpecies()[ agentType ]->getIdxMechForceRealY() );
  disp[2] = mechIntrctData.getModelReal( gBioModel->getAgentSpecies()[ agentType ]->getIdxMechForceRealZ() );
  
  brownianMotion( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  gBioModel->getAgentSpecies()[ agentType ]->adjustSpAgent( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  limitMotion(disp);
  
  /* MODEL END */

  return;
}

void ModelRoutine::divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd ) {
  /* MODEL START */

  gBioModelRW->divideSpAgent( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, motherState, motherDisp, daughterState, daughterDisp, v_junctionDivide, motherDaughterLinked, motherEnd, daughterEnd);
  
  /* MODEL END */

  return;
}
#endif

