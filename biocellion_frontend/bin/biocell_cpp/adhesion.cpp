#include "biomodel.h"

Adhesion::Adhesion( )
  :mStrength(0), mWithSpecies(0), mScale( 1.0 )
{
  //empty
}

Adhesion::Adhesion(const REAL& strength, const S32& withSpecies, const REAL& scale)
  :mStrength(strength), mWithSpecies(withSpecies), mScale(scale)
{
  //empty
}
void Adhesion::setStrength(const REAL& strength)
{
  mStrength = strength;
}
void Adhesion::setWithSpecies(const S32& withSpecies)
{
  mWithSpecies = withSpecies;
}
void Adhesion::setScale(const REAL& scale)
{
  mScale = scale;
}
