from biocell import *

#####################################################
# TightJunction
#####################################################
class TightJunction( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "withSpecies", "str", "", True ) )
        self.addParam( Param( "stiffness", "float", 1.0, False ) )
        self.addParam( Param( "scale", "um.hour-1", 1.0, False ) )

        self.mPrivateNumberHiddenParams = [ "stiffness", "scale" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = [ "withSpecies", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "tjunction"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "TightJunction *%s = new TightJunction( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = "%s->setWithSpecies( %s );" % ( varname, self.mReference.getEnumToken( ),  );
        lines.append( (depth*indent) + s )

        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<tightJunction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</tightJunction>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
