#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "biocellion.h"
#include <string>

class Particle {
public:
  Particle(const S32& id, const std::string& name, const REAL& density); 
  S32 getId() const;
  std::string getName() const; 
  REAL getDensity() const;
  void setId(const S32& id);
  void setName(const std::string& name);
  void setDensity(const REAL& density);

protected:
  S32 mId;
  std::string mName;
  REAL mDensity;
};


#endif /* _PARTICLE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
