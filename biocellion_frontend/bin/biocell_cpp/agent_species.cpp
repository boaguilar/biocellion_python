#include "biomodel.h"
#include <cmath>
#include <iomanip>

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

AgentSpeciesMolecule::AgentSpeciesMolecule( const S32& moleculeIdx, const S32& ODEIdx, const REAL& initialValue)
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

REAL AgentSpeciesMolecule::getInitialValue() const
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

void AgentSpeciesMolecule::setInitialValue( const REAL& value )
{
  mInitialValue = value;
}

/*********************************
 *  AgentSpeciesInteraction
 *********************************/

AgentSpeciesInteraction::AgentSpeciesInteraction( const S32& interactionIdx, const S32& modelRealIdx, const S32& mechRealIdx )
  : mInteractionIdx(interactionIdx), mModelRealIdx(modelRealIdx), mMechRealIdx(mechRealIdx)
{
  //empty
}

S32 AgentSpeciesInteraction::getInteractionIdx() const
{
  return mInteractionIdx;
}

S32 AgentSpeciesInteraction::getModelRealIdx() const
{
  return mModelRealIdx;
}

S32 AgentSpeciesInteraction::getMechRealIdx() const
{
  return mMechRealIdx;
}

void AgentSpeciesInteraction::setInteractionIdx( const S32& value )
{
  mInteractionIdx = value;
}

void AgentSpeciesInteraction::setModelRealIdx( const S32& value )
{
  mModelRealIdx = value;
}

void AgentSpeciesInteraction::setMechRealIdx( const S32& value )
{
  mMechRealIdx = value;
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
    mNumMechReals( 0 ), mNumODEVariables( 0 ), 
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
    mNumMechReals( 0 ), mNumODEVariables( 0 ), 
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

S32 AgentSpecies::getNumMechReals() const
{
  return mNumMechReals;
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

const Vector< ODENetwork >& AgentSpecies::getODENetworks( ) const {
  return mODENetworks;
}

Vector< ODENetwork >& AgentSpecies::getODENetworks( ) {
  return mODENetworks;
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

const Vector< AgentSpeciesInteraction >& AgentSpecies::getInteractions() const {
  return mInteractions;
}

Vector< AgentSpeciesInteraction >& AgentSpecies::getInteractions() {
  return mInteractions;
}

S32 AgentSpecies::globalInteractionIdxToLocalIdx( const S32& interactionIdx ) const {
  S32 idx = -1;
  for( idx = 0 ; idx < (S32) mInteractions.size( ) ; idx++ ) {
    if( mInteractions[ idx ].getInteractionIdx( ) == interactionIdx ) {
      break;
    }
  }
  return idx;
}

const Vector< S32 >& AgentSpecies::getODEReactions() const {
  return mODEReactions;
}

Vector< S32 >& AgentSpecies::getODEReactions() {
  return mODEReactions;
}

REAL AgentSpecies::getSurfaceArea( const SpAgentState& state ) const {
  if( false ) {
    OUTPUT( 0, "AgentSpecies::getSurfaceArea() "
            << " radius: " << state.getRadius( )
            << " radius^2: " << state.getRadius() * state.getRadius()
            << " PI: " << MODEL_PI
            << " area: " << 4.0 * MODEL_PI * ( state.getRadius() * state.getRadius() )
            );
  }
  return 4.0 * MODEL_PI * ( state.getRadius() * state.getRadius() );
}

REAL AgentSpecies::getGeometricVolume( const SpAgentState & state ) const {
  return ( 4.0 / 3.0 ) * MODEL_PI * ( state.getRadius() * state.getRadius() * state.getRadius() );
}

REAL AgentSpecies::getMassVolume( const SpAgentState & state ) const {
  REAL mass = 0.0;
  REAL volume = 0.0;
  S32 i;
  for( i = 0; i < (S32) mParticles.size( ); i++) {
    mass = state.getModelReal( mParticles[ i ].getModelRealIdx( ) );
    volume += mass / mModel->getParticles()[ mParticles[ i ].getParticleIdx() ]->getDensity( );
  }
  return volume;
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

void AgentSpecies::setNumMechReals(const S32& value)
{
  mNumMechReals = value;
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
    if( false ) {
      OUTPUT( 0, ""
	      << " ODEVar: " << gBioModel->getMolecules()[ mMolecules[ i ].getMoleculeIdx( ) ]->getName( )
	      << " mIdx: " << mMolecules[ i ].getMoleculeIdx( )
	      << " odeIdx: " << mMolecules[ i ].getODEIdx( )
	      << " init_value: " << mMolecules[ i ].getInitialValue( )
	      << " this: " << this
	      );
    }
    state.setODEVal( 0, mMolecules[ i ].getODEIdx( ), mMolecules[ i ].getInitialValue( ) );
  }
  // FIXMEE
  state.setModelInt( AGENT_SPECIES_INT_BOND_B, 0 );
  
}

void AgentSpecies::setNumODEVariables( const S32& numODEVariables ) {
  mNumODEVariables = numODEVariables;
}

REAL AgentSpecies::getInteractionValue( const S32& interactionIdx, const S32& dim, const SpAgentState& state ) const {
  S32 idx = globalInteractionIdxToLocalIdx( interactionIdx );
  CHECK( idx >= 0 );
  CHECK( idx < (S32) mInteractions.size( ) );
  CHECK( mInteractions[ idx ].getModelRealIdx( ) + dim >= 0 );
  CHECK( mInteractions[ idx ].getModelRealIdx( ) + dim < (S32) state.getModelRealArray( ).size( ) );
  return state.getModelReal( mInteractions[ idx ].getModelRealIdx( ) + dim );
}

void AgentSpecies::setInteractionValue( const S32& interactionIdx, const S32& dim, const REAL& value, SpAgentState& state ) const {
  S32 idx = globalInteractionIdxToLocalIdx( interactionIdx );
  CHECK( idx >= 0 );
  CHECK( idx < (S32) mInteractions.size( ) );
  CHECK( mInteractions[ idx ].getModelRealIdx( ) + dim >= 0 );
  CHECK( mInteractions[ idx ].getModelRealIdx( ) + dim < (S32) state.getModelRealArray( ).size( ) );
  state.setModelReal( mInteractions[ idx ].getModelRealIdx( ) + dim, value );
}

REAL AgentSpecies::getMechInteractionValue( const S32& interactionIdx, const S32& dim, const MechIntrctData& mechIntrctData ) const {
  S32 idx = globalInteractionIdxToLocalIdx( interactionIdx );
  if( BMD_DO_DEBUG( BMD_INTERACTION_OUTPUT ) ) {
    OUTPUT( 0, "AgentSpecies::getMechInteractionValue(): "
            << " interactionIdx: " << interactionIdx
            << " dim: " << dim
            << " local idx: " << idx
            << " local interaction size: " << mInteractions.size( )
            << " mechRealIdx: " << mInteractions[ idx ].getMechRealIdx( )
            << " mechRealSize: " << mechIntrctData.getModelRealArray( ).size( )
            << " >= 0: " << ( mInteractions[ idx ].getMechRealIdx( ) + dim >= 0 )
            );
  }

  CHECK( idx >= 0 );
  CHECK( idx < (S32) mInteractions.size( ) );
  CHECK( mInteractions[ idx ].getMechRealIdx( ) + dim >= 0 );
  CHECK( mInteractions[ idx ].getMechRealIdx( ) + dim < (S32) mechIntrctData.getModelRealArray( ).size( ) );
  return mechIntrctData.getModelReal( mInteractions[ idx ].getMechRealIdx( ) + dim );
}

void AgentSpecies::setMechInteractionValue( const S32& interactionIdx, const S32& dim, const REAL& value, MechIntrctData& mechIntrctData ) const {
  S32 idx = globalInteractionIdxToLocalIdx( interactionIdx );
  if( BMD_DO_DEBUG( BMD_INTERACTION_OUTPUT ) ) {
    OUTPUT( 0, "AgentSpecies::setMechInteractionValue(): "
            << " interactionIdx: " << interactionIdx
            << " dim: " << dim
            << " value: " << value
            << " local idx: " << idx
            << " local interaction size: " << mInteractions.size( )
            << " mechRealIdx: " << mInteractions[ idx ].getMechRealIdx( )
            << " mechRealSize: " << mechIntrctData.getModelRealArray( ).size( )
            << " >= 0: " << ( mInteractions[ idx ].getMechRealIdx( ) + dim >= 0 )
            );
  }

  CHECK( idx >= 0 );
  CHECK( idx < (S32) mInteractions.size( ) );
  CHECK( mInteractions[ idx ].getMechRealIdx( ) + dim >= 0 );
  CHECK( mInteractions[ idx ].getMechRealIdx( ) + dim < (S32) mechIntrctData.getModelRealArray( ).size( ) );
  mechIntrctData.setModelReal( mInteractions[ idx ].getMechRealIdx( ) + dim, value );
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

  const Vector< Reaction *>& reactions = mModel->getReactions();
  for( i = 0; i < (S32) mODEReactions.size(); i++ ) {
    REAL kinetic_factor;
    REAL yield;
    const Reaction* currentReaction = reactions[ mODEReactions[ i ] ];
    kinetic_factor = currentReaction->getKineticFactor( nbrUBEnv, spAgent, v_y );
    for( j = 0; j < (S32) mMolecules.size(); j++ ) {
      yield = currentReaction->getMoleculeYield( mMolecules[ j ].getMoleculeIdx( ), spAgent );
      // time step is not used here.  The ODE solver does time steps for us.
      v_f[ mMolecules[ j ].getODEIdx() ] += kinetic_factor * yield; // FIXME: unit analysis.

      if( BMD_DO_DEBUG( BMD_CRNODERHS_DETAILS ) ) { // enable in biomodel_debug.h
        // Display one term in the ODE

        S32 ii;
        // all Kinetic Factors for this term
        std::stringstream kf;
        kf << std::setprecision(9);
        kf << "  KF: ";
        for( ii = 0 ; ii < (S32) currentReaction->getKineticFactors( ).size( ) ; ii++ ) {
          REAL concentration_value = 0.0;
          if( currentReaction->getKineticFactors( )[ ii ]->isSolute( ) ) {
            concentration_value = gBioModel->getSubgridValue( currentReaction->getKineticFactors( )[ ii ]->getSoluteIdx( ), nbrUBEnv, spAgent.vOffset );
            kf << " KineticFactor: " << *(currentReaction->getKineticFactors( )[ ii ])
               << " solute: " << gBioModel->getSolutes( )[ currentReaction->getKineticFactors( )[ ii ]->getSoluteIdx( ) ]->getName( )
               << " concentration: " << concentration_value
               << " value: " << currentReaction->getKineticFactors( )[ ii ]->kineticValue( concentration_value );
          } else if ( currentReaction->getKineticFactors( )[ ii ]->isMolecule( ) ) {
            concentration_value = gBioModel->getAgentSpecies()[ spAgent.state.getType() ]->getMoleculeValue( currentReaction->getKineticFactors( )[ ii ]->getMoleculeIdx( ), spAgent.state, v_y );
            kf << " KineticFactor: " << *(currentReaction->getKineticFactors( )[ ii ])
               << " molecule: " << gBioModel->getMolecules( )[ currentReaction->getKineticFactors( )[ ii ]->getMoleculeIdx( ) ]->getName( )
               << " concentration: " << concentration_value
               << " value: " << currentReaction->getKineticFactors( )[ ii ]->kineticValue( concentration_value );
          } else if ( currentReaction->getKineticFactors( )[ ii ]->isAgent( ) ) {
            kf << " KineticFactor: " << *(currentReaction->getKineticFactors( )[ ii ])
               << " agent: " << gBioModel->getAgentSpecies( )[ spAgent.state.getType( ) ]->getName( )
               << " value: " << currentReaction->getKineticFactors( )[ ii ]->kineticValueAgent( spAgent.state );
          } else if( currentReaction->getKineticFactors( )[ ii ]->isNone( ) ) {
            kf << " KineticFactor: " << *(currentReaction->getKineticFactors( )[ ii ])
               << " none: " 
               << " value: " << currentReaction->getKineticFactors( )[ ii ]->kineticValue( concentration_value );
          } else {
            ERROR( "Should be unreachable." );
          }
        }

        // The yield for this term
        std::stringstream molecularYieldStream;
        molecularYieldStream << std::setprecision(9);
        Reaction::Yield molecule_yield;
        if( currentReaction->getYieldForMolecule( mMolecules[ j ].getMoleculeIdx( ), molecule_yield ) ) {
          molecularYieldStream << "Yield: Value: " << molecule_yield.getValue( ) << " "
                               << "isSolute: " << (int)molecule_yield.isSolute( ) << " "
                               << "isParticle: " << (int)molecule_yield.isParticle( ) << " "
                               << "isMolecule: " << (int)molecule_yield.isMolecule( ) << " ";
        } else {
          molecularYieldStream << "No Molecule Yield ";
        }

        OUTPUT( 0, ""
                << std::setprecision(9)
                << "reaction: " << currentReaction->getName( ) << " "
                << "molecule: " << gBioModel->getMolecules( )[ mMolecules[ j ].getMoleculeIdx( ) ]->getName( ) << " "
                << "kinetic_factor: " << kinetic_factor << " "
                << "yield: " << yield << " "
                << "delta: " << kinetic_factor * yield << " "
                << molecularYieldStream.str( )
                << " muMax: " << currentReaction->getMuMax( )
                << " KF: " << kf.str( )
                );
      }
    }
  }
 
  if ( false ) {
    for( i = 0; i < (S32) mMolecules.size(); i++ ) {
      OUTPUT( 0,
              "Molecule Index : " << mMolecules[ i ].getMoleculeIdx( ) << " \n"
              << "v_y value : " << v_y[ i ] << " \n"
              << "v_f value : " << v_f[ i ] << " \n"
              );
    }
  }
}

void AgentSpecies::updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state) const {

  const Vector< Reaction * >& reactions = mModel->getReactions( );
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
              "Agent particle: " << mModel->getParticles()[ mParticles[ pIdx ].getParticleIdx( ) ]->getName()
              << " mass: " << state.getModelReal( mParticles[ pIdx ].getModelRealIdx( ) )
              );
    }
  }

  // update bond with agar.
  if ( getParamReal( getIdxReal( SPECIES_tightJunctionToBoundaryStrength ) )  > 0.0 ) {
      REAL R0 = getParamReal( getIdxReal( SPECIES_shoveFactor) ) *state.getRadius(); 
      REAL x=((REAL)vIdx[0] +0.5)*mModel->getAgentGrid().getResolution() + vOffset[0];

      if (state.getModelInt(AGENT_SPECIES_INT_BOND_B)==0){
         if ( x < R0*getParamReal(getIdxReal(SPECIES_attachToBoundaryCreateFactor))){
            state.setModelInt(AGENT_SPECIES_INT_BOND_B,1);
         }
      }
      else{
         if ( x > R0*getParamReal(getIdxReal(SPECIES_attachToBoundaryDestroyFactor))){
            state.setModelInt(AGENT_SPECIES_INT_BOND_B,0);
         }
      }     
  }
  
}


void AgentSpecies::updateSpAgentRadius( SpAgentState& state ) const {

  //calculate new volume based off of updated mass
  REAL volume = getMassVolume( state );
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
  divide  = false;
  if ( spAgent.state.getRadius() >= getParamReal( getIdxReal( SPECIES_divRadius ) ) ) {
    divide = true;
  } 
  else if ( spAgent.state.getRadius() <=  getParamReal( getIdxReal( SPECIES_deathRadius ) )){
    disappear = true ;
  }
}

/*
**************************************** ADJUSTSPAGENT BEGIN ****************************************************
*/

void AgentSpecies::brownianMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state, VReal& disp ) const {

  for( S32 dim = 0 ; dim < 3 ; dim++ ) { // FIXME:  Use computation domain grid for dimension
    if( dim == 3-1 ) { 
      continue; 
    }
    // FIXME: Unit analysis.  Find cDynoMiCS brownian motion for comparison
    // diffusivity = um2.hour-1
    REAL diffusivity = getParamReal( getIdxReal( SPECIES_diffusivity ) );
    // dt = hour
    REAL dt = mModel->getAgentTimeStep( );
    REAL dx = sqrt( 2.0 * diffusivity * dt ) * Util::getModelRand( MODEL_RNG_GAUSSIAN );
    if( mModel->getInteractions( )[ INTERACTION_diffusion ]->getParamBool( INTERACTION_writeOutput ) ) {
      setInteractionValue( mModel->getInteractions( )[ INTERACTION_diffusion ]->getInteractionIdx( ), dim, dx, state );
    }
    disp[ dim ] += dx;
  }

}

void AgentSpecies::limitMotion(VReal& disp) const {

  /* limit the maximum displacement within a single time step */
  // FIXME: Should use resolution or radius?
  for( S32 dim = 0 ; dim < DIMENSION ; dim++ ) {
    if( disp[ dim ] > mModel->getAgentGrid( ).getResolution( ) ) {
      disp[ dim ] = mModel->getAgentGrid( ).getResolution( );
    }
    else if( disp[ dim ] < ( mModel->getAgentGrid( ).getResolution( ) * -1.0 ) ) {
      disp[ dim ] = mModel->getAgentGrid( ).getResolution( ) * -1.0;
    }
    if( false ) {
      OUTPUT( 0, "limited disp[ " << dim <<" ]: " << disp[ dim ] );
    }
  }

}

void AgentSpecies::setDisplacementFromMechanicalInteraction( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {
  
  S32 dim;
  REAL dt = mModel->getAgentTimeStep( ); 
  for( dim = 0 ; dim < 3 ; dim ++ ) {
    disp[ dim ] = mechIntrctData.getModelReal( mIdxMechForceReals[ dim ] ) * dt * 0.055 ;
    if( false ) {
      OUTPUT( 0, "disp[ " << dim <<" ]: " << disp[ dim ] );
    }
  }

  // Copy any desired mechIntrctData to modelReal, for output
  S32 i;
  REAL value;
  const Vector< Interaction* >& interactions = mModel->getInteractions( );
  for( i = 0 ; i < (S32) interactions.size( ) ; i++ ) {
    if( interactions[ i ]->getParamBool( interactions[ i ]->getIdxBool( INTERACTION_writeOutput ) ) ) {
      S32 idx = globalInteractionIdxToLocalIdx( interactions[ i ]->getInteractionIdx( ) );
      if( mInteractions[ idx ].getMechRealIdx( ) < 0 ) {
        continue;
      }

      for( dim = 0 ; dim < 3 ; dim++ ) {
        value = getMechInteractionValue( interactions[ i ]->getInteractionIdx( ), dim, mechIntrctData );
        setInteractionValue( interactions[ i ]->getInteractionIdx( ), dim, value, state );
      }
    }
  }

  // Force due to Bond with agar
  if (state.getModelInt( AGENT_SPECIES_INT_BOND_B ) == 1) {

       REAL x=((REAL)vIdx[0]+0.5)*mModel->getAgentGrid( ).getResolution( )  + vOffset[0];

       REAL xij = getParamReal( getIdxReal( SPECIES_shoveFactor) ) *state.getRadius() - x;

       REAL mag ;
       mag = xij * tanh( FABS(xij) *  getParamReal( getIdxReal( SPECIES_tightJunctionToBoundaryStrength ) )  );
       disp[0] += mag*dt*0.11 ;  // stress
  }  
}

void AgentSpecies::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {

  if( getParamBool( getIdxBool( SPECIES_fixed ) ) ) {
    S32 i;
    for( i = 0 ; i < 3 ; i++ ) {
      disp[ i ] = 0.0;
    }
    return;
  }
  setDisplacementFromMechanicalInteraction( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );  
  brownianMotion( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  adjustSpAgentChemotaxis( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
  limitMotion(disp);
  perturbAgents( vIdx, vOffset, state );

  
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

static inline void findChemoTaxisDirectionAndConcentration( const S32 elemIdx,  const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, const REAL& alpha, VReal& dir, REAL& delta ) {

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
  // this results in the maximum value asymptotically approaching 1/alpha
  // alpha = fg.um-3
  // fwdVal = fg.um-3
  // bckVal = fg.um-3
  fwdVal = fwdVal / (1.0 + alpha * fwdVal);
  bckVal = bckVal / (1.0 + alpha * bckVal);

  // delta = fg.um-3
  delta = fwdVal - bckVal;
  dir = fwdDir;
}


void AgentSpecies::adjustSpAgentChemotaxis( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {
  S32 i;
  for( i = 0 ; i < (S32) mChemotaxis.size( ) ; i++ ) {
    VReal dir;
    REAL delta;
    
    // contact inhibition
    if( mChemotaxis[ i ]->getContactInhibition( ) > 0 &&
        countTouches( junctionData ) >= mChemotaxis[ i ]->getContactInhibition( ) ) {
      return;
    }
    
    findChemoTaxisDirectionAndConcentration( mChemotaxis[ i ]->getSolute( ),  vOffset, nbrUBEnv, state, mChemotaxis[ i ]->getAlpha( ), dir, delta );
    if( delta > 0 ) {
      if( false ) {
        OUTPUT( 0, " Chemotaxis: magnitude: " << delta );
      }
      // strength = um.hour-1.fg-1.um3
      // delta = fg.um-3
      VReal chemDisp = dir * ( mChemotaxis[ i ]->getStrength( ) * delta );
      // FIXME: Unit analysis and cDynoMiCS comparison
      // chemDisp = um.hour-1
      // dt = hour
      // disp = um
      disp += chemDisp * mModel->getAgentTimeStep( );
      if( mModel->getInteractions( )[ INTERACTION_chemotaxis ]->getParamBool( INTERACTION_writeOutput ) ) {
        S32 dim;
        for( dim = 0 ; dim < 3 ; dim++ ) {
          setInteractionValue( mModel->getInteractions( )[ INTERACTION_chemotaxis ]->getInteractionIdx( ), dim, chemDisp[ dim ] * mModel->getAgentTimeStep( ), state );
        }
      }
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
  REAL originalVolume = getMassVolume( motherState );
  REAL originalDensity = 0;
  
  daughterState.setType( motherState.getType( ) );
  for( i = 0; i < (S32) mParticles.size(); i++) {
    motherMass = motherState.getModelReal( mParticles[ i ].getModelRealIdx( ) );
    REAL newMass = motherMass * motherFraction;
    motherState.setModelReal( mParticles[ i ].getModelRealIdx( ), newMass );
    daughterState.setModelReal( mParticles[ i ].getModelRealIdx( ), motherMass - newMass );
  }

  REAL newMotherVolume = getMassVolume( motherState );
  REAL newDaughterVolume = getMassVolume( daughterState );
  
  for( i = 0; i < (S32) mMolecules.size() ; i++ ) {
    originalDensity = motherState.getODEVal( 0, mMolecules[ i ].getODEIdx( ) );
    REAL originalMass = originalDensity * originalVolume;
    REAL newMotherMass = originalMass * motherFraction;
    REAL newDaughterMass = originalMass - newMotherMass;
    REAL newMotherDensity = newMotherMass / newMotherVolume;
    REAL newDaughterDensity = newDaughterMass / newDaughterVolume;
    motherState.setODEVal( 0, mMolecules[ i ].getODEIdx( ), newMotherDensity );
    daughterState.setODEVal( 0, mMolecules[ i ].getODEIdx( ), newDaughterDensity );
    if( false ) {
      OUTPUT( 0,
              "Original Density : " << originalDensity << "\n"
              << "New Mother density : " << newMotherDensity << "\n"
              << "New Daughter density : " << newDaughterDensity << "\n"
              << "Original Mass : " << originalMass << "\n"
              << "Mother Mass : " << newMotherMass << "\n"
              << "Daughter Mass : " << newDaughterMass << "\n"
              << "Original volume : " << originalVolume << "\n"
              << "New Mother volume : " << newMotherVolume << "\n"
              << "New Daughter volume : " << newDaughterVolume << "\n"	    
              << "Mother " << mModel->getMolecules( )[ i ]->getName() << ": "
              << motherState.getODEVal( 0, mMolecules[ i ].getODEIdx( ) ) << "\n"
              << "Daughter " << mModel->getMolecules( )[ i ]->getName() << ": "
              << daughterState.getODEVal( 0, mMolecules[ i ].getODEIdx( ) ) << "\n"
              );
    }
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

void AgentSpecies::perturbAgents(const VIdx& vIdx, const VReal& vOffset, SpAgentState& state) const {
  S32 j ;
  for( j = 0 ; j < (S32)mEntryConditions.size( ) ; j++ ) {
      mEntryConditions[ j ]->perturbAgents( vIdx, vOffset, state  );
  }

  return;
}

/**********************************************
 * support for model_routine_config.cpp
 **********************************************/
void AgentSpecies::updateSpAgentInfo( SpAgentInfo& spAgentInfo ) const {
  
  MechModelVarInfo mechModelVarInfo;
  mechModelVarInfo.syncMethod = VAR_SYNC_METHOD_DELTA;

  // FIXME: DMax not controlled from XML yet
  spAgentInfo.dMax = getDMax();
  CHECK( spAgentInfo.dMax <= mModel->getAgentGrid().getResolution( ) );
  // FIXME: num*ModelVars not controlled from XML yet
  spAgentInfo.numBoolVars = getNumModelBools( );
  spAgentInfo.numStateModelReals = getNumModelReals( );
  spAgentInfo.numStateModelInts = getNumModelInts( );
  if( getNumMechReals( ) > 0 ) {
    spAgentInfo.v_mechIntrctModelRealInfo.assign( getNumMechReals(), mechModelVarInfo );
  } else {
    spAgentInfo.v_mechIntrctModelRealInfo.clear( );
  }
  if( getNumMechModelInts( ) > 0 ) {
    spAgentInfo.v_mechIntrctModelIntInfo.assign( getNumMechModelInts( ), mechModelVarInfo );
  } else {
    spAgentInfo.v_mechIntrctModelIntInfo.clear();
  }
  if( getNumODEVariables( ) > 0 ) {
    // Currently, only 1 ODE Network is allowed.
    CHECK( mODENetworks.size( ) > 0 ); 
    CHECK( mODENetworks.size( ) < 2 );

    ODENetInfo odeNetInfo;
    odeNetInfo.numVars = getNumODEVariables( );
    odeNetInfo.stiff = mODENetworks[ 0 ].getStiffness( );
    odeNetInfo.h = mODENetworks[ 0 ].getParamReal( mODENetworks[ 0 ].getIdxReal( ODE_initStepSize ) );
    odeNetInfo.hm = mODENetworks[ 0 ].getParamReal( mODENetworks[ 0 ].getIdxReal( ODE_minimumStepSize ) );
    odeNetInfo.epsilon = mODENetworks[ 0 ].getParamReal( mODENetworks[ 0 ].getIdxReal( ODE_epsilon ) );
    odeNetInfo.threshold = mODENetworks[ 0 ].getParamReal( mODENetworks[ 0 ].getIdxReal( ODE_threshold ) );
    odeNetInfo.errorThresholdVal = 0.0;
    odeNetInfo.warningThresholdVal = 0.0;
    odeNetInfo.setNegToZero = false;
    
    spAgentInfo.v_odeNetInfo.assign( 1, odeNetInfo );
  } else {
    spAgentInfo.v_odeNetInfo.clear( );
  }
}

/**********************************************
 * support for model_routine_output.cpp
 **********************************************/
void AgentSpecies::updateSpAgentOutput( const VIdx& vIdx, const SpAgent& spAgent, REAL& color, Vector<REAL>& v_extraScalar, Vector<VReal>& v_extraVector ) const {

  color = spAgent.state.getType();

  S32 i, j, dim;
  S32 scalar_count = 0;

  const Vector< Particle* >& particles = mModel->getParticles( );
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

  const Vector< Molecule* >& molecules = mModel->getMolecules( );
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
  
  const Vector< Interaction* >& interactions = mModel->getInteractions( );
  for( i = 0 ; i < (S32) interactions.size( ) ; i++ ) {
    if( interactions[ i ]->getParamBool( interactions[ i ]->getIdxBool( INTERACTION_writeOutput ) ) ) {
      for( dim = 0 ; dim < 3 ; dim++ ) {
        v_extraScalar[ scalar_count ] = getInteractionValue( interactions[ i ]->getInteractionIdx( ), dim, spAgent.state );
        scalar_count ++;
      }
    }
  }

  v_extraVector[0][0] = spAgent.state.getRadius();
  v_extraVector[0][1] = spAgent.state.getRadius();
  v_extraVector[0][2] = spAgent.state.getRadius();
  v_extraVector[1][0] = 0.0;
  v_extraVector[1][1] = 0.0;
  v_extraVector[1][2] = 0.0;
  
  CHECK( v_extraScalar.size() == (size_t) scalar_count );
  CHECK( v_extraVector.size() == 2 );

}
