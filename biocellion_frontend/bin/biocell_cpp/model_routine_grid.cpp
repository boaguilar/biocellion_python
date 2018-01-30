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

  gBioModel->initIfGridVar( vIdx, ubAgentData, ubEnv );

  return;
}

void ModelRoutine::initIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) {/* relevant only if v_gridPhiOutputDivideByKappa[pdeIdx] is set to true in updateFileOutputInfo() */

  gBioModel->initIfSubgridKappa( pdeIdx, vIdx, subgridVOffset, ubAgentData, ubEnv, gridKappa );

  return;
}

void ModelRoutine::updateIfGridVar( const BOOL pre, const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) {

  gBioModel->updateIfGridVar( pre, iter, vIdx, nbrUBAgentData, nbrUBEnv );
  
  return;
}

void ModelRoutine::updateIfSubgridKappa( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) {

  gBioModel->updateIfSubgridKappa( pdeIdx, vIdx, subgridVOffset, ubAgentData, ubEnv, gridKappa );

  return;
}

void ModelRoutine::updateIfSubgridAlpha( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) {

  gBioModel->updateIfSubgridAlpha( elemIdx, vIdx, subgridVOffset, ubAgentData, ubEnv, gridAlpha );

  return;
}

void ModelRoutine::updateIfSubgridBetaInIfRegion( const S32 elemIdx, const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) {

  gBioModel->updateIfSubgridBetaInIfRegion( elemIdx, dim, vIdx0, subgridVOffset0, ubAgentData0, ubEnv0, vIdx1, subgridVOffset1, ubAgentData1, ubEnv1, gridBeta );

  return;
}

void ModelRoutine::updateIfSubgridBetaPDEBufferBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) {

  gBioModel->updateIfSubgridBetaPDEBufferBdry( elemIdx, dim, vIdx, subgridVOffset, ubAgentData, ubEnv, gridBeta );

  return;
}

void ModelRoutine::updateIfSubgridBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) {

  gBioModel->updateIfSubgridBetaDomainBdry( elemIdx, dim, vIdx, subgridVOffset, ubAgentData, ubEnv, gridBeta );

  return;
}

void ModelRoutine::updateIfSubgridRHSLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) {

  gBioModel->updateIfSubgridRHSLinear( elemIdx, vIdx, subgridVOffset, ubAgentData, ubEnv, gridRHS );

  return;
}

void ModelRoutine::adjustIfSubgridRHSTimeDependentLinear( const S32 elemIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) {
  /* MODEL START */

  gBioModel->adjustIfSubgridRHSTimeDependentLinear( elemIdx, vIdx, subgridVOffset, ubAgentData, ubEnvModelVar, gridPhi, gridRHS );

  return;
}

void ModelRoutine::updateIfSubgridRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) {/* for Wnt & SFRP */

  gBioModel->updateIfSubgridRHSTimeDependentSplitting( pdeIdx, vIdx, subgridVOffset, ubAgentData, ubEnvModelVar, v_gridPhi, v_gridRHS );

  return;
}

void ModelRoutine::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, Vector<S32>& v_finestLevel/* [pdeIdx] */ ) {

  gBioModel->updateIfGridAMRTags( vIdx, nbrUBAgentData, nbrUBEnv, v_finestLevel );

  return;
}

void ModelRoutine::updateIfGridDirichletBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) {

  gBioModel->updateIfGridDirichletBCVal( elemIdx, pos, dim, lowSide, a_ubEnvModelVar, av_gridPhi, bcVal );

  return;
}

void ModelRoutine::updateIfGridNeumannBCVal( const S32 elemIdx, const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) {

  gBioModel->updateIfGridNeumannBCVal( elemIdx, pos, dim, lowSide, a_ubEnvModelVar, av_gridPhi, bcVal );

  return;
}

void ModelRoutine::initPDEBufferPhi( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) {

  gBioModel->initPDEBufferPhi( pdeIdx, startVIdx, pdeBufferBoxSize, v_gridPhi );

  return;
}

void ModelRoutine::initPDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) {/* relevant only if v_gridPhiOutputDivideByKappa[pdeIdx] is set to true in updateFileOutputInfo() */

  gBioModel->initPDEBufferKappa( pdeIdx, startVIdx, pdeBufferBoxSize, gridKappa );

  return;
}

void ModelRoutine::updatePDEBufferKappa( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) {

  gBioModel->updatePDEBufferKappa( pdeIdx, startVIdx, pdeBufferBoxSize, gridKappa );

  return;
}

void ModelRoutine::updatePDEBufferAlpha( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) {

  gBioModel->updatePDEBufferAlpha( elemIdx, startVIdx, pdeBufferBoxSize, gridAlpha );

  return;
}

void ModelRoutine::updatePDEBufferBetaInPDEBufferRegion( const S32 elemIdx, const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) {

  gBioModel->updatePDEBufferBetaInPDEBufferRegion( elemIdx, dim, startVIdx0, startVIdx1, pdeBufferBoxSize, gridBeta );

  return;
}

void ModelRoutine::updatePDEBufferBetaDomainBdry( const S32 elemIdx, const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) {

  gBioModel->updatePDEBufferBetaDomainBdry( elemIdx, dim, startVIdx, pdeBufferBoxSize, gridBeta );

  return;
}

void ModelRoutine::updatePDEBufferRHSLinear( const S32 elemIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) {

  gBioModel->updatePDEBufferRHSLinear( elemIdx, startVIdx, pdeBufferBoxSize, gridPhi, gridRHS );

  return;
}

void ModelRoutine::updatePDEBufferRHSTimeDependentSplitting( const S32 pdeIdx, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) {

  gBioModel->updatePDEBufferRHSTimeDependentSplitting( pdeIdx, startVIdx, pdeBufferBoxSize, v_gridPhi, v_gridRHS );

  return;
}

void ModelRoutine::updatePDEBufferDirichletBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) {

  gBioModel->updatePDEBufferDirichletBCVal( elemIdx, startPos, pdeBufferFaceSize, dim, lowSide, bcVal );

  return;
}

void ModelRoutine::updatePDEBufferNeumannBCVal( const S32 elemIdx, const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) {
  
  gBioModel->updatePDEBufferNeumannBCVal( elemIdx, startPos, pdeBufferFaceSize, dim, lowSide, bcVal );

  return;
}

