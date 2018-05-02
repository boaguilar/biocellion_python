#ifndef _ENTRY_CONDITION_H
#define _ENTRY_CONDITION_H
#include "biomodel.h"
#include <string>

class EntryCondition : public ParamHolder {
public:
  EntryCondition();
  ~EntryCondition();
  S32 getTimestep() const { return mTimestep; };
  std::string getMolecule() const { return mMolecule; }; 
  std::string getType() const { return mType; }; 
  S32 getItemIdx() const { return mItemIdx; };
  REAL getValue() const { return mValue; }; 
  const Vector<Coordinates *>& getCoordinates( ) const;
  Vector<Coordinates *>& getCoordinates( );
  AgentSpecies* getAgentSpecies() const;

  void setMolecule( const std::string& molecule );
  void setType( const std::string& type );
  void setTimestep( const S32& timestep );
  void setItemIdx( const S32& ItemIdx );
  void setValue( const REAL& value );
  void setAgentSpecies(AgentSpecies* species);

  void perturbAgents( const VIdx& vIdx, const VReal& vOffset, SpAgentState& state );

protected:
  std::string mMolecule; 
  std::string mType;
  S32 mTimestep;
  S32 mItemIdx;
  REAL mValue;
  Vector<Coordinates *> mCoordinates;
  AgentSpecies *mAgentSpecies; 
};

#endif /*_ENTRY_CONDITION_H*/
