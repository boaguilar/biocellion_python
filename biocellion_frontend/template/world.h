#ifndef _WORLD_H
#define _WORLD_H
class World;

#include "biomodel.h"
#include <iostream>

class World {
public:
  World();
  const Bulk* getBulk( const S32& bulkIdx ) const;
  const Vector <Bulk *>& getBulks() const { return mBulks; };
  Vector <Bulk *>& getBulks() { return mBulks; };
  const Vector <Agar *>& getAgars() const { return mAgars; };
  Vector <Agar *>& getAgars() { return mAgars; };
  const Vector <ComputationDomain* >& getComputationDomains( ) const { return mComputationDomains; };
  Vector <ComputationDomain* >& getComputationDomains( ) { return mComputationDomains; };

protected:
  Vector < Bulk* > mBulks;
  Vector < Agar* > mAgars;
  Vector < ComputationDomain* > mComputationDomains;
};

std::ostream& operator<<( std::ostream& os, const World& world );

#endif /*_WORLD_H*/
