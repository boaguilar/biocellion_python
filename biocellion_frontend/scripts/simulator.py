from agent_species import Param, ParamHolder
from agent_species import AllAgentSpecies
from particle import AllParticles
from agent_grid import AgentGrid
from world import World

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
        self.mPrivateNumberHiddenParams = [ "randomSeed", "outputPeriod", "agentTimeStep" ]
        self.mPrivateBoolHiddenParams = [ "restartPreviousRun", "chemostat", "diffusionReactionOnAgentTime" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mTimeStep = TimeStep()
        return

    def getNumBaselineSteps( self ):
        self.getTimeStep().getParam( 'endOfSimulation' ).checkUnit( 'hr' )
        self.getParam( 'agentTimeStep' ).checkUnit( 'hr' )

        end_of_simulation = self.getTimeStep().getParam( 'endOfSimulation' ).getValue()
        agent_time_step = self.getParam( 'agentTimeStep' ).getValue()
        number_steps = int( end_of_simulation / agent_time_step )

        return number_steps

    def getBioModelH( self, indent, depth ):
        lines  = "// FIXME: Simulator \n"
        return lines

    def getInitializeBioModel( self, indent, depth ):
        varname = "gSimulator"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "%s = new Simulator( %s, %s, %s, %s, %s, %s );" % (varname, "false", "false", "true", "75321", "0.01", "0.01", ) )
        lines.append( ParamHolder.toCpp( self, varname, indent, depth ) )

        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      [] )
        lines.append( s )
        
        s = self.mTimeStep.getInitializeBioModel( "%s->getTimeStep()" % ( varname, ), indent, depth )
        lines.append( s )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

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

        self.mPrivateNumberHiddenParams = [ "timeStepIni", "timeStepMin", "timeStepMax", "endOfSimulation" ]
        self.mPrivateBoolHiddenParams = [ "adaptive" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        return

    def getInitializeBioModel( self, varname, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( ParamHolder.toCpp( self, varname, indent, depth ) )

        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      [] )
        lines.append( s )
        
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    

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

        self.mSimulator = Simulator( )
        self.mWorld = World( )
        self.mAgentGrid = AgentGrid()
        self.mAgentSpecies = AllAgentSpecies()
        self.mParticles = AllParticles()
        
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.mSimulator.getBioModelH( indent, depth ) )
        lines.append( self.mWorld.getBioModelH( indent, depth ) )
        lines.append( self.mAgentGrid.getBioModelH( indent, depth ) )
        lines.append( self.mAgentSpecies.getBioModelH( indent, depth ) )
        lines.append( self.mParticles.getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines  = [ ]
        lines.append( self.mSimulator.getInitializeBioModel( indent, depth ) )
        lines.append( self.mWorld.getInitializeBioModel( indent, depth ) )
        lines.append( self.mAgentGrid.getInitializeBioModel( indent, depth ) )
        lines.append( self.mAgentSpecies.getInitializeBioModel( indent, depth ) )
        lines.append( self.mParticles.getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def getSimulator( self ):
        return self.mSimulator
        
    def getWorld( self ):
        return self.mWorld
        
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
        s += str( self.mWorld ) + "\n"
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
        

