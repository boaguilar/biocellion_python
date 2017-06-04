from biocell import *

#####################################################
# DistanceJunction
#####################################################
class DistanceJunction( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "enabled", "bool", False, True ) )
        self.addAttribute( Param( "withSpecies", "str", "", True ) )

        self.mPrivateNumberHiddenParams = [ "withSpecies", ]
        self.mPrivateBoolHiddenParams = [ "enabled", ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "junction"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "DistanceJunction *%s = new DistanceJunction( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def __str__(self):
        s  = "<distanceJunction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</distanceJunction>\n"
        return s

    def __repr__(self):
        return str(self)


    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
