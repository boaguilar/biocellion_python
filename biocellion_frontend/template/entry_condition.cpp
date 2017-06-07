#include "biomodel.h"

EntryCondition::EntryCondition()
  :mType(0), mItemIdx(0), mFromSpecies(0)
{
  //empty
}

void EntryCondition::setType(const S32& type)
{
  mType = type;
}    

void EntryCondition::setItemIdx(const S32& ItemIdx)
{
  mItemIdx = ItemIdx;
}

void EntryCondition::setFromSpecies(const S32& FromSpecies)
{
  mFromSpecies = FromSpecies;
}
