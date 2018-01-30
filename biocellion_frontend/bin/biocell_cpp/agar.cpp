#include "biomodel.h"

Agar::Agar()
  :mName(""), mAgarIdx(0)
{
  //empty  
}

void Agar::setName(const std::string& name)
{
  mName = name;
}

void Agar::setAgarIdx(const S32& agarIdx)
{
  mAgarIdx = agarIdx;
}
