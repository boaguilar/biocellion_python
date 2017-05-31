#include "computation_domain.h"

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

ComputationDomain::ComputationDomain()
  :mName(""), mComputationDomainIndex(0)
{
  //empty
}
void ComputationDomain::setName(const std::string& name)
{
  mName = name;
}
void ComputationDomain::setComputationDomainIndex(const S32& computationDomainIndex)
{
  mComputationDomainIndex = computationDomainIndex;
}
