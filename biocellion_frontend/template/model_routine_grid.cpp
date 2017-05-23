/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

/* DO NOT USE FUNCTIONS THAT ARE NOT THREAD SAFE (e.g. rand(), use Util::getModelRand() instead) */

#include "biocellion.h"

#include "model_routine.h"

#include "model_define.h"

using namespace std;

void ModelRoutine::initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) {
  /* MODEL START */

#if USE_PHI_ONE
	for( S32 elemIdx = 0 ; elemIdx < NUM_GRID_PHIS ; elemIdx++ ) {
		ubEnv.setPhi( elemIdx, PHI_INIT_VAL[elemIdx] );
	}
  
  for( S32 elemIdx = 0 ; elemIdx < NUM_GRID_MODEL_REALS ; elemIdx++ ) {
		ubEnv.setModelReal( elemIdx, 0.0 );
	}
#endif
  /* MODEL END */

  return;
}

void ModelRoutine::initIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) {/* relevant only if v_gridPhiOutputDivideByKappa[pdeIdx] is set to true in updateFileOutputInfo() */
  /* MODEL START */

  gridKappa = 1.0;

  /* MODEL END */

  return;
}

#if USE_PHI_CENTER_DROPPER
static inline void center_dropper( const VIdx& vIdx, const S32 elemIdx, NbrUBEnv& nbrUBEnv ) {
  
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {
    idx_t mid = Info::getDomainSize(dim) / 2;
    if(mid != vIdx[dim]) {
      // we are not the middle
      return;
    }
  }
  
  REAL phi = nbrUBEnv.getPhi( 0, 0, 0, elemIdx );
  phi += PHI_DROPPER_CONCENTRATION[elemIdx];
  nbrUBEnv.setPhi( 0, 0, 0, elemIdx, phi );
}
#else
static inline void center_dropper( const VIdx& vIdx, const S32 elemIdx, NbrUBEnv& nbrUBEnv ) {
}
#endif

void ModelRoutine::updateIfGridVar( const BOOL pre, const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) {
  /* MODEL START */

  if (pre) {
    // PRE Iteration
#if USE_PHI_ONE
    center_dropper( vIdx, GRID_PHI_ONE_TYPE, nbrUBEnv );

#if USE_SECRETION

    REAL rhss[3][3][3][NUM_GRID_PHIS];
    S32 i, j, k, elemIdx, m;
    for(i = -1; i < 2; i++) {
      for(j = -1; j < 2; j++) {
        for(k = -1; k < 2; k++) {
          for(elemIdx = 0; elemIdx < NUM_GRID_PHIS; elemIdx++) {
            rhss[i+1][j+1][k+1][elemIdx] = 0.0;
          }
        }
      }
    }

		const UBAgentData& ubAgentData = *( nbrUBAgentData.getConstPtr( 0, 0, 0 ) );
    for( m = 0 ; m < ( S32 )ubAgentData.v_spAgent.size() ; m++ ) {
      const SpAgent& spAgent = ubAgentData.v_spAgent[m];
      agentType_t type = spAgent.state.getType();
      S32 maxLevel = 4; // 0-7  higher is better approximation, lower is faster to calculate
      REAL aaa_ratio[3][3][3];
      Util::computeSphereUBVolOvlpRatio(maxLevel, spAgent.vOffset, spAgent.state.getRadius(), aaa_ratio);

      for(elemIdx = 0; elemIdx < NUM_GRID_PHIS; elemIdx++) {
        for(i = -1; i < 2; i++) {
          for(j = -1; j < 2; j++) {
            for(k = -1; k < 2; k++) {
              if(aaa_ratio[i+1][j+1][k+1] > 0.0) {
                rhss[i+1][j+1][k+1][elemIdx] += PHI_CELL_SECRETION_RATE[elemIdx][type] * aaa_ratio[i+1][j+1][k+1];
              }
            }
          }
        }
      }
    }
    for(elemIdx = 0; elemIdx < NUM_GRID_PHIS; elemIdx++) {
      S32 idx = PHI_IDX_TO_RHS_IDX[elemIdx];
      if( idx >= 0 ) {
        for(i = -1; i < 2; i++) {
          for(j = -1; j < 2; j++) {
            for(k = -1; k < 2; k++) {
              if( nbrUBEnv.getValidFlag(i, j, k) ) {
                if(rhss[i+1][j+1][k+1][elemIdx] > 0.0) {
                  nbrUBEnv.setModelReal( i, j, k, idx, rhss[i+1][j+1][k+1][elemIdx] );
                }
              }
            }
          }
        }
      }
    }
    
#endif
    
#endif
  } else {
    // POST Iteration
    
#if USE_PHI_ONE
#if USE_SECRETION
    // reset to 0.0 for DELTA variables
    S32 elemIdx;
    for(elemIdx = 0; elemIdx < NUM_GRID_PHIS; elemIdx++) {
      S32 idx = PHI_IDX_TO_RHS_IDX[elemIdx];
      nbrUBEnv.setModelReal( 0, 0, 0, idx, 0.0 );
    }
#endif
#endif
  }
  
  /* MODEL END */
  
  return;
}

void ModelRoutine::updateIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) {
  /* MODEL START */

  gridKappa = 1.0;

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridAlpha( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) {
  /* MODEL START */

#if USE_PHI_ONE
	gridAlpha = PHI_DECAY_RATE[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridBetaInIfRegion( const S32 elemIdx, const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) {
  /* MODEL START */

#if USE_PHI_ONE
  gridBeta = PHI_DIFFUSION_COEFF[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridBetaPDEBufferBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) {
  /* MODEL START */

#if USE_PHI_ONE
  gridBeta = PHI_DIFFUSION_COEFF[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) {
  /* MODEL START */

#if USE_PHI_ONE
	gridBeta = 0.0;/* zero flux at domain boundary */
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridRHSLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) {
  /* MODEL START */

  //OUTPUT(5, "subgridVOffset: " << subgridVOffset[0] << " " << subgridVOffset[1] << " " << subgridVOffset[2])
  
#if USE_PHI_ONE
#if USE_SECRETION
  S32 j = PHI_IDX_TO_RHS_IDX[elemIdx];
  if( j >= 0 ) {
    gridRHS = ubEnv.getModelReal( GRID_MODEL_REAL_PHI_ONE_RHS );
    if(gridRHS < 0.0) {
      ERROR("vIdx: " << (vIdx[0]) << "," << (vIdx[1]) << "," << (vIdx[2])   << " error:  gridRHS: " << gridRHS);
    }
  } else {
    gridRHS = 0.0;
  }
#else
  gridRHS = 0.0;
#endif
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::adjustIfSubgridRHSTimeDependentLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) {
  /* MODEL START */

#if USE_PHI_ONE
#if USE_SECRETION
  S32 j = PHI_IDX_TO_RHS_IDX[elemIdx];
  if( j >= 0 ) {
    gridRHS = ubEnvModelVar.getModelReal( GRID_MODEL_REAL_PHI_ONE_RHS );
  } else {
    gridRHS = 0.0;
  }
#else
  gridRHS = 0.0;
#endif
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfSubgridRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) {/* for Wnt & SFRP */
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, Vector<S32>& v_finestLevel/* [pdeIdx] */ ) {
  /* MODEL START */

#if USE_PHI_ONE
	v_finestLevel.assign( NUM_GRID_PHIS, 0 );/* coarsest level */
  for( S32 i = 0; i < NUM_GRID_PHIS; i++ ) {
    v_finestLevel[i] = PHI_AMR_LEVELS[i] - 1;
  }
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfGridDirichletBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updateIfGridNeumannBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::initPDEBufferPhi( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::initPDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) {/* relevant only if v_gridPhiOutputDivideByKappa[pdeIdx] is set to true in updateFileOutputInfo() */
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferAlpha( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) {
  /* MODEL START */

#if USE_PHI_ONE
	gridAlpha = PHI_DECAY_RATE[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferBetaInPDEBufferRegion( const S32 elemIdx, const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) {
  /* MODEL START */

#if USE_PHI_ONE
  gridBeta = PHI_DIFFUSION_COEFF[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) {
  /* MODEL START */

#if USE_PHI_ONE
  gridBeta = PHI_DIFFUSION_COEFF[elemIdx];
#else
  ERROR( "unimplmented." );
#endif

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferRHSLinear( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) {
  /* MODEL START */

  ERROR( "unimplemented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferDirichletBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) {
  /* MODEL START */

  ERROR( "unimplmented." );

  /* MODEL END */

  return;
}

void ModelRoutine::updatePDEBufferNeumannBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) {
  /* MODEL START */

  ERROR( "unimplmented." );

  /* MODEL END */

  return;
}

