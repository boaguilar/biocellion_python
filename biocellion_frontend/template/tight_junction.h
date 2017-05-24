#ifndef _TIGHT_JUNCTION_H
#define _TIGHT_JUNCTION_H
#include "biocellion.h"
#include <string>

class TightJunction {
public:
  TightJunction(const REAL& stiffness, const std::string& withSpecies);
  REAL getStiffness() const { return mStiffness; };
  std::string getWithSpecies() const { return mWithSpecies; };
  void setStiffness(const REAL& stiffness);
  void setWithSpecies(const std::string& withSpecies);
protected:
  REAL mStiffness;
  std::string mWithSpecies;
};

#endif /*_TIGHT_JUNCTION_H*/
/* Local Variables: */
/* mode:c++         */
/* End:             */
