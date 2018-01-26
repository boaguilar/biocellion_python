from biocell import *

class ComputationDomainGrid( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "nDim", "int", 3, True ) )
        self.addAttribute( Param( "nI", "int", 1, True ) )
        self.addAttribute( Param( "nJ", "int", 1, True ) )
        self.addAttribute( Param( "nK", "int", 1, False ) )

        self.mPrivateNumberHiddenParams = [ "nDim", "nI", "nJ", "nK", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "grid"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "Grid %s;" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )
            
        if container_name:
            lines.append( (depth*indent) + "%s = %s;" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )


    def __str__(self):
        s  = "<grid" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += "</grid>\n"
        return s

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
