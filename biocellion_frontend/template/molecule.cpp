#include "biomodel.h"

Molecule::Molecule()
  : ParamHolder( ),
    mName( "" ), mMoleculeIdx( -1 ), mDomainIdx( -1 )
{
}

Molecule::Molecule( const std::string& name, const S32& molecule_idx, const S32& domain_idx ) 
  : ParamHolder( ),
    mName( name ), mMoleculeIdx( molecule_idx ), mDomainIdx( domain_idx )
{
}

const std::string& Molecule::getName() const {
  return mName;
}

S32 Molecule::getMoleculeIdx() const {
  return mMoleculeIdx;
}

S32 Molecule::getDomainIdx() const {
  return mDomainIdx;
}

void Molecule::setName(const std::string& name) {
  mName = name;
}

void Molecule::setMoleculeIdx(const S32& idx) {
  mMoleculeIdx = idx;
}

void Molecule::setDomainIdx(const S32& idx) {
  mDomainIdx = idx;
}
