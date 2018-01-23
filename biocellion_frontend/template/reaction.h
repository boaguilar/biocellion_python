#ifndef _REACTION_H_
#define _REACTION_H_
#include "biomodel.h"
#include <iostream>

class Reaction {
public:

  class Yield
  {
  public:
    Yield( );
    
    BOOL isSolute( ) const;
    BOOL isParticle( ) const;
    S32 getType( ) const;
    S32 getItemIdx( ) const;
    REAL getValue( ) const;
    BOOL isMolecule( ) const;

    void setSolute( );
    void setParticle( );
    void setType( const S32& value );
    void setItemIdx( const S32& value );
    void setValue( const REAL& value);
    void setMolecule( );

  protected:
    enum { TYPE_SOLUTE, TYPE_PARTICLE, TYPE_MOLECULE, NUM_TYPE };
    S32 mType;     // type of yield
    S32 mItemIdx;  // index into gBioModel->getSolutes() or gBioModel->getAgentSpecies()
    REAL mValue;   // yield amount
  };
  
  class KineticFactor
  {
  public:

    KineticFactor( );
    virtual ~KineticFactor( );

    const std::string& getClass( ) const;
    S32 getSoluteIdx( ) const;
    S32 getMoleculeIdx( ) const;
    S32 getSpeciesIdx( ) const;
    REAL getKi( ) const;
    REAL getKs( ) const;
    BOOL isSolute( ) const;
    BOOL isMolecule( ) const;
    BOOL isAgent( ) const;
    BOOL isNone( ) const;
    
    void setClass( const std::string& value );
    void setSoluteIdx( const S32& value );
    void setMoleculeIdx( const S32& value );
    void setSpeciesIdx( const S32& value );
    void setKi( const REAL& value );
    void setKs( const REAL& value );
    
    virtual REAL kineticValue( const REAL& solute_value ) const = 0;
    virtual REAL kineticValueAgent( const SpAgentState& state ) const;
  protected:
    
    std::string mKineticFactorClass;
    S32 mSoluteIdx;
    S32 mMoleculeIdx;
    S32 mSpeciesIdx;
    REAL mKi;
    REAL mKs;
  };

  class FirstOrderKinetic : public  KineticFactor {
  public:
    virtual REAL kineticValue( const REAL& solute_value ) const;
  };

  class SimpleInhibition : public  KineticFactor {
  public:
    virtual REAL kineticValue( const REAL& solute_value ) const;
  };

  class MonodKinetic : public  KineticFactor {
  public:
    virtual REAL kineticValue( const REAL& solute_value ) const;
  };

  class LinearKinetic : public  KineticFactor {
  public:
    virtual REAL kineticValue( const REAL& solute_value ) const;
  };

  class KineticPermeability : public KineticFactor {
  public:
    KineticPermeability();
    virtual REAL kineticValue( const REAL& solute_value ) const;
    REAL getPermeability() const;
    void setPermeability( const REAL& permeability );
  protected:
    REAL mPermeability;
  };
  
  class KineticAgentSurfaceArea : public  KineticFactor {
  public:
    virtual REAL kineticValue( const REAL& solute_value ) const;
    virtual REAL kineticValueAgent( const SpAgentState& state ) const;
  };

  Reaction( );
  Reaction( const std::string& name, const std::string& reactionClass,
            const S32& catalyzedBy, const S32& catalyst, const REAL& muMax
            );
  virtual ~Reaction( );

  const std::string& getName() const { return mName; };
  const std::string& getClass() const { return mReactionClass; };
  S32 getCatalyzedBy() const { return mCatalyzedBy; };
  S32 getCatalyst() const { return mCatalyst; };
  REAL getMuMax() const { return mMuMax;};

  const Vector< Yield >& getYields( ) const;
  Vector< Yield >& getYields( );
  BOOL getYieldForMolecule( const S32& moleculeIdx, Yield& yield ) const;
  const Vector< KineticFactor* >& getKineticFactors( ) const;
  Vector< KineticFactor* >& getKineticFactors( );
  const Vector< BOOL >& getActiveAgentSpecies( ) const;
  Vector< BOOL >& getActiveAgentSpecies( );

  void setName(const std::string& value);
  void setClass(const std::string& value);
  void setCatalyzedBy(const S32& value); 
  void setCatalyst(const S32& value); 
  void setMuMax(const REAL& value);

  REAL getKineticFactor( const NbrUBEnv& nbrUBEnv, const VReal& vOffset, SpAgentState& state ) const;
  REAL getKineticFactor( const NbrUBEnv& nbrUBEnv, const SpAgent& spAgent, const Vector< double >& v_y ) const;
  REAL getKineticFactor( const UBEnv& ubEnv, const VReal& vOffset ) const;
  REAL getKineticFactor( const UBEnv& ubEnv, const VReal& vOffset, const SpAgent& spAgent ) const;
  REAL getSoluteYield( const S32& solute_idx, const SpAgent& spAgent ) const;
  REAL getMoleculeYield( const S32& moleculeIdx, const SpAgent& spAgent ) const;
  REAL getParticleYield( const S32& particle_idx, const SpAgentState& spAgentState ) const;

protected:
  std::string mName;
  std::string mReactionClass;
  S32         mCatalyzedBy;
  S32         mCatalyst;
  REAL        mMuMax;

  Vector< Yield > mYields;
  Vector< KineticFactor* > mKineticFactors;
  // index is SpeciesIdx, value is whether this reaction is active in that species
  Vector< BOOL > mActiveAgentSpecies;
};

std::ostream& operator<<( std::ostream& os, const Reaction::Yield& rhs );
std::ostream& operator<<( std::ostream& os, const Reaction::KineticFactor& rhs );
// std::ostream& operator<<( std::ostream& os, const Reaction::FirstOrderKinetic& rhs );
// std::ostream& operator<<( std::ostream& os, const Reaction::SimpleInhibition& rhs );
// std::ostream& operator<<( std::ostream& os, const Reaction::MonodKinetic& rhs );
// std::ostream& operator<<( std::ostream& os, const Reaction::LinearKinetic& rhs );

#endif /* _REACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
