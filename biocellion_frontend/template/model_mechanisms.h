#ifndef _MODEL_MECHANISMS_H_
#define _MODEL_MECHANISMS_H_

#include "biocellion.h"

class MechIntrctSpAgent {
public:
  static MechIntrctSpAgent* create();
  virtual ~MechIntrctSpAgent();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) = 0;
};

class MechIntrctSpAgentShove : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentShove();
  MechIntrctSpAgentShove(const Vector<REAL>& scales, const S32& dimensions);
  virtual ~MechIntrctSpAgentShove();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type, const REAL& scale);
  void setDimensions(const S32& dimensions);

protected:
  Vector<REAL> mScales;
  S32 mDimensions;
  
};




#endif /* _MODEL_MECHANISMS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
