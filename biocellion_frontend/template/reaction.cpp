#include "reaction.h"
#include "biocellion.h"
#include <string>

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

// Reaction::KineticFactor

Reaction::KineticFactor::KineticFactor( )
  : mKineticFactorClass( "" ),
    mSoluteIdx( -1 ),
    mMoleculeIdx( -1 ),
    mKi( 0.0 ),
    mKs( 0.0 )
{
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
  // empty
}

const Vector< Reaction::Yield >& Reaction::getYields( ) const {
  return mYields;
}

Vector< Reaction::Yield >& Reaction::getYields( ) {
  return mYields;
}

const Vector< Reaction::KineticFactor >& Reaction::getKineticFactors( ) const {
  return mKineticFactors;
}

Vector< Reaction::KineticFactor >& Reaction::getKineticFactors( ) {
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

