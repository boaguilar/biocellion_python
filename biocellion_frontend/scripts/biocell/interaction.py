from biocell import *

class Interaction( ParamHolder ):

    def __init__( self, name, model ):
        self.mName = name
        self.mEnumToken = "INTERACTION_%s" % ( name, )
        self.mModel = model
        
        ParamHolder.__init__(self)
        self.setPrefix( "INTERACTION" )
        
        self.addAttribute( Param( "name", "str", "", True, "", [ "shove", "tightJunction", "adhesion", "chemotaxis", "diffusion" ] ) )
        self.addParam( Param( "writeOutput", "bool", False, False ) )

        self.mPrivateNumberHiddenParams = [ ]
        self.mPrivateBoolHiddenParams = [ ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        return

    def isMechanical( self ):
        return self.getName( ) in [ "shove", "tightJunction", "adhesion" ]

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitializeBioModel(self, indent, depth):
        varname = "interaction"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Interaction *%s = new Interaction( \"%s\", %s );" % ( varname, self.mName, self.mEnumToken, ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
            
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        lines.append( (depth*indent) + "gBioModelRW->getInteractions( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<interaction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</interaction>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllInteractions( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, Interaction )
        self.mModel = model
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getInteractionsEnum( indent, depth ) )
        return "\n".join( lines )

    def getInteractionsEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _interaction_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_INTERACTIONS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} interaction_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = []
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def addItem( self, name, item=None ):
        if item is None:
            item = self.mItemClass( name, self.mModel )
        return ItemHolder.addItem( self, name, item )
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
