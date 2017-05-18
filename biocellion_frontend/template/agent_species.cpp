#include "biocellion.h"
#include "agent_species.h"

AgentSpecies::AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param)
  : mName(name), mSpeciesName(speciesName), mSpeciesIdx(species_idx),
    mParamsReal(num_real_param),
    mParamsInt(num_int_param),
    mParamsBool(num_bool_param),
    mParamsString(num_string_param)
{
  //empty
}

const std::string& AgentSpecies::getName() const
{
  return mName;
}

const std::string& AgentSpecies::getSpeciesName() const
{
  return mSpeciesName;
}

S32 AgentSpecies::getSpeciesIdx() const
{
  return mSpeciesIdx;
}

REAL AgentSpecies::getParamReal(const S32& idx) const
{
  return mParamsReal[idx];
}

S32 AgentSpecies::getParamInt(const S32& idx) const
{
  return mParamsInt[idx];
}

BOOL AgentSpecies::getParamBool(const S32& idx) const
{
  return mParamsBool[idx];
}

const std::string& AgentSpecies::getParamString(const S32& idx) const
{
  return mParamsString[idx];
}

template <class T, class U>
S32 getIdxGeneric( const std::string& param_name, std::map<std::string, T>& idx_map, const U& size )
{
  S32 i = -1;
  typename std::map<std::string, T>::const_iterator it = idx_map.find(param_name);
  if( it != idx_map.end() ) {
    i = it->second;
  } else {
    for( it = idx_map.begin(); it != idx_map.end(); it++ ) {
      if( it->second > i ) {
        i = it->second;
      }
    }
    i++;
    idx_map[ param_name ] = i;
  }
  CHECK( i >= 0 );
  CHECK( ((U)i) < size );
  return i;
}

S32 AgentSpecies::getIdxReal(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxReal, mParamsReal.size() );
}

S32 AgentSpecies::getIdxInt(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxInt, mParamsInt.size() );
}

S32 AgentSpecies::getIdxBool(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxBool, mParamsBool.size() );
}

S32 AgentSpecies::getIdxString(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxString, mParamsString.size() );
}

void AgentSpecies::setName(const std::string& name)
{
  mName = name;
}

void AgentSpecies::setSpeciesName(const std::string& speciesName)
{
  mSpeciesName = speciesName;
}

void AgentSpecies::setSpeciesIdx(const S32& idx)
{
  mSpeciesIdx = idx;
}

void AgentSpecies::setParamReal(const S32& idx, const REAL& param)
{
  mParamsReal[idx] = param;
}

void AgentSpecies::setParamInt(const S32& idx, const S32& param)
{
  mParamsInt[idx] = param;
}

void AgentSpecies::setParamBool(const S32& idx, const BOOL& param)
{
  mParamsBool[idx] = param;
}

void AgentSpecies::setParamString(const S32& idx, const std::string& param)
{
  mParamsString[idx] = param;
}
