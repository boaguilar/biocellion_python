from agent_species import Param, ParamHolder, ItemHolder

class BulkSolute( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        
        ParamHolder.__init__(self)
        self.addAttribute( Param( "name", "str", "", True ) )
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
        

class Solute( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        self.mEnumToken = "SOLUTE_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.setPrefix( "SOLUTE" )
        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "domain", "str", "", True ) )
        self.addParam( Param( "diffusivity", "m2.day-1", 0.0, True ) )
        self.addParam( Param( "airDiffusivity", "m2.day-1", 0.0, False ) )
        self.addParam( Param( "decayRate", "float", 0.0, False ) )
        self.addParam( Param( "resolution", "um", -1.0, False ) )
        self.addParam( Param( "concentration", "g.L-1", -1.0, False ) )
        self.addParam( Param( "randomInit", "bool", False, False ) )
        self.addParam( Param( "rndMinConcentration", "g.L-1", 0.0, False ) )
        self.addParam( Param( "rndMaxConcentration", "g.L-1", 0.0, False ) )
        self.addParam( Param( "writeOutput", "bool", False, False ) )

        return

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitializeBioModel(self, indent, depth):
        varname = "solute"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Solute *%s = new Solute( \"%s\", %s, %s, %s, %s, %s, %s );" % ( varname, self.mName, self.mEnumToken, self.getAttribute( 'domain' ).getValue( ),
                                                                                                        self.countReal(), self.countInt(), self.countBool(), self.countString(), ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
        lines.append( (depth*indent) + "gSolutes.push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<solute" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</solute>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllSolutes( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, Solute )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getSoluteParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSolutesEnum( indent, depth ) )
        return "\n".join( lines )

    def getSoluteParamNames(self, indent, depth):
        all_params = { }
        all_order = [ ]
        for name in self.mOrder:
            params = self.mItems[ name ].getParams()
            for param_name in params:
                if param_name not in all_params:
                    all_params[ param_name ] = params[ param_name ]
                    all_order.append( param_name )
        lines = []
        for n in all_order:
            s = (depth*indent) + "const std::string %s = \"%s\";" % (all_params[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )
    
    def getSolutesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _solute_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_SOLUTES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} solute_type_e;" )
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
    print("UNTESTED")
    return
    
if __name__ == "__main__":
    main()
        

