from biocell import *

class Molecule( ParamHolder ):

    def __init__( self, name, model ):
        self.mName = name
        self.mEnumToken = "MOLECULE_%s" % ( name, )
        self.mModel = model
        
        ParamHolder.__init__(self)
        self.setPrefix( "MOLECULE" )
        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "domain", "str", "", True ) )
        self.addParam( Param( "writeOutput", "bool", False, False ) )

        self.mPrivateNumberHiddenParams = [ ]
        self.mPrivateBoolHiddenParams = [ ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = [ "domain" ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mDomainReference = None
        return

    def getDomainReference( self ):
        return self.mDomainReference

    def setDomainReference( self, domain ):
        self.mDomainReference = domain
        return

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitializeBioModel(self, indent, depth):
        varname = "molecule"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Molecule *%s = new Molecule( \"%s\", %s, %s );" % ( varname, self.mName, self.mEnumToken, self.mDomainReference.getEnumToken( ), ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
            
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        lines.append( (depth*indent) + "gBioModelRW->getMolecules( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<molecule" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</molecule>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllMolecules( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, Molecule )
        self.mModel = model
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getMoleculesEnum( indent, depth ) )
        return "\n".join( lines )

    def getMoleculesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _molecule_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_MOLECULES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} molecule_type_e;" )
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
