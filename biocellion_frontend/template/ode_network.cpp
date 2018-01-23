#include "biomodel.h"

ODENetwork::ODENetwork( )
  : mName( "" ),
    mStiffness( ODE_STIFF_NORMAL )
{
}

std::string ODENetwork::getName( ) const {
  return mName;
}

ode_stiff_e ODENetwork::getStiffness( ) const {
  return mStiffness;
}
  
void ODENetwork::setName( const std::string& value ) {
  mName = value;
}

void ODENetwork::setStiffness( const ode_stiff_e& value ) {
  mStiffness = value;
}
