#ifndef _BULK_H_
#define _BULK_H_
class Bulk;

#include "biomodel.h"
#include <string>

class BulkSolute : public ParamHolder {
public:
  BulkSolute();
  
  S32 getSolute( ) const;
  
  void setSolute( const S32& value );
  
protected:
  S32 mSoluteIdx;
};

class Bulk : public ParamHolder {
public:
  Bulk();
  virtual ~Bulk( );
  std::string getName() const { return mName; };
  S32 getBulkIdx() const { return mBulkIdx; };
  void setName(const std::string& name);
  void setBulkIdx(const S32& bulkIdx);
  const Vector < BulkSolute* >& getSolutes() const { return mSolutes; };
  Vector < BulkSolute* >& getSolutes() { return mSolutes; };
protected:
  std::string mName;
  S32 mBulkIdx;
  Vector< BulkSolute* > mSolutes;  
};

#endif /*_BULK_H_*/
