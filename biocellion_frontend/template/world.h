#ifndef _WORLD_H
#define _WORLD_H
class World;

#include "biomodel.h"

class World{
public:
  World();
  const Vector <Bulk *> getBulks() const { return Bulks; };
  Vector <Bulk *> getBulks() { return Bulks; };
  const Vector <Agar *> getAgars() const { return Agars; };
  Vector <Agar *> getAgars() { return Agars; };
  const Vector <ComputationDomain* > getComputationDomains( ) const { return ComputationDomains; };
  Vector <ComputationDomain* > getComputationDomains( ) { return ComputationDomains; };

protected:
  Vector < Bulk* > Bulks;
  Vector < Agar* > Agars;
  Vector < ComputationDomain* > ComputationDomains;
};

#endif /*_WORLD_H*/
