#ifndef _INTERACTION_H_
#define _INTERACTION_H_
#include "biomodel.h"

class Interaction : public ParamHolder {
public:
  Interaction( );
  Interaction( const std::string& name, const S32& interaction_idx );

  const std::string& getName() const;
  S32 getInteractionIdx() const;

  void setName(const std::string& name);
  void setInteractionIdx(const S32& idx);

protected:
  std::string mName;
  S32         mInteractionIdx;
};

#endif /* _INTERACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
