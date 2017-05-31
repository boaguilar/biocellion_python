#ifndef _BULK_H_
#define _BULK_H_
#include "biocellion.h"
#include "param_holder.h"
#include "solute.h"
#include <string>

class Bulk : public ParamHolder {
public:
  Bulk();
  std::string getName() const { return mName; };
  S32 getBulkIndex() const { return mBulkIndex; };
  void setName(const std::string& name);
  void setBulkIndex(const S32& bulkIndex);
protected:
  std::string mName;
  S32 mBulkIndex;
  BOOL mIsConstant;
  REAL D;
  std::string updateType;
  Vector< BulkSolute > mBulkSolutes;  
};

#endif /*_BULK_H_*/
