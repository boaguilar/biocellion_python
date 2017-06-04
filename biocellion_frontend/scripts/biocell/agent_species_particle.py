from biocell import *

#####################################################
# AgentSpeciesParticle
#####################################################
class AgentSpeciesParticle(ParamHolder):

    def __init__( self ):
        ParamHolder.__init__( self )        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "mass", "pg", 0.0, True ) )
        self.mParticle = None
        return

    def setParticle( self, particle ):
        self.mParticle = particle

    def getParticleEnumToken( self ):
        if self.mParticle is None:
            raise Exception( "ERROR: <species><particle> must be linked to <idynomics><particle>" )
        return self.mParticle.getEnumToken( )

    def __str__(self):
        s  = "<species-particle" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</species-particle>\n"
        return s

    def __repr__(self):
        return str(self)
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
