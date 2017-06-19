#include "biomodel.h"
#include <cmath>

/*
 * replace contents of vUnit with a unit vector
 */

static inline void randomUnitVector( VReal& vUnit ) {
  const REAL epsilon = 0.1;

  vUnit = VReal::ZERO;
  while(vUnit.length() < epsilon) {
    for (S32 dim = 0; dim < DIMENSION; dim++) {
      if( dim == DIMENSION-1 ) {
        vUnit[dim] = 0.0;
        continue;
      }
      vUnit[dim] = -0.5 + Util::getModelRand(MODEL_RNG_UNIFORM);
    }
    vUnit = VReal::normalize(epsilon, vUnit);
  }

}

/*********************************
 *  AgentSpeciesParticle
 *********************************/

AgentSpeciesParticle::AgentSpeciesParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue ) 
  : mParticleIdx( particleIdx ), mModelRealIdx( modelRealIdx ), mInitialValue( initialValue )
{
  // empty
}

S32 AgentSpeciesParticle::getParticleIdx( ) const {
  return mParticleIdx;
}

S32 AgentSpeciesParticle::getModelRealIdx( ) const {
  return mModelRealIdx;
}

REAL AgentSpeciesParticle::getInitialValue( ) const {
  return mInitialValue;
}

void AgentSpeciesParticle::setParticleIdx( const S32& value ) {
  mParticleIdx = value;
}

void AgentSpeciesParticle::setModelRealIdx( const S32& value ) {
  mModelRealIdx = value;
}

void AgentSpeciesParticle::setInitialValue( const REAL& value ) {
  mInitialValue = value;
}

/*********************************
 *  AgentSpeciesMolecule
 *********************************/

AgentSpeciesMolecule::AgentSpeciesMolecule( const S32& moleculeIdx, const S32& ODEIdx, const S32& initialValue)
  : mMoleculeIdx(moleculeIdx), mODEIdx(ODEIdx), mInitialValue(initialValue)
{
  //empty
}

S32 AgentSpeciesMolecule::getMoleculeIdx() const
{
  return mMoleculeIdx;
}

S32 AgentSpeciesMolecule::getODEIdx() const
{
  return mODEIdx;
}

S32 AgentSpeciesMolecule::getInitialValue() const
{
  return mInitialValue;
}

void AgentSpeciesMolecule::setMoleculeIdx( const S32& value )
{
  mMoleculeIdx = value;
}

void AgentSpeciesMolecule::setODEIdx( const S32& value )
{
  mODEIdx = value;
}

void AgentSpeciesMolecule::setInitialValue( const S32& value )
{
  mInitialValue = value;
}

/*********************************
 *  AgentSpecies
 *********************************/

AgentSpecies::AgentSpecies( )
  : ParamHolder( ),
    mModel( 0 ),
    mName( "" ), mSpeciesName( "" ), mSpeciesIdx( -1),
    mDMax( 0.0 ),
    mNumModelBools( 0 ), mNumModelReals( 0 ), mNumModelInts( 0 ),
    mUseMechForceReals(false)    
{
  // empty
}

AgentSpecies::AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param)
  : ParamHolder( num_real_param, num_int_param, num_bool_param, num_string_param ),
    mModel( 0 ),
    mName(name), mSpeciesName(speciesName), mSpeciesIdx(species_idx),
    mDMax(0.0),
    mNumModelBools(0), mNumModelReals(0), mNumModelInts(0), 
    mUseMechForceReals(false)
{
  //empty
}

AgentSpecies::~AgentSpecies()
{
  size_t i;
  for( i = 0 ; i < mInitAreas.size() ; i++ ) {
    if( mInitAreas[ i ] ) {
      delete mInitAreas[ i ];
      mInitAreas[ i ] = 0;
    }
  }
  mInitAreas.clear();

  for( i = 0 ; i < mAdhesions.size() ; i++ ) {
    if( mAdhesions[ i ] ) {
      delete mAdhesions[ i ];
      mAdhesions[ i ] = 0;
    }
  }
  mAdhesions.clear();

  for( i = 0 ; i < mDistanceJunctions.size() ; i++ ) {
    if( mDistanceJunctions[ i ] ) {
      delete mDistanceJunctions[ i ];
      mDistanceJunctions[ i ] = 0;
    }
  }
  mDistanceJunctions.clear();

  for( i = 0 ; i < mTightJunctions.size() ; i++ ) {
    if( mTightJunctions[ i ] ) {
      delete mTightJunctions[ i ];
      mTightJunctions[ i ] = 0;
    }
  }
  mTightJunctions.clear();

  mModel = 0;
}

const BioModel* AgentSpecies::getModel() const {
  return mModel;
}

const std::string& AgentSpecies::getName() const
{
  return mName;
}

const std::string& AgentSpecies::getSpeciesName() const
{
  return mSpeciesName;
}

S32 AgentSpecies::getSpeciesIdx() const
{
  return mSpeciesIdx;
}

REAL AgentSpecies::getDMax() const
{
  return mDMax;
}

S32 AgentSpecies::getNumModelBools() const
{
  return mNumModelBools;
}

S32 AgentSpecies::getNumModelReals() const
{
  return mNumModelReals;
}

S32 AgentSpecies::getNumModelInts() const
{
  return mNumModelInts;
}

BOOL AgentSpecies::getUseMechForceReals() const
{
  return mUseMechForceReals;
}

S32 AgentSpecies::getNumMechModelReals() const
{
  return (S32) mIdxMechForceReals.size();
}

S32 AgentSpecies::getIdxMechForceRealX() const
{
  return mIdxMechForceReals[0];
}

S32 AgentSpecies::getIdxMechForceRealY() const
{
  return mIdxMechForceReals[1];
}

S32 AgentSpecies::getIdxMechForceRealZ() const
{
  return mIdxMechForceReals[2];
}

S32 AgentSpecies::getNumMechModelInts() const
{
  return (S32) mIdxMechModelInts.size();
}

S32 AgentSpecies::getNumODEVariables() const
{
  return mNumODEVariables;
}

REAL AgentSpecies::getMoleculeValue(  const S32& moleculeIdx, const SpAgentState& spAgentState, const Vector< double >& v_y ) const {
  S32 i;
  for( i = 0; i < (S32) mMolecules.size(); i++ ) {
    if( mMolecules[ i ].getMoleculeIdx( ) == moleculeIdx ) {
      return v_y[ mMolecules[ i ].getODEIdx( ) ];
    }
  }
  ERROR( "No molecule value for index: " << moleculeIdx << " in AgentSpecies: " << mSpeciesIdx );
  return 0.0;
}

REAL AgentSpecies::getMoleculeValue(  const S32& moleculeIdx, const SpAgentState& spAgentState ) const {
  S32 i;
  for( i = 0; i < (S32) mMolecules.size(); i++ ) {
    if( mMolecules[ i ].getMoleculeIdx( ) == moleculeIdx ) {
      return spAgentState.getODEVal( 0, mMolecules[ i ].getODEIdx( ) );
    }
  }
  ERROR( "No molecule value for index: " << moleculeIdx << " in AgentSpecies: " << mSpeciesIdx );
  return 0.0;
}


const Vector<InitArea *>& AgentSpecies::getInitAreas( ) const
{
  return mInitAreas;
}

Vector<InitArea *>& AgentSpecies::getInitAreas( )
{
  return mInitAreas;
}

const Vector<Adhesion *>& AgentSpecies::getAdhesions( ) const
{
  return mAdhesions;
}

Vector<Adhesion *>& AgentSpecies::getAdhesions( )
{
  return mAdhesions;
}
Vector<TightJunction *>& AgentSpecies::getTightJunctions( )
{
  return mTightJunctions;
}

const Vector< AgentSpeciesParticle >& AgentSpecies::getParticles( ) const {
  return mParticles;
}

Vector< AgentSpeciesParticle >& AgentSpecies::getParticles( ) {
  return mParticles;
}

const Vector<DistanceJunction *>& AgentSpecies::getDistanceJunctions( ) const
{
  return mDistanceJunctions;
}

Vector<DistanceJunction *>& AgentSpecies::getDistanceJunctions( )
{
  return mDistanceJunctions;
}

const Vector< Chemotaxis * >& AgentSpecies::getChemotaxis() const
{
  return mChemotaxis;
}
Vector< Chemotaxis * >& AgentSpecies::getChemotaxis()
{
  return mChemotaxis;
}

const Vector< S32 >& AgentSpecies::getReactions() const
{
  return mReactions;
}

Vector< S32 >& AgentSpecies::getReactions()
{
  return mReactions;
}

const Vector< EntryCondition* >& AgentSpecies::getEntryConditions( ) const
{
  return mEntryConditions;
}

Vector< EntryCondition* >& AgentSpecies::getEntryConditions( )
{
  return mEntryConditions;
}

const Vector< AgentSpeciesMolecule >& AgentSpecies::getMolecules() const
{
  return mMolecules;
}

Vector< AgentSpeciesMolecule >& AgentSpecies::getMolecules()
{
  return mMolecules;
}

const Vector< S32 >& AgentSpecies::getODEReactions() const {
  return mODEReactions;
}

Vector< S32 >& AgentSpecies::getODEReactions() {
  return mODEReactions;
}

void AgentSpecies::setModel(const BioModel*& biomodel ) {
  mModel = biomodel;
}

void AgentSpecies::setName(const std::string& name)
{
  mName = name;
}

void AgentSpecies::setSpeciesName(const std::string& speciesName)
{
  mSpeciesName = speciesName;
}

void AgentSpecies::setSpeciesIdx(const S32& idx)
{
  mSpeciesIdx = idx;
}

void AgentSpecies::setDMax(const REAL& value)
{
  mDMax = value;
}

void AgentSpecies::setNumModelBools(const S32& value)
{
  mNumModelBools = value;
}

void AgentSpecies::setNumModelReals(const S32& value)
{
  mNumModelReals = value;
}

void AgentSpecies::setNumModelInts(const S32& value)
{
  mNumModelInts = value;
}

void AgentSpecies::setUseMechForceReals(const BOOL& value)
{
  mUseMechForceReals = value;
}

void AgentSpecies::setIdxMechForceReals(const S32& idx_x, const S32& idx_y, const S32& idx_z)
{
  mIdxMechForceReals.resize( 3 );
  mIdxMechForceReals[0] = idx_x;
  mIdxMechForceReals[1] = idx_y;
  mIdxMechForceReals[2] = idx_z;
}

void AgentSpecies::addParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue ) {
  mParticles.push_back( AgentSpeciesParticle( particleIdx, modelRealIdx, initialValue ) );
}

void AgentSpecies::addMolecule( const S32& moleculeIdx, const S32& odeVarIdx, const REAL& initialValue ) {
  mMolecules.push_back( AgentSpeciesMolecule( moleculeIdx, odeVarIdx, initialValue ) );
}

void AgentSpecies::setInitialAgentState( SpAgentState& state ) const {
  state.setType( mSpeciesIdx );
  S32 i;
  for( i = 0 ; i < (S32)mParticles.size() ; i++ ) {
    state.setModelReal( mParticles[ i ].getModelRealIdx(), mParticles[ i ].getInitialValue() );
  }
  updateSpAgentRadius( state );
  for( i = 0 ; i < (S32)mMolecules.size() ; i++ ) {
    state.setODEVal( 0, i, mMolecules[ i ].getInitialValue( ) );
  }
}

void AgentSpecies::setNumODEVariables( const S32& numODEVariables ) {
  mNumODEVariables = numODEVariables;
}


/**********************************************
 * support for model_routine_agent.cpp
 **********************************************/
void AgentSpecies::addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) const {

  REAL current_time = 0;
  if( !init ) {
    current_time = mModel->getCurrentTime( );
  }
  S32 j;
  for( j = 0 ; j < (S32) mInitAreas.size( ) ; j++ ) {
    REAL birth_time = mInitAreas[ j ]->getBirthday( );
    REAL dt = std::abs( birth_time - current_time );
    if(( init && birth_time < mModel->getAgentTimeStep( ) / 2.0 ) ||
       ( !init && birth_time > mModel->getAgentTimeStep( ) / 2.0 && dt < mModel->getAgentTimeStep( ) / 2.0 ) ) {
      mInitAreas[ j ]->addSpAgents( init, startVIdx, regionSize, ifGridHabitableBoxData, v_spAgentVIdx, v_spAgentState, v_spAgentOffset );
    }
  }

}

void AgentSpecies::spAgentCRNODERHS( const S32 odeNetIdx, const VIdx& vIdx, const SpAgent& spAgent, const NbrUBEnv& nbrUBEnv, const Vector<double>& v_y, Vector<double>& v_f ) const {

  S32 i, j;
  for( i = 0; i < (S32) v_f.size( ); i++ ) {
    v_f[ i ] = 0.0;
  }

  const Vector< Reaction *>& reactions = gBioModel->getReactions();
  for( i = 0; i < (S32) mODEReactions.size(); i++ ) {
    REAL kinetic_factor;
    REAL yield;
    const Reaction* currentReaction = reactions[ mODEReactions[ i ] ];
    kinetic_factor = currentReaction->getKineticFactor( nbrUBEnv, spAgent, v_y );
    for( j = 0; j < (S32) mMolecules.size(); j++ ) {
      yield = currentReaction->getMoleculeYield( mMolecules[ j ].getMoleculeIdx( ), spAgent );
      v_f[ mMolecules[ j ].getODEIdx() ] += kinetic_factor * yield; // FIXME: unit analysis.  ??*mModel->getAgentTimeStep()??
    }
  }

}

void AgentSpecies::updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state) const {

  const Vector< Reaction * >& reactions = gBioModel->getReactions( );
  S32 i, pIdx;
  for( i = 0 ; i < (S32) mReactions.size( ) ; i++ ) {
    const Reaction* currentReaction = reactions[ mReactions[ i ] ];
    const REAL currentKineticFactor = currentReaction->getKineticFactor( nbrUBEnv, vOffset, state );
    if( false ) {
      OUTPUT( 0,
              "AgentSpecies:: Reaction:: kineticFactor: " << currentKineticFactor
              );
    }
    for ( pIdx = 0; pIdx < (S32) mParticles.size( ); pIdx++) {
      REAL yield = currentReaction->getParticleYield( mParticles[ pIdx ].getParticleIdx( ) , state );
      if( false ) {
        OUTPUT( 0,
                "AgentSpecies:: Reaction:: yield: " << yield
                << "  total-change: " << currentKineticFactor * yield
                << "  delta-t: " << mModel->getAgentTimeStep()
                );
      }
      state.incModelReal( mParticles[ pIdx ].getModelRealIdx( ) , currentKineticFactor * yield * mModel->getAgentTimeStep( ) );
    }
  }
  //update radius here
  updateSpAgentRadius( state );
  if( false ) {
    for ( pIdx = 0; pIdx < (S32) mParticles.size( ); pIdx++) {
      OUTPUT( 0,
              "Agent particle: " << gBioModel->getParticles()[ mParticles[ pIdx ].getParticleIdx( ) ]->getName()
              << " mass: " << state.getModelReal( mParticles[ pIdx ].getModelRealIdx( ) )
              );
    }
  }
  
}


void AgentSpecies::updateSpAgentRadius( SpAgentState& state ) const {

  //calculate new volume based off of updated mass
  S32 i;
  REAL mass = 0.0;
  REAL volume = 0.0;
  for( i = 0; i < (S32) mParticles.size( ); i++) {
    mass = state.getModelReal( mParticles[ i ].getModelRealIdx( ) );
    volume += mass / gBioModel->getParticles()[ mParticles[ i ].getParticleIdx() ]->getDensity( );
  }
  //calculate new radius based off of updated mass
  REAL radius = cbrt( 3.0 * volume / (4.0 * MODEL_PI ) );
  
  // if radius is too big, then agent-agent interactions will be missed.
  if( false ) {
    OUTPUT( 0, ""
            << " radius: " << radius
            << " mDMax: " << mDMax
            << " mDMax/2.0: " << mDMax/2.0
            );
  }
  CHECK( radius <= mDMax / 2.0 );
  //update the radius
  state.setRadius( radius );

}


void AgentSpecies::spAgentSecretionBySpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentDisp ) const {

  /* nothing to do */
  
}

void AgentSpecies::updateSpAgentBirthDeath( const VIdx& vIdx, const SpAgent& spAgent, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, BOOL& divide, BOOL& disappear ) const {

  disappear = false;
  if ( spAgent.state.getRadius() >= getParamReal( getIdxReal( SPECIES_divRadius ) ) ) {
    divide = true;
  } else {
    divide = false;
  }

}

/*
 **************************************** ADJUSTSPAGENT BEGIN ****************************************************
 */

void AgentSpecies::brownianMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& disp ) const {

  for( S32 dim = 0 ; dim < 3 ; dim++ ) { // FIXME:  Use computation domain grid for dimension
    if( dim == 3-1 ) { 
      continue; 
    }
    // FIXME: Unit analysis.  Find cDynoMiCS brownian motion for comparison
    disp[ dim ] += state.getRadius() * ( Util::getModelRand( MODEL_RNG_GAUSSIAN ) ) * getParamReal( getIdxReal( SPECIES_brownianScale ) ) * mModel->getAgentTimeStep( );
  }

}

void AgentSpecies::limitMotion(VReal& disp) const {

  /* limit the maximum displacement within a single time step */
  // FIXME: Should use resolution or radius?
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {
    if( disp[ dim ] > gBioModel->getAgentGrid( ).getResolution( ) ) {
      disp[ dim ] = gBioModel->getAgentGrid( ).getResolution( );
    }
    else if( disp[ dim ] < ( gBioModel->getAgentGrid( ).getResolution( ) * -1.0 ) ) {
      disp[ dim ] = gBioModel->getAgentGrid( ).getResolution( ) * -1.0;
    }
    if( false ) {
      OUTPUT( 0, "limited disp[ " << dim <<" ]: " << disp[ dim ] );
    }
  }

}

void AgentSpecies::setDisplacementFromMechanicalInteraction( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {
  
  S32 dim;
  for( dim = 0 ; dim < 3 ; dim ++ ) {
    disp[ dim ] = mechIntrctData.getModelReal( mIdxMechForceReals[ dim ] );
    if( false ) {
      OUTPUT( 0, "disp[ " << dim <<" ]: " << disp[ dim ] );
    }
  }
  
}

void AgentSpecies::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {

  setDisplacementFromMechanicalInteraction( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );  
  brownianMotion( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  adjustSpAgentChemotaxis( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  limitMotion(disp);

}
/*
 **************************************** ADJUSTSPAGENT END ****************************************************
 */


/*
 **************************************** CHEMOTAXIS BEGIN ****************************************************
 */
static inline S32 countTouches( const JunctionData& junctionData ) {
  S32 count = 0;
  S32 i;
  for( i = 0 ; i < junctionData.getNumJunctions() ; i++ ) {
    if( junctionData.getJunctionEndRef( i ).getModelInt( JUNCTION_INT_TOUCHED ) ) {
      count++;
    }
  }
  return count;
}

static inline void findChemoTaxisDirectionAndConcentration( const S32 elemIdx,  const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& dir, REAL& delta ) {

  // Random direction of motion
  VReal fwdDir, bckDir;
  randomUnitVector( fwdDir );
  bckDir = VReal::ZERO - fwdDir;

  // Find solute concentration just ahead and just behind agent
  REAL offset_magnitude = 1.42 * gBioModel->getAgentGrid().getResolution( ) / gBioModel->getSolutes( )[ elemIdx ]->getSubgridDimension( );
  VReal curPos = vOffset;
  VReal fwdPos = vOffset + fwdDir * offset_magnitude; // 1.5 * state.getRadius()
  VReal bckPos = vOffset + bckDir * offset_magnitude; // 1.5 * state.getRadius()
  REAL curVal = gBioModel->getSolutes( )[ elemIdx ]->getSubgridValue( nbrUBEnv, curPos );
  REAL fwdVal = gBioModel->getSolutes( )[ elemIdx ]->getSubgridValue( nbrUBEnv, fwdPos );
  REAL bckVal = gBioModel->getSolutes( )[ elemIdx ]->getSubgridValue( nbrUBEnv, bckPos );
  if( fwdVal < 0 ) {
    fwdVal = curVal;
  }
  if( bckVal < 0 ) {
    bckVal = curVal;
  }

  // this scaling reduces the sharpness of the chemotactic pull
  // makes it about the relative difference, not the absolute difference
  // number borrowed from cDynoMiCS implementation
  // this results in the maximum value asymptotically approaching 1/alpha
  REAL alpha = 0.1;
  fwdVal = fwdVal / (1.0 + alpha * fwdVal);
  bckVal = bckVal / (1.0 + alpha * bckVal);

  delta = fwdVal - bckVal;
  dir = fwdDir;
}


void AgentSpecies::adjustSpAgentChemotaxis( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {
  S32 i;
  for( i = 0 ; i < (S32) mChemotaxis.size( ) ; i++ ) {
    VReal dir;
    REAL delta;
    
    // contact inhibition
    if( mChemotaxis[ i ]->getContactInhibition( ) != 0 &&
        countTouches( junctionData ) >= mChemotaxis[ i ]->getContactInhibition( ) ) {
      return;
    }
    
    findChemoTaxisDirectionAndConcentration( mChemotaxis[ i ]->getSolute( ),  vOffset, nbrUBEnv, state, dir, delta );
    if( delta > 0 ) {
      if( false ) {
        OUTPUT( 0, " Chemotaxis: magnitude: " << delta );
      }
      VReal chemDisp = dir * ( mChemotaxis[ i ]->getStrength( ) * delta );
      // FIXME: Unit analysis and cDynoMiCS comparison
      disp += chemDisp * mModel->getAgentTimeStep( );
    }
  }
}
/*
 **************************************** CHEMOTAXIS END ****************************************************
 */

void AgentSpecies::divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd ) const {
  
  S32 i;
  REAL motherFraction = 0.4 + 0.05 * Util::getModelRand( MODEL_RNG_UNIFORM );
  REAL motherMass = 0;
  REAL motherVar = 0;
  daughterState.setType( motherState.getType( ) );
  for( i = 0; i < (S32) mParticles.size(); i++) {
    motherMass = motherState.getModelReal( mParticles[ i ].getModelRealIdx( ) );
    REAL newMass = motherMass * motherFraction;
    motherState.setModelReal( mParticles[ i ].getModelRealIdx( ), newMass );
    daughterState.setModelReal( mParticles[ i ].getModelRealIdx( ), motherMass - newMass );
  }
  
  for( i = 0; i < (S32) mMolecules.size() ; i++ ) {
    motherVar = motherState.getODEVal( 0, mMolecules[ i ].getODEIdx( ) );
    REAL newVar = motherVar * motherFraction;
    motherState.setODEVal( 0, mMolecules[ i ].getODEIdx( ), newVar );
    daughterState.setODEVal( 0, mMolecules[ i ].getODEIdx( ), motherVar - newVar );
  }

  updateSpAgentRadius( motherState );
  updateSpAgentRadius( daughterState );
  REAL avg_radius = ( motherState.getRadius( ) + daughterState.getRadius( ) ) / 2;
  VReal unit_vector;
  randomUnitVector( unit_vector );
  
  setDisplacementFromMechanicalInteraction( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, motherState, motherDisp );  
  setDisplacementFromMechanicalInteraction( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, daughterState, daughterDisp );  
  motherDisp += unit_vector * avg_radius;
  daughterDisp += unit_vector * (-avg_radius);
  
  limitMotion(motherDisp);
  limitMotion(daughterDisp);

  S32 j;
  for( j = 0; j < (S32) v_junctionDivide.size(); j++ ) {
    if( Util::getModelRand( MODEL_RNG_UNIFORM ) < 0.5 ) {
      v_junctionDivide[ j ] = false;
    } else {
      v_junctionDivide[ j ] = true;
    }
  }
  
  motherDaughterLinked = false;
  
}

// support for model_routine_output.cpp
void AgentSpecies::updateSpAgentOutput( const VIdx& vIdx, const SpAgent& spAgent, REAL& color, Vector<REAL>& v_extraScalar, Vector<VReal>& v_extraVector ) const {

  color = spAgent.state.getType();

  S32 i,j;
  S32 scalar_count = 0;

  const Vector< Particle* >& particles = gBioModel->getParticles( );
  for( i = 0 ; i < (S32) particles.size( ) ; i++ ) {
    if( particles[ i ]->getParamBool( particles[ i ]->getIdxBool( PARTICLE_writeOutput ) ) ) {
      v_extraScalar[ scalar_count ] = 0.0;
      for( j = 0; j < (S32) mParticles.size(); j++ ) {
        if( mParticles[ j ].getParticleIdx( ) == particles[ i ]->getParticleIdx( ) ) {
          v_extraScalar[ scalar_count ] = spAgent.state.getModelReal( mParticles[ j ].getModelRealIdx( ) );
          break;
        }
      }
      scalar_count++;
    }
  }
  const Vector< Molecule* >& molecules = gBioModel->getMolecules( );
  for( i = 0 ; i < (S32) molecules.size( ) ; i++ ) {
    if( molecules[ i ]->getParamBool( molecules[ i ]->getIdxBool( MOLECULE_writeOutput ) ) ) {
      v_extraScalar[ scalar_count ] = 0.0;
      for( j = 0; j < (S32) mMolecules.size(); j++ ) {
        if( mMolecules[ j ].getMoleculeIdx( ) == molecules[ i ]->getMoleculeIdx( ) ) {
          v_extraScalar[ scalar_count ] = getMoleculeValue( molecules[ i ]->getMoleculeIdx( ), spAgent.state );
          break;
        }
      }
      scalar_count++;
    }
  }
  
  CHECK( v_extraScalar.size() == scalar_count );
  CHECK( v_extraVector.size() == 0 );

}
