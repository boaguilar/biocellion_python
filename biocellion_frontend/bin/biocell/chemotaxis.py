from biocell import *

#####################################################
# Chemotaxis
#####################################################
class Chemotaxis( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "withSolute", "str", "", True ) )

        self.addParam( Param( "strength", "um.hour-1.fg-1.um3", 0, True ) )
        self.addParam( Param( "contactInhibition", "int", 0, False ) )
        self.addParam( Param( "alpha", "fg.um-3", 0.0, False ) )

        self.mPrivateNumberHiddenParams = [ "strength", "contactInhibition", "alpha" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = [ "withSolute", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "chemotaxis"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "Chemotaxis *%s = new Chemotaxis( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        if self.mReference:
            token = self.mReference.getEnumToken( )
        else:
            token = "-1"
        lines.append( (depth*indent) + "%s->setSolute( %s );" % ( varname, token ) )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<chemotaxis" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</chemotaxis>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
