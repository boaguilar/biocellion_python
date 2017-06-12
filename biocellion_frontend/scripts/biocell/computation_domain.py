from biocell import *

class ComputationDomainBoundaryCondition( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "name", "str", "", True ) )
        return
    
class ComputationDomain( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "resolution", "um", 0 ) )
        self.addParam( Param( "specificArea", "m2.m-3", 0 ) )
        self.addParam( Param( "hasBulk", "bool", False ) )
        self.addParam( Param( "boundaryLayer", "um", 0 ) )
        self.addParam( Param( "biofilmDiffusivity", "float", 0 ) )
        self.mGrid = ComputationDomainGrid( )
        self.mBoundaryConditions = ItemHolder( ComputationDomainBoundaryCondition )
        return

    def getEnumToken(self):
        return "DOMAIN_%s" % ( self.getAttribute( 'name' ).getValue( ), )
    
    def getGrid( self ):
        return self.mGrid

    def getBoundaryConditions( self ):
        return self.mBoundaryConditions

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: ComputationDomain' )
        return "\n".join( lines )

    def getInitializeBioModel(self, indent, depth):
        lines = [ ]
        lines.append( '// FIXME: ComputationDomain' )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<computationDomain" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += str( self.mGrid ) + "\n"
        s += str( self.mBoundaryConditions ) + "\n"
        s += "</computationDomain>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllComputationDomains( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, ComputationDomain )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getDomainsEnum( indent, depth ) )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getBioModelH( indent, depth ) )
        return "\n".join( lines )
    
    def getDomainsEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _domain_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_DOMAINS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} domain_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: ComputationDomains' )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )


def main():
    all = AllComputationDomains()
    all.addItem( "a" )
    all.addItem( "b" )
    all.addItem( "c" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
