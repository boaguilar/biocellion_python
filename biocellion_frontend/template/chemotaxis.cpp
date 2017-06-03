#include "biomodel.h"

Chemotaxis::Chemotaxis( )
  :mWithSolute(0), mStrength(0), mContactInhibition(0)
{
  //empty
}
void Chemotaxis::setWithSolute(const S32& withSolute)
{
  mWithSolute = withSolute;
}
void Chemotaxis::setStrength(const REAL& strength)
{
  mStrength = strength;
}
void Chemotaxis::setContactInhibition(const S32& contactInhibition)
{
  mContactInhibition = contactInhibition;
}
