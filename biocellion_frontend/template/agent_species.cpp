#include "biomodel.h"

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
 *  AgentSpecies
 *********************************/

AgentSpecies::AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param)
  : mName(name), mSpeciesName(speciesName), mSpeciesIdx(species_idx),
    mParamsReal(num_real_param),
    mParamsInt(num_int_param),
    mParamsBool(num_bool_param),
    mParamsString(num_string_param),
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

REAL AgentSpecies::getParamReal(const S32& idx) const
{
  return mParamsReal[idx];
}

S32 AgentSpecies::getParamInt(const S32& idx) const
{
  return mParamsInt[idx];
}

BOOL AgentSpecies::getParamBool(const S32& idx) const
{
  return mParamsBool[idx];
}

const std::string& AgentSpecies::getParamString(const S32& idx) const
{
  return mParamsString[idx];
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

template <class T, class U>
S32 getIdxGeneric( const std::string& param_name, std::map<std::string, T>& idx_map, const U& size )
{
  S32 i = -1;
  typename std::map<std::string, T>::const_iterator it = idx_map.find(param_name);
  if( it != idx_map.end() ) {
    i = it->second;
  } else {
    for( it = idx_map.begin(); it != idx_map.end(); it++ ) {
      if( it->second > i ) {
        i = it->second;
      }
    }
    i++;
    idx_map[ param_name ] = i;
  }
  CHECK( i >= 0 );
  CHECK( ((U)i) < size );
  return i;
}

S32 AgentSpecies::getIdxReal(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxReal, mParamsReal.size() );
}

S32 AgentSpecies::getIdxInt(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxInt, mParamsInt.size() );
}

S32 AgentSpecies::getIdxBool(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxBool, mParamsBool.size() );
}

S32 AgentSpecies::getIdxString(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxString, mParamsString.size() );
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

void AgentSpecies::setParamReal(const S32& idx, const REAL& param)
{
  mParamsReal[idx] = param;
}

void AgentSpecies::setParamInt(const S32& idx, const S32& param)
{
  mParamsInt[idx] = param;
}

void AgentSpecies::setParamBool(const S32& idx, const BOOL& param)
{
  mParamsBool[idx] = param;
}

void AgentSpecies::setParamString(const S32& idx, const std::string& param)
{
  mParamsString[idx] = param;
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

void AgentSpecies::setInitialAgentState( SpAgentState& state ) const {
  state.setType( mSpeciesIdx );
  S32 i;
  REAL volume = 0.0;
  for( i = 0 ; i < (S32)mParticles.size() ; i++ ) {
    state.setModelReal( mParticles[ i ].getModelRealIdx(), mParticles[ i ].getInitialValue() );
    volume += mParticles[ i ].getInitialValue() / gParticles[ mParticles[ i ].getParticleIdx() ]->getDensity( );
  }
  REAL radius = cbrt( 3.0 * volume / (4.0 * MODEL_PI ) );

  // if radius is too big, then agent-agent interactions will be missed.
  if( false ) {
    OUTPUT( 0, ""
            << " radius: " << radius
            << " mDMax: " << mDMax
            << " mDMax/3.0: " << mDMax/3.0
            );
  }
  CHECK( radius <= mDMax / 3.0 );

  state.setRadius( radius );
}

void AgentSpecies::updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state ) const {
  const Vector< Reaction * >& reactions = gBioModel->getReactions( );
  S32 i, pIdx;
  for( i = 0 ; i < (S32) reactions.size( ) ; i++ ) {
    const Reaction* currentReaction = reactions[ i ];
    const REAL currentKineticFactor = currentReaction->getKineticFactor( nbrUBEnv, vOffset );
    for ( pIdx = 0; pIdx < (S32) mParticles.size( ); pIdx++) {
      REAL yield = currentReaction->getParticleYield( mParticles[ pIdx ].getParticleIdx( ) , state );
      state.incModelReal( mParticles[ pIdx ].getModelRealIdx( ) , currentKineticFactor * yield );
    }
  }

  for ( pIdx = 0; pIdx < (S32) mParticles.size( ); pIdx++) {
    OUTPUT( 0,
            "Agent particle: " << gParticles[ mParticles[ pIdx ].getParticleIdx( ) ]->getName()
            << " mass: " << state.getModelReal( mParticles[ pIdx ].getModelRealIdx( ) )
            );
  }
  
}

void AgentSpecies::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const {
  adjustSpAgentChemotaxis( vIdx, junctionData, vOffset, mechIntrctData, nbrUBEnv, state, disp );
}

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

static inline void findChemoTaxisDirectionAndConcentration( const S32 elemIdx,  const VReal& vOffset, const NbrUBEnv& nbrUBEnv, const SpAgentState& state, VReal& dir, REAL& delta ) {

  // Random direction of motion
  VReal fwdDir, bckDir;
  randomUnitVector( fwdDir );
  bckDir = VReal::ZERO - fwdDir;

  // Find solute concentration just ahead and just behind agent
  REAL offset_magnitude = 1.42 * gAgentGrid->getResolution( ) / gBioModel->getSolutes( )[ elemIdx ]->getSubgridDimension( );
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
      disp += chemDisp;
    }
  }
}
/*
 **************************************** CHEMOTAXIS END ****************************************************
 */
