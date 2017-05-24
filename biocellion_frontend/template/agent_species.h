#ifndef _AGENT_SPECIES_H_
#define _AGENT_SPECIES_H_
#include "biocellion.h"
#include "init_area.h"
class InitArea;
#include <string>
#include <map>

class AgentSpeciesParticle {
public:
  AgentSpeciesParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue );
  S32 getParticleIdx( ) const;
  S32 getModelRealIdx( ) const;
  REAL getInitialValue( ) const;

  void setParticleIdx( const S32& value );
  void setModelRealIdx( const S32& value );
  void setInitialValue( const REAL& value );

protected:
  S32 mParticleIdx;
  S32 mModelRealIdx;
  REAL mInitialValue;
  
};

class AgentSpecies {

public:
  AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);
  ~AgentSpecies();
  const std::string& getName() const;
  const std::string& getSpeciesName() const;
  S32 getSpeciesIdx() const;
  REAL getParamReal(const S32& idx) const;
  S32 getParamInt(const S32& idx) const;
  BOOL getParamBool(const S32& idx) const;
  const std::string& getParamString(const S32& idx) const;
  REAL getDMax() const;
  S32 getNumModelBools() const;
  S32 getNumModelReals() const;
  S32 getNumModelInts() const;
  BOOL getUseMechForceReals() const;
  S32 getNumMechModelReals() const;
  S32 getIdxMechForceRealX() const;
  S32 getIdxMechForceRealY() const;
  S32 getIdxMechForceRealZ() const;
  S32 getNumMechModelInts() const;
  const Vector<InitArea *>& getInitAreas( ) const;
  Vector<InitArea *>& getInitAreas( );

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
  void setDMax(const REAL& value);
  void setNumModelBools(const S32& value);
  void setNumModelReals(const S32& value);
  void setNumModelInts(const S32& value);
  void setUseMechForceReals(const BOOL& value);
  void setIdxMechForceReals(const S32& idx_x, const S32& idx_y, const S32& idx_z);
  void addParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue );
  void setInitialAgentState( SpAgentState& state ) const;
  
protected:
  std::string mName;
  std::string mSpeciesName;
  S32 mSpeciesIdx;
  Vector<REAL> mParamsReal;
  Vector<S32> mParamsInt;
  Vector<BOOL> mParamsBool;
  Vector<std::string> mParamsString;
  std::map<std::string, S32> mIdxReal, mIdxInt, mIdxBool, mIdxString;

  REAL        mDMax;
  S32         mNumModelBools, mNumModelReals, mNumModelInts;
  BOOL        mUseMechForceReals;
  Vector<S32> mIdxMechForceReals;
  Vector<S32> mIdxMechModelInts;
  Vector<InitArea *> mInitAreas;
  Vector<AgentSpeciesParticle> mParticles;
};

#endif /* _AGENT_SPECIES_H_ */
