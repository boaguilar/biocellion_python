from biocell import *

#####################################################
# Blocks
#####################################################
class Blocks( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "rows", "int", 0, True ) )
        self.addAttribute( Param( "cols", "int", 0, True ) )
        self.addAttribute( Param( "bars", "int", 0, False ) )
        return

    def __str__(self):
        s  = "<blocks" + self.formatAttributes() + "/>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# Coordinates
#####################################################
class Coordinates( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "x", "um", 0, True ) )
        self.addAttribute( Param( "y", "um", 0, True ) )
        self.addAttribute( Param( "z", "um", 0, True ) )
        self.addAttribute( Param( "r", "um", 0, False ) )
        self.mPrivateNumberHiddenParams = [ "x", "y", "z", "r" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "coordinates"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "Coordinates *%s = new Coordinates( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<coordinates" + self.formatAttributes() + "/>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# InitArea
#####################################################
class InitArea( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "number", "float", 0, True ) )
        self.addAttribute( Param( "shape", "str", "default", False ) )
        self.addParam( Param( "birthday", "hour", 0.0, True ) )
        self.mCoordinates = ItemHolder( Coordinates )
        self.mBlocks = ItemHolder( Blocks )


        self.mPrivateNumberHiddenParams = [ "number", "birthday" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "shape" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getCoordinates( self ):
        return self.mCoordinates

    def getBlocks( self ):
        return self.mBlocks

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "init_area"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "InitArea *%s = new InitArea( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = (depth*indent) + "%s%sset%s( %s );" % (varname, "->", "AgentSpecies", parent_varname, )
        lines.append( s )
            
        coordinate_container_name = "%s->getCoordinates()" % ( varname, )
        for i in range( len( self.mCoordinates ) ):
            lines.append( self.mCoordinates[ i ].getInitializeBioModel( coordinate_container_name, indent, depth ) )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<initArea" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mCoordinates )
        s += str( self.mBlocks )
        s += "</initArea>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
