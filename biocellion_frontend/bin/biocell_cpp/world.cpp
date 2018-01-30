#include "biomodel.h"

World::World()
{
  // empty
}

const Bulk* World::getBulk( const S32& bulkIdx ) const {
  S32 i;
  for( i = 0 ; i < (S32) mBulks.size( ) ; i++ ) {
    if( mBulks[ i ]->getBulkIdx( ) == bulkIdx ) {
      return mBulks[ i ];
    }
  }
  CHECK( false && "World::getBulk didn't find bulkIdx" );
  return static_cast< Bulk* >( 0 );
}

std::ostream& operator<<( std::ostream& os, const World& world ) {
  S32 i;
  os << "World: ";
  os << "Bulks(" << world.getBulks( ).size( ) << "): ";
  os << "Agars(" << world.getAgars( ).size( ) << "): ";
  os << "Domains(" << world.getComputationDomains( ).size( ) << "): ";
  for( i = 0 ; i < (S32) world.getComputationDomains( ).size( ) ; i ++ ) {
    os << world.getComputationDomains( )[ i ]->getName( ) << " ";
  }
  return os;
}
