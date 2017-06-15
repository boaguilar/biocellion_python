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
  : mDetail( "" ), mValue( 0.0 )
{
}

const std::string& IsPermeableTo::getDetail( ) const {
  return mDetail;
}

REAL IsPermeableTo::getValue( ) const {
  return mValue;
}

void IsPermeableTo::setDetail( const std::string& value ) {
  mDetail = value;
}

void IsPermeableTo::setValue( const REAL& value ) {
  mValue = value;
}

BoundaryCondition::BoundaryCondition()
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

void BoundaryCondition::setName( const std::string& value ) {
  mName = value;
}

void BoundaryCondition::setClass( const std::string& value ) {
  mClass = value;
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
