#ifndef _TIGHT_JUNCTION_H
#define _TIGHT_JUNCTION_H
#include "biocellion.h"

class TightJunction {
public:
  TightJunction( );
  TightJunction(const REAL& stiffness, const S32& withSpecies);
  REAL getStiffness() const { return mStiffness; };
  S32 getWithSpecies() const { return mWithSpecies; };
  REAL getScale() const { return mScale; };
  void setStiffness(const REAL& stiffness);
  void setWithSpecies(const S32& withSpecies);
  void setScale(const REAL& scale);
protected:
  REAL mStiffness;
  S32 mWithSpecies;
  REAL mScale;
};

#endif /*_TIGHT_JUNCTION_H*/
/* Local Variables: */
/* mode:c++         */
/* End:             */
