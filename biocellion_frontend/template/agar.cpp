#include "agar.h"

Agar::Agar()
  :mName(""), mAgarIndex(0)
{
  //empty  
}

void Agar::setName(const std::string& name)
{
  mName = name;
}

void Agar::setAgarIndex(const S32& agarIndex)
{
  mAgarIndex = agarIndex;
}
