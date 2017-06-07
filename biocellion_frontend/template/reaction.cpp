#include "biomodel.h"

// Reaction::Yield

Reaction::Yield::Yield( )
  : mType( TYPE_SOLUTE ),
    mItemIdx( -1 ),
    mValue( 0.0 )
{
}

BOOL Reaction::Yield::isSolute( ) const {
  return mType == TYPE_SOLUTE;
}

BOOL Reaction::Yield::isParticle( ) const {
  return mType == TYPE_PARTICLE;
}

S32 Reaction::Yield::getType( ) const {
  return mType;
}

S32 Reaction::Yield::getItemIdx( ) const {
  return mItemIdx;
}

REAL Reaction::Yield::getValue( ) const {
  return mValue;
}


void Reaction::Yield::setSolute( ) {
  mType = TYPE_SOLUTE;
}

void Reaction::Yield::setParticle( ) {
  mType = TYPE_PARTICLE;
}

void Reaction::Yield::setType( const S32& value ) {
  if( value == TYPE_SOLUTE || value == TYPE_PARTICLE ) {
    mType = value;
  } else {
    ERROR( "Bad Reaction::Yield::mType " << value )
  }
}

void Reaction::Yield::setItemIdx( const S32& value ) {
  mItemIdx = value;
}

void Reaction::Yield::setValue( const REAL& value) {
  mValue = value;
}

std::ostream& operator<<( std::ostream& os, const Reaction::Yield& rhs ) {
  os << rhs.getType( ) << "," << rhs.getItemIdx( ) << "," << rhs.getValue( );
  return os;
}

// Reaction::KineticFactor

Reaction::KineticFactor::KineticFactor( )
  : mKineticFactorClass( "" ),
    mSoluteIdx( -1 ),
    mMoleculeIdx( -1 ),
    mKi( 0.0 ),
    mKs( 0.0 )
{
}

Reaction::KineticFactor::~KineticFactor( ) {
  // empty
}

const std::string& Reaction::KineticFactor::getClass( ) const {
  return mKineticFactorClass;
}

S32 Reaction::KineticFactor::getSolute( ) const {
  return mSoluteIdx;
}

S32 Reaction::KineticFactor::getMolecule( ) const {
  return mMoleculeIdx;
}

REAL Reaction::KineticFactor::getKi( ) const {
  return mKi;
}

REAL Reaction::KineticFactor::getKs( ) const {
  return mKs;
}

void Reaction::KineticFactor::setClass(const std::string& value) {
  mKineticFactorClass = value;
}

void Reaction::KineticFactor::setSolute( const S32& value ) {
  mSoluteIdx = value;
}

void Reaction::KineticFactor::setMolecule( const S32& value ) {
  mMoleculeIdx = value;
}

void Reaction::KineticFactor::setKi( const REAL& value) {
  mKi = value;
}

void Reaction::KineticFactor::setKs( const REAL& value) {
  mKs = value;
}

REAL Reaction::FirstOrderKinetic::kineticValue( const REAL& solute_value ) const {
  return 1.0;
}

REAL Reaction::SimpleInhibition::kineticValue( const REAL& solute_value ) const {
  if( ( mKi + solute_value ) < 1.0e-7 ) {
    return 0.0;
  }
  return mKi / ( mKi + solute_value );
}

REAL Reaction::MonodKinetic::kineticValue( const REAL& solute_value ) const {
  if( ( mKs + solute_value ) < 1.0e-7 ) {
    return 0.0;
  }
  return solute_value / ( mKs + solute_value );
}

REAL Reaction::LinearKinetic::kineticValue( const REAL& solute_value ) const {
  return mKs * solute_value;
}

std::ostream& operator<<( std::ostream& os, const Reaction::KineticFactor& rhs ) {
  os << rhs.getClass( ) << "," << rhs.getSolute( ) << "," << rhs.getMolecule( ) << "," << rhs.getKi( ) << "," << rhs.getKs( );
  return os;
}
std::ostream& operator<<( std::ostream& os, const Reaction::FirstOrderKinetic& rhs ) {
  return os << dynamic_cast< const Reaction::KineticFactor& >( rhs );
}
std::ostream& operator<<( std::ostream& os, const Reaction::SimpleInhibition& rhs ) {
  return os << dynamic_cast< const Reaction::KineticFactor& >( rhs );
}
std::ostream& operator<<( std::ostream& os, const Reaction::MonodKinetic& rhs ) {
  return os << dynamic_cast< const Reaction::KineticFactor& >( rhs );
}
std::ostream& operator<<( std::ostream& os, const Reaction::LinearKinetic& rhs ) {
  return os << dynamic_cast< const Reaction::KineticFactor& >( rhs );
}

// Reaction
Reaction::Reaction( )
  : mName( "" ),
    mReactionClass( "" ),
    mCatalyzedBy( -1 ),
    mCatalyst( -1 ),
    mMuMax( 0.0 )
{
  // empty
}

Reaction::Reaction( const std::string& name, const std::string& reactionClass,
                    const S32& catalyzedBy, const S32& catalyst, const REAL& muMax
                    )
  : mName( name ),
    mReactionClass( reactionClass ),
    mCatalyzedBy( catalyzedBy ),
    mCatalyst( catalyst ),
    mMuMax( muMax )
{
  // empty
}

Reaction::~Reaction( ) {
  S32 i;
  for( i = 0 ; i < (S32) mKineticFactors.size( ) ; i++ ) {
    if( mKineticFactors[ i ] ) {
      delete mKineticFactors[ i ];
      mKineticFactors[ i ] = 0;
    }
  }
  mKineticFactors.clear( );
}

const Vector< Reaction::Yield >& Reaction::getYields( ) const {
  return mYields;
}

Vector< Reaction::Yield >& Reaction::getYields( ) {
  return mYields;
}

const Vector< Reaction::KineticFactor* >& Reaction::getKineticFactors( ) const {
  return mKineticFactors;
}

Vector< Reaction::KineticFactor* >& Reaction::getKineticFactors( ) {
  return mKineticFactors;
}


void Reaction::setName(const std::string& value) {
  mName = value;
}

void Reaction::setClass(const std::string& value) {
  mReactionClass = value;
}

void Reaction::setCatalyzedBy(const S32& value) {
  mCatalyzedBy = value;
}
 
void Reaction::setCatalyst(const S32& value) {
  mCatalyst = value;
}
 
void Reaction::setMuMax(const REAL& value) {
  mMuMax = value;
}

REAL Reaction::getKineticFactor( const UBEnv& ubEnv, const VIdx& subgridVOffset ) const {
  REAL factor = mMuMax;
  REAL solute_value = 0;
  S32 i;
  for( i = 0 ; i < (S32) mKineticFactors.size( ) ; i++ ) {
    if( mKineticFactors[ i ]->getSolute( ) >= 0 ) {
      solute_value = gBioModel->getSubgridValue( mKineticFactors[ i ]->getSolute( ), ubEnv, subgridVOffset );
    } else if( mKineticFactors[ i ]->getMolecule( ) >= 0 ) {
      ERROR( "unimplemented" );
    } else if( mKineticFactors[ i ]->getSolute( ) == -1 && mKineticFactors[ i ]->getMolecule( ) == -1 ) {
      solute_value = 0;
    } else {
      ERROR( "Should be unreachable." );
    }
    factor *= mKineticFactors[ i ]->kineticValue( solute_value );
  }
  return factor;
}

/*
 * Assumes that spAgent is within the subgrid container in question.
 * Finds the total yield this agent produces for this solute, due
 * to this reaction.
 */
REAL Reaction::getSoluteYield( const S32& solute_idx, const SpAgent& spAgent ) const {
  S32 i, j;
  REAL yield = 0.0;
  S32 agentType = spAgent.state.getType( );
  if( getCatalyst( ) == -1 || getCatalyst( ) == agentType ) {
    // Agent's type matches catalyst
    for( i = 0; i < (S32) mYields.size( ) ; i++ ) {
      if( mYields[ i ].isSolute( ) && mYields[ i ].getItemIdx( ) == solute_idx ) {
        // Yield matches solute
        const Vector< AgentSpeciesParticle >& particles = gAgentSpecies[ agentType ]->getParticles( );
        for( j = 0 ; j < (S32) particles.size( ) ; j++ ) {
          if( particles[ j ].getParticleIdx( ) == getCatalyzedBy( ) ) {
            // Agent's particle matches catalyzed by.
            REAL mass = spAgent.state.getModelReal( particles[ j ].getModelRealIdx( ) );
            yield += mYields[ i ].getValue( ) * mass;
          } // if particle matches catalyzedby
        } // for all particles of agent
      } // if yield matches solute_idx
    } // for all yields
  } // if agent matches catalyst
  return yield;
}

/*
 * Finds the total yield this agent produces for this particle_idx, due
 * to this reaction.
 */
REAL Reaction::getParticleYield( const S32& particle_idx, const SpAgent& spAgent ) const {
  S32 i, j;
  REAL yield = 0.0;
  S32 agentType = spAgent.state.getType( );
  if( getCatalyst( ) == -1 || getCatalyst( ) == agentType ) {
    // Agent's type matches catalyst
    for( i = 0; i < (S32) mYields.size( ) ; i++ ) {
      if( mYields[ i ].isParticle( ) && mYields[ i ].getItemIdx( ) == particle_idx ) {
        // Yield matches solute
        const Vector< AgentSpeciesParticle >& particles = gAgentSpecies[ agentType ]->getParticles( );
        for( j = 0 ; j < (S32) particles.size( ) ; j++ ) {
          if( particles[ j ].getParticleIdx( ) == getCatalyzedBy( ) ) {
            // Agent's particle matches catalyzed by.
            REAL mass = spAgent.state.getModelReal( particles[ j ].getModelRealIdx( ) );
            yield += mYields[ i ].getValue( ) * mass;
          } // if particle matches catalyzedby
        } // for all particles in agent
      } // if yield is correct particle
    } // for all yields
  } // catalyst matches
  return yield;
}
