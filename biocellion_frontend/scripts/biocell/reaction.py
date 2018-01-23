from biocell import *

class Reaction( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        self.mEnumToken = "REACTION_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.setPrefix( "REACTION" )
        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "class", "str", "", True, "", [ "ReactionFactor", ] ) )
        self.addAttribute( Param( "catalyzedBy", "str", "", True ) )  # Particle reference
        self.addAttribute( Param( "catalyst", "str", "", False ) )    # Species reference
        self.addParam( Param( "muMax", "hour-1", 0.0, True ) )

        self.mPrivateNumberHiddenParams = [ "catalyzedBy", "catalyst", "muMax", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "name", "class", ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        self.mYields = Yields( )        
        self.mKineticFactors = ItemHolder( KineticFactor )
        self.mAgentSpecies = ItemHolder( AgentSpecies ) # all AgentSpecies that have this reaction active
        return
    
    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken
    
    def getYields( self ):
        return self.mYields

    def getKineticFactors( self ):
        return self.mKineticFactors

    def getAgentSpecies( self ):
        return self.mAgentSpecies

    def getInitializeBioModel(self, indent, depth):
        varname = "reaction"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Reaction *%s = new Reaction(  );" % ( varname, ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        container_name = "%s->getYields()" % ( varname, )
        s = self.mYields.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )

        container_name = "%s->getKineticFactors()" % ( varname, )
        s = self.mKineticFactors.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )

        # agent species active with this reaction
        lines.append( (depth*indent) + "%s->getActiveAgentSpecies( ).resize( NUM_AGENT_SPECIES );" % ( varname, ) )
        lines.append( (depth*indent) + "%s->getActiveAgentSpecies( ).assign( NUM_AGENT_SPECIES, false );" % ( varname, ) )
        for species_name in self.mAgentSpecies.getKeys( ):
            species = self.mAgentSpecies.getItem( species_name )
            lines.append( (depth*indent) + "%s->getActiveAgentSpecies( )[ %s ] = true;" % ( varname, species.getEnumToken( ) ) )
        
        lines.append( (depth*indent) + "gBioModelRW->getReactions( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<reaction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mYields )
        s += str( self.mKineticFactors )
        s += "</reaction>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllReactions( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, Reaction )
        self.mModel = model
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getReactionsEnum( indent, depth ) )
        return "\n".join( lines )

    def getReactionsEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _reaction_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_REACTIONS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} reaction_type_e;" )
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
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
