#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "biomodel.h"

class Particle: public ParamHolder {
public:
  Particle(const S32& id, const std::string& name, const REAL& density);
  
  S32 getParticleIdx( ) const;
  std::string getName( ) const; 
  REAL getDensity( ) const;
  
  void setParticleIdx( const S32& id );
  void setName( const std::string& name );
  void setDensity( const REAL& density );

protected:
  S32 mParticleIdx;
  std::string mName;
  REAL mDensity;
};


#endif /* _PARTICLE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
