#ifndef _AGENT_SPECIES_H_
#define _AGENT_SPECIES_H_
#include "biomodel.h"

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

class AgentSpecies : public ParamHolder {

public:
  AgentSpecies( );
  AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);
  ~AgentSpecies();
  const std::string& getName() const;
  const std::string& getSpeciesName() const;
  S32 getSpeciesIdx() const;
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
  const Vector<Adhesion *>& getAdhesions( ) const;
  Vector<Adhesion *>& getAdhesions( );
  const Vector<DistanceJunction *>& getDistanceJunctions( ) const;
  Vector<DistanceJunction *>& getDistanceJunctions( );
  const Vector<TightJunction *>& getTightJunctions( ) const;
  Vector<TightJunction *>& getTightJunctions( );
  const Vector< AgentSpeciesParticle >& getParticles( ) const;
  Vector< AgentSpeciesParticle >& getParticles( );
  const Vector< EntryCondition* >& getEntryConditions( ) const;
  Vector< EntryCondition* >& getEntryConditions( );

  const Vector< Chemotaxis * >& getChemotaxis() const;
  Vector< Chemotaxis * >& getChemotaxis();
  const Vector< S32 >& getReactions() const;
  Vector< S32 >& getReactions();

  void setName(const std::string& name);
  void setSpeciesName(const std::string& speciesName);
  void setSpeciesIdx(const S32& idx);
  void setDMax(const REAL& value);
  void setNumModelBools(const S32& value);
  void setNumModelReals(const S32& value);
  void setNumModelInts(const S32& value);
  void setUseMechForceReals(const BOOL& value);
  void setIdxMechForceReals(const S32& idx_x, const S32& idx_y, const S32& idx_z);
  void addParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue );
  void setInitialAgentState( SpAgentState& state ) const;

  // support for model_routine_agent.cpp
  void adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const;
  void adjustSpAgentChemotaxis( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const;
  void updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state) const;
  void updateSpAgentRadius( SpAgentState& state ) const;
  void updateSpAgentBirthDeath( const VIdx& vIdx, const SpAgent& spAgent, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, BOOL& divide, BOOL& disappear );
  void divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd );
protected:
  std::string mName;
  std::string mSpeciesName;
  S32 mSpeciesIdx;
  REAL        mDMax;
  S32         mNumModelBools, mNumModelReals, mNumModelInts;
  BOOL        mUseMechForceReals;
  Vector<S32> mIdxMechForceReals;
  Vector<S32> mIdxMechModelInts;
  Vector<InitArea *> mInitAreas;
  Vector<Adhesion *> mAdhesions;
  Vector<DistanceJunction *> mDistanceJunctions;
  Vector<TightJunction *> mTightJunctions;
  Vector<AgentSpeciesParticle> mParticles;
  Vector <Chemotaxis * > mChemotaxis;
  Vector < S32 > mReactions;
  Vector < EntryCondition* > mEntryConditions;
};

#endif /* _AGENT_SPECIES_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
