#ifndef _SOLVER_H_
#define _SOLVER_H_
class Solver;

#include "biomodel.h"
#include <string>

class Solver : public ParamHolder {
public:
  Solver();
  virtual ~Solver( );
  std::string getName() const { return mName; };
  std::string getClass() const { return mClass; };
  std::string getDomain() const { return mDomain; };
  S32 getSolverIdx() const { return mSolverIdx; };
  pde_type_e getPDEType( ) const { return mPDEType; };
  
  void setName( const std::string& value );
  void setClass( const std::string& value );
  void setDomain( const std::string& value );
  void setSolverIdx( const S32& value );
  void setPDEType( const pde_type_e& value );
  const Vector < S32 >& getReactions( ) const { return mReactions; };
  Vector < S32 >& getReactions( ) { return mReactions; };
protected:
  std::string mName;
  std::string mClass;
  std::string mDomain;
  S32 mSolverIdx;
  pde_type_e mPDEType;
  Vector< S32 > mReactions;  
};

#endif /* _SOLVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
