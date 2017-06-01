#include "biomodel.h"

TightJunction::TightJunction( )
  :mStiffness(0), mWithSpecies(0)
{
  //empty
}

TightJunction::TightJunction(const REAL& stiffness, const S32& withSpecies)
  :mStiffness(stiffness), mWithSpecies(withSpecies)
{
  //empty
}

void TightJunction::setStiffness(const REAL& stiffness)
{
  mStiffness = stiffness;
}

void TightJunction::setWithSpecies(const S32& withSpecies)
{
  mWithSpecies = withSpecies;
}

void TightJunction::setScale(const REAL& scale)
{
  mScale = scale;
}
