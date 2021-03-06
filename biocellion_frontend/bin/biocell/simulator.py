from biocell import *

class Simulator( ParamHolder ):

    def __init__(self):
        self.mName = "simulator"
        ParamHolder.__init__(self)
        self.setPrefix( "SIMULATOR" )
        self.addParam( Param( "restartPreviousRun", "bool", False, False ) )
        self.addParam( Param( "randomSeed", "int", 75321, False ) )
        self.addParam( Param( "outputPeriod", "hr", 0.01, True ) )
        self.addParam( Param( "chemostat", "bool", False, False ) )
        self.addParam( Param( "diffusionReactionOnAgentTime", "bool", False, False ) )
        self.addParam( Param( "agentTimeStep", "hr", 0.01, True ) )
        self.addParam( Param( "numStateAndGridTimeStepsPerBaseline", "int", 1, False ) )
        self.mPrivateNumberHiddenParams = [ "randomSeed", "outputPeriod", "agentTimeStep" ]
        self.mPrivateBoolHiddenParams = [ "restartPreviousRun", "chemostat", "diffusionReactionOnAgentTime" ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mTimeStep = SimulatorTimeStep()
        return

    def getOutputInterval( self ):
        interval = int( 0.5 + self.getParam( 'outputPeriod' ).getValue( ) / self.getParam( 'agentTimeStep' ).getValue( ) )
        return interval

    def getNumBaselineSteps( self ):
        self.getTimeStep().getParam( 'endOfSimulation' ).checkUnit( 'hour' )
        self.getParam( 'agentTimeStep' ).checkUnit( 'hour' )

        end_of_simulation = self.getTimeStep().getParam( 'endOfSimulation' ).getValue()
        agent_time_step = self.getParam( 'agentTimeStep' ).getValue()
        number_steps = int( end_of_simulation / agent_time_step )

        return number_steps

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        return "\n".join( lines )

    def getAllParamNames(self, indent, depth):
        all_params = { }
        all_order = [ ]
        for param_name in self.mParamOrder:
            if param_name not in all_params:
                all_params[ param_name ] = self.getParam( param_name )
                all_order.append( param_name )
        lines = []
        for n in all_order:
            s = (depth*indent) + "const std::string %s = \"%s\";" % ( all_params[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )


    def getInitializeBioModel( self, indent, depth ):
        varname = "gBioModelRW->getSimulator( )"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth, "." ) )

        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = self.mTimeStep.getInitializeBioModel( "%s.getTimeStep()" % ( varname, ), indent, depth )
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
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
