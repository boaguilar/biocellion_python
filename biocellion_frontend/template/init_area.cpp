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

  /**** find how many agents to create in the intersecting region ****/
  // find intersection of this region and the bounding box region
  VIdx  intersectStartVIdx, intersectRegionSize;
  VReal intersectStartVOffset, intersectEndVOffset, intersectLen;
  REAL intersect_volume = 1.0;
  S32 dimensions = 0;
  for( dim = 0 ; dim < 3 ; dim++ ) {

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
  }
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

      if( intersectRegionSize[ dim ] > 0 ) {
        vPosAgent[ dim ] = ( ( ( REAL )intersectStartVIdx[ dim ] - 0.5 ) * gAgentGrid->getResolution( ) - intersectStartVOffset[ dim ]
                             + intersectLen[ dim ] * randScale );
      } else {
        vPosAgent[ dim ] = ( REAL )intersectStartVIdx[ dim ] * gAgentGrid->getResolution( );
      }

    }
    Util::changePosFormat1LvTo2Lv( vPosAgent, vIdxAgent, vOffsetAgent );
    
    for( dim = 0 ; dim < 3 ; dim++ ) {
      CHECK( vIdx[dim] >= startVIdx[dim] );
      CHECK( vIdx[dim] < startVIdx[dim] + regionSize[dim] );

      CHECK( vOffset[dim] >= gAgentGrid->getResolution( ) * -0.5 );
      CHECK( vOffset[dim] < gAgentGrid->getResolution( ) * 0.5 );
    }

    mAgentSpecies->setInitialAgentState( state );
    
    v_spAgentVIdx.push_back( vIdxAgent );
    v_spAgentState.push_back( state );
    v_spAgentOffset.push_back( vOffsetAgent );
  }
  
}