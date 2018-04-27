#include "biomodel.h"

EntryCondition::EntryCondition()
  :mTimestep(-1), mItemIdx(0), mValue(0)
{
  //empty
}

EntryCondition::~EntryCondition( ) {
  size_t i;
  for( i = 0 ; i < mCoordinates.size() ; i++ ) {
    if( mCoordinates[ i ] ) {
      delete mCoordinates[ i ];
      mCoordinates[ i ] = 0;
    }
  }
  mCoordinates.clear();
}

AgentSpecies* EntryCondition::getAgentSpecies() const {
  return mAgentSpecies;
}

const Vector<Coordinates *>& EntryCondition::getCoordinates( ) const {
  return mCoordinates;
}

Vector<Coordinates *>& EntryCondition::getCoordinates( ) {
  return mCoordinates;
}

void EntryCondition::setMolecule( const std::string& molecule ) 
{ 
   mMolecule = molecule;
   if ( molecule == "biomass" ) {
      mItemIdx = 0 ;
   }
}

void EntryCondition::setType( const std::string& type )
{
   mType = type ;
}


void EntryCondition::setTimestep(const S32& timestep)
{
  mTimestep = timestep;
}    

void EntryCondition::setItemIdx(const S32& ItemIdx)
{
  mItemIdx = ItemIdx;
}

void EntryCondition::setValue(const REAL& value)
{
  mValue = value;
}

void EntryCondition::setAgentSpecies(AgentSpecies* value) {
  mAgentSpecies = value;
}

void EntryCondition::perturbAgents( const VIdx& vIdx, const VReal& vOffset, SpAgentState& state ) {
  if ( Info::getCurBaselineTimeStep() ==  mTimestep ){ 
     if ( ( mAgentSpecies->getSpeciesIdx() == -1) || ( mAgentSpecies->getSpeciesIdx() == state.getType()  ) ) {
        
        REAL gridsize = gBioModel->getAgentGrid().getResolution( ); 
        REAL xx = ( REAL(vIdx[0]) + 0.5 )* gridsize + vOffset[0] ;
        REAL yy = ( REAL(vIdx[1]) + 0.5 )* gridsize + vOffset[1] ;
        REAL zz = ( REAL(vIdx[2]) + 0.5 )* gridsize + vOffset[2] ;
        
        // FICME ; make sure the size of  mCoorinates is 2
        // find dimensions and position of bounding box
        REAL box_xmin = std::min(mCoordinates[0]->getX(), mCoordinates[1]->getX() );
        REAL box_xmax = std::max(mCoordinates[0]->getX(), mCoordinates[1]->getX() );
        REAL box_ymin = std::min(mCoordinates[0]->getY(), mCoordinates[1]->getY() );
        REAL box_ymax = std::max(mCoordinates[0]->getY(), mCoordinates[1]->getY() );
        REAL box_zmin = std::min(mCoordinates[0]->getZ(), mCoordinates[1]->getZ() );
        REAL box_zmax = std::max(mCoordinates[0]->getZ(), mCoordinates[1]->getZ() );

        BOOL inside = false ;
        if ((xx > box_xmin ) && (xx < box_xmax )) { 
           if ((yy > box_ymin ) && (yy < box_ymax )) { 
              if ((zz > box_zmin ) && (zz < box_zmax )) {
                 inside = true;
              }
           }
        }
             
        if ( inside )  {
           if ( mItemIdx > -1  )
              state.setModelReal( mItemIdx , mValue ) ;
        }
       
     }
  }
}


