#ifndef _ODE_NETWORK_H_
#define _ODE_NETWORK_H_
#include "biomodel.h"

class ODENetwork: public ParamHolder {
public:
  ODENetwork( );

  std::string getName( ) const;
  ode_stiff_e getStiffness( ) const;
  
  void setName( const std::string& value );
  void setStiffness( const ode_stiff_e& value );
  
protected:
  std::string mName;
  ode_stiff_e mStiffness;
};

#endif /* _ODE_NETWORK_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
