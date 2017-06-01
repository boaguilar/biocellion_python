#include "biomodel.h"

/*********************************
 *  ParamHolder
 *********************************/
ParamHolder::ParamHolder( ) 
  : mParamsReal( 0 ),
    mParamsInt( 0 ),
    mParamsBool( 0 ),
    mParamsString( 0 )
{
}


ParamHolder::ParamHolder(const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param)
  : mParamsReal(num_real_param),
    mParamsInt(num_int_param),
    mParamsBool(num_bool_param),
    mParamsString(num_string_param)
{
  //empty
}

ParamHolder::~ParamHolder()
{
}

REAL ParamHolder::getParamReal(const S32& idx) const
{
  return mParamsReal[idx];
}

S32 ParamHolder::getParamInt(const S32& idx) const
{
  return mParamsInt[idx];
}

BOOL ParamHolder::getParamBool(const S32& idx) const
{
  return mParamsBool[idx];
}

const std::string& ParamHolder::getParamString(const S32& idx) const
{
  return mParamsString[idx];
}

template <class T, class U>
S32 getIdxGeneric( const std::string& param_name, std::map<std::string, T>& idx_map, Vector<U>& params )
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
  if( i >= (S32 )params.size( ) ) {
    params.resize( i+1 );
  }
  CHECK( i >= 0 );
  CHECK( i < (S32) params.size( ) );
  return i;
}


template <class T, class U>
S32 getIdxGeneric( const std::string& param_name, const std::map<std::string, T>& idx_map, const Vector<U>& params )
{
  S32 i = -1;
  typename std::map<std::string, T>::const_iterator it = idx_map.find(param_name);
  if( it != idx_map.end() ) {
    i = it->second;
  }
  CHECK( i >= 0 );
  CHECK( i < (S32) params.size( ) );
  return i;
}

S32 ParamHolder::getIdxReal(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxReal, mParamsReal );
}

S32 ParamHolder::getIdxInt(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxInt, mParamsInt );
}

S32 ParamHolder::getIdxBool(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxBool, mParamsBool );
}

S32 ParamHolder::getIdxString(const std::string& param_name)
{
  return getIdxGeneric( param_name, mIdxString, mParamsString );
}

S32 ParamHolder::getIdxReal(const std::string& param_name) const
{
  return getIdxGeneric( param_name, mIdxReal, mParamsReal );
}

S32 ParamHolder::getIdxInt(const std::string& param_name) const
{
  return getIdxGeneric( param_name, mIdxInt, mParamsInt );
}

S32 ParamHolder::getIdxBool(const std::string& param_name) const
{
  return getIdxGeneric( param_name, mIdxBool, mParamsBool );
}

S32 ParamHolder::getIdxString(const std::string& param_name) const
{
  return getIdxGeneric( param_name, mIdxString, mParamsString );
}

void ParamHolder::setParamReal(const S32& idx, const REAL& param)
{
  mParamsReal[idx] = param;
}

void ParamHolder::setParamInt(const S32& idx, const S32& param)
{
  mParamsInt[idx] = param;
}

void ParamHolder::setParamBool(const S32& idx, const BOOL& param)
{
  mParamsBool[idx] = param;
}

void ParamHolder::setParamString(const S32& idx, const std::string& param)
{
  mParamsString[idx] = param;
}
