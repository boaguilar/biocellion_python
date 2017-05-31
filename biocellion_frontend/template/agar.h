#ifndef _AGAR_H_
#define _AGAR_H_
#include "biocellion.h"
#include "param_holder.h"
#include <string>

class Agar : public ParamHolder {
public:
  Agar();
  std::string getName() const { return mName; };
  S32 getAgarIndex() const { return mAgarIndex; };
  void setName(const std::string& name);
  void setAgarIndex(const S32& AgarIndex);
protected:
  std::string mName;
  S32 mAgarIndex;
  S32 mDepth;
  S32 mWidth;
  S32 mHeight;
  REAL mConcentration;
  REAL D;
  S32 T;
  S32 mUpdateFreq;
};

#endif /*_AGAR_H_*/
