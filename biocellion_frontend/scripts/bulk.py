from agent_species import Param, ParamHolder, ItemHolder

class Bulk( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "" ) )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Bulk' )
        return "\n".join( lines )

    def getInitializeBioModel(self, indent, depth):
        lines = [ ]
        lines.append( '// FIXME: Bulk' )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<bulk" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</bulk>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllBulks( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, Bulk )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Bulks' )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        lines.append( '// FIXME: Bulks' )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )


def main():
    all = AllBulks()
    all.addItem( "a" )
    all.addItem( "b" )
    all.addItem( "c" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
        

