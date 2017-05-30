#ifndef _SOLUTE_H_
#define _SOLUTE_H_
#include "biocellion.h"
#include "param_holder.h"
#include <string>

class Solute : public ParamHolder {
public:
  Solute();
  Solute(const std::string& name, const S32& solute_idx, const S32& domain_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);

  const std::string& getName() const;
  S32 getSoluteIdx() const;
  S32 getDomainIdx() const;

  void setName(const std::string& name);
  void setSoluteIdx(const S32& idx);
  void setDomainIdx(const S32& idx);
  
protected:
  std::string mName;
  S32 mSoluteIdx;
  S32 mDomainIdx;
  
};

class BulkSolute {
public:
  BulkSolute();
  BulkSolute(const REAL& SBulk, const REAL& sin, const BOOL& isConstant, const REAL& sPulse, const REAL& pulseRate);
  REAL getSBulk() const { return mSBulk; }
  REAL getSin() const { return mSin; };
  BOOL getIsConstant() const { return mIsConstant; };
  REAL getSPulse() const { return mSPulse; };
  REAL getPulseRate() const { return mPulseRate; };
  void setSBulk(const REAL& SBulk);
  void setSin(const REAL& sin);
  void setIsConstant(const BOOL& isConstant);
  void setSPulse(const REAL& sPulse);
  void setPulseRate(const REAL& pulseRate);
protected:
  REAL mSBulk;
  REAL mSin;
  BOOL mIsConstant;
  REAL mSPulse;
  REAL mPulseRate;
};

#endif /* _SOLUTE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
