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

BOOL Reaction::Yield::isMolecule( ) const {
  return mType ==  TYPE_MOLECULE;
}


void Reaction::Yield::setSolute( ) {
  mType = TYPE_SOLUTE;
}

void Reaction::Yield::setParticle( ) {
  mType = TYPE_PARTICLE;
}

void Reaction::Yield::setMolecule( ) {
  mType = TYPE_MOLECULE;
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
    mSpeciesIdx( -1 ),
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

S32 Reaction::KineticFactor::getSoluteIdx( ) const {
  return mSoluteIdx;
}

S32 Reaction::KineticFactor::getMoleculeIdx( ) const {
  return mMoleculeIdx;
}

S32 Reaction::KineticFactor::getSpeciesIdx( ) const {
  return mSpeciesIdx;
}

REAL Reaction::KineticFactor::getKi( ) const {
  return mKi;
}

REAL Reaction::KineticFactor::getKs( ) const {
  return mKs;
}

BOOL Reaction::KineticFactor::isSolute( ) const {
  return mSoluteIdx >= 0 && mMoleculeIdx < 0;
}

BOOL Reaction::KineticFactor::isMolecule( ) const {
  return mSoluteIdx < 0 && mMoleculeIdx >= 0;
}

BOOL Reaction::KineticFactor::isAgent( ) const {
  return mSpeciesIdx >= 0;
}

BOOL Reaction::KineticFactor::isNone( ) const {
  return mSoluteIdx < 0 && mMoleculeIdx < 0 && mSpeciesIdx < 0;
}

void Reaction::KineticFactor::setClass(const std::string& value) {
  mKineticFactorClass = value;
}

void Reaction::KineticFactor::setSoluteIdx( const S32& value ) {
  mSoluteIdx = value;
}

void Reaction::KineticFactor::setMoleculeIdx( const S32& value ) {
  mMoleculeIdx = value;
}

void Reaction::KineticFactor::setSpeciesIdx( const S32& value ) {
  mSpeciesIdx = value;
}

void Reaction::KineticFactor::setKi( const REAL& value) {
  mKi = value;
}

void Reaction::KineticFactor::setKs( const REAL& value) {
  mKs = value;
}

REAL Reaction::KineticFactor::kineticValueAgent( const SpAgentState& state ) const {
  if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR_DETAIL ) ) {
    OUTPUT( 0, "Reaction::KineticFactor::kineticValueAgent() "
            << " radius: " << state.getRadius( )
            );
  }
  return 0;
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
  if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR_DETAIL ) ) {
    OUTPUT( 0, "Reaction::LinearKinetic::kineticValue() "
            << " Ks: " << mKs
            << " solute_value: " << solute_value
            << " result: " << mKs * solute_value
            );
  }
  return mKs * solute_value;
}

Reaction::KineticPermeability::KineticPermeability( )
  : mPermeability(0)
{
  //empty
}

REAL Reaction::KineticPermeability::getPermeability() const {
  return mPermeability;
};

void Reaction::KineticPermeability::setPermeability( const REAL& permeability ) {
  mPermeability = permeability;
};

REAL Reaction::KineticPermeability::kineticValue( const REAL& solute_value ) const {
  return mPermeability;
}

REAL Reaction::KineticAgentSurfaceArea::kineticValue( const REAL& solute_value ) const {
  ERROR( "KineticAgentSurfaceArea::kineticValue() should not be called.  Ever." );
  return 0;
}

REAL Reaction::KineticAgentSurfaceArea::kineticValueAgent( const SpAgentState& state ) const {
  if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR_DETAIL ) ) {
    OUTPUT( 0, "Reaction::KineticAgentSurfaceArea::kineticValueAgent() "
            << " radius: " << state.getRadius( )
            );
  }
  return gBioModel->getAgentSpecies()[ state.getType() ]->getSurfaceArea( state ) / gBioModel->getAgentSpecies()[ state.getType() ]->getGeometricVolume( state );
}

std::ostream& operator<<( std::ostream& os, const Reaction::KineticFactor& rhs ) {
  os << rhs.getClass( ) << "," << rhs.getSoluteIdx( ) << "," << rhs.getMoleculeIdx( ) << "," << rhs.getSpeciesIdx( ) << "," << rhs.getKi( ) << "," << rhs.getKs( );
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

BOOL Reaction::getYieldForMolecule( const S32& moleculeIdx, Yield& yield ) const {
  S32 i;
  for( i = 0; i < (S32) mYields.size(); i++ ) {
    yield = mYields[ i ];
    return true;
  }
  return false;
}


const Vector< Reaction::KineticFactor* >& Reaction::getKineticFactors( ) const {
  return mKineticFactors;
}

Vector< Reaction::KineticFactor* >& Reaction::getKineticFactors( ) {
  return mKineticFactors;
}

const Vector< BOOL >& Reaction::getActiveAgentSpecies( ) const {
  return mActiveAgentSpecies;
}

Vector< BOOL >& Reaction::getActiveAgentSpecies( ) {
  return mActiveAgentSpecies;
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

REAL Reaction::getKineticFactor( const NbrUBEnv& nbrUBEnv, const VReal& vOffset, SpAgentState& state ) const {
  /* Called by:
   *   AgentSpecies::updateSpAgentState()
   */
  REAL factor = mMuMax;
  REAL concentration_value = 0;
  S32 i;
  for( i = 0 ; i < (S32) mKineticFactors.size( ) ; i++ ) {
    if( mKineticFactors[ i ]->isSolute( ) ) {
      concentration_value = gBioModel->getSubgridValue( mKineticFactors[ i ]->getSoluteIdx( ), nbrUBEnv, vOffset );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " solute: " << gBioModel->getSolutes( )[ mKineticFactors[ i ]->getSoluteIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if( mKineticFactors[ i ]->isMolecule( ) ) {
      concentration_value = gBioModel->getAgentSpecies()[ state.getType() ]->getMoleculeValue( mKineticFactors[ i ]->getMoleculeIdx( ), state );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " molecule: " << gBioModel->getMolecules( )[ mKineticFactors[ i ]->getMoleculeIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if ( mKineticFactors[ i ]->isAgent( ) ) {
      factor *= mKineticFactors[ i ]->kineticValueAgent( state );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " agent: " << gBioModel->getAgentSpecies( )[ state.getType( ) ]->getName( )
                << " value: " << mKineticFactors[ i ]->kineticValueAgent( state )
                );
      }
    } else if( mKineticFactors[ i ]->isNone( ) ) {
      concentration_value = 0;
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " none: " 
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else {
      ERROR( "Should be unreachable." );
    }
  }
  return factor;
}

REAL Reaction::getKineticFactor( const UBEnv& ubEnv, const VReal& vOffset ) const {
  /* Called by:
   *   Solute::updateIfSubgridRHSLinear()  // non-agent related
   */
  REAL factor = mMuMax;
  REAL concentration_value = 0;
  S32 i;
  for( i = 0 ; i < (S32) mKineticFactors.size( ) ; i++ ) {
    if( mKineticFactors[ i ]->isSolute( ) ) {
      concentration_value = gBioModel->getSubgridValue( mKineticFactors[ i ]->getSoluteIdx( ), ubEnv, vOffset );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " solute: " << gBioModel->getSolutes( )[ mKineticFactors[ i ]->getSoluteIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if( mKineticFactors[ i ]->isMolecule( ) ) {
      ERROR( "Molecular kinetic factors not allowed here." );
    } else if ( mKineticFactors[ i ]->isAgent( ) ) {
      ERROR( "Agent kinetic factors not allowed here." );
    } else if ( mKineticFactors[ i ]->isNone( ) ) {
      concentration_value = 0;
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " none: " 
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else {
      ERROR( "Should be unreachable." );
    }
  }
  return factor;
}

REAL Reaction::getKineticFactor( const NbrUBEnv& nbrUBEnv, const SpAgent& spAgent, const Vector< double >& v_y ) const {
  /* Called by:
   *   AgentSpecies::spAgentCRNODERHS()
   */
  REAL factor = mMuMax;
  REAL concentration_value = 0.0;
  S32 i;
  for( i = 0; i < (S32) mKineticFactors.size(); i++ ) {
    if( mKineticFactors[ i ]->isSolute( ) ) {
      concentration_value = gBioModel->getSubgridValue( mKineticFactors[ i ]->getSoluteIdx( ), nbrUBEnv, spAgent.vOffset );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " solute: " << gBioModel->getSolutes( )[ mKineticFactors[ i ]->getSoluteIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if ( mKineticFactors[ i ]->isMolecule( ) ) {
      concentration_value = gBioModel->getAgentSpecies()[ spAgent.state.getType() ]->getMoleculeValue( mKineticFactors[ i ]->getMoleculeIdx( ), spAgent.state, v_y );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " molecule: " << gBioModel->getMolecules( )[ mKineticFactors[ i ]->getMoleculeIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if ( mKineticFactors[ i ]->isAgent( ) ) {
      factor *= mKineticFactors[ i ]->kineticValueAgent( spAgent.state );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " agent: " << gBioModel->getAgentSpecies( )[ spAgent.state.getType( ) ]->getName( )
                << " value: " << mKineticFactors[ i ]->kineticValueAgent( spAgent.state )
                );
      }
    } else if( mKineticFactors[ i ]->isNone( ) ) {
      concentration_value = 0;
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " none: " 
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else {
      ERROR( "Should be unreachable." );
    }
  }
  return factor;
}

REAL Reaction::getKineticFactor( const UBEnv& ubEnv, const VReal& vOffset, const SpAgent& spAgent ) const {
  /* Called by:
   *   Solute::updateIfSubgridRHSLinear() // agent related
   */
  
  REAL factor = mMuMax;
  REAL concentration_value = 0;
  S32 i;
  for( i = 0 ; i < (S32) mKineticFactors.size( ) ; i++ ) {
    if( mKineticFactors[ i ]->isSolute( ) ) {
      concentration_value = gBioModel->getSubgridValue( mKineticFactors[ i ]->getSoluteIdx( ), ubEnv, vOffset );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " solute: " << gBioModel->getSolutes( )[ mKineticFactors[ i ]->getSoluteIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if( mKineticFactors[ i ]->isMolecule( ) ) {
      concentration_value = gBioModel->getAgentSpecies()[ spAgent.state.getType() ]->getMoleculeValue( mKineticFactors[ i ]->getMoleculeIdx( ), spAgent.state );
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " molecule: " << gBioModel->getMolecules( )[ mKineticFactors[ i ]->getMoleculeIdx( ) ]->getName( )
                << " concentration: " << concentration_value
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else if ( mKineticFactors[ i ]->isAgent( ) ) {
      factor *= mKineticFactors[ i ]->kineticValueAgent( spAgent.state );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " agent: " << gBioModel->getAgentSpecies( )[ spAgent.state.getType( ) ]->getName( )
                << " value: " << mKineticFactors[ i ]->kineticValueAgent( spAgent.state )
                );
      }
    } else if( mKineticFactors[ i ]->isNone( ) ) {
      concentration_value = 0;
      factor *= mKineticFactors[ i ]->kineticValue( concentration_value );
      if( BMD_DO_DEBUG( BMD_KINETIC_FACTOR ) ) {
        OUTPUT( 0, ""
                << " KineticFactor: " << *(this->mKineticFactors[ i ])
                << " none: " 
                << " value: " << mKineticFactors[ i ]->kineticValue( concentration_value )
                );
      }
    } else {
      ERROR( "Should be unreachable." );
    }
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
  if( ( getCatalyst( ) == -1 || getCatalyst( ) == agentType ) &&
      mActiveAgentSpecies[ agentType ] ) {
    // Agent's type matches catalyst, and agent's type is active
    for( i = 0; i < (S32) mYields.size( ) ; i++ ) {
      if( mYields[ i ].isSolute( ) && mYields[ i ].getItemIdx( ) == solute_idx ) {
        // Yield matches solute
        const Vector< AgentSpeciesParticle >& particles = gBioModel->getAgentSpecies( )[ agentType ]->getParticles( );
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


REAL Reaction::getMoleculeYield( const S32& moleculeIdx, const SpAgent& spAgent ) const {
  REAL yield = 0.0;
  S32 i;
  for( i = 0; i < (S32) mYields.size(); i++ ) {
    const Yield& currentYield = mYields[ i ];
    if( currentYield.isMolecule() && currentYield.getItemIdx() == moleculeIdx ) {
      yield += currentYield.getValue();
    }
  }
  return yield;
}

/*
 * Finds the total yield this agent produces for this particle_idx, due
 * to this reaction.
 */
REAL Reaction::getParticleYield( const S32& particle_idx, const SpAgentState& spAgentState ) const {
  S32 i, j;
  REAL yield = 0.0;
  S32 agentType = spAgentState.getType( );
  if( getCatalyst( ) == -1 || getCatalyst( ) == agentType ) {
    // Agent's type matches catalyst
    for( i = 0; i < (S32) mYields.size( ) ; i++ ) {
      if( mYields[ i ].isParticle( ) && mYields[ i ].getItemIdx( ) == particle_idx ) {
        // Yield matches solute
        const Vector< AgentSpeciesParticle >& particles = gBioModel->getAgentSpecies( )[ agentType ]->getParticles( );
        for( j = 0 ; j < (S32) particles.size( ) ; j++ ) {
          if( particles[ j ].getParticleIdx( ) == getCatalyzedBy( ) ) {
            // Agent's particle matches catalyzed by.
            REAL mass = spAgentState.getModelReal( particles[ j ].getModelRealIdx( ) );
            yield += mYields[ i ].getValue( ) * mass;
          } // if particle matches catalyzedby
        } // for all particles in agent
      } // if yield is correct particle
    } // for all yields
  } // catalyst matches
  return yield;
}
