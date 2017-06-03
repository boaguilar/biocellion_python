#ifndef _CHEMOTAXIS_H
#define _CHEMOTAXIS_H
#include "biomodel.h"

class Chemotaxis {
public:
    Chemotaxis( );
    S32 getWithSolute() const { return mWithSolute; };
    REAL getStrength() const { return mStrength; };
    S32 getContactInhibition() const { return mContactInhibition; };
    void setWithSolute(const S32& withSolute);
    void setStrength(const REAL& strength);
    void setContactInhibition(const S32& contactInhibition);
    
protected:
  S32 mWithSolute;
  REAL mStrength;
  S32 mContactInhibition;
};

#endif /*_CHEMOTAXIS_H*/

