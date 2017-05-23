#include "reaction.h"
#include "biocellion.h"
#include <string>

Reaction::Reaction(
		   const std::string& catalyzedBy, const std::string& reactionClass,
		   const std::string& name
		   )
  : mCatalyzedBy(catalyzedBy), mReactionClass(reactionClass), mName(name)
{
  //empty
}
void Reaction::setCatalyzedBy(const std::string& catalyzedBy)
{
  mCatalyzedBy = catalyzedBy;
}
void Reaction::setReactionClass(const std::string& reactionClass)
{
  mReactionClass = reactionClass;
}
void Reaction::setName(const std::string& name)
{
  mName = name;
}
void Reaction::setKineticFactorClass(const std::string& kineticFactorClass)
{
  mKineticFactorClass = kineticFactorClass;
}
void Reaction::setTimeUnit(const std::string& timeUnit)
{
  mTimeUnit = timeUnit;
}
void Reaction::setMuMax(const REAL& muMax)
{
  mMuMax = muMax;
}

void Reaction::addYield(const REAL& value, const std::string& yieldName)
{
  yield temp;
  temp.mValue = value;
  temp.mYieldName = yieldName;
  mReactionYield.push_back(temp);
}



