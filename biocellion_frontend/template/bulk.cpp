#include "bulk.h"

Bulk::Bulk()
  :mName(""), mBulkIndex(0)
{
  //empty  
}

void Bulk::setName(const std::string& name)
{
  mName = name;
}

void Bulk::setBulkIndex(const S32& bulkIndex)
{
  mBulkIndex = bulkIndex;
}
