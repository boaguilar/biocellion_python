#ifndef _COMPUTATION_DOMAIN_H
#define _COMPUTATION_DOMAIN_H
#include "biocellion.h"
#include "param_holder.h"
#include <string>

class Grid {
public:
  Grid();
  S32 getNDim() const { return mNDim; };
  S32 getNI() const { return mNI; };
  S32 getNJ() const { return mNJ; };
  S32 getNK() const { return mNK; };
  void setNDim(const S32& NDim);
  void setNI(const S32& NI);
  void setNJ(const S32& NJ);
  void setNK(const S32& NK);  
protected:
  S32 mNDim;
  S32 mNI;
  S32 mNJ;
  S32 mNK;
};

class BoundaryCondition {};

class ComputationDomain : public ParamHolder{
public:
  ComputationDomain();
  std::string getName() const { return mName; };
  S32 getComputationDomainIndex() const { return mComputationDomainIndex; };
  void setName(const std::string& name);
  void setComputationDomainIndex(const S32& computationDomainIndex);
  
protected:
  std::string mName;
  S32 mComputationDomainIndex;
  Grid nDim;
  S32 mResolution;
  REAL mSpecificArea;
  BOOL mHasBulk;
  S32 mBoundaryLayer;
  REAL mBiofilmDiffusivity;
  BoundaryCondition mBoundaryCyclic;
};


#endif /*_COMPUTATION_DOMAIN_H*/
