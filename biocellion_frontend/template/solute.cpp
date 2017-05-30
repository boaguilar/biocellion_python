#include "solute.h"


Solute::Solute()
  : ParamHolder( ),
    mName( "" ), mSoluteIdx( -1 ), mDomainIdx( -1 )
{
  //empty
}

Solute::Solute(const std::string& name, const S32& solute_idx, const S32& domain_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param) 
  : ParamHolder( num_real_param, num_int_param, num_bool_param, num_string_param ),
    mName( name ), mSoluteIdx( solute_idx ), mDomainIdx( domain_idx )
{

}

const std::string& Solute::getName() const {
  return mName;
}

S32 Solute::getSoluteIdx() const {
  return mSoluteIdx;
}

S32 Solute::getDomainIdx() const {
  return mDomainIdx;
}

void Solute::setName(const std::string& name) {
  mName = name;
}

void Solute::setSoluteIdx(const S32& idx) {
  mSoluteIdx = idx;
}

void Solute::setDomainIdx(const S32& idx) {
  mDomainIdx = idx;
}



BulkSolute::BulkSolute()
  :mSBulk(0), mSin(0), mIsConstant(false), mSPulse(0), mPulseRate(0)
{
  //empty
}
BulkSolute::BulkSolute(const REAL& SBulk, const REAL& sin, const BOOL& isConstant, const REAL& sPulse, const REAL& pulseRate)
 :mSBulk(SBulk), mSin(sin), mIsConstant(isConstant), mSPulse(sPulse), mPulseRate(pulseRate)
{
  //empty
}
void BulkSolute::setSBulk(const REAL& SBulk)
{
  mSBulk = SBulk;
}
void BulkSolute::setSin(const REAL& sin)
{
  mSin = sin;
}
void BulkSolute::setIsConstant(const BOOL& isConstant)
{
  mIsConstant = isConstant;
}
void BulkSolute::setSPulse(const REAL& sPulse)
{
  mSPulse = sPulse;
}
void BulkSolute::setPulseRate(const REAL& pulseRate)
{
  mPulseRate = pulseRate;
}
