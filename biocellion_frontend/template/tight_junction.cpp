#include "biocellion.h"
#include "tight_junction.h"

TightJunction::TightJunction(const REAL& stiffness, const std::string& withSpecies)
  :mStiffness(stiffness), mWithSpecies(withSpecies)
{
  //empty
}

void TightJunction::setStiffness(const REAL& stiffness)
{
  mStiffness = stiffness;
}

void TightJunction::setWithSpecies(const std::string& withSpecies)
{
  mWithSpecies = withSpecies;
}
