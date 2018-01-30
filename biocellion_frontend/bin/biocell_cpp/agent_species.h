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

class AgentSpeciesMolecule {
public:
  AgentSpeciesMolecule( const S32& moleculeIdx, const S32& ODEIdx, const REAL& initialValue);
  S32 getMoleculeIdx() const;
  S32 getODEIdx() const;
  REAL getInitialValue() const;
  void setMoleculeIdx( const S32& value );
  void setODEIdx( const S32& value );
  void setInitialValue( const REAL& value );

protected:
  S32 mMoleculeIdx;
  S32 mODEIdx;
  REAL mInitialValue;
};

class AgentSpeciesInteraction {
public:
  AgentSpeciesInteraction( const S32& interactionIdx, const S32& modelRealIdx, const S32& mechRealIdx );
  
  S32 getInteractionIdx( ) const;
  S32 getModelRealIdx( ) const;
  S32 getMechRealIdx( ) const;

  void setInteractionIdx( const S32& value );
  void setModelRealIdx( const S32& value );
  void setMechRealIdx( const S32& value );
  
protected:
  S32 mInteractionIdx;
  S32 mModelRealIdx;
  S32 mMechRealIdx;
  
};


class AgentSpecies : public ParamHolder {

public:
  AgentSpecies( );
  AgentSpecies(const std::string& name, const std::string& speciesName, const S32& species_idx, const S32& num_real_param, const S32& num_int_param, const S32& num_bool_param, const S32& num_string_param);
  ~AgentSpecies();
  const BioModel* getModel() const;
  const std::string& getName() const;
  const std::string& getSpeciesName() const;
  S32 getSpeciesIdx() const;
  REAL getDMax() const;
  S32 getNumModelBools() const;
  S32 getNumModelReals() const;
  S32 getNumModelInts() const;
  S32 getNumMechReals() const;
  BOOL getUseMechForceReals() const;
  S32 getNumMechModelReals() const;
  S32 getIdxMechForceRealX() const;
  S32 getIdxMechForceRealY() const;
  S32 getIdxMechForceRealZ() const;
  S32 getNumMechModelInts() const;
  S32 getNumODEVariables() const;
  REAL getMoleculeValue(  const S32& moleculeIdx, const SpAgentState& spAgentState, const Vector< double >& v_y ) const;
  REAL getMoleculeValue(  const S32& moleculeIdx, const SpAgentState& spAgentState ) const;
  REAL getSurfaceArea( const SpAgentState& state ) const;
  REAL getGeometricVolume( const SpAgentState& state ) const;
  REAL getMassVolume( const SpAgentState& state ) const;

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
  const Vector< ODENetwork >& getODENetworks( ) const;
  Vector< ODENetwork >& getODENetworks( );
  const Vector< AgentSpeciesMolecule >& getMolecules() const;
  Vector< AgentSpeciesMolecule >& getMolecules();
  const Vector< AgentSpeciesInteraction >& getInteractions() const;
  Vector< AgentSpeciesInteraction >& getInteractions();
  S32 globalInteractionIdxToLocalIdx( const S32& interactionIdx ) const;
  const Vector< S32 >& getODEReactions() const;
  Vector< S32 >& getODEReactions();

  void setModel(const BioModel*& biomodel );
  void setName(const std::string& name);
  void setSpeciesName(const std::string& speciesName);
  void setSpeciesIdx(const S32& idx);
  void setDMax(const REAL& value);
  void setNumModelBools(const S32& value);
  void setNumModelReals(const S32& value);
  void setNumModelInts(const S32& value);
  void setNumMechReals(const S32& value);
  void setUseMechForceReals(const BOOL& value);
  void setIdxMechForceReals(const S32& idx_x, const S32& idx_y, const S32& idx_z);
  void addParticle( const S32& particleIdx, const S32& modelRealIdx, const REAL& initialValue );
  void addMolecule( const S32& moleculeIdx, const S32& odeVarIdx, const REAL& initialValue );
  void setInitialAgentState( SpAgentState& state ) const;
  void setNumODEVariables( const S32& numODEVariables );
  REAL getInteractionValue( const S32& interactionIdx, const S32& dim, const SpAgentState& state ) const;
  void setInteractionValue( const S32& interactionIdx, const S32& dim, const REAL& value, SpAgentState& state ) const;
  REAL getMechInteractionValue( const S32& interactionIdx, const S32& dim, const MechIntrctData& mechIntrctData ) const;
  void setMechInteractionValue( const S32& interactionIdx, const S32& dim, const REAL& value, MechIntrctData& mechIntrctData ) const;
  
  /**********************************************
   * support for model_routine_agent.cpp
   **********************************************/
  void addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) const;
  void spAgentCRNODERHS( const S32 odeNetIdx, const VIdx& vIdx, const SpAgent& spAgent, const NbrUBEnv& nbrUBEnv, const Vector<double>& v_y, Vector<double>& v_f ) const;
  void updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state) const;
  void updateSpAgentRadius( SpAgentState& state ) const;
  void spAgentSecretionBySpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentDisp ) const;
  void updateSpAgentBirthDeath( const VIdx& vIdx, const SpAgent& spAgent, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, BOOL& divide, BOOL& disappear ) const;
  void brownianMotion( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state, VReal& disp ) const;
  void limitMotion(VReal& disp) const;
  void setDisplacementFromMechanicalInteraction( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const;
  void adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const;
  void adjustSpAgentChemotaxis( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) const;
  void divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd ) const;

  /**********************************************
   * support for model_routine_config.cpp
   **********************************************/
  void updateSpAgentInfo( SpAgentInfo& spAgentInfo ) const;

  /**********************************************
   * support for model_routine_output.cpp
   **********************************************/
  void updateSpAgentOutput( const VIdx& vIdx, const SpAgent& spAgent, REAL& color, Vector<REAL>& v_extraScalar, Vector<VReal>& v_extraVector ) const;
  
protected:
  const BioModel *mModel;
  std::string mName;
  std::string mSpeciesName;
  S32 mSpeciesIdx;
  REAL        mDMax;
  S32         mNumModelBools, mNumModelReals, mNumModelInts, mNumMechReals, mNumODEVariables;
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
  Vector < ODENetwork > mODENetworks;
  Vector < EntryCondition* > mEntryConditions;
  Vector< AgentSpeciesMolecule > mMolecules;
  Vector< S32 > mODEReactions;
  Vector< AgentSpeciesInteraction > mInteractions;
};

#endif /* _AGENT_SPECIES_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
