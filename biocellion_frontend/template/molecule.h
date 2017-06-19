#ifndef _MOLECULE_H_
#define _MOLECULE_H_
#include "biomodel.h"

class Molecule : public ParamHolder {
public:
  Molecule( );
  Molecule( const std::string& name, const S32& molecule_idx, const S32& domain_idx );

  const std::string& getName() const;
  S32 getMoleculeIdx() const;
  S32 getDomainIdx() const;

  void setName(const std::string& name);
  void setMoleculeIdx(const S32& idx);
  void setDomainIdx(const S32& idx);

protected:
  std::string mName;
  S32         mMoleculeIdx;
  S32         mDomainIdx;
};

#endif /* _MOLECULE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
