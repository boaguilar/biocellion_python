from biocell import *

class IsPermeableToParam( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "unit", "str", "um2.hour-1", True ) )
        self.addAttribute( Param( "detail", "str", "", True ) )

        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = [ "name", "detail", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mValue = 0.0
        return

    def getValue( self ):
        return self.mValue

    def setValue( self, v ):
        self.mValue = v
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "is_permeable_to"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "IsPermeableTo %s;" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        lines.append( (depth*indent) + "%s.setSoluteIdx( %s );" % ( varname, self.mReference.getEnumToken( ) ) )
        lines.append( (depth*indent) + "%s.setValue( %s );" % ( varname, str( self.mValue ) ) )
        
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


    def __str__( self ):
        s  = "<param %s>%s</param>" % ( self.formatAttributes( ), str( self.mValue ) )
        return s

class ComputationDomainBoundaryCondition( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.setPrefix( "BOUNDARY_CONDITION" )
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "canAttachTo", "bool", False, False ) )
        self.addParam( Param( "bulk", "str", "", False ) )
        self.addParam( Param( "agar", "str", "", False ) )
        self.addParam( Param( "isPermeableTo", "um2.hour-1", 0.0, False ) ) # only a place-holder, to fool validator. not really  used.

        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "class", "name", ]
        self.mPrivateHiddenParams = [ "isPermeableTo", "bulk", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mIsPermeableTos = ItemHolder( IsPermeableToParam )
        self.mShapes = ItemHolder( ComputationDomainBoundaryConditionShape )
        return

    def getEnumToken(self):
        return "%s_%s" % ( self.getPrefix(), self.getAttribute( 'name' ).getValue( ), )
        
    def getIsPermeableTos( self ):
        return self.mIsPermeableTos

    def getShapes( self ):
        return self.mShapes

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "boundary_condition"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "BoundaryCondition *%s = new BoundaryCondition( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )

        if self.mReference:
            lines.append( (depth*indent) + "%s->setBulkIdx( %s );" % ( varname, self.mReference.getEnumToken( ) ) )

        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )

        my_container_name = "%s->getIsPermeableTos()" % ( varname, )
        s = self.mIsPermeableTos.getInitializeBioModel( my_container_name, indent, depth )
        if s:
            lines.append( s )
        my_container_name = "%s->getShapes()" % ( varname, )
        s = self.mShapes.getInitializeBioModel( my_container_name, indent, depth )
        if s:
            lines.append( s )
            
        if container_name:
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )


    def __str__( self ):
        s  = "<boundaryCondition" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += str( self.mIsPermeableTos ) + "\n"
        s += str( self.mShapes ) + "\n"
        s += "</boundaryCondition>\n"
        return s

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
