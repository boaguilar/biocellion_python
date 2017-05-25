#ifndef _ADHESION_H
#define _ADHESION_H
#include "biocellion.h"

class Adhesion
{
public:
  Adhesion( );
  Adhesion(const REAL& strength, const S32& withSpecies, const REAL& scale);
  REAL getStrength() const { return mStrength; };
  S32 getWithSpecies() const { return mWithSpecies; };
  REAL getScale() const { return mScale; };
  void setStrength(const REAL& strength);
  void setWithSpecies(const S32& withSpecies);
  void setScale(const REAL& scale);
protected:
  REAL mStrength;
  S32 mWithSpecies;
  REAL mScale;
};

#endif /* _ADHESION_H */
/* Local Variables: */
/* mode:c++         */
/* End:             */
