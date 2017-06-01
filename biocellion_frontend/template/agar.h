#ifndef _AGAR_H_
#define _AGAR_H_
#include "biomodel.h"
#include <string>

class Agar : public ParamHolder {
public:
  Agar();
  std::string getName() const { return mName; };
  S32 getAgarIdx() const { return mAgarIdx; };
  void setName(const std::string& name);
  void setAgarIdx(const S32& AgarIdx);
protected:
  std::string mName;
  S32 mAgarIdx;
};

#endif /*_AGAR_H_*/
