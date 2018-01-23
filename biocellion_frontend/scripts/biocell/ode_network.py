from biocell import *

class ODENetwork( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.setPrefix( "ODE" )
        self.addAttribute( Param( "name", "str", "", True, ) )
        self.addParam( Param( "stiffness", "str", "normal", False, "", [ "Normal", "High", ], ) )
        self.addParam( Param( "initStepSize", "float", 0.1, False ) )
        self.addParam( Param( "minimumStepSize", "float", 1.0e-12, False ) )
        self.addParam( Param( "epsilon", "float", 1.0e-6, False ) )
        self.addParam( Param( "threshold", "float", 1.0e-3, False ) )

        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "name" ]
        self.mPrivateHiddenParams = [ "stiffness" ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        return

    def getEnumToken(self):
        return "%s_%s" % ( self.getPrefix( ), self.getAttribute( 'name' ).getValue( ), )

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "ode_network"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "ODENetwork %s;;" % (varname, ) )
        
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth, "." ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        stiffness = self.getParam( 'stiffness' ).getValue( )
        if stiffness == 'Normal':
            stiffness_name = "ODE_STIFF_NORMAL"
        elif stiffness == 'High':
            stiffness_name = "ODE_STIFF_HIGH"
        else:
            raise Exception( "Unexpected odeNetwork.stiffness: " + str( stiffness ) )
        lines.append( (depth*indent) + "%s.setStiffness( %s );" % ( varname, stiffness_name, ) )
        
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<odeNetwork" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</odeNetwork>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
