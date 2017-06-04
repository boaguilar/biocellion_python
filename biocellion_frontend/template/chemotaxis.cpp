#include "biomodel.h"

Chemotaxis::Chemotaxis( )
  :mSolute(0), mStrength(0), mContactInhibition(0)
{
  //empty
}
void Chemotaxis::setSolute(const S32& solute)
{
  mSolute = solute;
}
void Chemotaxis::setStrength(const REAL& strength)
{
  mStrength = strength;
}
void Chemotaxis::setContactInhibition(const S32& contactInhibition)
{
  mContactInhibition = contactInhibition;
}
