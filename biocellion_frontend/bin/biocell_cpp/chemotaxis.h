#ifndef _CHEMOTAXIS_H
#define _CHEMOTAXIS_H
#include "biomodel.h"

class Chemotaxis {
public:
  Chemotaxis( );
  
  S32 getSolute() const { return mSolute; };
  REAL getStrength() const { return mStrength; };
  S32 getContactInhibition() const { return mContactInhibition; };
  REAL getAlpha() const { return mAlpha; };
  
  void setSolute(const S32& solute);
  void setStrength(const REAL& strength);
  void setContactInhibition(const S32& contactInhibition);
  void setAlpha(const REAL& alpha);
  
protected:
  S32 mSolute;
  REAL mStrength;
  S32 mContactInhibition;
  REAL mAlpha;
};

#endif /*_CHEMOTAXIS_H*/

