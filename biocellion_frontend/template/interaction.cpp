#include "biomodel.h"

Interaction::Interaction()
  : ParamHolder( ),
    mName( "" ), mInteractionIdx( -1 )
{
}

Interaction::Interaction( const std::string& name, const S32& interaction_idx ) 
  : ParamHolder( ),
    mName( name ), mInteractionIdx( interaction_idx )
{
}

const std::string& Interaction::getName() const {
  return mName;
}

S32 Interaction::getInteractionIdx() const {
  return mInteractionIdx;
}

void Interaction::setName(const std::string& name) {
  mName = name;
}

void Interaction::setInteractionIdx(const S32& idx) {
  mInteractionIdx = idx;
}
