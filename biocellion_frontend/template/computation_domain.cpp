#include "biomodel.h"

Grid::Grid()
  :mNDim(0), mNI(0), mNJ(0), mNK(0)
{
  //empty
}
void Grid::setNDim(const S32& NDim)
{
  mNDim = NDim;
}
void Grid::setNI(const S32& NI)
{
  mNI = NI;
}
void Grid::setNJ(const S32& NJ)
{
  mNJ = NJ;
}
void Grid::setNK(const S32& NK)
{
  mNK = NK;
}

XYZParam::XYZParam( )
  : mName( "" ),
    mX( -1 ), mY( -1 ), mZ( -1 )
{
  // empty
}

const std::string& XYZParam::getName( ) const {
  return mName;
}

S32 XYZParam::getX( ) const {
  return mX;
}

S32 XYZParam::getY( ) const {
  return mY;
}

S32 XYZParam::getZ( ) const {
  return mZ;
}

void XYZParam::setName( const std::string& value ) {
  mName = value;
}

void XYZParam::setX( const S32& value ) {
  mX = value;
}

void XYZParam::setY( const S32& value ) {
  mY = value;
}

void XYZParam::setZ( const S32& value ) {
  mZ = value;
}


Shape::Shape( )
{
}

const Vector< XYZParam >& Shape::getXYZParams( ) const {
  return mXYZParams;
}

Vector< XYZParam >& Shape::getXYZParams( ) {
  return mXYZParams;
}


IsPermeableTo::IsPermeableTo( )
  : mSoluteIdx( -1 ), mValue( 0.0 )
{
}

S32 IsPermeableTo::getSoluteIdx( ) const {
  return mSoluteIdx;
}

REAL IsPermeableTo::getValue( ) const {
  return mValue;
}

void IsPermeableTo::setSoluteIdx( const S32& value ) {
  mSoluteIdx = value;
}

void IsPermeableTo::setValue( const REAL& value ) {
  mValue = value;
}

BoundaryCondition::BoundaryCondition()
  : mBulkIdx( -1 )
{
  //empty
}

BoundaryCondition::~BoundaryCondition() {
  S32 i;
  for( i = 0 ; i < (S32) mShapes.size( ) ; i++ ) {
    if( mShapes[ i ] ) {
      delete mShapes[ i ];
      mShapes[ i ] = 0;
    }
  }
  mShapes.clear( );
}

const std::string& BoundaryCondition::getName( ) const {
  return mName;
}

const std::string& BoundaryCondition::getClass( ) const {
  return mClass;
}

S32 BoundaryCondition::getBulkIdx( ) const {
  return mBulkIdx;
}

const Vector< Shape* >& BoundaryCondition::getShapes( ) const {
  return mShapes;
}

Vector< Shape* >& BoundaryCondition::getShapes( ) {
  return mShapes;
}

const Vector< IsPermeableTo >& BoundaryCondition::getIsPermeableTos( ) const {
  return mIsPermeableTos;
}

Vector< IsPermeableTo >& BoundaryCondition::getIsPermeableTos( ) {
  return mIsPermeableTos;
}

BOOL BoundaryCondition::isPermeableTo( const S32& soluteIdx ) const {
  S32 i;
  for( i = 0 ; i < (S32) mIsPermeableTos.size( ) ; i++ ) {
    if( mIsPermeableTos[ i ].getSoluteIdx( ) == soluteIdx ) {
      return true;
    }
  }
  return false;
}

REAL BoundaryCondition::permeability( const S32& soluteIdx ) const {
  S32 i;
  for( i = 0 ; i < (S32) mIsPermeableTos.size( ) ; i++ ) {
    if( mIsPermeableTos[ i ].getSoluteIdx( ) == soluteIdx ) {
      return mIsPermeableTos[ i ].getValue( );
    }
  }
  return 0.0;
}



void BoundaryCondition::setName( const std::string& value ) {
  mName = value;
}

void BoundaryCondition::setClass( const std::string& value ) {
  mClass = value;
}

void BoundaryCondition::setBulkIdx( const S32& value ) {
  mBulkIdx = value;
}

ComputationDomain::ComputationDomain()
  :mName(""), mComputationDomainIndex(0)
{
  //empty
}

ComputationDomain::~ComputationDomain() {
  S32 i;
  for( i = 0 ; i < (S32) mBoundaryConditions.size() ; i++ ) {
    if( mBoundaryConditions[ i ] ) {
      delete mBoundaryConditions[ i ];
      mBoundaryConditions[ i ] = 0;
    }
  }
  mBoundaryConditions.clear( );
}

void ComputationDomain::setName(const std::string& name)
{
  mName = name;
}

void ComputationDomain::setComputationDomainIndex(const S32& computationDomainIndex)
{
  mComputationDomainIndex = computationDomainIndex;
}

const BoundaryCondition* ComputationDomain::getBoundaryCondition( const std::string& key ) const {
  S32 i;
  for( i = 0 ; i < (S32) mBoundaryConditions.size( ) ; i++ ) {
    if( mBoundaryConditions[ i ]->getName( ) == key ) {
      return mBoundaryConditions[ i ];
    }
  }
  return static_cast< BoundaryCondition* >( 0 );
}

static const std::string dim_prekey[ 3 ] = { "y", "x", "x" };
static const std::string dim_postkey[ 3 ] = { "z", "z", "y" };
static const std::string face_key[ 2 ] = { "0", "N" };
static inline std::string bc_key( const S32& dim, const S32& face ) {
  return dim_prekey[ dim ] + face_key[ face ] + dim_postkey[ dim ];
}

const BoundaryCondition* ComputationDomain::getBoundaryCondition( const S32& dim, const S32& face ) const {
  CHECK( dim >= 0 && dim < 3);
  CHECK( face >= 0 && face < 2 );
  return getBoundaryCondition( bc_key( dim, face ) );
}


BOOL ComputationDomain::boundaryIsPeriodic( const S32& dim ) const {
  static const std::string keys[ 3 ] = { "y0z", "x0z", "x0y" };
  const BoundaryCondition* bc = getBoundaryCondition( keys[ dim ] );
  if( !bc ) {
    return false;
  }
  if( bc->getClass( ) == "BoundaryCyclic" ) {
    return true;
  }
  return false;
}

void ComputationDomain::updateGridPhiBoundaryInfo( const S32& soluteIdx, const S32& dim, const S32& face, bc_type_e& bcType, REAL& bcVal, REAL& gridBeta ) const {
  /*
   *  If applicable GasMembrane can be found, use it.
   *  If applicable ZeroFlux can be found, then use it.
   *  Default to 0.0 derivative (BC_TYPE_NEUMANN_CONST, 0.0)
   */


  /*
   * GasMembrane only applies if the boundary is permeable to
   * the solute, and the connected Bulk has Sbulk entry for
   * the solute.
   *
   * Sets the boundary to have a constant value, as specified by Sbulk.
   */
  std::string key = bc_key( dim, face );
  S32 i;
  BoundaryCondition *bc = 0;
  for( i = 0 ; i < (S32) mBoundaryConditions.size( ) ; i++ ) {
    bc = mBoundaryConditions[ i ];
    if( bc->getName( ) == key ) {
      if( bc->getClass( ) == "BoundaryGasMembrane" ) {
        /*
         * applies if the boundary:
         *  is permeable to this solute
         *  is connected to a bulk with this solute
         */
        const Bulk* bulk = gBioModel->getWorld( ).getBulk( bc->getBulkIdx( ) );
        CHECK( bulk != 0 );
        const BulkSolute* bulk_solute = bulk->getSolute( soluteIdx );
        
        if( bc->isPermeableTo( soluteIdx ) && ( bulk_solute != 0 ) ) {
          // Tie the value to a configured value.
          bcType = BC_TYPE_DIRICHLET_CONST;
          bcVal = bulk_solute->getParamReal( bulk_solute->getIdxReal( BULK_SOLUTE_Sbulk ) );
          gridBeta = bc->permeability( soluteIdx );
          return;
        }
      }
    }
  }

  /*
   * ZeroFlux applies if no other type has applied.
   *
   * Sets the boundary to have a constant derivative, 0.
   */
  for( i = 0 ; i < (S32) mBoundaryConditions.size( ) ; i++ ) {
    bc = mBoundaryConditions[ i ];
    if( bc->getName( ) == key ) {
      if( bc->getClass( ) == "BoundaryZeroFlux" ) {
        // Tie the derivative to 0.
        bcType = BC_TYPE_NEUMANN_CONST;
        bcVal = 0.0;
        gridBeta = 0.0;
        return;
      }
    }
  }

  bcType = BC_TYPE_NEUMANN_CONST;
  bcVal = 0.0;
  gridBeta = 0.0;

  /* FIXME
  WARNING( "updateGridPhiBoundaryInfo using default type of BC_TYPE_NEUMANN_CONST and value of 0.0"
           << "  soluteIdx: " << soluteIdx
           << "  dim: " << dim
           << "  face: " << face
           );
  */
  
  return;
}
