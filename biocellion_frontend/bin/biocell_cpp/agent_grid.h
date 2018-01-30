#ifndef _AGENT_GRID_H_
#define _AGENT_GRID_H_

#include "biomodel.h"
#include <string>

class AgentGrid {
  
public:
  AgentGrid();

  std::string getComputationDomain( ) const;
  REAL getResolution( ) const;
  REAL getShovingFraction( ) const;
  S32 getShovingMaxIter( ) const;
  BOOL getShovingMutual( ) const;
  BOOL getErosionMethod( ) const;
  BOOL getSloughDetachedBiomass( ) const;

  void setComputationDomain( const std::string& value );
  void setResolution( const REAL& value );
  void setShovingFraction( const REAL& value );
  void setShovingMaxIter( const S32& value );
  void setShovingMutual( const BOOL& value );
  void setErosionMethod( const BOOL& value );
  void setSloughDetachedBiomass( const BOOL& value );

protected:
  std::string mComputationDomain;
  REAL mResolution;
  REAL mShovingFraction;
  S32 mShovingMaxIter;
  BOOL mShovingMutual;
  BOOL mErosionMethod;
  BOOL mSloughDetachedBiomass;

};

#endif /* _AGENT_GRID_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
