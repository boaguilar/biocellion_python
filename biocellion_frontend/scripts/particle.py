from agent_species import Param, ParamHolder

class Particle( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        self.mEnumToken = "PARTICLE_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "" ) )
        self.addAttribute( Param( "regulator", "bool", False ) )
        self.addParam( Param( "density", "g.L-1", 0.0 ) )
        self.mHiddenParams = self.mHiddenParams + [ "density" ]
        return

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken


    def getInitializeBioModel(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Particle *particle = new Particle( %s, \"%s\", %s );" % (self.mEnumToken, self.mName, self.getParams()[ "density" ].getValue(), ) )
        s = ParamHolder.getInitializeBioModel( self, "particle", indent, depth )
        if s:
            lines.append( s )
        lines.append( (depth*indent) + "gParticles.push_back( particle );" )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
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

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getParticlesEnum( indent, depth ) )
        return "\n".join( lines )

    def getParticlesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _particle_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mParticles[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_PARTICLES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} particle_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = []
        for name in self.mOrder:
            lines.append( self.mParticles[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

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
        

