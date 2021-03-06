from biocell import *

class AgentGrid( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addParam( Param( "computationDomain", "str", "", True ) )
        self.addParam( Param( "resolution", "um", 1.0, True ) )
        self.addParam( Param( "shovingFraction", "float", 0.0025 ) )
        self.addParam( Param( "shovingMaxIter", "int", 1 ) )
        self.addParam( Param( "shovingMutual", "bool", True ) )
        self.addParam( Param( "erosionMethod", "bool", True ) )
        self.addParam( Param( "sloughDetachedBiomass", "bool", False ) )

        self.mPrivateNumberHiddenParams = [ "resolution", "shovingFraction", "shovingMaxIter" ]
        self.mPrivateBoolHiddenParams = [ "shovingMutual", "erosionMethod", "sloughDetachedBiomass" ]
        self.mPrivateStringHiddenParams = [ "computationDomain" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        varname = "gBioModelRW->getAgentGrid( )"
        lines = [ ]
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
        s  = "<agentGrid" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</agentGrid>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
