#ifndef _MODEL_MECHANISMS_H_
#define _MODEL_MECHANISMS_H_

#include "biocellion.h"
#include <string>

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
  MechIntrctSpAgentShove(const Vector<REAL>& scales, const Vector<REAL>& factors, const Vector<REAL>& limits, const S32& dimensions);
  virtual ~MechIntrctSpAgentShove();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type, const REAL& value);
  void setFactor(const S32& agent_type, const REAL& value);
  void setLimit(const S32& agent_type, const REAL& value);
  void setDimensions(const S32& dimensions);

protected:
  Vector<REAL> mScales;
  Vector<REAL> mFactors;
  Vector<REAL> mLimits;
  S32 mDimensions;
  
};

class MechIntrctSpAgentAdhesion : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentAdhesion();
  virtual ~MechIntrctSpAgentAdhesion();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setDistanceScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);

protected:
  Vector< Vector< REAL > > mScales;
  Vector< Vector< REAL > > mDistanceScales;
};


class MechIntrctSpAgentDistanceJunction : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentDistanceJunction();
  virtual ~MechIntrctSpAgentDistanceJunction();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setEnabled(const S32& agent_type0, const S32& agent_type1, const BOOL& value);
  void setLinkScale(const S32& agent_type, const REAL& value);
  void setUnlinkScale(const S32& agent_type, const REAL& value);
  void setJunctionIdx(const S32& value);
  void setRealDistanceIdx(const S32& value);
  void setIntTouchedIdx(const S32& value);

protected:
  Vector< Vector< BOOL > > mEnabled;
  Vector< REAL > mLinkScales;
  Vector< REAL > mUnlinkScales;
  S32 mJunctionIdx;
  S32 mRealDistanceIdx;
  S32 mIntTouchedIdx;
};

class MechIntrctSpAgentTightJunction : public MechIntrctSpAgent {

public:
  static MechIntrctSpAgent* create();
  MechIntrctSpAgentTightJunction();
  virtual ~MechIntrctSpAgentTightJunction();
  virtual void compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink );
  void setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setStiffness(const S32& agent_type0, const S32& agent_type1, const REAL& value);
  void setIntTouchedIdx(const S32& value);

protected:
  Vector< Vector< REAL > > mScales;
  Vector< Vector< REAL > > mStiffnesses;
  S32 mIntTouchedIdx;
};

#endif /* _MODEL_MECHANISMS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
