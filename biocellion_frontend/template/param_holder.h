#ifndef _PARAM_HOLDER_H_
#define _PARAM_HOLDER_H_
#include "biomodel.h"
#include <map>

class ParamHolder {

public:
  ParamHolder( );
  ParamHolder(const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);
  virtual ~ParamHolder();

  // These routines don not make an index, if there isn't one.
  // It is an error to use invalid indexes.
  REAL getParamReal(const S32& idx) const;
  S32 getParamInt(const S32& idx) const;
  BOOL getParamBool(const S32& idx) const;
  const std::string& getParamString(const S32& idx) const;

  // These routines make an index, if there isn't one.
  // They are not efficient for inside tight loops.
  // This needs to be replaced with enumerated indexes.
  S32 getIdxReal(const std::string& param_name);
  S32 getIdxInt(const std::string& param_name);
  S32 getIdxBool(const std::string& param_name);
  S32 getIdxString(const std::string& param_name);
  
  // These routines do not make an index
  S32 getIdxReal(const std::string& param_name) const;
  S32 getIdxInt(const std::string& param_name) const;
  S32 getIdxBool(const std::string& param_name) const;
  S32 getIdxString(const std::string& param_name) const;
  
  // These routines do not make an index, if there isn't one.
  // It is an error to use invalid indexes.
  void setParamReal(const S32& idx, const REAL& param);
  void setParamInt(const S32& idx, const S32& param);
  void setParamBool(const S32& idx, const BOOL& param);
  void setParamString(const S32& idx, const std::string& param);
  
protected:
  Vector<REAL> mParamsReal;
  Vector<S32> mParamsInt;
  Vector<BOOL> mParamsBool;
  Vector<std::string> mParamsString;
  std::map<std::string, S32> mIdxReal, mIdxInt, mIdxBool, mIdxString;
};


#endif /* _PARAM_HOLDER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
