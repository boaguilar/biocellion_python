#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include "biocellion.h"

class TimeStep
{
public:
  TimeStep(const BOOL& adaptive, const REAL& timeStepIni, const REAL& timeStepMin, const REAL& timeStepMax, const REAL& endOfSimulation);
  TimeStep();
  BOOL getAdaptive() const { return mAdaptive; };
  REAL getTimeStepIni() const { return mTimeStepIni; };
  REAL getTimeStepMin() const { return mTimeStepMin; };
  REAL getTimeStepMax() const { return mTimeStepMax; };
  REAL getEndOfSimulation() const { return mEndOfSimulation; };
  void setAdaptive(const BOOL& adaptive);
  void setTimeStepIni(const REAL& timeStepIni);
  void setTimeStepMin(const REAL& timeStepMin);
  void setTimeStepMax(const REAL& timeStepMax);
  void setEndOfSimulation(const REAL& endOfSimulation);
  
protected:
  BOOL mAdaptive;
  REAL mTimeStepIni;
  REAL mTimeStepMin;
  REAL mTimeStepMax;
  REAL mEndOfSimulation;
};

class Simulator
{

public:
  Simulator(
		     const BOOL& restartPreviousRun, const BOOL& chemostat, const BOOL& diffusionReactionOnAgentTime,
		     const REAL& randomSeed, const REAL& outputPeriod, const REAL& agentTimeStep
		     );
  Simulator(
	    const BOOL& restartPreviousRun, const BOOL& chemostat, const BOOL& diffusionReactionOnAgentTime,
	    const REAL& randomSeed, const REAL& outputPeriod, const REAL& agentTimeStep, const TimeStep& simTimeStep
	    );
  BOOL getRestartPreviousRun() const { return mRestartPreviousRun; };
  BOOL getChemostat() const { return mChemostat; };
  BOOL getDiffusionReactionOnAgentTime() const { return mDiffusionReactionOnAgentTime; };
  REAL getRandomSeed() const { return mRandomSeed; };
  REAL getOutputPeriod() const { return mOutputPeriod; };
  REAL getAgentTimeStep() const { return mAgentTimeStep; };
  const TimeStep& getTimeStep() const;
  TimeStep& getTimeStep();
  void setRestartPreviousRun(const BOOL& restartPreviousRun);
  void setChemostat(const BOOL& chemostat);
  void setDiffusionReactionOnAgentTime(const BOOL& diffusionReactionOnAgentTime);
  void setRandomSeed(const REAL& randomSeed);
  void setOutputPeriod(const REAL& outputPeriod);
  void setAgentTimeStep(const REAL& agentTimeStep);
  
protected:
  BOOL mRestartPreviousRun;
  BOOL mChemostat;
  BOOL mDiffusionReactionOnAgentTime;
  REAL mRandomSeed;
  REAL mOutputPeriod;
  REAL mAgentTimeStep;
  TimeStep mTimeStep;
};

#endif /* _SIMULATOR_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
