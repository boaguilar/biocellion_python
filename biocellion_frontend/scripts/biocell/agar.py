from biocell import *

    
class Agar( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "depth", "int", 0 ) )
        self.addParam( Param( "width", "int", 0 ) )
        self.addParam( Param( "height", "int", 0 ) )
        self.addParam( Param( "concentration", "g.L-1", 0 ) )
        self.addParam( Param( "D", "float", 0 ) )
        self.addParam( Param( "advRate", "float", 0 ) )
        self.addParam( Param( "T", "int", 0 ) )
        self.addParam( Param( "updateFreq", "int", 0 ) )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Agar' )
        return "\n".join( lines )

    def getInitializeBioModel(self, indent, depth):
        lines = [ ]
        lines.append( '// FIXME: Agar' )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<agar" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</agar>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllAgars( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, Agar )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Agars' )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Agars' )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )


def main():
    all = AllAgars()
    all.addItem( "a" )
    all.addItem( "b" )
    all.addItem( "c" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
        
