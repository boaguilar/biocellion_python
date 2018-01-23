#include "model_define.h"
#include "model_mechanisms.h"
#include <iomanip>
#include <cmath>

MechIntrctSpAgent* MechIntrctSpAgent::create()
{
  return 0;
}

MechIntrctSpAgent::~MechIntrctSpAgent()
{
  // empty
}

// SHOVE

MechIntrctSpAgent* MechIntrctSpAgentShove::create()
{
  MechIntrctSpAgentShove* intrct = new MechIntrctSpAgentShove();

  S32 i;
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    intrct->setScale( i , gBioModel->getAgentSpecies()[ i ]->getParamReal( gBioModel->getAgentSpecies()[ i ]->getIdxReal( SPECIES_shoveScale ) ) );
    intrct->setFactor( i , gBioModel->getAgentSpecies()[ i ]->getParamReal( gBioModel->getAgentSpecies()[ i ]->getIdxReal( SPECIES_shoveFactor ) ) );
    intrct->setLimit( i , gBioModel->getAgentSpecies()[ i ]->getParamReal( gBioModel->getAgentSpecies()[ i ]->getIdxReal( SPECIES_shoveLimit ) ) );
  }
  intrct->setDimensions(3);

  return intrct;
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove()
  : mScales(0), mDimensions(3)
{
  // empty
}

MechIntrctSpAgentShove::MechIntrctSpAgentShove(const Vector<REAL>& scales, const Vector<REAL>& factors, const Vector<REAL>& limits, const S32& dimensions)
  : mScales(scales), mFactors(factors), mLimits(limits), mDimensions(dimensions)
{
  // empty
}

MechIntrctSpAgentShove::~MechIntrctSpAgentShove()
{
  // empty
}

void MechIntrctSpAgentShove::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();

#if 0
  if( !gBioModel->getMechIntrctShoveEnabled()[ agentType0 ][ agentType1 ] ) {
    return;
  }
#endif

  REAL R = mFactors[ agentType0 ] * spAgent0.state.getRadius() + mFactors[ agentType1 ] *  spAgent1.state.getRadius()
    + ( ( mLimits[ agentType0 ] + mLimits[ agentType1 ] ) / 2.0 );

  if( dist <= R ) { /* shoving */
    REAL scale = (mScales[ agentType0 ] + mScales[ agentType1 ] ) / 2.0;
    // FIXME: unit analysis, compare to cDynoMiCS
    // 0.5 to share between the agents
    REAL mag = 0.5 * scale * ( R - dist ) * gBioModel->getAgentTimeStep( );
    if( false ) {
      OUTPUT( 0, "ShoveMag: " << mag);
    }
    
    mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealX(), dir[0] * mag );
    mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealY(), dir[1] * mag );

    mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealX(), -dir[0] * mag );
    mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealY(), -dir[1] * mag );

    if(mDimensions > 2) {
      mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealZ(), dir[2] * mag );
      mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealZ(), -dir[2] * mag );
    } else {
      /* No need to set to 0, since DELTA updates are used */
    }
    
    if( BMD_DO_DEBUG( BMD_INTERACTION_OUTPUT ) ) {
      OUTPUT( 0, "Shove: Pre write mechIntrctData for output."
              );
    }
    if( gBioModel->getInteractions( )[ INTERACTION_shove ]->getParamBool( INTERACTION_writeOutput ) ) {
      S32 dim;
      for( dim = 0 ; dim < 3 ; dim ++ ) {
        if( BMD_DO_DEBUG( BMD_INTERACTION_OUTPUT ) ) {
          OUTPUT( 0, "Shove: Write"
                  << " type0: " << gBioModel->getAgentSpecies()[ agentType0 ]->getName( )
                  << " type1: " << gBioModel->getAgentSpecies()[ agentType1 ]->getName( )
                  << " dim: " << dim
                  << " idx: " << gBioModel->getInteractions( )[ INTERACTION_shove ]->getInteractionIdx( )
                  << " value: " << dir[ dim ] * mag
                  );
        }

        gBioModel->getAgentSpecies()[ agentType0 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_shove ]->getInteractionIdx( ), dim,  dir[ dim ] * mag, mechIntrctData0 );
        gBioModel->getAgentSpecies()[ agentType1 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_shove ]->getInteractionIdx( ), dim, -dir[ dim ] * mag, mechIntrctData1 );
      }
    }
    if( BMD_DO_DEBUG( BMD_INTERACTION_OUTPUT ) ) {
      OUTPUT( 0, "Shove: Post write mechIntrctData for output."
              );
    }
  }
}

void MechIntrctSpAgentShove::setScale(const S32& agent_type, const REAL& value)
{
  if(( S32 )mScales.size() <= agent_type) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = 0.0;
    }
  }
  mScales[agent_type] = value;
}

void MechIntrctSpAgentShove::setFactor(const S32& agent_type, const REAL& value)
{
  if(( S32 )mFactors.size() <= agent_type) {
    S32 old_size = ( S32 )mFactors.size();
    mFactors.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mFactors.size(); i++) {
      mFactors[i] = 0.0;
    }
  }
  mFactors[agent_type] = value;
}

void MechIntrctSpAgentShove::setLimit(const S32& agent_type, const REAL& value)
{
  if(( S32 )mLimits.size() <= agent_type) {
    S32 old_size = ( S32 )mLimits.size();
    mLimits.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mLimits.size(); i++) {
      mLimits[i] = 0.0;
    }
  }
  mLimits[agent_type] = value;
}

void MechIntrctSpAgentShove::setDimensions(const S32& dimensions)
{
  mDimensions = dimensions;
}

// ADHESION

MechIntrctSpAgent* MechIntrctSpAgentAdhesion::create()
{
  MechIntrctSpAgentAdhesion* intrct = new MechIntrctSpAgentAdhesion();

  S32 i, j;
  /* default values for all slots, makes sure all slots exist */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    for( j = 0 ; j < NUM_AGENT_SPECIES ; j++ ) {
      intrct->setScale( i, j, 0.0 );
      intrct->setDistanceScale( i, j, 0.0 );
    }
  }
  /* values based on configuration information */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    const Vector< Adhesion * >& adhesions = gBioModel->getAgentSpecies()[ i ]->getAdhesions( );
    for( j = 0 ; j < (S32)adhesions.size( ) ; j++ ) {
      intrct->setScale( i, adhesions[ j ]->getWithSpecies( ), adhesions[ j ]->getScale( ) );
      intrct->setDistanceScale( i, adhesions[ j ]->getWithSpecies( ), adhesions[ j ]->getStrength( ) );
    }
  }

  return intrct;
}

MechIntrctSpAgentAdhesion::MechIntrctSpAgentAdhesion()
{
  // empty
}

MechIntrctSpAgentAdhesion::~MechIntrctSpAgentAdhesion()
{
  // empty
}

void MechIntrctSpAgentAdhesion::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();

  if( mScales[ agentType0 ][ agentType1 ] <= 0.0 || mDistanceScales[ agentType0 ][ agentType1 ] <= 0.0 ) {
    return;
  }

  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
  if( dist > R ) {
    REAL x = dist / R;
    REAL a = 0.5 * mScales[ agentType0 ][ agentType1 ]; // 0.5 to share between the agents
    REAL d = mDistanceScales[ agentType0 ][ agentType1 ];
    // FIXME: unit analysis.  compare to cDynoMiCS
    REAL mag = -a * ( dist - R ) * exp( -1.0 * ( x - 1.0 ) * ( x - 1.0 ) / d ) * gBioModel->getAgentTimeStep( );
    
    mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealX(), dir[0] * mag );
    mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealY(), dir[1] * mag );
    mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealZ(), dir[2] * mag );

    mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealX(), -dir[0] * mag );
    mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealY(), -dir[1] * mag );
    mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealZ(), -dir[2] * mag );

    if( gBioModel->getInteractions( )[ INTERACTION_adhesion ]->getParamBool( INTERACTION_writeOutput ) ) {
      S32 dim;
      for( dim = 0 ; dim < 3 ; dim ++ ) {
        gBioModel->getAgentSpecies()[ agentType0 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_adhesion ]->getInteractionIdx( ), dim,  dir[ dim ] * mag, mechIntrctData0 );
        gBioModel->getAgentSpecies()[ agentType1 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_adhesion ]->getInteractionIdx( ), dim, -dir[ dim ] * mag, mechIntrctData1 );
      }
    }
    
  }
}

void MechIntrctSpAgentAdhesion::setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mScales.size() <= agent_type0) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mScales[ agent_type0 ].size();
    mScales[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type0 ].size(); i++) {
      mScales[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mScales.size() <= agent_type1) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mScales[ agent_type1 ].size();
    mScales[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type1 ].size(); i++) {
      mScales[ agent_type1 ][ i ] = 0.0;
    }
  }

  mScales[ agent_type0 ][ agent_type1 ] = value;
  mScales[ agent_type1 ][ agent_type0 ] = value;
}


void MechIntrctSpAgentAdhesion::setDistanceScale(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mDistanceScales.size() <= agent_type0) {
    S32 old_size = ( S32 )mDistanceScales.size();
    mDistanceScales.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales.size(); i++) {
      mDistanceScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mDistanceScales[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mDistanceScales[ agent_type0 ].size();
    mDistanceScales[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales[ agent_type0 ].size(); i++) {
      mDistanceScales[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mDistanceScales.size() <= agent_type1) {
    S32 old_size = ( S32 )mDistanceScales.size();
    mDistanceScales.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales.size(); i++) {
      mDistanceScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mDistanceScales[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mDistanceScales[ agent_type1 ].size();
    mDistanceScales[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mDistanceScales[ agent_type1 ].size(); i++) {
      mDistanceScales[ agent_type1 ][ i ] = 0.0;
    }
  }

  mDistanceScales[ agent_type0 ][ agent_type1 ] = value;
  mDistanceScales[ agent_type1 ][ agent_type0 ] = value;
}

// DISTANCE JUNCTIONS

MechIntrctSpAgent* MechIntrctSpAgentDistanceJunction::create()
{
  MechIntrctSpAgentDistanceJunction* intrct = new MechIntrctSpAgentDistanceJunction();

  S32 i, j;
  /* default values for all slots, makes sure all slots exist */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    for( j = 0 ; j < NUM_AGENT_SPECIES ; j++ ) {
      intrct->setEnabled( i, j, false );
    }
    intrct->setLinkScale( i, 0.0 );
    intrct->setUnlinkScale( i, 0.0 );
  }

  /* values based on configuration information */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    const Vector< DistanceJunction * >& junctions = gBioModel->getAgentSpecies()[ i ]->getDistanceJunctions( );
    for( j = 0 ; j < (S32)junctions.size( ) ; j++ ) {
      intrct->setEnabled( i, junctions[ j ]->getWithSpecies( ), junctions[ j ]->getEnabled( ) );
    }
    intrct->setLinkScale( i,  gBioModel->getAgentSpecies()[ i ]->getParamReal( gBioModel->getAgentSpecies()[ i ]->getIdxReal( SPECIES_attachCreateFactor ) ) );
    intrct->setUnlinkScale( i,  gBioModel->getAgentSpecies()[ i ]->getParamReal( gBioModel->getAgentSpecies()[ i ]->getIdxReal( SPECIES_attachDestroyFactor ) ) );
  }
  intrct->setJunctionIdx( JUNCTION_TYPE_DISTANCE );
  intrct->setRealDistanceIdx( JUNCTION_REAL_DISTANCE );
  intrct->setIntTouchedIdx( JUNCTION_INT_TOUCHED );

  return intrct;
}

MechIntrctSpAgentDistanceJunction::MechIntrctSpAgentDistanceJunction()
{
  // empty
}

MechIntrctSpAgentDistanceJunction::~MechIntrctSpAgentDistanceJunction()
{
  // empty
}

void MechIntrctSpAgentDistanceJunction::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  link = false;
  unlink = false;

  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();

  if( !mEnabled[ agentType0 ][ agentType1 ] ) {
    return;
  }

  REAL agentRadius0 = spAgent0.state.getRadius();
  REAL agentRadius1 = spAgent1.state.getRadius();

  REAL link_dist = mLinkScales[ agentType0 ] * agentRadius0 + mLinkScales[ agentType1 ] * agentRadius1;
  REAL unlink_dist = mUnlinkScales[ agentType0 ] * agentRadius0 + mUnlinkScales[ agentType1 ] * agentRadius1;

  if(dist <= link_dist) {
    // close enough to form the link, careful, they may already be linked...
    link = true;

    // set type
    end0.setType( mJunctionIdx );
    end1.setType( mJunctionIdx );

    // set distance
    end0.setModelReal( mRealDistanceIdx, dist );
    end1.setModelReal( mRealDistanceIdx, dist );

    // set/update touched status
    REAL R = agentRadius0 + agentRadius1;
    S32 idx0, idx1;
    if( dist <= R ) {
      // currently touching, mark as touched
      end0.setModelInt( mIntTouchedIdx, 1 );
      end1.setModelInt( mIntTouchedIdx, 1 );
    } else if( spAgent0.junctionData.isLinked( spAgent1.junctionData, idx0, idx1 ) ) {
      // not touching, have linked, preserve touch status
      end0.setModelInt( mIntTouchedIdx, spAgent0.junctionData.getJunctionEndRef( idx0 ).getModelInt( mIntTouchedIdx ) );
      end1.setModelInt( mIntTouchedIdx, spAgent1.junctionData.getJunctionEndRef( idx1 ).getModelInt( mIntTouchedIdx ) );
    } else {
      // not touching, newly linked
      end0.setModelInt( mIntTouchedIdx, 0 );
      end1.setModelInt( mIntTouchedIdx, 0 );
    }
  } else if(dist > unlink_dist) {
    // far enough to break link
    if( spAgent0.junctionData.isLinked( spAgent1.junctionData ) ) {
      unlink = true;
    }
  } else {
    // dist in between forming and breaking junction
    // if already linked, update the junction
    S32 idx0, idx1;
    if( spAgent0.junctionData.isLinked( spAgent1.junctionData, idx0, idx1 ) ) {
      link = true;

      // set type
      end0.setType( mJunctionIdx );
      end1.setType( mJunctionIdx );

      // update distance
      end0.setModelReal( mRealDistanceIdx, dist );
      end1.setModelReal( mRealDistanceIdx, dist );

      // preserve touch status
      end0.setModelInt( mIntTouchedIdx, spAgent0.junctionData.getJunctionEndRef( idx0 ).getModelInt( mIntTouchedIdx ) );
      end1.setModelInt( mIntTouchedIdx, spAgent1.junctionData.getJunctionEndRef( idx1 ).getModelInt( mIntTouchedIdx ) );
    }
  }
}

void MechIntrctSpAgentDistanceJunction::setEnabled(const S32& agent_type0, const S32& agent_type1, const BOOL& value) {
  if(( S32 )mEnabled.size() <= agent_type0) {
    S32 old_size = ( S32 )mEnabled.size();
    mEnabled.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mEnabled.size(); i++) {
      mEnabled[i] = Vector< BOOL >();
    }
  }
  if(( S32 )mEnabled[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mEnabled[ agent_type0 ].size();
    mEnabled[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mEnabled[ agent_type0 ].size(); i++) {
      mEnabled[ agent_type0 ][ i ] = false;
    }
  }

  if(( S32 )mEnabled.size() <= agent_type1) {
    S32 old_size = ( S32 )mEnabled.size();
    mEnabled.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mEnabled.size(); i++) {
      mEnabled[i] = Vector< BOOL >();
    }
  }
  if(( S32 )mEnabled[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mEnabled[ agent_type1 ].size();
    mEnabled[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mEnabled[ agent_type1 ].size(); i++) {
      mEnabled[ agent_type1 ][ i ] = false;
    }
  }

  mEnabled[ agent_type0 ][ agent_type1 ] = value;
  mEnabled[ agent_type1 ][ agent_type0 ] = value;
}


void MechIntrctSpAgentDistanceJunction::setLinkScale(const S32& agent_type, const REAL& value) {
  if(( S32 )mLinkScales.size() <= agent_type) {
    S32 old_size = ( S32 )mLinkScales.size();
    mLinkScales.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mLinkScales.size(); i++) {
      mLinkScales[i] = 0.0;
    }
  }
  mLinkScales[agent_type] = value;
}

void MechIntrctSpAgentDistanceJunction::setUnlinkScale(const S32& agent_type, const REAL& value) {
  if(( S32 )mUnlinkScales.size() <= agent_type) {
    S32 old_size = ( S32 )mUnlinkScales.size();
    mUnlinkScales.resize(agent_type + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mUnlinkScales.size(); i++) {
      mUnlinkScales[i] = 0.0;
    }
  }
  mUnlinkScales[agent_type] = value;
}

void MechIntrctSpAgentDistanceJunction::setJunctionIdx( const S32& idx ) {
  mJunctionIdx = idx;
}

void MechIntrctSpAgentDistanceJunction::setRealDistanceIdx( const S32& idx ) {
  mRealDistanceIdx = idx;
}
void MechIntrctSpAgentDistanceJunction::setIntTouchedIdx( const S32& idx ) {
  mIntTouchedIdx = idx;
}

// TIGHT JUNCTIONS

MechIntrctSpAgent* MechIntrctSpAgentTightJunction::create()
{
  MechIntrctSpAgentTightJunction* intrct = new MechIntrctSpAgentTightJunction();

  S32 i, j;
  /* default values for all slots, makes sure all slots exist */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    for( j = 0 ; j < NUM_AGENT_SPECIES ; j++ ) {
      intrct->setScale( i, j, 0.0 );
      intrct->setStiffness( i, j, 0.0 );
    }
  }
  /* values based on configuration information */
  for( i = 0 ; i < NUM_AGENT_SPECIES ; i++ ) {
    const Vector< TightJunction * >& tjunctions = gBioModel->getAgentSpecies()[ i ]->getTightJunctions( );
    for( j = 0 ; j < (S32)tjunctions.size( ) ; j++ ) {
      intrct->setScale( i, tjunctions[ j ]->getWithSpecies( ), tjunctions[ j ]->getScale( ) );
      intrct->setStiffness( i, tjunctions[ j ]->getWithSpecies( ), tjunctions[ j ]->getStiffness( ) );
    }
  }
  intrct->setIntTouchedIdx( JUNCTION_INT_TOUCHED );

  return intrct;
}

MechIntrctSpAgentTightJunction::MechIntrctSpAgentTightJunction()
{
  // empty
}

MechIntrctSpAgentTightJunction::~MechIntrctSpAgentTightJunction()
{
  // empty
}

void MechIntrctSpAgentTightJunction::compute( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink )
{
  S32 agentType0 = spAgent0.state.getType();
  S32 agentType1 = spAgent1.state.getType();
  REAL scale = mScales[ agentType0 ][ agentType1 ];
  REAL stiffness = mStiffnesses[ agentType0 ][ agentType1 ];

  if( scale <= 0.0 || stiffness <= 0.0 ) {
    if( BMD_DO_DEBUG( BMD_TIGHT_JUNCTION ) ) {
      OUTPUT( 0, "tightJunction::compute: bail out: scale: " << scale << " stiffness: " << stiffness );
    }
    return;
  }

  S32 idx0, idx1;
  if( !spAgent0.junctionData.isLinked( spAgent1.junctionData, idx0, idx1 ) ) {
    // these two agents do not have a link, not close enough for tight junction
    if( BMD_DO_DEBUG( BMD_TIGHT_JUNCTION ) ) {
      OUTPUT( 0, "tightJunction::compute: bail out: no junction" );
    }
    return;
  }
  // Tight Junctions only occur if the agents have touched
  if( !spAgent0.junctionData.getJunctionEndRef( idx0 ).getModelInt( mIntTouchedIdx ) ) {
    if( BMD_DO_DEBUG( BMD_TIGHT_JUNCTION ) ) {
      OUTPUT( 0, "tightJunction::compute: bail out: junction, but not touched." );
    }
    return;
  }

  REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
  REAL difference = (dist - R) / R;
  // If two agents are separated by part of another agent,
  // we don't want them pulling through the sandwiched agent.
  // reducing max difference from 1.0 to 0.6
  // FIXME: 0.6, 1.0, ???
  if( difference > 1.0 || stiffness == 0.0 ) { // too far apart
    if( BMD_DO_DEBUG( BMD_TIGHT_JUNCTION ) ) {
      OUTPUT( 0, "tightJunction::compute: bail out: difference: " << difference << " stiffness: " << stiffness );
    }
    return;
  }
  REAL hyper = tanh( difference * stiffness );
  // FIXME: unit analysis.  compare with cDynoMiCS
  // 0.5 to share between the agents
  // scale = um.hour-1
  // difference = unitless [ -1, 1 ]
  // hyper = unitless [ -1, 1 ]
  // stiffness = unitless [ 0, oo ) 
  // scales the responsiveness of tanh to difference
  // if <= 1, also limits the maximum value of tanh
  //             0   -> hyper = 0
  //             0.5 -> medium
  //             1   -> fast
  //             10  -> faster
  //             
  REAL mag = - 0.5 * scale * fabs( difference ) * hyper * gBioModel->getAgentTimeStep( );
  
  mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealX(), dir[0] * mag );
  mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealY(), dir[1] * mag );
  mechIntrctData0.setModelReal( gBioModel->getAgentSpecies()[ agentType0 ]->getIdxMechForceRealZ(), dir[2] * mag );

  mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealX(), -dir[0] * mag );
  mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealY(), -dir[1] * mag );
  mechIntrctData1.setModelReal( gBioModel->getAgentSpecies()[ agentType1 ]->getIdxMechForceRealZ(), -dir[2] * mag );

  if( gBioModel->getInteractions( )[ INTERACTION_tightJunction ]->getParamBool( INTERACTION_writeOutput ) ) {
    S32 dim;
    if( BMD_DO_DEBUG( BMD_TIGHT_JUNCTION ) ) {
      OUTPUT( 0, "tightJunction::compute: setting output values." );
    }
    for( dim = 0 ; dim < 3 ; dim ++ ) {
      gBioModel->getAgentSpecies()[ agentType0 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_tightJunction ]->getInteractionIdx( ), dim,  dir[ dim ] * mag, mechIntrctData0 );
      gBioModel->getAgentSpecies()[ agentType1 ]->setMechInteractionValue( gBioModel->getInteractions( )[ INTERACTION_tightJunction ]->getInteractionIdx( ), dim, -dir[ dim ] * mag, mechIntrctData1 );
    }
  }
  
}

void MechIntrctSpAgentTightJunction::setScale(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mScales.size() <= agent_type0) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mScales[ agent_type0 ].size();
    mScales[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type0 ].size(); i++) {
      mScales[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mScales.size() <= agent_type1) {
    S32 old_size = ( S32 )mScales.size();
    mScales.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales.size(); i++) {
      mScales[i] = Vector< REAL >();
    }
  }
  if(( S32 )mScales[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mScales[ agent_type1 ].size();
    mScales[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mScales[ agent_type1 ].size(); i++) {
      mScales[ agent_type1 ][ i ] = 0.0;
    }
  }

  mScales[ agent_type0 ][ agent_type1 ] = value;
  mScales[ agent_type1 ][ agent_type0 ] = value;
}


void MechIntrctSpAgentTightJunction::setStiffness(const S32& agent_type0, const S32& agent_type1, const REAL& value)
{
  if(( S32 )mStiffnesses.size() <= agent_type0) {
    S32 old_size = ( S32 )mStiffnesses.size();
    mStiffnesses.resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mStiffnesses.size(); i++) {
      mStiffnesses[i] = Vector< REAL >();
    }
  }
  if(( S32 )mStiffnesses[ agent_type0 ].size() <= agent_type1) {
    S32 old_size = ( S32 )mStiffnesses[ agent_type0 ].size();
    mStiffnesses[ agent_type0 ].resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mStiffnesses[ agent_type0 ].size(); i++) {
      mStiffnesses[ agent_type0 ][ i ] = 0.0;
    }
  }

  if(( S32 )mStiffnesses.size() <= agent_type1) {
    S32 old_size = ( S32 )mStiffnesses.size();
    mStiffnesses.resize(agent_type1 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mStiffnesses.size(); i++) {
      mStiffnesses[i] = Vector< REAL >();
    }
  }
  if(( S32 )mStiffnesses[ agent_type1 ].size() <= agent_type0) {
    S32 old_size = ( S32 )mStiffnesses[ agent_type1 ].size();
    mStiffnesses[ agent_type1 ].resize(agent_type0 + 1);
    S32 i;
    for(i = old_size; i < ( S32 )mStiffnesses[ agent_type1 ].size(); i++) {
      mStiffnesses[ agent_type1 ][ i ] = 0.0;
    }
  }

  mStiffnesses[ agent_type0 ][ agent_type1 ] = value;
  mStiffnesses[ agent_type1 ][ agent_type0 ] = value;
}

void MechIntrctSpAgentTightJunction::setIntTouchedIdx( const S32& idx ) {
  mIntTouchedIdx = idx;
}
