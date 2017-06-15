#ifndef _COMPUTATION_DOMAIN_H
#define _COMPUTATION_DOMAIN_H
#include "biomodel.h"

class Grid {
public:
  Grid();
  S32 getNDim() const { return mNDim; };
  S32 getNI() const { return mNI; };
  S32 getNJ() const { return mNJ; };
  S32 getNK() const { return mNK; };
  void setNDim(const S32& NDim);
  void setNI(const S32& NI);
  void setNJ(const S32& NJ);
  void setNK(const S32& NK);  
protected:
  S32 mNDim;
  S32 mNI;
  S32 mNJ;
  S32 mNK;
};

class XYZParam {
public:
  XYZParam( );

  const std::string& getName( ) const;
  S32 getX( ) const;
  S32 getY( ) const;
  S32 getZ( ) const;
  
  void setName( const std::string& value );
  void setX( const S32& value );
  void setY( const S32& value );
  void setZ( const S32& value );

protected:
  std::string mName;
  S32 mX, mY, mZ;
};

class Shape {
public:
  Shape( );

  const Vector< XYZParam >& getXYZParams( ) const;
  Vector< XYZParam >& getXYZParams( );
  
protected:
  Vector< XYZParam > mXYZParams;
};

class IsPermeableTo {
public:
  IsPermeableTo( );

  const std::string& getDetail( ) const;
  REAL getValue( ) const;

  void setDetail( const std::string& value );
  void setValue( const REAL& value );
  
protected:
  std::string mDetail;
  REAL mValue;
};

class BoundaryCondition : public ParamHolder {
public:
  BoundaryCondition( );
  virtual ~BoundaryCondition( );

  const std::string& getName( ) const;
  const std::string& getClass( ) const;
  const Vector< Shape* >& getShapes( ) const;
  Vector< Shape* >& getShapes( );
  const Vector< IsPermeableTo >& getIsPermeableTos( ) const;
  Vector< IsPermeableTo >& getIsPermeableTos( );
  
  void setName( const std::string& value );
  void setClass( const std::string& value );

protected:
  std::string mName;
  std::string mClass;
  Vector< Shape* > mShapes;
  Vector< IsPermeableTo > mIsPermeableTos;
};

class ComputationDomain : public ParamHolder {
public:
  ComputationDomain();
  virtual ~ComputationDomain();
  std::string getName() const { return mName; };
  S32 getComputationDomainIndex() const { return mComputationDomainIndex; };
  void setName(const std::string& name);
  void setComputationDomainIndex(const S32& computationDomainIndex);
  const Grid& getGrid() const { return mGrid; };
  Grid& getGrid() { return mGrid; };
  const Vector <BoundaryCondition* >& getBoundaryConditions() const { return mBoundaryConditions; };
  Vector <BoundaryCondition* >& getBoundaryConditions() { return mBoundaryConditions; };
  
protected:
  std::string mName;
  S32 mComputationDomainIndex;
  Grid mGrid;
  Vector< BoundaryCondition* > mBoundaryConditions;
};


#endif /*_COMPUTATION_DOMAIN_H*/
