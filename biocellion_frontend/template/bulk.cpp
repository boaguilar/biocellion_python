#include "biomodel.h"

BulkSolute::BulkSolute()
  : ParamHolder( ),
    mSoluteIdx( -1 )
{
  //empty
}

  
S32 BulkSolute::getSolute( ) const {
  return mSoluteIdx;
}

void BulkSolute::setSolute( const S32& value ) {
  mSoluteIdx = value;
}


Bulk::Bulk()
  :  ParamHolder( ), mName(""), mBulkIdx(0)
{
  //empty
}

Bulk::~Bulk( ) {
  S32 i;
  for( i = 0 ; i < (S32) mSolutes.size( ) ; i++ ) {
    if( mSolutes[ i ] ) {
      delete mSolutes[ i ];
      mSolutes[ i ] = 0;
    }
  }
  mSolutes.clear( );
}

void Bulk::setName(const std::string& name)
{
  mName = name;
}

void Bulk::setBulkIdx(const S32& bulkIdx)
{
  mBulkIdx = bulkIdx;
}
