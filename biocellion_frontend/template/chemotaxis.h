#ifndef _CHEMOTAXIS_H
#define _CHEMOTAXIS_H
#include "biomodel.h"

class Chemotaxis {
public:
    Chemotaxis( );
    S32 getSolute() const { return mSolute; };
    REAL getStrength() const { return mStrength; };
    S32 getContactInhibition() const { return mContactInhibition; };
    void setSolute(const S32& solute);
    void setStrength(const REAL& strength);
    void setContactInhibition(const S32& contactInhibition);
    
protected:
  S32 mSolute;
  REAL mStrength;
  S32 mContactInhibition;
};

#endif /*_CHEMOTAXIS_H*/

