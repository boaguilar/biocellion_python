#ifndef _AGENT_SPECIES_H_
#define _AGENT_SPECIES_H_
#include "biocellion.h"
#include <string>
#include <map>

class AgentSpecies {

public:
  AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);
  
  const std::string& getName() const;
  const std::string& getSpeciesName() const;
  S32 getSpeciesIdx() const;
  REAL getParamReal(const S32& idx) const;
  S32 getParamInt(const S32& idx) const;
  BOOL getParamBool(const S32& idx) const;
  const std::string& getParamString(const S32& idx) const;

  // these routines make an index, if there isn't one.
  // they are not efficient for inside tight loops
  S32 getIdxReal(const std::string& param_name);
  S32 getIdxInt(const std::string& param_name);
  S32 getIdxBool(const std::string& param_name);
  S32 getIdxString(const std::string& param_name);
  
  void setName(const std::string& name);
  void setSpeciesName(const std::string& speciesName);
  void setSpeciesIdx(const S32& idx);
  void setParamReal(const S32& idx, const REAL& param);
  void setParamInt(const S32& idx, const S32& param);
  void setParamBool(const S32& idx, const BOOL& param);
  void setParamString(const S32& idx, const std::string& param);
  
protected:
  std::string mName;
  std::string mSpeciesName;
  S32 mSpeciesIdx;
  Vector<REAL> mParamsReal;
  Vector<S32> mParamsInt;
  Vector<BOOL> mParamsBool;
  Vector<std::string> mParamsString;
  std::map<std::string, S32> mIdxReal, mIdxInt, mIdxBool, mIdxString;
};

#endif /* _AGENT_SPECIES_H_ */
