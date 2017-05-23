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
  ifGridSpacing = gAgentGrid->getResolution( );/* sets the grid resolution */

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

  CHECK( DIMENSION == 3 );

  // FIXME: Not controlled by XML yet
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {
    a_domainBdryType[dim] = DOMAIN_BDRY_TYPE_NONPERIODIC_HARD_WALL;
  }

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
  timeStepInfo.durationBaselineTimeStep = gSimulator->getTimeStep().getTimeStepMin();
  
  // FIXME: Not controlled by XML yet
  timeStepInfo.numStateAndGridTimeStepsPerBaseline = NUM_STATE_AND_GRID_TIME_STEPS_PER_BASELINE;

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

  v_spAgentInfo.resize( gAgentSpecies.size() );
  S32 i;
  for( i = 0; i < (S32) gAgentSpecies.size(); i++ ) {
    MechModelVarInfo mechModelVarInfo;
    mechModelVarInfo.syncMethod = VAR_SYNC_METHOD_DELTA;
    SpAgentInfo info;
    
    // FIXME: DMax not controlled from XML yet
    info.dMax = gAgentSpecies[ i ]->getDMax();
    CHECK( info.dMax <= gAgentGrid->getResolution( ) );
    // FIXME: num*ModelVars not controlled from XML yet
    info.numBoolVars = gAgentSpecies[ i ]->getNumModelBools();
    info.numStateModelReals = gAgentSpecies[ i ]->getNumModelReals();
    info.numStateModelInts = gAgentSpecies[ i ]->getNumModelInts();
    if( gAgentSpecies[ i ]->getNumMechModelReals() > 0 ) {
      info.v_mechIntrctModelRealInfo.assign( gAgentSpecies[ i ]->getNumMechModelReals(), mechModelVarInfo );
    } else {
      info.v_mechIntrctModelRealInfo.clear();
    }
    if( gAgentSpecies[ i ]->getNumMechModelInts() > 0 ) {
      info.v_mechIntrctModelIntInfo.assign( gAgentSpecies[ i ]->getNumMechModelInts(), mechModelVarInfo );
    } else {
      info.v_mechIntrctModelIntInfo.clear();
    }
    // FIXME: odeNetInfo not controlled from XML yet
    info.v_odeNetInfo.clear();

    v_spAgentInfo[i] = info;
  }

  /* MODEL END */

  return;
}
#endif

void ModelRoutine::updateJunctionEndInfo( Vector<JunctionEndInfo>& v_junctionEndInfo ) {/* set the numbers of model specific variables */
  /* MODEL START */

  // FIXME: junction info not controlled from XML yet
  v_junctionEndInfo.clear(  );
  //v_junctionEndInfo.resize( NUM_JUNCTION_TYPES );
  
#if USE_DISTANCE_JUNCTIONS
  v_junctionEndInfo[JUNCTION_TYPE_DISTANCE].numModelReals = 0;
  v_junctionEndInfo[JUNCTION_TYPE_DISTANCE].numModelInts = 0;
#endif

  /* MODEL END */

  return;
}

#if USE_PHI_ONE
static inline void setPhiPDEChemoattractant( Vector<PDEInfo>& v_phiPDEInfo , S32 idx ) {
  PDEInfo pdeInfo;
  GridPhiInfo gridPhiInfo;
  MGSolveInfo mgSolveInfo;
  SplittingInfo splittingInfo;

  pdeInfo.pdeIdx = idx;
  pdeInfo.pdeType = PDE_TYPE_REACTION_DIFFUSION_TIME_DEPENDENT_LINEAR;
  pdeInfo.numLevels = PHI_AMR_LEVELS[idx];
  //pdeInfo.ifLevel = PHI_AMR_LEVELS[idx]-1;
  pdeInfo.ifLevel = 0;
  pdeInfo.v_tagExpansionSize.assign( PHI_AMR_LEVELS[idx], 0 );
  pdeInfo.numTimeSteps = NUM_PDE_TIME_STEPS_PER_STATE_AND_GRID_TIME_STEP[idx];
  pdeInfo.callAdjustRHSTimeDependentLinear = false;

  // set mgSolveInfo.* here
  mgSolveInfo.numPre = 3;/* multigrid parameters */
  mgSolveInfo.numPost = 3;/* multigrid parameters */
  mgSolveInfo.numBottom = 3;/* multigrid parameters */
  mgSolveInfo.vCycle = true;/* multigrid parameters */
  mgSolveInfo.maxIters = 30;/* multigrid parameters */
  mgSolveInfo.epsilon = GRID_PHI_EPSILON;/* multigrid parameters */
  mgSolveInfo.hang = 1e-6;/* multigrid parameters */
  mgSolveInfo.normThreshold = GRID_PHI_NORM_THRESHOLD;/* multigrid parameters */
  
  pdeInfo.mgSolveInfo = mgSolveInfo;

  // set splittingInfo here
  pdeInfo.splittingInfo = splittingInfo;

  // set gridPhiInfo here
  pdeInfo.v_gridPhiInfo.resize( 1 );

  gridPhiInfo.elemIdx = idx;
  gridPhiInfo.name = GRID_PHI_NAMES[idx];
  gridPhiInfo.syncMethod = VAR_SYNC_METHOD_DELTA;
  gridPhiInfo.aa_bcType[0][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[0][0] = 0.0;
  gridPhiInfo.aa_bcType[0][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[0][1] = 0.0;
  gridPhiInfo.aa_bcType[1][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[1][0] = 0.0;
  gridPhiInfo.aa_bcType[1][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[1][1] = 0.0;
  gridPhiInfo.aa_bcType[2][0] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[2][0] = 0.0;
  gridPhiInfo.aa_bcType[2][1] = BC_TYPE_NEUMANN_CONST;
  gridPhiInfo.aa_bcVal[2][1] = 0.0;

  gridPhiInfo.errorThresholdVal = GRID_PHI_NORM_THRESHOLD * -1.0;
  gridPhiInfo.warningThresholdVal = GRID_PHI_NORM_THRESHOLD * -1.0;
  gridPhiInfo.setNegToZero = true;

  pdeInfo.v_gridPhiInfo[0] = gridPhiInfo;

  v_phiPDEInfo[idx] = pdeInfo;
}
#else
#endif

void ModelRoutine::updatePhiPDEInfo( Vector<PDEInfo>& v_phiPDEInfo ) {
  /* MODEL START */

  // FIXME: solute info not controlled from XML yet

#if USE_PHI_ONE
  CHECK( NUM_GRID_PHIS == 1 );

  v_phiPDEInfo.resize( NUM_GRID_PHIS );
  setPhiPDEChemoattractant(v_phiPDEInfo, GRID_PHI_ONE_TYPE );
#else
  CHECK( NUM_GRID_PHIS == 0 );

  v_phiPDEInfo.clear();
#endif

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

#if BROWNIAN_MOTION_ON
  CHECK( NUM_MODEL_RNGS == 2 );
#else
  CHECK( NUM_MODEL_RNGS == 1 );
#endif

  RNGInfo rngInfo;

  rngInfo.type = RNG_TYPE_UNIFORM;
  rngInfo.param0 = 0.0;
  rngInfo.param1 = 1.0;
  rngInfo.param2 = 0.0;/* dummy */

  v_rngInfo.push_back( rngInfo );

#if BROWNIAN_MOTION_ON
  rngInfo.type = RNG_TYPE_GAUSSIAN;
  rngInfo.param0 = 0.0;
  rngInfo.param1 = 1.0;
  rngInfo.param2 = 0.0;/* dummy */

  v_rngInfo.push_back( rngInfo );
#endif
  
  /* MODEL END */

  return;
}

void ModelRoutine::updateFileOutputInfo( FileOutputInfo& fileOutputInfo ) {
  /* MODEL START */

  // FIXME: output not controlled from XML yet

  fileOutputInfo.particleOutput = true;
  fileOutputInfo.v_particleExtraOutputScalarVarName.clear();
  fileOutputInfo.v_particleExtraOutputVectorVarName.clear();
#if USE_PHI_ONE
  fileOutputInfo.v_gridPhiOutput.assign( NUM_GRID_PHIS, true );
  fileOutputInfo.v_gridPhiOutputDivideByKappa.assign( NUM_GRID_PHIS, false );
#else
  fileOutputInfo.v_gridPhiOutput.clear();
  fileOutputInfo.v_gridPhiOutputDivideByKappa.clear();
#endif

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

