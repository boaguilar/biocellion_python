#include "biomodel.h"

Particle::Particle(const S32& id, const std::string& name, const REAL& density)
  : ParamHolder( ), mParticleIdx( id ), mName( name ), mDensity( density )
{
  //empty
}

S32 Particle:: getParticleIdx() const
{
  return mParticleIdx;
}

std::string Particle::getName() const 
{ 
  return mName; 
}
 
REAL Particle::getDensity() const
{
  return mDensity;
}
 
void Particle::setParticleIdx(const S32& id)
{
  mParticleIdx = id;
}

void Particle::setName(const std::string& name)
{
  mName = name;
}

void Particle::setDensity(const REAL& density)
{
  mDensity = density;
}
