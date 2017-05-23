#ifndef _REACTION_H_
#define _REACTION_H_
#include "biocellion.h"
#include <string>

class Reaction
{
public:
  Reaction(
		   const std::string& catalyzedBy, const std::string& reactionClass,
		   const std::string& name
           );
  struct yield
  {
    REAL mValue;
    std::string mYieldName;
  };
  std::string getCatalyzedBy() const { return mCatalyzedBy; };
  std::string getReactionClass() const { return mReactionClass; };
  std::string getName() const { return mName; };
  std::string getKineticFactorClass() const { return mKineticFactorClass; };
  std::string getTimeUnit() const { return mTimeUnit; };
  REAL getMuMax() const { return mMuMax;};
  void setCatalyzedBy(const std::string& catalyzedBy); 
  void setReactionClass(const std::string& reactionClass);
  void setName(const std::string& name);
  void setKineticFactorClass(const std::string& kineticFactorClass);
  void setTimeUnit(const std::string& timeUnit);
  void setMuMax(const REAL& muMax);
  const Vector<yield>& getReactionYield() const { return mReactionYield; };
  void addYield(const REAL& value, const std::string& yieldName);
protected:
  std::string mCatalyzedBy;
  std::string mReactionClass;
  std::string mName;
  std::string mKineticFactorClass;
  std::string mTimeUnit;
  REAL mMuMax;
  Vector<yield> mReactionYield;
};


#endif /* _REACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
