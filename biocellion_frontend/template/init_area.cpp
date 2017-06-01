#include "init_area.h"
#include "biomodel.h"
#include "model_define.h"
#include <cmath>


Coordinates::Coordinates( )
  : mX( 0 ), mY( 0 ), mZ( 0 ), mR( 0 )
{
  // empty
}

REAL Coordinates::getX( ) const {
  return mX;
}

REAL Coordinates::getY( ) const {
  return mY;
}

REAL Coordinates::getZ( ) const {
  return mZ;
}

REAL Coordinates::getR( ) const {
  return mR;
}

void Coordinates::setX( const REAL& value ) {
  mX = value;
}

void Coordinates::setY( const REAL& value ) {
  mY = value;
}

void Coordinates::setZ( const REAL& value ) {
  mZ = value;
}

void Coordinates::setR( const REAL& value ) {
  mR = value;
}



InitArea::InitArea( )
  : mNumber(0), mShape("unfilledBlock"), mBirthday(0),
    mAgentSpecies( 0 )
{
  // empty
}

InitArea::~InitArea( ) {
  size_t i;
  for( i = 0 ; i < mCoordinates.size() ; i++ ) {
    if( mCoordinates[ i ] ) {
      delete mCoordinates[ i ];
      mCoordinates[ i ] = 0;
    }
  }
  mCoordinates.clear();
}

REAL InitArea::getNumber() const {
  return mNumber;
}

std::string InitArea::getShape() const {
  return mShape;
}
 
REAL InitArea::getBirthday() const {
  return mBirthday;
}

AgentSpecies* InitArea::getAgentSpecies() const {
  return mAgentSpecies;
}

const Vector<Coordinates *>& InitArea::getCoordinates( ) const {
  return mCoordinates;
}

Vector<Coordinates *>& InitArea::getCoordinates( ) {
  return mCoordinates;
}

void InitArea::setNumber(const REAL& value) {
  mNumber = value;
}

void InitArea::setShape(const std::string& value) {
  mShape = value;
}

void InitArea::setBirthday(const REAL& value) {
  mBirthday = value;
}

void InitArea::setAgentSpecies(AgentSpecies* value) {
  mAgentSpecies = value;
}

void InitArea::addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) {

  if( getShape() == "unfilledBlock" ) {
    addSpAgentsUnfilledBlock( init, startVIdx, regionSize, ifGridHabitableBoxData, v_spAgentVIdx, v_spAgentState, v_spAgentOffset );
  } else {
    ERROR( "Unexpected shape:" << getShape() );
  }
  
}

void InitArea::addSpAgentsUnfilledBlock( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) {

  /* FIXME BEGIN REFACTOR 
   * This code should only happen once, not every time it is used.
   * refactor into biomodel.cpp initialization.
   */
  // should have two Coordinates, use them to define the corners of the box to add agents into
  CHECK( mCoordinates.size() == 2 );
  S32 dim;

  // find dimensions and position of bounding box
  REAL box_xmin = std::min( mCoordinates[ 0 ]->getX(), mCoordinates[ 1 ]->getX() );
  REAL box_xmax = std::max( mCoordinates[ 0 ]->getX(), mCoordinates[ 1 ]->getX() );
  REAL box_ymin = std::min( mCoordinates[ 0 ]->getY(), mCoordinates[ 1 ]->getY() );
  REAL box_ymax = std::max( mCoordinates[ 0 ]->getY(), mCoordinates[ 1 ]->getY() );
  REAL box_zmin = std::min( mCoordinates[ 0 ]->getZ(), mCoordinates[ 1 ]->getZ() );
  REAL box_zmax = std::max( mCoordinates[ 0 ]->getZ(), mCoordinates[ 1 ]->getZ() );
  
  // find index and offset coordinates for the bounding box
  VReal pos_min, pos_max;
  VIdx  vIdx_min, vIdx_max;
  VReal vOffset_min, vOffset_max;
  pos_min[ 0 ] = box_xmin; pos_min[ 1 ] = box_ymin; pos_min[ 2 ] = box_zmin;
  pos_max[ 0 ] = box_xmax; pos_max[ 1 ] = box_ymax; pos_max[ 2 ] = box_zmax;

  Util::changePosFormat1LvTo2Lv( pos_min, vIdx_min, vOffset_min );
  Util::changePosFormat1LvTo2Lv( pos_max, vIdx_max, vOffset_max );

  // find agent_density
  REAL volume = 1.0;
  for( dim = 0 ; dim < 3 ; dim++ ) {
    REAL len = pos_max[ dim ] - pos_min[ dim ];
    if( len == 0 ) {
      len = 1.0;
    }
    volume *= len;
  }
  REAL agent_density = mNumber / volume;

  /* FIXME END REFACTOR */
  
  VReal partition_min, partition_max;
  VIdx  vPartitionIdx_min, vPartitionIdx_max;
  VReal vPartitionOffset_min, vPartitionOffset_max;
  for( dim = 0 ; dim < 3 ; dim++ ) {
    vPartitionOffset_min[ dim ] = -0.5 * gAgentGrid->getResolution( );
    vPartitionOffset_max[ dim ] = -0.5 * gAgentGrid->getResolution( );
    vPartitionIdx_min[ dim ] = startVIdx[ dim ];
    vPartitionIdx_max[ dim ] = startVIdx[ dim ] + regionSize[ dim ];
  }
  Util::changePosFormat2LvTo1Lv( vPartitionIdx_min, vPartitionOffset_min, partition_min );
  Util::changePosFormat2LvTo1Lv( vPartitionIdx_max, vPartitionOffset_max, partition_max );

  
  /**** find how many agents to create in the intersecting region ****/
  // find intersection of this region and the bounding box region
  VReal intersectStartPos, intersectEndPos;
  VIdx  intersectStartVIdx, intersectEndVIdx, intersectRegionSize;
  VReal intersectStartVOffset, intersectEndVOffset, intersectLen;
  REAL intersect_volume = 1.0;
  S32 dimensions = 0;
  for( dim = 0 ; dim < 3 ; dim++ ) {

    if( partition_min[ dim ] <= pos_min[ dim ] ) {
      intersectStartPos[ dim ] = pos_min[ dim ];
    } else {
      intersectStartPos[ dim ] = partition_min[ dim ];
    }

    if( partition_max[ dim ] <= pos_max[ dim ] ) {
      intersectEndPos[ dim ] = partition_max[ dim ];
    } else {
      intersectEndPos[ dim ] = pos_max[ dim ];
    }
    intersectLen[ dim ] = intersectEndPos[ dim ] - intersectStartPos[ dim ];
    if( intersectLen[ dim ] > 0 ) {
      intersect_volume *= intersectLen[ dim ];
      dimensions++;
    }

#if 0
    // find lower side of intersection
    if( startVIdx[ dim ] <= vIdx_min[ dim ] ) {
      intersectStartVIdx[ dim ] = vIdx_min[ dim ];
      intersectStartVOffset[ dim ] = vOffset_min[ dim ];

      OUTPUT( 10, "C-startVIdx [ " << dim << " ] = " << startVIdx[ dim ] << " <= vIdx_min [ " << dim << " ] = " << vIdx_min[ dim ] );
      OUTPUT( 10, "C-intersectStartVIdx [ " << dim << " ] = " << intersectStartVIdx[ dim ] );
    } else {
      intersectStartVIdx[ dim ] = startVIdx[ dim ];
      intersectStartVOffset[ dim ] = -0.5 * gAgentGrid->getResolution( );
      
      OUTPUT( 10, "D-startVIdx [ " << dim << " ] = " << startVIdx[ dim ] << " > vIdx_min [ " << dim << " ] = " << vIdx_min[ dim ] );
      OUTPUT( 10, "D-intersectStartVIdx [ " << dim << " ] = " << intersectStartVIdx[ dim ] );
    }

    // find upper side of intersection
    if( startVIdx[ dim ] + regionSize[ dim ] <= vIdx_max[ dim ] ) {
      intersectRegionSize[ dim ] = ( startVIdx[ dim ] + regionSize[ dim ] ) - intersectStartVIdx[ dim ];
      intersectEndVOffset[ dim ] = 0.5 * gAgentGrid->getResolution( );
      
      OUTPUT( 10, "A-startVIdx [ " << dim << " ] = " << startVIdx[ dim ] << " + regionSize [ " << dim << " ] = " << regionSize[ dim ] << " <= vIdx_max [ " << dim << " ] = " << vIdx_max[ dim ] );
      OUTPUT( 10, "A-intersectRegionSize [ " << dim << " ] = " << intersectRegionSize[ dim ] );
    } else {
      intersectRegionSize[ dim ] = vIdx_max[ dim ] - intersectStartVIdx[ dim ] + 1;
      intersectEndVOffset[ dim ] = vOffset_max[ dim ];
      if( intersectRegionSize[ dim ] < 1 ) {
        intersectRegionSize[ dim ] = 0;
        intersectEndVOffset[ dim ] = - 0.5 * gAgentGrid->getResolution( );
      }
      OUTPUT( 10, "B-vOffset_max[ "<< dim << " ] = " << vOffset_max[ dim ]
              << "  vIdx_max[ " << dim << " ] = " << vIdx_max[ dim ]   );
      OUTPUT( 10, "B-startVIdx [ " << dim << " ] = " << startVIdx[ dim ] << " + regionSize [ " << dim << " ] = " << regionSize[ dim ] << " > vIdx_max [ " << dim << " ] = " << vIdx_max[ dim ] << " ::: intersectRegionSize [ " << dim << " ] = " << intersectRegionSize[ dim ] );
    }

    intersectLen[ dim ] = 1.0;
    if( intersectRegionSize[ dim ] > 0 ) {
      intersectLen[ dim ] = ( intersectRegionSize[ dim ] * gAgentGrid->getResolution( )
                              - ( intersectStartVOffset[ dim ] + 0.5 * gAgentGrid->getResolution( ) )
                              + ( intersectEndVOffset[ dim ] - 0.5 * gAgentGrid->getResolution( ) ) );
      dimensions++;
    }
    intersect_volume *= intersectLen[ dim ];
    OUTPUT( 10, "intersectLen[ " << dim << " ] = ( " << intersectLen[ dim ] 
            << " intersectRegionSize[ " << dim << " ] " << intersectRegionSize[ dim ] 
            << " - intersectStartVOffset[ " << dim << " ] " << intersectStartVOffset[ dim ]
            << " - intersectEndVOffset[ " << dim << " ] " << intersectEndVOffset[ dim ]
            );
#endif
  }

  Util::changePosFormat1LvTo2Lv( intersectStartPos, intersectStartVIdx, intersectStartVOffset );
  Util::changePosFormat1LvTo2Lv( intersectEndPos, intersectEndVIdx, intersectEndVOffset );
  
  // FIXME: 2 should come from the computationdomain.grid.nDim
  if( dimensions <  2 ) {
    intersect_volume = 0.0;
  }

  REAL agent_number = intersect_volume * agent_density;

  OUTPUT( 10, "IF_GRID_SPACE = " << gAgentGrid->getResolution( ) );
  OUTPUT( 10, "total_agents = " << mNumber );
  OUTPUT( 10, "total_volume = " << volume );
  OUTPUT( 10, "agent_density = " << agent_density );
  OUTPUT( 10, "intersect_volume = " << intersect_volume );
  OUTPUT( 10, "agent_number = " << agent_number );

  if( 0 ) {
    OUTPUT( 0, "INTERSECTION:"
            << "  startIdx: " << startVIdx[ 0 ] << "," << startVIdx[ 1 ] << ","<< startVIdx[ 2 ]
            << "  regionSize: " << regionSize[ 0 ] << "," << regionSize[ 1 ] << ","<< regionSize[ 2 ]
            << "  intersectStartVIdx: " << intersectStartVIdx[ 0 ] << "," << intersectStartVIdx[ 1 ] << "," << intersectStartVIdx[ 2 ]
            << "  intersectRegionSize: " << intersectRegionSize[ 0 ] << "," << intersectRegionSize[ 1 ] << "," << intersectRegionSize[ 2 ]
            << "  intersectStartVOffset: " << intersectStartVOffset[ 0 ] << "," << intersectStartVOffset[ 1 ] << "," << intersectStartVOffset[ 2 ]
            << "  intersectEndVOffset: " << intersectEndVOffset[ 0 ] << "," << intersectEndVOffset[ 1 ] << "," << intersectEndVOffset[ 2 ]
            << "  intersectLen: " << intersectLen[ 0 ] << "," << intersectLen[ 1 ] << "," << intersectLen[ 2 ]
            << "  mNumber: " << mNumber
            << "  volume: " << volume
            << "  agent_density: " << agent_density
            << "  intersect_volume: " << intersect_volume 
            << "  agent_number: " << agent_number
          );
    OUTPUT( 0, "vIdx_min = " << vIdx_min[ 0 ] << "," << vIdx_min[ 1 ] << "," << vIdx_min[ 2 ]
            << "  vOffset_min = " << vOffset_min[ 0 ] << "," << vOffset_min[ 1 ] << "," << vOffset_min[ 2 ]
            << "  pos_min = " << pos_min[ 0 ] << "," << pos_min[ 1 ] << "," << pos_min[ 2 ]
            << "  vIdx_max = " << vIdx_max[ 0 ] << "," << vIdx_max[ 1 ] << "," << vIdx_max[ 2 ]
            << "  vOffset_max = " << vOffset_max[ 0 ] << "," << vOffset_max[ 1 ] << "," << vOffset_max[ 2 ]
            << "  pos_max = " << pos_max[ 0 ] << "," << pos_max[ 1 ] << "," << pos_max[ 2 ]
            << "  partition_min = " << partition_min[ 0 ] << "," << partition_min[ 1 ] << "," << partition_min[ 2 ]
            << "  partition_max = " << partition_max[ 0 ] << "," << partition_max[ 1 ] << "," << partition_max[ 2 ]
            << "  startVIdx = " << startVIdx[ 0 ] << "," << startVIdx[ 1 ] << "," << startVIdx[ 2 ]
            << "  regionSize = " << regionSize[ 0 ] << "," << regionSize[ 1 ] << "," << regionSize[ 2 ]
            << "  agent_number: " << agent_number );
  }
  
    /**** create the agents now ****/
  S64 j;
  for( j = 0 ; j < ( S64 ) agent_number ; j ++ ) {
    VReal vPosAgent;
    VIdx vIdxAgent;
    VReal vOffsetAgent;
    SpAgentState state;

    // random starting position
    for( dim = 0 ; dim < 3 ; dim++ ) {
      REAL randScale = Util::getModelRand( MODEL_RNG_UNIFORM ); /* [0.0,1.0) */
      if( randScale >= 1.0 ) {
        randScale = 1.0 - 0.0001;
      }
      CHECK( randScale >= 0.0 );
      CHECK( randScale < 1.0 );
#if 0
      if( intersectRegionSize[ dim ] > 0 ) {
        vPosAgent[ dim ] = ( ( intersectStartVIdx[ dim ] + 0.5 ) * gAgentGrid->getResolution( ) + intersectStartVOffset[ dim ]
                             + intersectLen[ dim ] * randScale );
      } else {
        vPosAgent[ dim ] = ( REAL )intersectStartVIdx[ dim ] * gAgentGrid->getResolution( );
      }
#else
      if( intersectLen[ dim ] > 0 ) {
        vPosAgent[ dim ] = ( intersectStartPos[ dim ] + intersectLen[ dim ] * randScale );
      } else {
        vPosAgent[ dim ] = intersectStartPos[ dim ];
      }
#endif

    }
    Util::changePosFormat1LvTo2Lv( vPosAgent, vIdxAgent, vOffsetAgent );
    mAgentSpecies->setInitialAgentState( state );

    if( false && ( vIdxAgent[0] < startVIdx[0] || vIdxAgent[1] < startVIdx[1] || vIdxAgent[2] < startVIdx[2] ) ) {
      OUTPUT( 0, "INTERSECTION:"
              << "  startIdx: " << startVIdx[ 0 ] << "," << startVIdx[ 1 ] << ","<< startVIdx[ 2 ]
              << "  regionSize: " << regionSize[ 0 ] << "," << regionSize[ 1 ] << ","<< regionSize[ 2 ]
              << "  intersectStartVIdx: " << intersectStartVIdx[ 0 ] << "," << intersectStartVIdx[ 1 ] << "," << intersectStartVIdx[ 2 ]
              << "  intersectRegionSize: " << intersectRegionSize[ 0 ] << "," << intersectRegionSize[ 1 ] << "," << intersectRegionSize[ 2 ]
              << "  intersectStartVOffset: " << intersectStartVOffset[ 0 ] << "," << intersectStartVOffset[ 1 ] << "," << intersectStartVOffset[ 2 ]
              << "  intersectEndVOffset: " << intersectEndVOffset[ 0 ] << "," << intersectEndVOffset[ 1 ] << "," << intersectEndVOffset[ 2 ]
              << "  intersectLen: " << intersectLen[ 0 ] << "," << intersectLen[ 1 ] << "," << intersectLen[ 2 ]
              );
      OUTPUT( 0, "NEW AGENT"
              << "  idx: " << vIdxAgent[ 0 ] << "," << vIdxAgent[ 1 ] << ","<< vIdxAgent[ 2 ]
              << "  offset: " << vOffsetAgent[ 0 ] << "," << vOffsetAgent[ 1 ] << "," << vOffsetAgent[ 2 ]
              << "  state: r: " << state.getRadius() << " t: " << state.getType( )
              << "  pos: " << vPosAgent[ 0 ] << "," << vPosAgent[ 1 ] << ","<< vPosAgent[ 2 ]
              );
    }
    
    CHECK( vIdxAgent[0] >= startVIdx[0] );
    CHECK( vIdxAgent[1] >= startVIdx[1] );
    CHECK( vIdxAgent[2] >= startVIdx[2] );
    for( dim = 0 ; dim < 3 ; dim++ ) {
      CHECK( vIdxAgent[dim] >= startVIdx[dim] );
      CHECK( vIdxAgent[dim] < startVIdx[dim] + regionSize[dim] );

      CHECK( vOffsetAgent[dim] >= gAgentGrid->getResolution( ) * -0.5 );
      CHECK( vOffsetAgent[dim] < gAgentGrid->getResolution( ) * 0.5 );
    }

    v_spAgentVIdx.push_back( vIdxAgent );
    v_spAgentState.push_back( state );
    v_spAgentOffset.push_back( vOffsetAgent );
  }
  
}
