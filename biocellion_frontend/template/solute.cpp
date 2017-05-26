#include "solute.h"


Solute::Solute()
  :mDomain(0), mDiffusivity(0), mAirDiffusivity(0), mDecayRate(0), mWriteOutput(false)
{
  //empty
}
Solute::Solute(const S32& domain, const REAL& diffusivity, const REAL& airDiffusivity, const REAL& decayRate, const BOOL& writeOutput)
  :mDomain(domain), mDiffusivity(diffusivity), mAirDiffusivity(airDiffusivity), mDecayRate(decayRate), mWriteOutput(writeOutput)
{

}
void Solute::setDomain(const S32& domain)
{
  mDomain = domain;
}
void Solute::setDiffusivity(const REAL& diffusivity)
{
  mDiffusivity = diffusivity;
}
void Solute::setAirDiffusivity(const REAL& airDiffusivity)
{
  mAirDiffusivity = airDiffusivity;
}
void Solute::setDecayRate(const REAL& decayRate)
{
  mDecayRate = decayRate;
}
void Solute::setWriteOutput(const BOOL& writeOutput)
{
  mWriteOutput = writeOutput; 
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
