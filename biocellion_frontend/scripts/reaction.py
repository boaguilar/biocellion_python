from agent_species import Param, ParamHolder, ItemHolder
import sys

class KineticFactor( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "molecule", "str", "", False ) )  # Molecule reference
        self.addAttribute( Param( "solute", "str", "", False ) )    # Solute reference

        self.addParam( Param( "Ki", "g.L-1", 0.0, False ) )
        self.addParam( Param( "Ks", "g.L-1", 0.0, False ) )

        self.mPrivateNumberHiddenParams = [ "solute", "molecule", "Ki", "Ks", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "class", ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "kinetic_factor"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "Reaction::KineticFactor %s;" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )
        if container_name:
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def __str__(self):
        s  = "<kineticFactor" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</kineticFactor>\n"
        return s

    
class Yields( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__( self )
        self.addParam( Param( "*", "g.g-1", 0.0, False ) )
        self.mSolutes = [ ]
        self.mParticles = [ ]
        self.mIdxs = { }
        return

    def setSolute( self, name, idx ):
        self.mSolutes.append( name )
        self.mIdxs[ name ] = idx
        return

    def setParticle( self, name, idx ):
        self.mParticles.append( name )
        self.mIdxs[ name ] = idx
        return
    
    def getKeys( self ):
        return self.getParamKeys( )

    def getItem( self, name ):
        return self.getParam( name )

    def getInitializeBioModel(self, container_name, indent, depth):
        varname = "yield"
        lines = []
        for n in self.mParams:
            if n == '*':
                continue
            
            param = self.mParams[ n ]
            lines.append( (depth*indent) + "{" )
            depth += 1
            lines.append( (depth*indent) + "Reaction::Yield %s;" % (varname, ) )
            if n in self.mSolutes:
                lines.append( (depth*indent) + "%s.setSolute( );" % (varname, ) )
            elif n in self.mParticles:
                lines.append( (depth*indent) + "%s.setParticle( );" % (varname, ) )
            else:
                sys.exit( "ERROR: Yield must be solute or particle" )

            lines.append( (depth*indent) + "%s.setItemIdx( %s );" % ( varname, self.mIdxs[ n ] ) )
            lines.append( (depth*indent) + "%s.setValue( %s );" % ( varname, param.getValue( ) ) )
            
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
            depth -= 1
            lines.append( (depth*indent) + "}" )
            

        return "\n".join( lines )

class Reaction( ParamHolder ):

    def __init__(self, name):
        self.mName = name
        self.mEnumToken = "REACTION_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.setPrefix( "REACTION" )
        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "catalyzedBy", "str", "", True ) )  # Particle reference
        self.addAttribute( Param( "catalyst", "str", "", False ) )    # Species reference
        self.addParam( Param( "muMax", "hr-1", 0.0, True ) )

        self.mPrivateNumberHiddenParams = [ "catalyzedBy", "catalyst", "muMax", ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "name", "class", ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        self.mYields = Yields( )        
        self.mKineticFactors = ItemHolder( KineticFactor )
        print( "FIXME: <reaction><kineticFactor/></reaction> not yet scanned" )

        return
    
    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken
    
    def getYields( self ):
        return self.mYields

    def getKineticFactors( self ):
        return self.mKineticFactors

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
        
        lines.append( (depth*indent) + "gReactions.push_back( %s );" % ( varname, ) )
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
    print("UNTESTED")
    return
    
if __name__ == "__main__":
    main()
        

