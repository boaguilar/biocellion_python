#ifndef _SOLUTE_H
#define _SOLUTE_H
#include "biocellion.h"

class Solute {
public:
  Solute();
  Solute(const S32& domain, const REAL& diffusivity, const REAL& airDiffusivity, const REAL& decayRate, const BOOL& writeOutput);
  S32 getDomain() const { return mDomain; };
  REAL getDiffusivity() const { return mDiffusivity; };
  REAL getAirDiffusivity() const { return mAirDiffusivity; };
  REAL getDecayRate() const { return mDecayRate; };
  BOOL getWriteOutput() const { return mWriteOutput; };
  void setDomain(const S32& domain);
  void setDiffusivity(const REAL& diffusivity);
  void setAirDiffusivity(const REAL& airDiffusivity);
  void setDecayRate(const REAL& decayRate);
  void setWriteOutput(const BOOL& writeOutput);
  
protected:
  S32 mDomain;
  REAL mDiffusivity;
  REAL mAirDiffusivity;
  REAL mDecayRate;
  BOOL mWriteOutput;
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

#endif /*_SOLUTE_H*/
