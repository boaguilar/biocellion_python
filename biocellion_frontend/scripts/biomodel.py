import sys
import xml.etree.ElementTree as ET
from agent_species import Param
import simulator, particle, agent_species, agent_grid

def same_type( a, b ):
    if type( a ).__name__ == 'instance':
        ta = a.__class__.__name__
    elif type( a ).__name__ == 'classobj':
        ta = a.__name__
    else:
        sys.exit( "ERROR: Unable to find type of object: " + str( a ) )

    if type( b ).__name__ == 'instance':
        tb = b.__class__.__name__
    elif type( b ).__name__ == 'classobj':
        tb = b.__name__
    else:
        sys.exit( "ERROR: Unable to find type of object: " + str( b ) )

    return ta == tb

class BioModel:

    def __init__( self ):
        self.mIDynoMiCS = simulator.IDynoMiCS()
        self.SCAN_VALIDATE_MODE = 1
        self.SCAN_PARSE_MODE = 2
        return

    def getBioModelH( self, indent, depth ):
        return self.mIDynoMiCS.getBioModelH( indent, depth )

    def getInitializeBioModel( self, indent, depth ):
        return self.mIDynoMiCS.getInitializeBioModel( indent, depth )

    def parseXML( self, xmlfilename ):
        ## read parameters from input file.
        tree = ET.parse(xmlfilename)
        root = tree.getroot()

        ## validate format/content of XML
        self.mScanMode = self.SCAN_VALIDATE_MODE
        self.scanXML( root )

        ## parse full XML content
        self.mScanMode = self.SCAN_PARSE_MODE
        self.scanXML( root )
        
        if True:
            self.mIDynoMiCS.organizeChildren()
            # print( str( self ) )
            # sys.exit( "Early termination" )
            return
        
        return

    def parseSpeciesXML( self, root ):
        for species in root.findall('species'):
            name = species.get('name')

            ## parse parameters
            for param in species.findall('param'):
                agent_param = Param( param.get('name'), param.get('unit'), param.text )
                if not self.mAgentSpecies.getSpecies( name ).updateParam( agent_param ):
                    sys.exit( "ERROR : Unknown param (" + str( agent_param ) + ") for species (" + name + ")" )

            
        return

    def scanXML( self, root ):
        ok = self.scanIdynomicsXML( root )
        if self.mScanMode == self.SCAN_VALIDATE_MODE:
            if ok:
                print( "XML file validated." )
            else:
                sys.exit( "ERROR : Bad XML file" )
        elif self.mScanMode == self.SCAN_PARSE_MODE:
            pass
        else:
            sys.exit( "ERROR : Bad scan mode." )
            
        return

    def validateNodeXML( self, node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, param_validator ):
        found_attributes = set()
        found_children = set()
        found_params = set()
        
        ok = True
        for (name, value) in node.items():
            if name in required_attributes:
                found_attributes.add( name )
            elif name in may_attributes:
                pass
            else:
                ok = False
                print( "Attribute ( " + name + " = " + value + " ) of ( " + node.tag + " ) not expected, but found." )
            
        for name in required_attributes:
            if name not in found_attributes:
                ok = False
                print( "Attribute ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )
            
        for child in list(node):
            if child.tag in required_children:
                found_children.add( child.tag )
            elif child.tag in may_children:
                pass
            else:
                ok = False
                print( "Child ( " + child.tag + " ) of ( " + node.tag + " ) not expected, but found." )

            if child.tag == "param":
                found_params.add( child.get('name') )
                param = Param( child.get('name'), child.get('unit'), child.text )
                if not param_validator.validateParam( param ):
                    print( "Param ( " + str( param ) + " ) of ( " + node.tag + " ) not valid, check name, units, and value." )


        for name in required_children:
            if name not in found_children:
                ok = False
                print( "Child ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

        for name in required_params:
            if name not in found_params:
                ok = False
                print( "Param ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

        for child in list( node ):
            ok = child_methods[ child.tag ]( child ) and ok

        return ok

    def parseNodeXML( self, node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object ):

        found_attributes = set()
        found_children = set()
        found_params = set()
        
        ok = True
        for (name, value) in node.items():
            if name in may_attributes or name in must_attributes:
                found_attributes.add( name )
                if ( not same_type( node_object, Param ) ):
                    node_attr = Param( name, None, value )
                    if not node_object.validateAttribute( node_attr ):
                        ok = False
                        sys.exit( "ERROR : Unknown attribute (" + str( node_attr ) + ") for tag (" + node.tag + ")" )
                    if not node_object.updateAttribute( node_attr ):
                        ok = False
                        sys.exit( "ERROR : Unknown attribute (" + str( node_attr ) + ") for tag (" + node.tag + ")" )
            else:
                ok = False
                sys.exit( "ERROR : Attribute ( " + name + " = " + value + " ) of ( " + node.tag + " ) not expected, but found." )
            
        for name in required_attributes:
            if name not in found_attributes:
                ok = False
                sys.exit( "ERROR : Attribute ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )
            
        for child in list(node):
            if child.tag in required_children or child.tag in may_children:
                found_children.add( child.tag )
            else:
                ok = False
                sys.exit( "ERROR : Child ( " + child.tag + " ) of ( " + node.tag + " ) not expected, but found." )

            if child.tag == "param":
                found_params.add( child.get('name') )
                child_param = Param( child.get('name'), child.get('unit'), child.text )
                if not node_object.validateParam( child_param ):
                    ok = False
                    sys.exit( "ERROR : ( " + str( child_param ) + " ) of ( " + node.tag + " ) not valid, check name, unit, and value." )
                if not node_object.updateParam( child_param ):
                    ok = False
                    sys.exit("ERROR : Unknown param (" + str( child_param ) + ") for tag (" + node.tag + ") : check name, unit, and value.")

        for name in required_children:
            if name not in found_children:
                ok = False
                sys.exit("ERROR : Child ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

        for name in required_params:
            if name not in found_params:
                ok = False
                sys.exit("ERROR : Param ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

        for child in list( node ):
            ok = child_methods[ child.tag ]( child, node_object ) and ok

        if ( parent_object is not None ) and ( not same_type( node_object, Param ) ):
            parent_object.addChild( node_object )

        return ok


    def scanGenericParamXML( self, node, parent_object=None ):
        may_attributes = ( "name", "unit", )
        required_attributes = ( "name", )
        may_children = (  )
        required_children = (  )
        required_children = (  )
        may_params = (  )
        required_params = (  )
        node_object = Param( node.get('name'), node.get('unit'), node.text )
        child_methods = {
        }

        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanNodeXML( self, node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object ):
        if self.mScanMode == self.SCAN_VALIDATE_MODE:
            return self.validateNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object )
        elif self.mScanMode == self.SCAN_PARSE_MODE:
            return self.parseNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        else:
            sys.exit( "ERROR : Bad scan mode." )
            return False
        
    def scanIdynomicsXML( self, node, parent_object=None ):
        may_attributes = (  )
        required_attributes = (  )
        may_children = ( "simulator", "input", "solute", "particle", "world", "reaction", "molecularReactions", "solver", "agentGrid", "species", )
        required_children = ( "simulator", )
        may_params = (  )
        required_params = (  )
        node_object = self.mIDynoMiCS = simulator.IDynoMiCS()
        child_methods = {
            "simulator": self.scanSimulatorXML,
            "input": self.scanInputXML,
            "solute": self.scanSoluteXML,
            "particle": self.scanParticleXML,
            "world": self.scanWorldXML,
            "reaction": self.scanReactionXML,
            "molecularReactions": self.scanMolecularReactionsXML,
            "solver": self.scanSolverXML,
            "agentGrid": self.scanAgentGridXML,
            "species": self.scanSpeciesXML,
        }

        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        return ok

    def scanSimulatorXML( self, node, parent_object=None ):
        may_attributes = (  )
        required_attributes = (  )
        may_children = ( "param", "timeStep", )
        required_children = (  )
        may_params = ( "restartPreviousRun", "randomSeed", "outputPeriod", "chemostat", "diffusionReactionOnAgentTime", "agentTiemStep", )
        required_params = (  )
        node_object = self.mIDynoMiCS.getSimulator()
        parent_object = None
        child_methods = {
            "param": self.scanGenericParamXML,
            "timeStep": self.scanSimulatorTimeStepXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        return ok

    def scanSimulatorTimeStepXML( self, node, parent_object=None ):
        may_attributes = (  )
        required_attributes = (  )
        may_children = ( "param", )
        required_children = (  )
        may_params = ( "adaptive", "timeStepIni", "timeStepMin", "timeStepMax", "endOfSimulation", )
        required_params = (  )
        node_object = self.mIDynoMiCS.getSimulator().getTimeStep()
        parent_object = None
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanInputXML( self, node, parent_object=None ):
        ok = True
        print( "input not scaned yet" )
        return ok

    def scanSoluteXML( self, node, parent_object=None ):
        ok = True
        print( "solute not scaned yet" )
        return ok

    def scanParticleXML( self, node, parent_object=None ):
        may_attributes = ( "name", "regulator", )
        required_attributes = ( "name",  )
        may_children = ( "param", )
        required_children = (  )
        may_params = ( "density", )
        required_params = ( "density", )
        if not self.mIDynoMiCS.getParticles().addParticle( node.get('name') ):
            sys.exit( "ERROR : couldn't add a particle." )
        node_object = self.mIDynoMiCS.getParticles().getLastParticle()
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldXML( self, node, parent_object=None ):
        ok = True
        print( "world not scaned yet" )
        return ok

    def scanReactionXML( self, node, parent_object=None ):
        ok = True
        print( "reaction not scaned yet" )
        return ok

    def scanMolecularReactionsXML( self, node, parent_object=None ):
        ok = True
        print( "molecularReactions not scaned yet" )
        return ok

    def scanSolverXML( self, node, parent_object=None ):
        ok = True
        print( "solver not scaned yet" )
        return ok

    def scanAgentGridXML( self, node, parent_object=None ):
        may_attributes = (  )
        required_attributes = (   )
        may_children = ( "param", )
        required_children = ( "param", )
        may_params = ( "computationDomain", "resolution", "shovingFraction", "shovingMaxIter", "shovingMutual", "erosionMethod", "sloughDetachedBiomass", )
        required_params = ( "computationDomain", "resolution", "shovingFraction", "shovingMaxIter", "shovingMutual", )
        node_object = self.mIDynoMiCS.getAgentGrid()
        parent_object = None # don't need to add this as a child

        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesXML( self, node, parent_object=None ):
        may_attributes = ( "class", "name", )
        required_attributes = ( "class", "name",  )
        may_children = ( "param", "particle", "reaction", "tightJunctions", "initArea", "entryConditions", "chemotaxis", "switchingLags",  )
        required_children = (  )
        may_params = ( "density", )
        required_params = (  )
        if not self.mIDynoMiCS.getAgentSpecies().addSpecies( node.get('class'), node.get('name') ):
            sys.exit( "ERROR : couldn't add a species." )
        node_object = self.mIDynoMiCS.getAgentSpecies().getLastSpecies()
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "particle": self.scanSpeciesParticleXML,
            "reaction": self.scanSpeciesReactionXML,
            "tightJunctions": self.scanSpeciesTightJunctionsXML,
            "initArea": self.scanSpeciesInitAreaXML,
            "entryConditions": self.scanSpeciesEntryConditionsXML,
            "chemotaxis": self.scanSpeciesChemotaxisXML,
            "switchingLags": self.scanSpeciesSwitchLagsXML,
            #"": self.scanSpeciesXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesParticleXML( self, node, parent_object=None ):
        may_attributes = ( "name", )
        required_attributes = ( "name",  )
        may_children = ( "param", )
        required_children = ( "param", )
        may_params = ( "mass", )
        required_params = ( "mass", )
        if parent_object is None:
            node_object = agent_species.AgentSpeciesParticle()
        else:
            if not parent_object.getParticles().addItem( node.get('name') ):
                sys.exit( "ERROR : couldn't add a species particle." )
            node_object = parent_object.getParticles().getLastItem()
            parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesReactionXML( self, node, parent_object=None ):
        ok = True
        print( "species::reaction not scaned yet" )
        return ok

    def scanSpeciesTightJunctionsXML( self, node, parent_object=None ):
        ok = True
        print( "species::tightJunctions not scaned yet" )
        return ok

    def scanSpeciesInitAreaXML( self, node, parent_object=None ):
        may_attributes = ( "number", "shape", )
        required_attributes = ( "number",  )
        may_children = ( "param", "coordinates", "blocks", )
        required_children = ( "param", )
        may_params = ( "birthday", )
        required_params = ( "birthday", )
        if parent_object is None:
            node_object = agent_species.InitArea()
        else:
            if not parent_object.getInitAreas(  ).addItem(  ):
                sys.exit( "ERROR : couldn't add a species initArea." )
            node_object = parent_object.getInitAreas().getLastItem()
            parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "coordinates": self.scanCoordinatesXML,
            "blocks": self.scanBlocksXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanCoordinatesXML( self, node, parent_object=None ):
        may_attributes = ( "x", "y", "z", "r", )
        required_attributes = ( "x", "y", )
        may_children = (  )
        required_children = (  )
        may_params = (  )
        required_params = (  )
        if parent_object is None:
            node_object = agent_species.Coordinates()
        else:
            if not parent_object.getCoordinates(  ).addItem(  ):
                sys.exit( "ERROR : couldn't add a coordinates." )
            node_object = parent_object.getCoordinates().getLastItem()
            parent_object = None # don't need to add this as a child
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanBlocksXML( self, node, parent_object=None ):
        may_attributes = ( "rows", "cols", "bars", )
        required_attributes = ( "rows", "cols", )
        may_children = (  )
        required_children = (  )
        may_params = (  )
        required_params = (  )
        if parent_object is None:
            node_object = agent_species.Blocks()
        else:
            if not parent_object.getBlocks(  ).addItem(  ):
                sys.exit( "ERROR : couldn't add a blocks." )
            node_object = parent_object.getBlocks().getLastItem()
            parent_object = None # don't need to add this as a child
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesEntryConditionsXML( self, node, parent_object=None ):
        ok = True
        print( "species::entryConditions not scaned yet" )
        return ok

    def scanSpeciesChemotaxisXML( self, node, parent_object=None ):
        ok = True
        print( "species::chemotaxis not scaned yet" )
        return ok

    def scanSpeciesSwitchLagsXML( self, node, parent_object=None ):
        ok = True
        print( "species::switchLags not scaned yet" )
        return ok

    def __str__( self ):
        s = "<BioModel>\n"
        s += str( self.mIDynoMiCS ) + "\n"
        s += "</BioModel>"
        return s

    def __repr__( self ):
        return str( self )
