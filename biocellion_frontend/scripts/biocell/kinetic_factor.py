from biocell import *

class KineticFactor( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "molecule", "str", "", False ) )  # Molecule reference
        self.addAttribute( Param( "solute", "str", "", False ) )    # Solute reference

        self.addParam( Param( "Ki", "g.L-1", 0.0, False ) )
        self.addParam( Param( "Ks", "g.L-1", 0.0, False ) )

        self.mPrivateNumberHiddenParams = [ "solute", "molecule", "Ki", "Ks", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "class", ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "kinetic_factor"
        class_name = self.getAttribute( 'class' ).getValue( )
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "Reaction::KineticFactor *%s = new Reaction::%s( );" % (varname, class_name, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )
        if container_name:
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def __str__(self):
        s  = "<kineticFactor" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</kineticFactor>\n"
        return s

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
