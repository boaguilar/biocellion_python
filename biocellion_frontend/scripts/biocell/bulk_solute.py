from biocell import *

class BulkSolute( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        
        ParamHolder.__init__(self)
        self.setPrefix( "BULK_SOLUTE" )

        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "Sbulk", "g.L-1", 0.0, False ) )
        self.addParam( Param( "Sin", "g.L-1", 0.0, False ) )
        self.addParam( Param( "isConstant", "bool", True, False ) )
        self.addParam( Param( "Spulse", "g.L-1", 0.0, False ) )
        self.addParam( Param( "pulseRate", "h-1", 0.0, False ) )

        self.addAttribute( Param( "solute", "str", "", False ) )    # Solute reference
        self.mPrivateNumberHiddenParams = [ "solute", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getName(self):
        return self.mName


    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "bulk_solute"

        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "BulkSolute *%s = new BulkSolute( );" % (varname, ) )
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
        s  = "<bulk-solute" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</bulk-solute>\n"
        return s

class AllBulkSolutes( ItemHolder ):

    def __init__( self ):
        ItemHolder.__init__( self, BulkSolute )
        return

    def addItem( self, name, item=None ):
        if item is None:
            item = self.mItemClass( name )
        return ItemHolder.addItem( self, name, item )

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
