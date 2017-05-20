from agent_species import Param, ParamHolder

class Particle( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "" ) )
        self.addAttribute( Param( "regulator", "bool", False ) )
        self.addParam( Param( "density", "g.L-1", 0.0 ) )
        return

    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<particle" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</particle>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllParticles:

    def __init__(self):
        self.mParticles = {}
        self.mOrder = []
        return

    def getParticles( self ):
        return self.mParticles

    def getParticle(self, name):
        return self.mParticles[ name ]

    def addParticle( self, name, particle=None ):
        if particle is None:
            particle = Particle( name )
        self.mParticles[ name ] = particle
        self.mOrder.append( name )
        return True

    def getLastParticle( self ):
        return self.mParticles[ self.mOrder[ len( self.mOrder ) - 1 ] ]

    def __str__( self ):
        s = "<ALL_PARTICLES>\n"

        for name in self.mOrder:
            s += str( self.mParticles[ name ] )

        s += "</ALL_PARTICLES>"
        return s

    def __repr__( self ):
        return str( self )


def main():
    all = AllParticles()
    all.addParticle( "biomass" )
    all.addParticle( "inert" )
    all.addParticle( "capsule" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
        

