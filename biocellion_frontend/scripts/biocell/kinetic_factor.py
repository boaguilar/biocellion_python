from biocell import *

class KineticFactor( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        
        self.addAttribute( Param( "class", "str", "", True, "",
                                  [ "FirstOrderKinetic", "SimpleInhibition", "MonodKinetic",
                                    "LinearKinetic", "KineticAgentSurfaceArea", "KineticPermeability" ] ) )
        self.addAttribute( Param( "molecule", "str", "", False ) )  # Molecule reference
        self.addAttribute( Param( "solute", "str", "", False ) )    # Solute reference
        self.addAttribute( Param( "species", "str", "", False ) )   # AgentSpecies reference

        self.addParam( Param( "Ki", "g.L-1", 0.0, False ) )
        self.addParam( Param( "Ks", "g.L-1", 0.0, False ) )
        self.addParam( Param( "permeability", "um2.hour-1", 0.0, False ) )

        self.mPrivateNumberHiddenParams = [ ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "class", ]
        self.mPrivateHiddenParams = [ "solute", "molecule", "species", "Ki", "Ks", "permeability", ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mSoluteReference = None
        self.mMoleculeReference = None
        self.mAgentSpeciesReference = None
        return

    def getSoluteReference( self ):
        return self.mSoluteReference

    def setSoluteReference( self, solute ):
        self.mSoluteReference = solute
        return

    def getMoleculeReference( self ):
        return self.mMoleculeReference

    def setMoleculeReference( self, molecule ):
        self.mMoleculeReference = molecule
        return

    def getAgentSpeciesReference( self ):
        return self.mAgentSpeciesReference

    def setAgentSpeciesReference( self, species ):
        self.mAgentSpeciesReference = species
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "kinetic_factor"
        class_name = self.getAttribute( 'class' ).getValue( )
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1
        
        lines.append( (depth*indent) + "Reaction::%s *%s = new Reaction::%s( );" % ( class_name, varname, class_name, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        if class_name in [ "SimpleInhibition", ]:
            lines.append( (depth*indent) + "%s->setKi( %s );" % ( varname, self.getParam( 'Ki' ).getValue( ) ) )
        elif class_name in [ "MonodKinetic", "LinearKinetic", ]:
            lines.append( (depth*indent) + "%s->setKs( %s );" % ( varname, self.getParam( 'Ks' ).getValue( ) ) )
        elif class_name in [ "FirstOrderKinetic", "KineticAgentSurfaceArea", ]:
            pass
        elif class_name in [ "KineticPermeability", ]:
            lines.append( (depth*indent) + "%s->setPermeability( %s );" % ( varname, self.getParam( 'permeability' ).getValue( ) ) )
        else:
            raise Exception( "Unexpected KineticFactor class: " + str( class_name ) )
        
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        if s:
            lines.append( s )
            
        if self.mSoluteReference:
            token = self.mSoluteReference.getEnumToken( )
        else:
            token = "-1"
        lines.append( (depth*indent) + "%s->setSoluteIdx( %s );" % ( varname, token ) )
        
        if self.mMoleculeReference:
            token = self.mMoleculeReference.getEnumToken( )
        else:
            token = "-1"
        lines.append( (depth*indent) + "%s->setMoleculeIdx( %s );" % ( varname, token ) )

        if self.mAgentSpeciesReference:
            token = self.mAgentSpeciesReference.getEnumToken( )
        else:
            token = "-1"
        lines.append( (depth*indent) + "%s->setSpeciesIdx( %s );" % ( varname, token ) )

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

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
