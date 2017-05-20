from agent_species import Param, ParamHolder
from agent_species import AllAgentSpecies
from particle import AllParticles
from agent_grid import AgentGrid

class Simulator( ParamHolder ):

    def __init__(self):
        self.mName = "simulator"
        ParamHolder.__init__(self)
        self.addParam( Param( "restartPreviousRun", "bool", False ) )
        self.addParam( Param( "randomSeed", "int", 75321 ) )
        self.addParam( Param( "outputPeriod", "hr", 0.01 ) )
        self.addParam( Param( "chemostat", "bool", False ) )
        self.addParam( Param( "diffusionReactionOnAgentTime", "bool", False ) )
        self.addParam( Param( "agentTimeStep", "hr", 0.01 ) )

        self.mTimeStep = TimeStep()
        return

    def getTimeStep( self ):
        return self.mTimeStep
        
    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += str( self.mTimeStep ) + "\n"
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)
        
class TimeStep( ParamHolder ):

    def __init__(self):
        self.mName = "timeStep"
        ParamHolder.__init__(self)
        self.addParam( Param( "adaptive", "bool", False ) )
        self.addParam( Param( "timeStepIni", "hr", 0.01 ) )
        self.addParam( Param( "timeStepMin", "hr", 0.01 ) )
        self.addParam( Param( "timeStepMax", "hr", 0.01 ) )
        self.addParam( Param( "endOfSimulation", "hr", 0.01 ) )
        return

    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)


class IDynoMiCS( ParamHolder ):

    def __init__( self ):
        self.mName = "idynomics"
        ParamHolder.__init__(self)

        self.mSimulator = Simulator()
        self.mAgentGrid = AgentGrid()
        self.mAgentSpecies = AllAgentSpecies()
        self.mParticles = AllParticles()
        
        return

    def getSimulator( self ):
        return self.mSimulator
        
    def getAgentGrid( self ):
        return self.mAgentGrid
        
    def getAgentSpecies( self ):
        return self.mAgentSpecies
        
    def getParticles( self ):
        return self.mParticles
        
    def getName(self):
        return self.mName

    def organizeChildren( self ):
        for t in self.mChildren:
            print( "Still have children of type: " + str( t ) )
        #     for i in range( len( self.mChildren[ t ] ) ):
        #         if t == 'Particle':
        #             c = self.mChildren[ t ][ i ]
        #             if c.getName() not in self.mParticles.getParticles():
        #                 self.mParticles.addParticle( c.getName(), c )
        #             self.mChildren[ t ][ i ] = None

        # t = 'Particle'
        # if t in self.mChildren:
        #     print( "Cleaning up " + t + " children." )
        #     del self.mChildren[ t ]
                    
        return

    def __str__(self):
        s  = "<%s>:\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += str( self.mSimulator ) + "\n"
        s += str( self.mAgentGrid ) + "\n"
        s += str( self.mAgentSpecies ) + "\n"
        s += str( self.mParticles )  + "\n"
        s += "</%s>:\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)
        

def main():
    print("UNCHECKED")
    return
    
if __name__ == "__main__":
    main()
        

