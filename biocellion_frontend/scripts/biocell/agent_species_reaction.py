from biocell import *

#####################################################
# AgentSpeciesReaction
#####################################################
class AgentSpeciesReaction( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "status", "str", "inactive", True ) )
        self.mReaction = None
        return

    def isActive( self ):
        return self.getAttribute( 'status' ).getValue( ) == 'active'

    def setReaction( self, reaction ):
        self.mReaction = reaction

    def getReactionEnumToken( self ):
        if self.mReaction is None:
            raise Exception( "ERROR: <species><reaction> must be linked to <idynomics><reaction>" )
        return self.mReaction.getEnumToken( )

    def __str__(self):
        s  = "<species-reaction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</species-reaction>\n"
        return s

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
