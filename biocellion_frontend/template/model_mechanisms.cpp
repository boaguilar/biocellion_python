#include "model_define.h"
#include "model_mechanisms.h"

MechIntrctSpAgent* MechIntrctSpAgent::create()
{
  return 0;
}

MechIntrctSpAgent::~MechIntrctSpAgent()
{
}


MechIntrctSpAgent* MechIntrctSpAgentShove::create()
{
  MechIntrctSpAgentShove* intrct = new MechIntrctSpAgentShove();

  S32 i;
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    intrct->setScale( i , gAgentSpecies[ i ]->getParamReal( gAgentSpecies[ i ]->getIdxReal( SP_shoveFactor ) ) );
  }
  intrct->setDimensions(3);

  return intrct;
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove()
  : mScales(0), mDimensions(3)
{
  // empty
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove(const Vector<REAL>& scales, const S32& dimensions)
  : mScales(scales), mDimensions(dimensions)
{
  // empty
}

MechIntrctSpAgentShove::~MechIntrctSpAgentShove()
{
}

void MechIntrctSpAgentShove::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();

  if( dist <= R ) {/* shoving */
    REAL scale = (mScales[spAgent0.state.getType()] + mScales[spAgent1.state.getType()])/2.0;
    REAL mag = scale * ( R - dist );

    mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_X, dir[0] * mag );
    mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Y, dir[1] * mag );

    mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_X, -dir[0] * mag );
    mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Y, -dir[1] * mag );

    if(mDimensions > 2) {
      // TODO: enum not always available.
      //mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Z, dir[2] * mag );
      //mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Z, -dir[2] * mag );
    }
  }
}

void MechIntrctSpAgentShove::setScale(const S32& agent_type, const REAL& scale)
{
  if(( S32 )mScales.size() <= agent_type) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = 0.0;
    }
  }
  mScales[agent_type] = scale;
}

void MechIntrctSpAgentShove::setDimensions(const S32& dimensions)
{
  mDimensions = dimensions;
}

