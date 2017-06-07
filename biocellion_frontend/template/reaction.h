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

    void setSolute( );
    void setParticle( );
    void setType( const S32& value );
    void setItemIdx( const S32& value );
    void setValue( const REAL& value);
    
  protected:
    enum { TYPE_SOLUTE, TYPE_PARTICLE, NUM_TYPE };
    S32 mType;     // type of yield
    S32 mItemIdx;  // index into gBioModel->getSolutes() or gAgentSpecies
    REAL mValue;   // yield amount
  };
  
  class KineticFactor
  {
  public:

    KineticFactor( );
    virtual ~KineticFactor( );

    const std::string& getClass( ) const;
    S32 getSolute( ) const;
    S32 getMolecule( ) const;
    REAL getKi( ) const;
    REAL getKs( ) const;

    void setClass( const std::string& value );
    void setSolute( const S32& value );
    void setMolecule( const S32& value );
    void setKi( const REAL& value );
    void setKs( const REAL& value );

    virtual REAL kineticValue( const REAL& solute_value ) const = 0;

  protected:
    
    std::string mKineticFactorClass;
    S32 mSoluteIdx;
    S32 mMoleculeIdx;
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
  const Vector< KineticFactor* >& getKineticFactors( ) const;
  Vector< KineticFactor* >& getKineticFactors( );

  void setName(const std::string& value);
  void setClass(const std::string& value);
  void setCatalyzedBy(const S32& value); 
  void setCatalyst(const S32& value); 
  void setMuMax(const REAL& value);

  REAL getKineticFactor( const UBEnv& ubEnv, const VReal& vOffset ) const;
  REAL getSoluteYield( const S32& solute_idx, const SpAgent& spAgent ) const;
  REAL getParticleYield( const S32& particle_idx, const SpAgent& spAgent ) const;

protected:
  std::string mName;
  std::string mReactionClass;
  S32         mCatalyzedBy;
  S32         mCatalyst;
  REAL        mMuMax;

  Vector< Yield > mYields;
  Vector< KineticFactor* > mKineticFactors;
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
