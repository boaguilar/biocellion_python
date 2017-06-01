#include "biomodel.h"

AgentGrid::AgentGrid() 
  :
  mComputationDomain( "" ),
  mResolution( 4 ),
  mShovingFraction( 0.01 ),
  mShovingMaxIter( 100 ),
  mShovingMutual( true ),
  mErosionMethod( false ),
  mSloughDetachedBiomass( false)
{
}

std::string AgentGrid::getComputationDomain( ) const {
  return mComputationDomain;
}

REAL AgentGrid::getResolution( ) const {
  return mResolution;
}

REAL AgentGrid::getShovingFraction( ) const {
  return mShovingFraction;
}

S32 AgentGrid::getShovingMaxIter( ) const {
  return mShovingMaxIter;
}

BOOL AgentGrid::getShovingMutual( ) const {
  return mShovingMutual;
}

BOOL AgentGrid::getErosionMethod( ) const {
  return mErosionMethod;
}

BOOL AgentGrid::getSloughDetachedBiomass( ) const {
  return mSloughDetachedBiomass;
}


void AgentGrid::setComputationDomain( const std::string& value ) {
  mComputationDomain = value;
}

void AgentGrid::setResolution( const REAL& value ) {
  mResolution = value;
}

void AgentGrid::setShovingFraction( const REAL& value ) {
  mShovingFraction = value;
}

void AgentGrid::setShovingMaxIter( const S32& value ) {
  mShovingMaxIter = value;
}

void AgentGrid::setShovingMutual( const BOOL& value ) {
  mShovingMutual = value;
}

void AgentGrid::setErosionMethod( const BOOL& value ) {
  mErosionMethod = value;
}

void AgentGrid::setSloughDetachedBiomass( const BOOL& value ) {
  mSloughDetachedBiomass = value;
}

