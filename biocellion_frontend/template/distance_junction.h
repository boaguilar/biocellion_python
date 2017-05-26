#ifndef _DISTANCE_JUNCTION_H_
#define _DISTANCE_JUNCTION_H_
#include "biocellion.h"

class DistanceJunction
{
public:
  DistanceJunction( );
  BOOL getEnabled() const { return mEnabled; };
  S32 getWithSpecies() const { return mWithSpecies; };
  void setEnabled(const BOOL& value);
  void setWithSpecies(const S32& withSpecies);
protected:
  BOOL mEnabled;
  S32 mWithSpecies;
};

#endif /* _DISTANCE_JUNCTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
