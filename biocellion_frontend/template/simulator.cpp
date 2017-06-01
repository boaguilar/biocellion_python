#include "biomodel.h"

Simulator::Simulator(
		     const BOOL& restartPreviousRun, const BOOL& chemostat, const BOOL& diffusionReactionOnAgentTime,
		     const REAL& randomSeed, const REAL& outputPeriod, const REAL& agentTimeStep
		     )
  : mRestartPreviousRun(restartPreviousRun), mChemostat(chemostat), mDiffusionReactionOnAgentTime(diffusionReactionOnAgentTime),
    mRandomSeed(randomSeed), mOutputPeriod(outputPeriod), mAgentTimeStep(agentTimeStep)
{
  //empty
}

Simulator::Simulator(
		     const BOOL& restartPreviousRun, const BOOL& chemostat, const BOOL& diffusionReactionOnAgentTime,
		     const REAL& randomSeed, const REAL& outputPeriod, const REAL& agentTimeStep, const TimeStep& simTimeStep
		     )
  :  mRestartPreviousRun(restartPreviousRun), mChemostat(chemostat), mDiffusionReactionOnAgentTime(diffusionReactionOnAgentTime),
     mRandomSeed(randomSeed), mOutputPeriod(outputPeriod), mAgentTimeStep(agentTimeStep), mTimeStep(simTimeStep)
{
  //empty

}

const TimeStep& Simulator::getTimeStep() const
{
  return mTimeStep;
}

TimeStep& Simulator::getTimeStep()
{
  return mTimeStep;
}

void Simulator::setRestartPreviousRun(const BOOL& restartPreviousRun)
{
  mRestartPreviousRun = restartPreviousRun;
}
void Simulator::setChemostat(const BOOL& chemostat) 
  {
    mChemostat = chemostat;
  }

void Simulator::setDiffusionReactionOnAgentTime(const BOOL& diffusionReactionOnAgentTime)  
  {
    mDiffusionReactionOnAgentTime = diffusionReactionOnAgentTime;
  }
void Simulator::setRandomSeed(const REAL& randomSeed)
  {
    mRandomSeed = randomSeed; 
  }
void Simulator::setOutputPeriod(const REAL& outputPeriod)
  {
    mOutputPeriod = outputPeriod;
  }
void Simulator::setAgentTimeStep(const REAL& agentTimeStep)
  {
    mAgentTimeStep = agentTimeStep;
  }

TimeStep::TimeStep(const BOOL& adaptive, const REAL& timeStepIni, const REAL& timeStepMin, const REAL& timeStepMax, const REAL& endOfSimulation)
  :mAdaptive(adaptive), mTimeStepIni(timeStepIni), mTimeStepMin(timeStepMin), mTimeStepMax(timeStepMax), mEndOfSimulation(endOfSimulation)
{
  //empty
}
TimeStep::TimeStep()
  :mAdaptive(false), mTimeStepIni(0), mTimeStepMin(0), mTimeStepMax(0), mEndOfSimulation(0)
{
  //empty
}


void TimeStep::setAdaptive(const BOOL& adaptive)
{
  mAdaptive = adaptive;
}
void TimeStep::setTimeStepIni(const REAL& timeStepIni)
{
  mTimeStepIni = timeStepIni;
}
void TimeStep::setTimeStepMin(const REAL& timeStepMin)
{
  mTimeStepMin = timeStepMin;
}
void TimeStep::setTimeStepMax(const REAL& timeStepMax)
{
  mTimeStepMax = timeStepMax;
}
void TimeStep::setEndOfSimulation(const REAL& endOfSimulation)
{
  mEndOfSimulation = endOfSimulation;
}
