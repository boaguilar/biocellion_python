#include "biocellion.h"
#include "particle.h"

Particle::Particle(const S32& id, const std::string& name, const REAL& density)
  : mId(id), mName(name), mDensity(density)
{
  //empty
}

S32 Particle:: getId() const
{
  return mId;
}

std::string Particle::getName() const 
{ 
  return mName; 
}
 
REAL Particle::getDensity() const
{
  return mDensity;
}
 
void Particle::setId(const S32& id)
{
  mId = id;
}

void Particle::setName(const std::string& name)
{
  mName = name;
}

void Particle::setDensity(const REAL& density)
{
  mDensity = density;
}
