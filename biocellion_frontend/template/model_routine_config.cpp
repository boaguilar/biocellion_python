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

void ModelRoutine::updateIfGridSpacing( REAL& ifGridSpacing ) {
  /* MODEL START */
  
  initializeBioModel();
  ifGridSpacing = gBioModel->getAgentGrid().getResolution( );/* sets the grid resolution */

  /* MODEL END */

  return;
}

void ModelRoutine::updateOptModelRoutineCallInfo( OptModelRoutineCallInfo& callInfo ) {
  /* MODEL START */

  // FIXME: Not controlled by XML yet
  callInfo.numComputeMechIntrctIters = 1;
  callInfo.numUpdateIfGridVarPreStateAndGridStepIters = 1;
  callInfo.numUpdateIfGridVarPostStateAndGridStepIters = 1;

  /* MODEL END */

  return;
}

void ModelRoutine::updateDomainBdryType( domain_bdry_type_e a_domainBdryType[DIMENSION] ) {
  /* MODEL START */

  initializeBioModel();
  gBioModel->updateDomainBdryType( a_domainBdryType );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferBdryType( pde_buffer_bdry_type_e& pdeBufferBdryType ) {
  /* MODEL START */

  // FIXME: Not controlled by XML yet
  pdeBufferBdryType = PDE_BUFFER_BDRY_TYPE_HARD_WALL;/* relevant to agents only ( not a PDE boundary condition), agents cannot penetrate into the PDE buffer region */

  /* MODEL END */

  return;
}

void ModelRoutine::updateTimeStepInfo( TimeStepInfo& timeStepInfo ) {
  /* MODEL START */

  initializeBioModel();
  gBioModel->updateTimeStepInfo( timeStepInfo );

  /* MODEL END */

  return;
}

void ModelRoutine::updateSyncMethod( sync_method_e& mechIntrctSyncMethod, sync_method_e& updateIfGridVarSyncMethod/* dummy if both callUpdateIfGridVarPreStateAndGridStep and callUpdateIfGridVarPostStateAndGridStep are set to false in ModelRoutine::updateOptModelRoutineCallInfo */ ) {
  /* MODEL START */

  mechIntrctSyncMethod = SYNC_METHOD_PER_ATTR;
  updateIfGridVarSyncMethod = SYNC_METHOD_PER_ATTR;

  /* MODEL END */

  return;
}

#if HAS_SPAGENT
void ModelRoutine::updateSpAgentInfo( Vector<SpAgentInfo>& v_spAgentInfo ) {/* set the mechanical interaction range & the numbers of model specific variables */
  /* MODEL START */
  
  initializeBioModel();

  v_spAgentInfo.resize( gBioModel->getAgentSpecies().size() );
  S32 i;
  for( i = 0; i < (S32) gBioModel->getAgentSpecies().size(); i++ ) {
    MechModelVarInfo mechModelVarInfo;
    mechModelVarInfo.syncMethod = VAR_SYNC_METHOD_DELTA;
    SpAgentInfo info;
    ODENetInfo odeNetInfo;
    odeNetInfo.numVars = gBioModel->getAgentSpecies()[ i ]->getNumODEVariables( );
    odeNetInfo.stiff = ODE_STIFF_NORMAL;
    odeNetInfo.h = 0.1;
    odeNetInfo.hm = 0.01;
    odeNetInfo.epsilon = 1e-6;
    odeNetInfo.threshold = 1e-3;
    odeNetInfo.errorThresholdVal = 0.0;
    odeNetInfo.warningThresholdVal = 0.0;
    odeNetInfo.setNegToZero = false;
  
    // FIXME: DMax not controlled from XML yet
    info.dMax = gBioModel->getAgentSpecies()[ i ]->getDMax();
    CHECK( info.dMax <= gBioModel->getAgentGrid().getResolution( ) );
    // FIXME: num*ModelVars not controlled from XML yet
    info.numBoolVars = gBioModel->getAgentSpecies()[ i ]->getNumModelBools();
    info.numStateModelReals = gBioModel->getAgentSpecies()[ i ]->getNumModelReals();
    info.numStateModelInts = gBioModel->getAgentSpecies()[ i ]->getNumModelInts();
    if( gBioModel->getAgentSpecies()[ i ]->getNumMechModelReals() > 0 ) {
      info.v_mechIntrctModelRealInfo.assign( gBioModel->getAgentSpecies()[ i ]->getNumMechModelReals(), mechModelVarInfo );
    } else {
      info.v_mechIntrctModelRealInfo.clear();
    }
    if( gBioModel->getAgentSpecies()[ i ]->getNumMechModelInts() > 0 ) {
      info.v_mechIntrctModelIntInfo.assign( gBioModel->getAgentSpecies()[ i ]->getNumMechModelInts(), mechModelVarInfo );
    } else {
      info.v_mechIntrctModelIntInfo.clear();
    }
    if( gBioModel->getAgentSpecies()[ i ]->getNumODEVariables() > 0 ) {
      info.v_odeNetInfo.assign( 1, odeNetInfo );
    } else {
      info.v_odeNetInfo.clear();
    }
    v_spAgentInfo[i] = info;
  }

  /* MODEL END */

  return;
}
#endif

void ModelRoutine::updateJunctionEndInfo( Vector<JunctionEndInfo>& v_junctionEndInfo ) {/* set the numbers of model specific variables */
  /* MODEL START */

  initializeBioModel();
  if( gBioModel->getDistanceJunctionsEnabled( ) ) {
    v_junctionEndInfo.resize( NUM_JUNCTION_TYPES );
    v_junctionEndInfo[JUNCTION_TYPE_DISTANCE].numModelReals = NUM_JUNCTION_REAL_TYPES;
    v_junctionEndInfo[JUNCTION_TYPE_DISTANCE].numModelInts = NUM_JUNCTION_INT_TYPES;
  } else {
    v_junctionEndInfo.clear(  );
  }

  /* MODEL END */

  return;
}

void ModelRoutine::updatePhiPDEInfo( Vector<PDEInfo>& v_phiPDEInfo ) {
  /* MODEL START */

  initializeBioModel();
  gBioModel->updatePhiPDEInfo( v_phiPDEInfo );

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfGridModelVarInfo( Vector<IfGridModelVarInfo>& v_ifGridModelRealInfo, Vector<IfGridModelVarInfo>& v_ifGridModelIntInfo ) {
  /* MODEL START */

  // FIXME: grid vars not controlled from XML yet

  IfGridModelVarInfo info;
  
  v_ifGridModelRealInfo.clear( );
#if USE_PHI_ONE
  v_ifGridModelRealInfo.resize( NUM_GRID_MODEL_REALS);
  
#if USE_SECRETION
  info.name = "phi_one_rhs";
  info.syncMethod = VAR_SYNC_METHOD_DELTA;/* updateIfGridVar */
  v_ifGridModelRealInfo[GRID_MODEL_REAL_PHI_ONE_RHS] = info;
#endif
#endif  
  v_ifGridModelIntInfo.clear();

  /* MODEL END */

  return;
}

void ModelRoutine::updateRNGInfo( Vector<RNGInfo>& v_rngInfo ) {
  /* MODEL START */

  CHECK( NUM_MODEL_RNGS == 2 );

  RNGInfo rngInfo;

  rngInfo.type = RNG_TYPE_UNIFORM;
  rngInfo.param0 = 0.0;
  rngInfo.param1 = 1.0;
  rngInfo.param2 = 0.0;/* dummy */

  v_rngInfo.push_back( rngInfo );

  rngInfo.type = RNG_TYPE_GAUSSIAN;
  rngInfo.param0 = 0.0;
  rngInfo.param1 = 1.0;
  rngInfo.param2 = 0.0;/* dummy */

  v_rngInfo.push_back( rngInfo );
  
  /* MODEL END */

  return;
}

void ModelRoutine::updateFileOutputInfo( FileOutputInfo& fileOutputInfo ) {
  /* MODEL START */

  initializeBioModel();
  gBioModel->updateFileOutputInfo( fileOutputInfo );

  /* MODEL END */

  return;
}

void ModelRoutine::updateSummaryOutputInfo( Vector<SummaryOutputInfo>& v_summaryOutputRealInfo, Vector<SummaryOutputInfo>& v_summaryOutputIntInfo ) {
  /* MODEL START */

  // FIXME: summary not controlled from XML yet
  v_summaryOutputRealInfo.clear();
  v_summaryOutputIntInfo.clear();

  /* MODEL END */

  return;
}

void ModelRoutine::initGlobal( Vector<U8>& v_globalData ) {/* called once per simulation */
  /* MODEL START */

  terminateBioModel();

  /* MODEL END */

  return;
}

void ModelRoutine::init( void ) {/* called once per (MPI) process */
  /* MODEL START */
  
  initializeBioModel();

  /* MODEL END */

  return;
}

void ModelRoutine::term( void ) {/* called once per (MPI) process */
  /* MODEL START */

  terminateBioModel();

  /* MODEL END */

  return;
}

void ModelRoutine::setPDEBuffer( const VIdx& startVIdx, const VIdx& regionSize, BOOL& isPDEBuffer ) {
  /* MODEL START */

  isPDEBuffer = false;

  /* MODEL END */

  return;
}

void ModelRoutine::setHabitable( const VIdx& vIdx, BOOL& isHabitable ) {
  /* MODEL START */

  isHabitable = true;

  /* MODEL END */

  return;
}

