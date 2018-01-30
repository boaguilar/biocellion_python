#ifndef _INIT_AREA_H_
#define _INIT_AREA_H_
#include "biomodel.h"

class AgentSpecies;
class InitArea;

#include <string>

class Coordinates {
public:
  Coordinates( );

  REAL getX( ) const;
  REAL getY( ) const;
  REAL getZ( ) const;
  REAL getR( ) const;

  void setX( const REAL& value );
  void setY( const REAL& value );
  void setZ( const REAL& value );
  void setR( const REAL& value );
  
protected:
  REAL mX, mY, mZ, mR;
};

class InitArea {
public:
  InitArea( ); 
  ~InitArea( );
  REAL getNumber() const;
  std::string getShape() const; 
  REAL getBirthday() const;
  const Vector<Coordinates *>& getCoordinates( ) const;
  Vector<Coordinates *>& getCoordinates( );
  AgentSpecies* getAgentSpecies() const;
  
  void setNumber(const REAL& value);
  void setShape(const std::string& value);
  void setBirthday(const REAL& value);
  void setAgentSpecies(AgentSpecies* species);

  void addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset );
  void addSpAgentsDefault( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset );
  
protected:
  REAL mNumber;
  std::string mShape;
  REAL mBirthday;
  Vector<Coordinates *> mCoordinates;
  AgentSpecies *mAgentSpecies;
};

#endif /* _INIT_AREA_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
