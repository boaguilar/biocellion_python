#include "biomodel.h"

DistanceJunction::DistanceJunction( )
  : mEnabled(false), mWithSpecies(0)
{
  //empty
}

void DistanceJunction::setEnabled(const BOOL& value)
{
  mEnabled = value;
}
void DistanceJunction::setWithSpecies(const S32& withSpecies)
{
  mWithSpecies = withSpecies;
}
