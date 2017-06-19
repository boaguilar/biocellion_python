#include "biomodel.h"

Solute::Solute()
  : ParamHolder( ),
    mModel( 0 ),
    mName( "" ), mSoluteIdx( -1 ), mDomainIdx( -1 ), mSolverIdx( -1 ),
    mAMRLevels( 3 ), mInterfaceAMRLevel( 2 ),
    mNumTimeSteps( 1 ), mSubgridDimension(-1)
{
  calcSubgridDimension( );
}

Solute::Solute(const std::string& name, const S32& solute_idx, const S32& domain_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param) 
  : ParamHolder( num_real_param, num_int_param, num_bool_param, num_string_param ),
    mModel( 0 ),
    mName( name ), mSoluteIdx( solute_idx ), mDomainIdx( domain_idx ), mSolverIdx( -1 ),
    mAMRLevels( 3 ), mInterfaceAMRLevel( 2 ),
    mNumTimeSteps( 1 ), mSubgridDimension(-1)
{
  calcSubgridDimension( );
}

const BioModel* Solute::getModel() const {
  return mModel;
}

const std::string& Solute::getName() const {
  return mName;
}

S32 Solute::getSoluteIdx() const {
  return mSoluteIdx;
}

S32 Solute::getDomainIdx() const {
  return mDomainIdx;
}

S32 Solute::getSolverIdx() const {
  return mSolverIdx;
}

S32 Solute::getAMRLevels() const {
  return mAMRLevels;
}

S32 Solute::getInterfaceAMRLevel() const {
  return mInterfaceAMRLevel;
}

S32 Solute::getNumTimeSteps() const {
  return mNumTimeSteps;
}

const Vector< S32 >& Solute::getReactions() const {
  return mReactions;
}

Vector< S32 >& Solute::getReactions() {
  return mReactions;
}

const Vector< S32 >& Solute::getMoleculeReactions() const {
  return mMoleculeReactions;
}

Vector< S32 >& Solute::getMoleculeReactions() {
  return mMoleculeReactions;
}

void Solute::setModel(const BioModel*& biomodel ) {
  mModel = biomodel;
}

void Solute::setName(const std::string& name) {
  mName = name;
}

void Solute::setSoluteIdx(const S32& idx) {
  mSoluteIdx = idx;
}

void Solute::setDomainIdx(const S32& idx) {
  mDomainIdx = idx;
}

void Solute::setSolverIdx(const S32& idx) {
  mSolverIdx = idx;
}

void Solute::setAMRLevels(const S32& value) {
  mAMRLevels = value;
  calcSubgridDimension( );
}

void Solute::setInterfaceAMRLevel(const S32& value) {
  mInterfaceAMRLevel = value;
  calcSubgridDimension( );
}

void Solute::setNumTimeSteps(const S32& value) {
  mNumTimeSteps = value;
}


idx_t Solute::getSubgridDimension() const {
  CHECK( mSubgridDimension > 0  );
  return mSubgridDimension;
}

void Solute::calcSubgridDimension() {
  S32 l = ( mAMRLevels -1 ) - mInterfaceAMRLevel;
  mSubgridDimension = 1;
  for( S32 i = l ; i > 0 ; i-- ) {
    mSubgridDimension *= Info::getAMRRatio();
  }
  return;
}

void Solute::getSubgridOffset( const VReal& vOffset, VIdx& subgridVOffset ) const {
  for( S32 dim = 0; dim < 3; dim++ ) {
    REAL fractional_offset  = vOffset[ dim ] / gBioModel->getAgentGrid().getResolution( );
    subgridVOffset[ dim ] = fractional_offset * getSubgridDimension();
  }
}

void Solute::getSubgridCenter( const VIdx& subgridVOffset, VReal& vOffset ) const {
  S32 dim;
  for( dim = 0 ; dim < 3 ; dim++ ) {
    vOffset[ dim ] = ( subgridVOffset[ dim ] + 0.5 ) * gBioModel->getAgentGrid().getResolution( ) / mSubgridDimension;
  }
}

BOOL Solute::offsetIsInSubgrid(const VReal& vOffset, const VIdx& vSubgrid) const {
  VIdx sgridVOffset;
  getSubgridOffset( vOffset, sgridVOffset );
  return sgridVOffset == vSubgrid;
}

REAL Solute::getSubgridValue(const NbrUBEnv& nbrUBEnv, const VReal& vOffset) const {
  // Find the subgridphi value in the subgrid at vOffset from the center of nbrUBEnv.
  // If vOffset is not a valid position, return a value < 0
  idx_t subgrid_dimension = getSubgridDimension();
  S32 dim;
  VIdx vidx, vsubidx;
  REAL step = gBioModel->getAgentGrid().getResolution( ) / subgrid_dimension;
  for( dim = 0 ; dim < 3 ; dim++ ) {
    vidx[ dim ] = 0;
    vsubidx[ dim ] = (idx_t)( vOffset[ dim ] + 0.5 ) / step;
    while( vsubidx[ dim ] < 0 ) {
      vsubidx[ dim ] += subgrid_dimension;
      vidx[ dim ] -= 1;
    }
    while( vsubidx[ dim ] >= subgrid_dimension ) {
      vsubidx[ dim ] -= subgrid_dimension;
      vidx[ dim ] += 1;
    }
  }

  if(! nbrUBEnv.getValidFlag( vidx ) ) {
    return -1.0;
  }

  return nbrUBEnv.getSubgridPhi( vidx, vsubidx, mSoluteIdx );
}

REAL Solute::getSubgridValue( const UBEnv& ubEnv, const VReal& vOffset ) const {
  VIdx subgridVOffset;
  getSubgridOffset( vOffset, subgridVOffset );
  return getSubgridValue( ubEnv, subgridVOffset );
}

REAL Solute::getSubgridValue( const UBEnv& ubEnv, const VIdx& subgridVOffset ) const {
  return ubEnv.getSubgridPhi( subgridVOffset, mSoluteIdx );
}

REAL Solute::getSubgridVolume( ) const {
  REAL volume = 1.0;
  S32 dim;
  for( dim = 0 ; dim < 3 ; dim++ ) {
    volume *= getParamReal( getIdxReal ( SOLUTE_resolution ) );
  }
  return volume;
}

REAL Solute::getPDETimeStepDuration( ) const {
  return gBioModel->getSimulator().getAgentTimeStep( ) / gBioModel->getSimulator().getParamInt( gBioModel->getSimulator().getIdxInt( SIMULATOR_numStateAndGridTimeStepsPerBaseline ) ) / mNumTimeSteps;
}

// support for model_routine_config.cpp
void Solute::calculateBoundaryConditions( ) {
  
  mGridBeta = getParamReal( getIdxReal( SOLUTE_diffusivity ) );
  
  const ComputationDomain* domain = mModel->getWorld( ).getComputationDomains( )[0];
  S32 dim, face;
  for( dim = 0 ; dim < DIMENSION ; dim++ ) {
    if( domain->boundaryIsPeriodic( dim ) ) {
      mGridBCType[ dim ][ 0 ] = BC_TYPE_NEUMANN_CONST;
      mGridBCType[ dim ][ 1 ] = BC_TYPE_NEUMANN_CONST;
      mGridBCVal[ dim ][ 0 ] = 0.0;
      mGridBCVal[ dim ][ 1 ] = 0.0;
      mGridBoundaryBeta[ dim ][ 0 ] = 0.0;
      mGridBoundaryBeta[ dim ][ 1 ] = 0.0;
    } else {
      for( face = 0 ; face < 2 ; face++ ) {
        domain->updateGridPhiBoundaryInfo( mSoluteIdx, dim, face,
                                           mGridBCType[ dim ][ face ],
                                           mGridBCVal[ dim ][ face ],
                                           mGridBoundaryBeta[ dim ][ face ] );
        if( false ) {
          OUTPUT( 0, ""
                  << "Solute: " << mName
                  << " BC: " << dim << ":" << face
                  << " type: " << mGridBCType[ dim ][ face ]
                  << " val: " << mGridBCVal[ dim ][ face ] 
                  << " boundary beta: " << mGridBoundaryBeta[ dim ][ face ] );
        }
      }
    }
  }
}

void Solute::updatePhiPDEInfo( PDEInfo& pdeInfo ) const {
  // WARNING( "updatePhiPDEInfo still has numerous hard-coded values." );
  GridPhiInfo gridPhiInfo;
  SplittingInfo splittingInfo;

  pdeInfo.pdeIdx = mSoluteIdx;
  pdeInfo.pdeType = PDE_TYPE_REACTION_DIFFUSION_TIME_DEPENDENT_LINEAR;
  pdeInfo.numLevels = mAMRLevels;
  pdeInfo.ifLevel = mInterfaceAMRLevel;
  pdeInfo.v_tagExpansionSize.assign( mAMRLevels, 0 ); // ???FIXME: Need to configure this correctly
  pdeInfo.numTimeSteps = mNumTimeSteps; // ???FIXME: Need to configure this correctly
  pdeInfo.callAdjustRHSTimeDependentLinear = false;


  { /* this code configures the multi grid part of the CHOMBO solver
     * see 4.4.4 Class AMRMultiGrid in the CHOMBO documentation */
    
    // Directly from the CHOMBO documentation PDF
    // – pre is the number of smoothings before averaging.
    // – post is the number of smoothings after averaging.
    // – bottom is the number of smoothings at the bottom level.
    // – numMG = 1 for v-cycle, 2 for w-cycle and so on (in most cases, use 1).
    // – itermax is the max number of v cycles.
    // – hang is the minimum amount of change per vcycle.
    // – eps is the solution tolerance.
    // – normThresh is how close to zero eps*resid is allowed to get.
    
    CHECK( mSolverIdx >= 0 && mSolverIdx < NUM_SOLVERS );
    const Solver* solver = gBioModel->getSolvers( )[ mSolverIdx ];
    pdeInfo.mgSolveInfo.numPre = solver->getParamInt( solver->getIdxInt( SOLVER_preStep ) );
    pdeInfo.mgSolveInfo.numPost = solver->getParamInt( solver->getIdxInt( SOLVER_postStep ) );
    pdeInfo.mgSolveInfo.numBottom = 3;
    pdeInfo.mgSolveInfo.vCycle = true;/* multigrid parameters */
    pdeInfo.mgSolveInfo.maxIters =  solver->getParamInt( solver->getIdxInt( SOLVER_nCycles ) );
    pdeInfo.mgSolveInfo.epsilon = 1e-8;/* multigrid parameters */
    pdeInfo.mgSolveInfo.hang = 1e-6;/* multigrid parameters */
    pdeInfo.mgSolveInfo.normThreshold = 1e-10;/* multigrid parameters */
  }

  // set splittingInfo here
  pdeInfo.splittingInfo = splittingInfo;

  // set gridPhiInfo here
  pdeInfo.v_gridPhiInfo.resize( 1 );

  gridPhiInfo.elemIdx = mSoluteIdx;
  gridPhiInfo.name = mName;
  gridPhiInfo.syncMethod = VAR_SYNC_METHOD_DELTA;

  { // boundary conditions
    S32 dim, face;
    for( dim = 0 ; dim < DIMENSION ; dim++ ) {
      for( face = 0 ; face < 2 ; face++ ) {
        gridPhiInfo.aa_bcType[ dim ][ face ] = mGridBCType[ dim ][ face ];
        gridPhiInfo.aa_bcVal[ dim ][ face ] = mGridBCVal[ dim ][ face ];
      }
    }
  }

  // ???FIXME: Need to configure this correctly
  gridPhiInfo.errorThresholdVal = pdeInfo.mgSolveInfo.normThreshold * -1.0;
  gridPhiInfo.warningThresholdVal = pdeInfo.mgSolveInfo.normThreshold * -1.0;
  gridPhiInfo.setNegToZero = true;

  pdeInfo.v_gridPhiInfo[0] = gridPhiInfo;
}


// support for model_routine_grid.cpp
void Solute::initIfGridVar( const VIdx& vIdx, const UBAgentData& ubAgentData, UBEnv& ubEnv ) const {
  ubEnv.setPhi( mSoluteIdx, getParamReal( getIdxReal( SOLUTE_concentration ) ) );
}

void Solute::initIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {
  gridKappa = 1.0;
}

void Solute::updateIfGridVarPre( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const {
  /* noop */
}

void Solute::updateIfGridVarPost( const S32 iter, const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, NbrUBEnv& nbrUBEnv/* [INOUT] */ ) const {
  /* noop */
}

void Solute::updateIfSubgridKappa( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridKappa ) const {
  // if( mDoDivideByKappa ) {
  //   gridKappa = 1.0;
  //   // FIXME: This is not real
  //   for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
  //     const SpAgent& spAgent = ubAgentData.v_spAgent[l];
  //     VIdx agentOffset;
  //     getSubgridOffset(spAgent.vOffset, agentOffset);
  //     if( agentOffset == subgridVOffset ) {
  //       gridKappa *= 0.5;
  //     }
  //   }
  // } else {
  gridKappa = 1.0;
  // }
  
}

void Solute::updateIfSubgridAlpha( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridAlpha/* decay (-) */ ) const {
  //gridAlpha = -mDecayRate;
  gridAlpha = -0.04;
}

void Solute::updateIfSubgridBetaInIfRegion( const S32 dim, const VIdx& vIdx0, const VIdx& subgridVOffset0, const UBAgentData& ubAgentData0, const UBEnv& ubEnv0, const VIdx& vIdx1, const VIdx& subgridVOffset1, const UBAgentData& ubAgentData1, const UBEnv& ubEnv1, REAL& gridBeta ) const {
  /* flux between subgrids */
  gridBeta = mGridBeta;
}

void Solute::updateIfSubgridBetaPDEBufferBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {
  /* flux between grid and buffer */
  gridBeta = mGridBeta;
}

void Solute::updateIfSubgridBetaDomainBdry( const S32 dim, const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridBeta ) const {
  /* flux at domain boundary */
  S32 face = -1;
  if( vIdx[ dim ] == 0 && subgridVOffset[ dim ] == 0 ) {
    face = 0;
    if( false ) {
      OUTPUT( 0, "lower: "
              << " solute: " << mName
              << " dim: " << dim
              << " vIdx: " << vIdx[0] << "," << vIdx[1] << "," << vIdx[2]
              << " subgridVOffset: " << subgridVOffset[0] << "," << subgridVOffset[1] << "," << subgridVOffset[2]
              << " domain_size: " << Info::getDomainSize( dim )
              << " subgrid_dimension:" << getSubgridDimension( )
              );
    }
  } else if( vIdx[ dim ] == ( Info::getDomainSize( dim ) - 1 ) && subgridVOffset[ dim ] == ( getSubgridDimension( ) - 1 ) ) {
    face = 1;
    if( false ) {
      OUTPUT( 0, "upper: "
              << " solute: " << mName
              << " dim: " << dim
              << " vIdx: " << vIdx[0] << "," << vIdx[1] << "," << vIdx[2]
              << " subgridVOffset: " << subgridVOffset[0] << "," << subgridVOffset[1] << "," << subgridVOffset[2]
              << " domain_size: " << Info::getDomainSize( dim )
              << " subgrid_dimension:" << getSubgridDimension( )
              );
    }
  } else {
    OUTPUT( 0, "updateIfSubgridBetaDomainBdry called. "
            << " solute: " << mName
            << " dim: " << dim
            << " vIdx: " << vIdx[0] << "," << vIdx[1] << "," << vIdx[2]
            << " subgridVOffset: " << subgridVOffset[0] << "," << subgridVOffset[1] << "," << subgridVOffset[2]
            << " domain_size: " << Info::getDomainSize( dim )
            << " subgrid_dimension:" << getSubgridDimension( )
            );
    ERROR( "Called on non-boundary subgrid." );
  }
  gridBeta = mGridBoundaryBeta[ dim ][ face ];
  if( false ) {
    OUTPUT( 0, "updateIfSubgridBetaDomainBdry called. "
            << " solute: " << mName
            << " dim: " << dim
            << " face: " << face
            << " vIdx: " << vIdx[0] << "," << vIdx[1] << "," << vIdx[2]
            << " subgridVOffset: " << subgridVOffset[0] << "," << subgridVOffset[1] << "," << subgridVOffset[2]
            << " domain_size: " << Info::getDomainSize( dim )
            << " subgrid_dimension:" << getSubgridDimension( )
            << " gridBeta: " << gridBeta
            );
  }
}

void Solute::updateIfSubgridRHSLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnv& ubEnv, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  
  // for every reaction with a yield that involves this solute, add/subtract gridRHS.
  // amount is controlled by kineticFactors
  // FIXME: Need to apply computationDomains

  // _specRate = muMax * PI_i( kineticFactor_i( current_concentration ) ) for all KF that apply to this solute.
  // RHS = _specRate * mass * yield_rate

  gridRHS = 0.0;
  S32 i;
  VReal vOffset;
  getSubgridCenter( subgridVOffset, vOffset );
  const Vector< Reaction * >& reactions = gBioModel->getReactions( );

  // process all reactions that involve this solute, but don't require agent molecular concentrations for kinetic factors
  for( i = 0 ; i < (S32) mReactions.size( ) ; i++ ) {
    const Reaction *reaction = reactions[ mReactions[ i ] ];
    REAL factor = reaction->getKineticFactor( ubEnv, vOffset );
    if( false ) {
      std::stringstream kf;
      S32 ii;
      for( ii = 0 ; ii < (S32) reaction->getKineticFactors( ).size( ) ; ii++ ) {
        kf << "  KF:" << ii << " " << *(reaction->getKineticFactors( )[ ii ]);
      }
      
      OUTPUT( 0, ""
              << " loc: " << vIdx[ 0 ] << "," << vIdx[ 1 ] << "," << vIdx[ 2 ]
              << " " << subgridVOffset[ 0 ] << "," << subgridVOffset[ 1 ] << "," << subgridVOffset[ 2 ]
              << " solute: " << getSoluteIdx( )
              << " reaction: " << i
              << " factor: " << factor
              << " muMax: " << reaction->getMuMax( )
              << " KF: " << kf.str( )
              );
    }
    
    // Check for all agents that apply
    REAL yield = 0;
    for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
      const SpAgent& spAgent = ubAgentData.v_spAgent[ l ];
      VIdx sgridVOffset;
      getSubgridOffset( spAgent.vOffset, sgridVOffset );
      if( sgridVOffset == subgridVOffset ) {
        // agent is in the subgrid
        yield += reaction->getSoluteYield( getSoluteIdx( ), spAgent );
        if( false ) {
          std::stringstream y;
          S32 ii;
          for( ii = 0 ; ii < (S32) reaction->getYields( ).size( ) ; ii++ ) {
            y << "  YLD:" << ii << " " << reaction->getYields( )[ ii ];
          }
          REAL mass = spAgent.state.getModelReal( reaction->getCatalyzedBy( ) );
          OUTPUT( 0, ""
                  << " loc: " << vIdx[ 0 ] << "," << vIdx[ 1 ] << "," << vIdx[ 2 ]
                  << " " << subgridVOffset[ 0 ] << "," << subgridVOffset[ 1 ] << "," << subgridVOffset[ 2 ]
                  << " solute: " << getSoluteIdx( )
                  << " reaction: " << i
                  << " agent: " << l
                  << " yield: " << yield
                  << " YIELDS: " << y.str( )
                  << " mass: " << mass
                  );
        }
      }
    }
    //(fg.um-3.hour-1) =(fg)  * (hour-1) * (um-3)
    gridRHS += yield * factor / getSubgridVolume();
  }

  // process all reactions that involve this solute, and require agent molecular concentrations for kinetic factors
  for( i = 0 ; i < (S32) mMoleculeReactions.size( ) ; i++ ) {
    const Reaction *reaction = reactions[ mMoleculeReactions[ i ] ];

    // Check for all agents that apply
    for( ubAgentIdx_t l = 0 ; l < ( ubAgentIdx_t )ubAgentData.v_spAgent.size() ; l++ ) {
      const SpAgent& spAgent = ubAgentData.v_spAgent[ l ];
      VIdx sgridVOffset;
      getSubgridOffset( spAgent.vOffset, sgridVOffset );
      if( sgridVOffset == subgridVOffset ) {
        // agent is in the subgrid
        REAL factor = reaction->getKineticFactor( ubEnv, vOffset, spAgent );
        if( false ) {
          std::stringstream kf;
          S32 ii;
          for( ii = 0 ; ii < (S32) reaction->getKineticFactors( ).size( ) ; ii++ ) {
            kf << "  KF:" << ii << " " << *(reaction->getKineticFactors( )[ ii ]);
          }
          
          OUTPUT( 0, ""
                  << " loc: " << vIdx[ 0 ] << "," << vIdx[ 1 ] << "," << vIdx[ 2 ]
                  << " " << subgridVOffset[ 0 ] << "," << subgridVOffset[ 1 ] << "," << subgridVOffset[ 2 ]
                  << " solute: " << getSoluteIdx( )
                  << " reaction: " << i
                  << " factor: " << factor
                  << " muMax: " << reaction->getMuMax( )
                  << " KF: " << kf.str( )
                  );
        }

        REAL yield = reaction->getSoluteYield( getSoluteIdx( ), spAgent );
        if( false ) {
          std::stringstream y;
          S32 ii;
          for( ii = 0 ; ii < (S32) reaction->getYields( ).size( ) ; ii++ ) {
            y << "  YLD:" << ii << " " << reaction->getYields( )[ ii ];
          }
          REAL mass = spAgent.state.getModelReal( reaction->getCatalyzedBy( ) );
          OUTPUT( 0, ""
                  << " loc: " << vIdx[ 0 ] << "," << vIdx[ 1 ] << "," << vIdx[ 2 ]
                  << " " << subgridVOffset[ 0 ] << "," << subgridVOffset[ 1 ] << "," << subgridVOffset[ 2 ]
                  << " solute: " << getSoluteIdx( )
                  << " reaction: " << i
                  << " agent: " << l
                  << " yield: " << yield
                  << " YIELDS: " << y.str( )
                  << " mass: " << mass
                  );
        }
        //(fg.um-3.hour-1) =(fg)  * (hour-1) * (um-3)
        gridRHS += yield * factor / getSubgridVolume();
      }
    }
  }

  if( false ) {
    if( true || !( gridRHS >= 0. || ( ubEnv.getSubgridPhi( subgridVOffset, getSoluteIdx() ) >= -gridRHS ) ) ) {
      OUTPUT( 0, ""
              << " loc: " << vIdx[ 0 ] << "," << vIdx[ 1 ] << "," << vIdx[ 2 ]
              << " " << subgridVOffset[ 0 ] << "," << subgridVOffset[ 1 ] << "," << subgridVOffset[ 2 ]
              << " solute: " << getSoluteIdx( )
              << " volume: " << getSubgridVolume()
              << " gridRHS: " << gridRHS
              << " currentValue: " << ubEnv.getSubgridPhi( subgridVOffset, getSoluteIdx() )
              );
    }
  }
  // Can't uptake more than exists.
  if( gridRHS < 0. && getSubgridValue( ubEnv, subgridVOffset ) <= 0 ) {
    gridRHS = 0.0;
  }
  if( gridRHS < 0. && getSubgridValue( ubEnv, subgridVOffset ) < -gridRHS * getPDETimeStepDuration( ) ) {
    gridRHS = -getSubgridValue( ubEnv, subgridVOffset );
  }
  // This check is over-zealous.  Need to understand PDE solver better
  //CHECK( gridRHS >= 0. || ( ubEnv.getSubgridPhi( subgridVOffset, getSoluteIdx() ) >= -gridRHS ) );
}

void Solute::adjustIfSubgridRHSTimeDependentLinear( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL gridPhi, REAL& gridRHS/* INOUT */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfSubgridRHSTimeDependentSplitting( const VIdx& vIdx, const VIdx& subgridVOffset, const UBAgentData& ubAgentData, const UBEnvModelVar& ubEnvModelVar, const REAL& gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfGridAMRTags( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, S32& finestLevel ) const {
  // this is the default maximum.
  finestLevel = mInterfaceAMRLevel;
}

void Solute::updateIfGridDirichletBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::updateIfGridNeumannBCVal( const VReal& pos, const S32 dim, const BOOL lowSide, const UBEnvModelVar a_ubEnvModelVar[3], const Vector<REAL> av_gridPhi[3]/* av_gridPhi[].size() == ratio * raito * ratio (ratio = Info::envAuxDataInfo.v_phiRatioFromIfGridToIfSubgrid[elemIdx]), use VIdx::getIdx3DTo1D() to index */, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::initPDEBufferPhi( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, Vector<REAL>& v_gridPhi/* [idx] */ ) const {
  ERROR( "unimplmented." );
}

void Solute::initPDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferKappa( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridKappa ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferAlpha( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridAlpha/* decay (-) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferBetaInPDEBufferRegion( const S32 dim, const VIdx& startVIdx0, const VIdx& startVIdx1, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferBetaDomainBdry( const S32 dim, const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, REAL& gridBeta ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferRHSLinear( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const REAL gridPhi, REAL& gridRHS/* uptake(-) and secretion (+) */ ) const {
  gridRHS = 0.0; // no uptake or secretion
}

void Solute::updatePDEBufferRHSTimeDependentSplitting( const VIdx& startVIdx, const VIdx& pdeBufferBoxSize, const Vector<double>& v_gridPhi/* [idx] */, Vector<double>& v_gridRHS/* [idx], uptake(-) and secretion (+) */ ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferDirichletBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}

void Solute::updatePDEBufferNeumannBCVal( const VReal& startPos, const VReal& pdeBufferFaceSize, const S32 dim, const BOOL lowSide, REAL& bcVal ) const {
  ERROR( "unimplmented." );
}
