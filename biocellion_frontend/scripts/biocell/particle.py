from biocell import *


class Particle( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        self.mEnumToken = "PARTICLE_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.setPrefix( "PARTICLE" )
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "regulator", "bool", False, False ) )
        self.addParam( Param( "density", "g.L-1", 0.0, True ) )
        self.addParam( Param( "writeOutput", "bool", False, False ) )
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
        lines.append( (depth*indent) + "gBioModelRW->getParticles( ).push_back( particle );" )
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
        
class AllParticles( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, Particle )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getParticlesEnum( indent, depth ) )
        return "\n".join( lines )

    def getParticlesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _particle_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_PARTICLES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} particle_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = []
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def addItem( self, name, item=None ):
        if item is None:
            item = self.mItemClass( name )
        return ItemHolder.addItem( self, name, item )

def main():
    all = AllParticles()
    all.addParticle( "biomass" )
    all.addParticle( "inert" )
    all.addParticle( "capsule" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
