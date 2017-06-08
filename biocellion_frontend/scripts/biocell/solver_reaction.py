from biocell import *

#####################################################
# SolverReaction
#####################################################
class SolverReaction( ParamHolder ):

    def __init__( self, name ):
        self.mName = name
        ParamHolder.__init__( self )        
        self.addAttribute( Param( "name", "str", "", True ) )
        return

    def getName( self ):
        return self.mName

    def getInitializeBioModel( self, container_name, indent, depth ):
        if self.mReference is None:
            raise Exception( "ERROR : SolverReaction needs to be linked to Reaction" )

        lines = [ ]
        if container_name:
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, self.mReference.getEnumToken( ), ) )
        return "\n".join( lines )


    def __str__(self):
        s  = "<solver-reaction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</solver-reaction>\n"
        return s

    def __repr__(self):
        return str(self)

class AllSolverReactions( ItemHolder ):

    def __init__( self ):
        ItemHolder.__init__( self, SolverReaction )
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
