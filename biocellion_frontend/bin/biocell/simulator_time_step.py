from biocell import *

class SimulatorTimeStep( ParamHolder ):

    def __init__(self):
        self.mName = "timeStep"
        ParamHolder.__init__(self)
        self.addParam( Param( "adaptive", "bool", False, False ) )
        self.addParam( Param( "timeStepIni", "hr", 0.01, False ) )
        self.addParam( Param( "timeStepMin", "hr", 0.01, False ) )
        self.addParam( Param( "timeStepMax", "hr", 0.01, False ) )
        self.addParam( Param( "endOfSimulation", "hr", 0.01, True ) )

        self.mPrivateNumberHiddenParams = [ "timeStepIni", "timeStepMin", "timeStepMax", "endOfSimulation" ]
        self.mPrivateBoolHiddenParams = [ "adaptive" ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        return

    def getInitializeBioModel( self, varname, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )

        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
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
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
