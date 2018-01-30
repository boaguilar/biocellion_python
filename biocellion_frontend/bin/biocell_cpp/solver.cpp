#include "biomodel.h"

Solver::Solver()
  :  ParamHolder( ), mName(""), mClass(""), mDomain(""), mSolverIdx(-1), mPDEType( PDE_TYPE_REACTION_DIFFUSION_STEADY_STATE_LINEAR )
{
  //empty
}

Solver::~Solver( ) {
  mReactions.clear( );
}

void Solver::setName(const std::string& value)
{
  mName = value;
}

void Solver::setClass(const std::string& value)
{
  mClass = value;
}

void Solver::setDomain(const std::string& value)
{
  mDomain = value;
}

void Solver::setSolverIdx(const S32& value)
{
  mSolverIdx = value;
}

void Solver::setPDEType( const pde_type_e& value ) {
  mPDEType = value;
}
