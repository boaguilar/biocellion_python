from biocell import *

class XYZParam( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "x", "int", 0, True ) )
        self.addAttribute( Param( "y", "int", 0, True ) )
        self.addAttribute( Param( "z", "int", 0, True ) )
        
        self.mPrivateNumberHiddenParams = [ "x", "y", "z",  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "name", ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        return

    def __str__( self ):
        s  = "<param %s/>" % ( self.formatAttributes( ), )
        return s

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "xyz_param"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "XYZParam %s;" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
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


class ComputationDomainBoundaryConditionShape( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addParam( Param( "pointIn", "str", "", True ) ) # only a place-holder, to fool validator. not really  used.
        self.addParam( Param( "vectorOut", "str", "", True ) ) # only a place-holder, to fool validator. not really  used.

        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = [ "pointIn", "vectorOut", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mXYZParams = ItemHolder( XYZParam )
        return

    def getXYZParams( self ):
        return self.mXYZParams

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "shape"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "Shape *%s = new Shape( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )

        param_container_name = "%s->getXYZParams()" % ( varname, )
        s = self.mXYZParams.getInitializeBioModel( param_container_name, indent, depth )
        if s:
            lines.append( s )
            
        if container_name:
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )


    def __str__(self):
        s  = "<bc-shape" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += str( self.mXYZParams ) + "\n"
        s += "</bc-shape>\n"
        return s
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
