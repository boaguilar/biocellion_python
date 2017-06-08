from biocell import *

#####################################################
# AgentSpecies
#####################################################
class AgentSpecies(ParamHolder):

    def __init__(self, class_name, name, model):
        self.mClassName = class_name
        self.mName = name
        self.mModel = model
        self.mEnumToken = "AGENT_SPECIES_%s_%s" % (class_name, name)
        self.mUseMechForceReals = False
        self.mMechForceReals = [ ]
        
        ParamHolder.__init__(self)
        self.setPrefix( "SPECIES" )
        
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "name", "str", "", True ) )
        
        self.addParam( Param( "initialMassCV", "float", 0.1 ) )
        self.addParam( Param( "color", "str", "white" ) )
        self.addParam( Param( "tightJunctionToBoundaryStrength", "float", 0.0 ) )
        self.addParam( Param( "attachToBoundaryCreateFactor", "float", 1.1 ) )
        self.addParam( Param( "attachToBoundaryDestroyFactor", "float" , 2.0 ) )
        self.addParam( Param( "attachCreateFactor", "float", 1.1 ) )
        self.addParam( Param( "attachDestroyFactor", "float", 2.0 ) )
        self.addParam( Param( "DoNotRandomizeMassOnCreation", "bool", False ) )
        self.addParam( Param( "visible", "bool", True ) )
        self.addParam( Param( "computationDomain", "str", "" ) )

        self.mInitAreas = ItemHolder( InitArea )
        self.mAdhesions = ItemHolder( Adhesion )
        self.mDistanceJunctions = ItemHolder( DistanceJunction )
        self.mTightJunctions = ItemHolder( TightJunction )
        self.mChemotaxis = ItemHolder( Chemotaxis )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getEnums( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        self.updateUseMechForceReals(  )
        varname = "species"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "AgentSpecies *%s = new AgentSpecies( \"%s\", \"%s\", %s, %d, %d, %d, %d );" % (varname, self.mName, self.mClassName, self.mEnumToken, self.countReal(), self.countInt(), self.countBool(), self.countString()) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )

        s = (depth*indent) + "%s->setDMax( %s );" % (varname, self.mModel.getAgentGrid( ).getParam( 'resolution' ).getValue(), )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelBools( %s_NUM_BOOLS );" % (varname, self.mEnumToken, )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelReals( %s_NUM_REALS );" % (varname, self.mEnumToken, )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelInts( %s_NUM_INTS );" % (varname, self.mEnumToken, )
        lines.append( s )
        if self.mUseMechForceReals:
            s = (depth*indent) + "%s->setUseMechForceReals( true );" % (varname, )
            lines.append( s )
            s = (depth*indent) + "%s->setIdxMechForceReals( %s_MECH_REAL_FORCE_X, %s_MECH_REAL_FORCE_Y, %s_MECH_REAL_FORCE_Z );" % (varname, self.mEnumToken, self.mEnumToken, self.mEnumToken, )
            lines.append( s )
        else:
            s = (depth*indent) + "%s->setUseMechForceReals( false );" % (varname, )
            lines.append( s )

        container_name = "%s->getInitAreas()" % ( varname )
        for i in range( len( self.mInitAreas ) ):
            lines.append( self.mInitAreas[ i ].getInitializeBioModel( varname, container_name, indent, depth ) )
        container_name = "%s->getAdhesions()" % ( varname )
        for i in range( len( self.mAdhesions ) ):
            lines.append( self.mAdhesions[ i ].getInitializeBioModel( varname, container_name, indent, depth ) )
        container_name = "%s->getDistanceJunctions()" % ( varname )
        for i in range( len( self.mDistanceJunctions ) ):
            lines.append( self.mDistanceJunctions[ i ].getInitializeBioModel( varname, container_name, indent, depth ) )
        container_name = "%s->getTightJunctions()" % ( varname )
        for i in range( len( self.mTightJunctions ) ):
            lines.append( self.mTightJunctions[ i ].getInitializeBioModel( varname, container_name, indent, depth ) )
        container_name = "%s->getChemotaxis()" % ( varname )
        for i in range( len( self.mChemotaxis ) ):
            lines.append( self.mChemotaxis[ i ].getInitializeBioModel( varname, container_name, indent, depth ) )
        s = self.getSpecificInitializeBioModel( varname, indent, depth )
        if s:
            lines.append( s )
        lines.append( (depth*indent) + "gAgentSpecies.push_back( %s );" % (varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getSpecificRealsEnum( self, indent, depth ):
        line = [ ]
        return "\n".join( lines )
        
    def getSpecificInitializeBioModel( self, varname, indent, depth ):
        line = [ ]
        return "\n".join( lines )

    def getClassName(self):
        return self.mClassName

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitAreas( self ):
        return self.mInitAreas

    def getAdhesions( self ):
        return self.mAdhesions

    def getDistanceJunctions( self ):
        return self.mDistanceJunctions

    def getTightJunctions( self ):
        return self.mTightJunctions

    def getChemotaxis( self ):
        return self.mChemotaxis

    def createDistanceJunctions( self ):
        # Create all necessary distance junctions
        # If adhesion is in play, contact inhibition may be needed, so create distance junctions
        for key in self.mAdhesions.getKeys( ):
            item = self.getAdhesions( ).getItem( key )
            species_name = item.getAttribute( "withSpecies" ).getValue( )

            junction = DistanceJunction( )
            node_attr = Param( "enabled", None, "true" )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            node_attr = Param( "withSpecies", None, species_name )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            self.mDistanceJunctions.addItem( species_name, junction )
            
        # If tight junctions are in play, distance junctions are needed
        for key in self.mTightJunctions.getKeys( ):
            item = self.getTightJunctions( ).getItem( key )
            species_name = item.getAttribute( "withSpecies" ).getValue( )

            junction = DistanceJunction( )
            node_attr = Param( "enabled", None, "true" )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            node_attr = Param( "withSpecies", None, species_name )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            self.mDistanceJunctions.addItem( species_name, junction )

        # If chemotaxis is in play, contact inhibition may be needed, so create distance junctions
        for key in self.mChemotaxis.getKeys( ):
            item = self.getChemotaxis( ).getItem( key )
            species_name = self.getEnumToken( )

            junction = DistanceJunction( )
            node_attr = Param( "enabled", None, "true" )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            node_attr = Param( "withSpecies", None, species_name )
            if not junction.validateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                raise Exception( "ERROR : Failed to create DistanceJunction" )

            self.mDistanceJunctions.addItem( species_name, junction )
            
        return

    def getEnums( self, indent, depth ):
        self.updateUseMechForceReals(  )
        lines = [ ]
        lines.append( self.getBoolsEnum( indent, depth ) )
        lines.append( self.getRealsEnum( indent, depth ) )
        lines.append( self.getIntsEnum( indent, depth ) )
        lines.append( self.getMechForceRealsEnum( indent, depth ) )
        return "\n".join( lines )
    
    def getBoolsEnum( self, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "typedef enum _%s_bool_type_e {" % ( self.getEnumToken(), ) )
        depth += 1
        s = (depth*indent) + "%s_NUM_BOOLS" % ( self.getEnumToken(), )
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} %s_bool_type_e;" % ( self.getEnumToken(), ) )
        return "\n".join( lines )
    
    def getRealsEnum( self, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "typedef enum _%s_real_type_e {" % ( self.getEnumToken(), ) )
        depth += 1
        s = self.getSpecificRealsEnum( indent, depth )
        if s:
            lines.append( s )
        s = (depth*indent) + "%s_NUM_REALS" % ( self.getEnumToken(), )
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} %s_real_type_e;" % ( self.getEnumToken(), ) )
        return "\n".join( lines )
    
    def getIntsEnum( self, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "typedef enum _%s_int_type_e {" % ( self.getEnumToken(), ) )
        depth += 1
        s = (depth*indent) + "%s_NUM_INTS" % ( self.getEnumToken(), )
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} %s_int_type_e;" % ( self.getEnumToken(), ) )
        return "\n".join( lines )
    
    def getMechForceRealsEnum( self, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "typedef enum _%s_mech_force_real_type_e {" % ( self.getEnumToken(), ) )
        depth += 1
        for name in self.mMechForceReals:
            s = (depth*indent) + "%s," % ( name, )
            lines.append( s )
        s = (depth*indent) + "%s_NUM_MECH_FORCE_REALS" % ( self.getEnumToken(), )
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} %s_mech_force_real_type_e;" % ( self.getEnumToken(), ) )
        return "\n".join( lines )
    
    def updateUseMechForceReals( self ):
        # Mech Forces used by shove, adhesion, tightJunction, chemotaxis
        trigger_names = [ "shoveLimit", "shoveFactor", "shoveScale" ]
        update = False
        for name in trigger_names:
            if self.paramExists( name ) and self.getParam( name ).getValue() > 0.0:
                update = True
                break
        if len( self.mAdhesions ) > 0:
            update = True
        if len( self.mTightJunctions ) > 0:
            update = True
        if len( self.mChemotaxis ) > 0:
            update = True
        if update:
            self.setUseMechForceReals( True )
        else:          
            self.setUseMechForceReals( False )
        return

    def setUseMechForceReals( self, value ):
        self.mUseMechForceReals = value
        if value:
            for s0 in [ "MECH_REAL_FORCE_X", "MECH_REAL_FORCE_Y", "MECH_REAL_FORCE_Z",  ]:
                s1 = "%s_%s" % ( self.getEnumToken(), s0 )
                if s1 not in self.mMechForceReals:
                    self.mMechForceReals.append( s1 )
        else:
            removes = [ ]
            for s0 in [ "MECH_REAL_FORCE_X", "MECH_REAL_FORCE_Y", "MECH_REAL_FORCE_Z",  ]:
                s1 = "%s_%s" % ( self.getEnumToken(), s0 )
                removes.append( s1 )

            reals = [ ]
            for s1 in self.mMechForcesReals:
                if s1 not in removes:
                    reals.append( s1 )
            self.mMechForceReals = reals
            
        return

    def toString( self, additional ):
        s  = "<species" + self.formatAttributes() + " enumToken=\"" + self.mEnumToken + "\">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mInitAreas )
        s += str( self.mAdhesions )
        s += str( self.mDistanceJunctions )
        s += str( self.mTightJunctions )
        s += str( self.mChemotaxis )
        s += additional
        s += "</species>"
        return s
        
    def __str__(self):
        return self.toString( "" )
        return s

    def __repr__(self):
        return str(self)


class AgentSpeciesActive(AgentSpecies):
    
    def __init__(self, class_name="", name="", model=None):
        if not class_name: class_name = "Active"
        AgentSpecies.__init__(self, class_name, name, model)
        
        self.mParticles = ItemHolder( AgentSpeciesParticle )
        self.mReactions = ItemHolder( AgentSpeciesReaction )
        print("FIXME: <species><molecules></molecules></species> not yet parsed.")
        print("FIXME: <species><agentMolecularReactions></agentMolecularReactions></species> not yet parsed.")
        return

    def getParticleEnumToken( self, particle ):
        s = "%s_%s" % ( self.getEnumToken(), particle.getAttribute( 'name' ).getValue() )
        return s

    def getSpecificRealsEnum( self, indent, depth ):
        lines = [ ]
        if len( self.mParticles ) > 0:
            for i in range( len( self.mParticles ) ):
                p = self.mParticles[ i ]
                s = "%s," % ( self.getParticleEnumToken( p ), );
                lines.append( (depth*indent) + s )
        return "\n".join( lines )

    def getSpecificInitializeBioModel( self, varname, indent, depth ):
        lines = [ ]
        if len( self.mParticles ) > 0:
            lines.append( (depth*indent) + "{" )
            depth += 1
            for i in range( len( self.mParticles ) ):
                p = self.mParticles[ i ]
                s = "%s->addParticle( %s, %s, %s );" % ( varname, p.getParticleEnumToken( ), self.getParticleEnumToken( p ), p.getParam( 'mass' ).getValue( ), );
                lines.append( (depth*indent) + s )
            depth -= 1
            lines.append( (depth*indent) + "}" )
        if len( self.mReactions ) > 0:
            lines.append( (depth*indent) + "{" )
            depth += 1
            for i in range( len( self.mReactions ) ):
                r = self.mReactions[ i ]
                if r.isActive( ):
                    s = "%s->getReactions( ).push_back( %s );" % ( varname, r.getReactionEnumToken( ), );
                    lines.append( (depth*indent) + s )
            depth -= 1
            lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getParticles( self ):
        return self.mParticles

    def getReactions( self ):
        return self.mReactions

    def toString( self, additional ):
        additional += str( self.mParticles )
        additional += str( self.mReactions )
        return AgentSpecies.toString( self, additional )

class AgentSpeciesLocated(AgentSpeciesActive):
    
    def __init__(self, class_name="", name="", model=None):
        if not class_name: class_name = "Located"
        AgentSpeciesActive.__init__(self, class_name, name, model)
        self.addParam( Param( "divRadius", "um",  0.97) )
        self.addParam( Param( "divRadiusCV", "float",  0.1) )
        self.addParam( Param( "deathRadius", "um",  0.2) )
        self.addParam( Param( "deathRadiusCV", "float",  0.1) )
        self.addParam( Param( "babyMassFrac", "um",  0.5) )
        self.addParam( Param( "babyMassFracCV", "float",  0.1) )
        self.addParam( Param( "shoveLimit", "um",  0.0) )    # addition to desired radius
        self.addParam( Param( "shoveFactor", "um",  1.15) )  # listed as um/length, but treated as radius scalar
        self.addParam( Param( "shoveScale", "float",  1.0) ) # biocellion-biomodel only
        self.addParam( Param( "brownianScale", "float",  0.0) ) # biocellion-biomodel only
        self.addParam( Param( "fixed", "bool",  False) )
        self.addParam( Param( "noSkinBottomLayerBoundary", "int", 0 ) )
        return

class AgentSpeciesBacterium(AgentSpeciesLocated):
    
    def __init__(self, class_name="", name="", model=None):
        if not class_name: class_name = "Bacterium"
        AgentSpeciesLocated.__init__(self, class_name, name, model)
        self.addParam( Param( "epsMax", "float", 0.15 ) )
        self.addParam( Param( "epsColor", "str", "lightGray" ) )
        print("FIXME: <particle name='capsule'> <particle name='inert'> special names not parsed for Bacterium.")
        return

class AgentSpeciesBactEPS(AgentSpeciesBacterium):
    
    def __init__(self, class_name="", name="", model=None):
        if not class_name: class_name = "BactEPS"
        AgentSpeciesBacterium.__init__(self, class_name, name, model)
        self.addParam( Param( "kHyd", "hr-1", 0.007 ) )
        return

class AgentSpeciesYeast(AgentSpeciesBactEPS):
    
    def __init__(self, class_name="", name="", model=None):
        if not class_name: class_name = "Yeast"
        AgentSpeciesBactEPS.__init__(self, class_name, name, model)
        self.addParam( Param( "useActivationInhibition", "bool", False ) )
        self.addParam( Param( "neighborhoodRadiusCoefficient", "float", 2.5 ) )
        self.addParam( Param( "startingTimeActivationInhibition", "int", 0 ) )
        print("FIXME: <species><entryConditions></species> not yet parsed <entryCondition type='type' name='name'><switch/><fromSpecies/> </entryCondition>")
        print("FIXME: <species><switchingLags></species> not yet parsed")
        return


class AllAgentSpecies( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, AgentSpecies )
        self.mModel = model
        return

    def addSpecies( self, class_name, name, species=None ):
        if species is None:
            if class_name == "Species":
                species = AgentSpecies(class_name, name, self.mModel)
            elif class_name == "Active":
                species = AgentSpeciesActive(class_name, name, self.mModel)
            elif class_name == "Located":
                species = AgentSpeciesLocated(class_name, name, self.mModel)
            elif class_name == "Bacterium":
                species = AgentSpeciesBacterium(class_name, name, self.mModel)
            elif class_name == "BactEPS":
                species = AgentSpeciesBactEPS(class_name, name, self.mModel)
            elif class_name == "Yeast":
                species = AgentSpeciesYeast(class_name, name, self.mModel)
            else:
                species = None
                print("Unknown species class: %s" % (class_name, ))
                print("Needs to be implemented")
                return False

        return self.addItem( name, species )

    def getBioModelH(self, indent, depth):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSpeciesEnum( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSpeciesBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getSpeciesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _agent_species_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_AGENT_SPECIES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} agent_species_type_e;" )
        return "\n".join( lines )

    def getSpeciesBioModelH(self, indent, depth):
        lines = [ ]
        for name in self.mOrder:
            s = self.mItems[ name ].getBioModelH( indent, depth )
            lines.append( s );
        return "\n".join( lines )
    
    def getInitializeBioModel(self, indent, depth):
        lines = [ ]
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
