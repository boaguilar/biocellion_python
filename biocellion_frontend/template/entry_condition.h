#ifndef _ENTRY_CONDITION_H
#define _ENTRY_CONDITION_H
#include "biomodel.h"
class EntryCondition : public ParamHolder {
public:
  EntryCondition();
  S32 getType() const { return mType; };
  S32 getItemIdx() const { return mItemIdx; };
  S32 getFromSpecies() const { return mFromSpecies; };  
  void setType(const S32& type);
  void setItemIdx(const S32& ItemIdx);
  void setFromSpecies(const S32& FromSpecies);

protected:
  S32 mType;
  S32 mItemIdx;
  S32 mFromSpecies;
};

#endif /*_ENTRY_CONDITION_H*/
