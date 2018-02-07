from biocell import *

#####################################################
# AgentSpeciesReaction
#####################################################
class AgentSpeciesReaction( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "status", "str", "inactive", True ) )
        self.mIsParticle = False
        self.mIsMolecule = False
        return

    def isParticle( self ):
        return self.mIsParticle

    def isMolecule( self ):
        return self.mIsMolecule
        
    def setParticle( self ):
        self.mIsParticle = True
        return

    def setMolecule( self ):
        self.mIsMolecule = True
        return
        
    def isActive( self ):
        return self.getAttribute( 'status' ).getValue( ) == 'active'

    def getReactionEnumToken( self ):
        if self.mReference is None:
            raise Exception( "ERROR: <species><reaction> must be linked to <model><reaction>" )
        return self.mReference.getEnumToken( )

    def __str__(self):
        s  = "<species-reaction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        if self.isParticle():
            s += "isParticle\n"
        if self.isMolecule():
            s += "isMolecule\n"
        s += "</species-reaction>\n"
        return s

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
