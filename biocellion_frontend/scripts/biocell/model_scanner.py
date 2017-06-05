from biocell import *
import xml.etree.ElementTree as ET

def same_type( a, b ):
    if type( a ).__name__ == 'instance':
        ta = a.__class__.__name__
    elif type( a ).__name__ == 'classobj':
        ta = a.__name__
    else:
        raise Exception( "ERROR: Unable to find type of object: " + str( a ) )

    if type( b ).__name__ == 'instance':
        tb = b.__class__.__name__
    elif type( b ).__name__ == 'classobj':
        tb = b.__name__
    else:
        raise Exception( "ERROR: Unable to find type of object: " + str( b ) )

    return ta == tb

class ModelScanner:

    def __init__( self, model ):
        self.mModel = model
        self.SCAN_VALIDATE_MODE = 1
        self.SCAN_PARSE_MODE = 2
        return

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
        
        ## connect the related elements
        self.mModel.organizeChildren()
        return

    def scanXML( self, root ):
        ok = self.scanIdynomicsXML( root )
        if self.mScanMode == self.SCAN_VALIDATE_MODE:
            if ok:
                print( "XML file validated." )
            else:
                raise Exception( "ERROR : Bad XML file" )
        elif self.mScanMode == self.SCAN_PARSE_MODE:
            pass
        else:
            raise Exception( "ERROR : Bad scan mode." )
            
        return

    def validateNodeXML( self, node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, param_validator ):
        found_attributes = set()
        found_children = set()
        found_params = set()
        
        ok = True
        for (name, value) in node.items():
            if name in may_attributes or name in required_attributes:
                found_attributes.add( name )
                if ( not same_type( param_validator, Param ) ):
                    node_attr = Param( name, None, value )
                    if not param_validator.validateAttribute( node_attr ):
                        ok = False
                        default_attr = param_validator.getAttribute( name )
                        msg  = "Attribute ( " + str( node_attr ) + " ) of ( " + node.tag + " ) not valid."
                        msg += " Expected attribute is " + str( default_attr )
                        print( msg )
            else:
                ok = False
                print( "Attribute ( " + name + " = " + value + " ) of ( " + node.tag + " ) not expected, but found." )
            
        for name in required_attributes:
            if name not in found_attributes:
                ok = False
                print( "Attribute ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )
            
        for child in list(node):
            if child.tag in required_children or child.tag in may_children:
                found_children.add( child.tag )
            else:
                ok = False
                print( "Child ( " + child.tag + " ) of ( " + node.tag + " ) not expected, but found." )

            if child.tag == "param":
                name = child.get('name')
                if ( name in may_params ) or ( name in required_params ) or ( '*' in may_params ):
                    found_params.add( name )
                else:
                    ok = False
                    print( "Param ( " + name + " = " + child.text + " ) of ( " + node.tag + " ) not expected, but found." )
                    print( "Allowed Params = " + str( may_params ) )
                
                param = Param( child.get('name'), child.get('unit'), child.text )
                if not param_validator.validateParam( param ):
                    ok = False
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
            if name in may_attributes or name in required_attributes:
                found_attributes.add( name )
                if ( not same_type( node_object, Param ) ):
                    node_attr = Param( name, None, value )
                    if not node_object.validateAttribute( node_attr ):
                        ok = False
                        raise Exception( "ERROR : Unknown attribute (" + str( node_attr ) + ") for tag (" + node.tag + ")" )
                    if not node_object.updateAttribute( node_attr ):
                        ok = False
                        raise Exception( "ERROR : Unknown attribute (" + str( node_attr ) + ") for tag (" + node.tag + ")" )
            else:
                ok = False
                raise Exception( "ERROR : Attribute ( " + name + " = " + value + " ) of ( " + node.tag + " ) not expected, but found." )
            
        for name in required_attributes:
            if name not in found_attributes:
                ok = False
                raise Exception( "ERROR : Attribute ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )
            
        for child in list(node):
            if child.tag in required_children or child.tag in may_children:
                found_children.add( child.tag )
            else:
                ok = False
                raise Exception( "ERROR : Child ( " + child.tag + " ) of ( " + node.tag + " ) not expected, but found." )

            if child.tag == "param":
                name = child.get('name')
                child_param = Param( name, child.get('unit'), child.text )
                if ( name in may_params ) or ( name in required_params ) or ( '*' in may_params ):
                    found_params.add( name )
                else:
                    ok = False
                    raise Exception( "ERROR : Param ( " + str( child_param ) + " ) of ( " + node.tag + " ) not expected, but found." )

                if not node_object.validateParam( child_param ):
                    ok = False
                    raise Exception( "ERROR : ( " + str( child_param ) + " ) of ( " + node.tag + " ) not valid, check name, unit, and value." )
                if not node_object.updateParam( child_param ):
                    ok = False
                    raise Exception("ERROR : Unknown param (" + str( child_param ) + ") for tag (" + node.tag + ") : check name, unit, and value.")

        for name in required_children:
            if name not in found_children:
                ok = False
                raise Exception("ERROR : Child ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

        for name in required_params:
            if name not in found_params:
                ok = False
                raise Exception("ERROR : Param ( " + name + " ) of ( " + node.tag + " ) expected, but not found." )

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
            raise Exception( "ERROR : Bad scan mode." )
            return False

    def findAllKeyedItems( self, root ):
        """Find all lists of items that need to have keys generated, and generate their keys"""

        # loop over all children
        for child in list( root ):
            if child.tag in ( 'simulator', 'input', 'world', 'solver', 'agentGrid' ):
                # not a list element
                pass
            elif child.tag in ( 'solute', 'particle', 'reaction', 'molecularReactions', 'species' ):
                tag = child.tag
                name = child.get( 'name' )
                class_name = child.get( 'class' )
                item = self.mModel.getItemAddIfNeeded( tag, name, class_name )
            else:
                raise Exception( "Unexpected child of the XML root: " + str( child.tag ) )
        
        return
        
    def scanIdynomicsXML( self, node, parent_object=None ):
        may_children = ( "simulator", "input", "solute", "particle", "world", "reaction", "molecularReactions", "solver", "agentGrid", "species", )
        required_children = ( "simulator", )
        self.mModel.resetIDynoMiCS( )
        node_object = self.mModel.getIDynoMiCS( )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
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

        self.findAllKeyedItems( node )
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        return ok

    def scanSimulatorXML( self, node, parent_object=None ):
        may_children = ( "param", "timeStep", )
        required_children = (  )
        node_object = self.mModel.getIDynoMiCS( ).getSimulator()
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None
        child_methods = {
            "param": self.scanGenericParamXML,
            "timeStep": self.scanSimulatorTimeStepXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        return ok

    def scanSimulatorTimeStepXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = (  )
        node_object = self.mModel.getIDynoMiCS( ).getSimulator().getTimeStep()
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
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
        may_children = ( "param", )
        required_children = (  )
        node_object = self.mModel.getItemAddIfNeeded( node.tag, node.get( 'name' ), node.get( 'class' ) )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        return ok

    def scanParticleXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = (  )
        node_object = self.mModel.getItemAddIfNeeded( node.tag, node.get( 'name' ), node.get( 'class' ) )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldXML( self, node, parent_object=None ):
        may_children = ( "bulk", "agar", "computationDomain", )
        required_children = (  )
        node_object = self.mModel.getIDynoMiCS( ).getWorld( )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "bulk": self.scanWorldBulkXML,
            "agar": self.scanWorldAgarXML,
            "computationDomain": self.scanWorldComputationDomainXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldBulkXML( self, node, parent_object=None ):
        may_children = ( "solute", "param", )
        required_children = ( )
        if not self.mModel.getIDynoMiCS( ).getWorld().getBulks().addItem( node.get('name') ):
            raise Exception( "ERROR : couldn't add a bulk." )
        node_object = self.mModel.getIDynoMiCS( ).getWorld().getBulks().getLastItem( )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "solute": self.scanWorldBulkSoluteXML,
        }

        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok


    def scanWorldBulkSoluteXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = (  )
        if parent_object is None:
            node_object = BulkSolute( node.get( 'name' ) )
        else:
            if not parent_object.getSolutes().addItem( node.get('name') ):
                raise Exception( "ERROR : couldn't add a bulk solute." )
            node_object = parent_object.getSolutes().getLastItem( )
            node_object.setReference( self.mModel.getItem( node.tag, node.get( 'name' ) ) )
            parent_object = None
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldAgarXML( self, node, parent_object=None ):
        ok = True
        print( "world-agar not scaned yet" )
        return ok

    def scanWorldComputationDomainXML( self, node, parent_object=None ):
        may_children = ( "grid", "boundaryCondition", "param", )
        required_children = ( "grid", )
        if not self.mModel.getIDynoMiCS( ).getWorld().getComputationDomains().addItem( node.get('name') ):
            raise Exception( "ERROR : couldn't add a computation domain." )
        node_object = self.mModel.getIDynoMiCS( ).getWorld().getComputationDomains().getLastItem( )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "grid": self.scanWorldComputationDomainGridXML,
            "boundaryCondition": self.scanWorldComputationDomainBoundaryConditionXML,
        }

        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldComputationDomainGridXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = ComputationDomainGrid( )
        else:
            node_object = parent_object.getGrid( )
            parent_object = None # don't need to add this as a child
            
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        
        child_methods = {
        }

        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanWorldComputationDomainBoundaryConditionXML( self, node, parent_object=None ):
        ok = True
        print( "world-computationdomain-boundarycondition not scaned yet" )
        return ok

    def scanReactionXML( self, node, parent_object=None ):
        may_children = ( "param", "kineticFactor", "yield", )
        required_children = (  )
        node_object = self.mModel.getItemAddIfNeeded( node.tag, node.get( 'name' ), node.get( 'class' ) )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "kineticFactor": self.scanReactionKineticFactorXML,
            "yield": self.scanReactionYieldXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanReactionKineticFactorXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = (  )
        if parent_object is None:
            node_object = KineticFactor( )
        else:
            if not parent_object.getKineticFactors().addItem(  ):
                raise Exception( "ERROR : couldn't add a reaction kinetic factor." )
            node_object = parent_object.getKineticFactors().getLastItem()
            if node.get( 'solute' ):
                node_object.setReference( self.mModel.getItem( 'solute', node.get( 'solute' ) ) )
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok
    
    def scanReactionYieldXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = (  )
        if parent_object is None:
            node_object = Yields( )
        else:
            node_object = parent_object.getYields( )
            parent_object = None
        
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )

        for yield_key in node_object.getKeys( ):
            if yield_key == '*': continue
            yield_item = node_object.getItem( yield_key )
            yield_name = yield_item.getName( )
            if self.mModel.getIDynoMiCS( ).getParticles( ).hasKey( yield_name ):
                node_object.setParticle( yield_name, self.mModel.getItem( 'particle', yield_name ) )
            elif self.mModel.getIDynoMiCS( ).getSolutes( ).hasKey( yield_name ):
                node_object.setSolute( yield_name, self.mModel.getItem( 'solute', yield_name ) )
            else:
                msg  = "ERROR: Reaction.Yield.Param should name a solute or a particle. " + str( yield_name ) + " is not in either list.\n"
                msg += "ERROR: Known solutes: " + ", ".join( self.mModel.getIDynoMiCS( ).getSolutes( ).getKeys( ) ) + ".\n"
                msg += "ERROR: Known particles: " + ", ".join( self.mModel.getIDynoMiCS( ).getParticles( ).getKeys( ) ) + ".\n"
                raise Exception( msg )
        
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
        may_children = ( "param", )
        required_children = ( "param", )
        node_object = self.mModel.getIDynoMiCS( ).getAgentGrid()
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child

        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesXML( self, node, parent_object=None ):
        may_children = ( "param", "particle", "reaction", "tightJunctions", "adhesions", "initArea", "entryConditions", "chemotaxis", "switchingLags",  )
        required_children = (  )
        node_object = self.mModel.getItemAddIfNeeded( node.tag, node.get( 'name' ), node.get( 'class' ) )
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        parent_object = None # don't need to add this as a child
        child_methods = {
            "param": self.scanGenericParamXML,
            "particle": self.scanSpeciesParticleXML,
            "reaction": self.scanSpeciesReactionXML,
            "tightJunctions": self.scanSpeciesTightJunctionsXML,
            "adhesions": self.scanSpeciesAdhesionsXML,
            "initArea": self.scanSpeciesInitAreaXML,
            "entryConditions": self.scanSpeciesEntryConditionsXML,
            "chemotaxis": self.scanSpeciesChemotaxisXML,
            "switchingLags": self.scanSpeciesSwitchLagsXML,
            #"": self.scanSpeciesXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesParticleXML( self, node, parent_object=None ):
        may_children = ( "param", )
        required_children = ( "param", )
        if parent_object is None:
            node_object = AgentSpeciesParticle()
        else:
            if not parent_object.getParticles().addItem( node.get('name') ):
                raise Exception( "ERROR : couldn't add a species particle." )
            node_object = parent_object.getParticles().getLastItem()
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
            "param": self.scanGenericParamXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesReactionXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = AgentSpeciesReaction( )
        else:
            if not parent_object.getReactions().addItem( node.get('name') ):
                raise Exception( "ERROR : couldn't add a species reaction." )
            node_object = parent_object.getReactions().getLastItem()
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesTightJunctionsXML( self, node, parent_object=None ):
        may_children = ( "tightJunction", )
        required_children = ( "tightJunction", )
        if parent_object is None:
            node_object = ItemHolder( TightJunction )
        else:
            node_object = parent_object.getTightJunctions( )
            parent_object = None # don't need to add this as a child
        may_attributes = ( )
        required_attributes = ( )
        may_params = ( )
        required_params = ( )
        child_methods = {
            "tightJunction": self.scanSpeciesTightJunctionXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesTightJunctionXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = TightJunction( )
        else:
            if not parent_object.addItem(  ):
                raise Exception( "ERROR : couldn't add a species tight junction." )
            node_object = parent_object.getLastItem( )
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    
    def scanSpeciesAdhesionsXML( self, node, parent_object=None ):
        may_children = ( "adhesion", )
        required_children = ( "adhesion", )
        if parent_object is None:
            node_object = ItemHolder( Adhesion )
        else:
            node_object = parent_object.getAdhesions( )
            parent_object = None # don't need to add this as a child
        may_attributes = ( )
        required_attributes = ( )
        may_params = ( )
        required_params = ( )
        child_methods = {
            "adhesion": self.scanSpeciesAdhesionXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesAdhesionXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = Adhesion( )
        else:
            if not parent_object.addItem(  ):
                raise Exception( "ERROR : couldn't add a species adhesion." )
            node_object = parent_object.getLastItem( )
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok
    

    def scanSpeciesInitAreaXML( self, node, parent_object=None ):
        may_children = ( "param", "coordinates", "blocks", )
        required_children = ( "param", )
        if parent_object is None:
            node_object = InitArea()
        else:
            if not parent_object.getInitAreas(  ).addItem(  ):
                raise Exception( "ERROR : couldn't add a species initArea." )
            node_object = parent_object.getInitAreas().getLastItem()
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
            "param": self.scanGenericParamXML,
            "coordinates": self.scanCoordinatesXML,
            "blocks": self.scanBlocksXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanCoordinatesXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = Coordinates()
        else:
            if not parent_object.getCoordinates(  ).addItem(  ):
                raise Exception( "ERROR : couldn't add a coordinates." )
            node_object = parent_object.getCoordinates().getLastItem()
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanBlocksXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = Blocks()
        else:
            if not parent_object.getBlocks(  ).addItem(  ):
                raise Exception( "ERROR : couldn't add a blocks." )
            node_object = parent_object.getBlocks().getLastItem()
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesEntryConditionsXML( self, node, parent_object=None ):
        ok = True
        print( "species::entryConditions not scaned yet" )
        return ok

    def scanSpeciesChemotaxisXML( self, node, parent_object=None ):
        may_children = ( "chemotactic", )
        required_children = ( "chemotactic", )
        if parent_object is None:
            node_object = ItemHolder( Chemotaxis )
        else:
            node_object = parent_object.getChemotaxis( )
            parent_object = None # don't need to add this as a child
        may_attributes = ( )
        required_attributes = ( )
        may_params = ( )
        required_params = ( )
        child_methods = {
            "chemotactic": self.scanSpeciesChemotacticXML,
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesChemotacticXML( self, node, parent_object=None ):
        may_children = (  )
        required_children = (  )
        if parent_object is None:
            node_object = Chemotaxis( )
        else:
            if not parent_object.addItem(  ):
                raise Exception( "ERROR : couldn't add a species chemotactic." )
            node_object = parent_object.getLastItem( )
            if node.get( 'withSolute' ):
                node_object.setReference( self.mModel.getItem( 'solute', node.get( 'withSolute' ) ) )
            parent_object = None # don't need to add this as a child
        may_attributes = node_object.getMayAttributes( )
        required_attributes = node_object.getRequiredAttributes( )
        may_params = node_object.getMayParams( )
        required_params = node_object.getRequiredParams( )
        child_methods = {
        }
        
        ok = self.scanNodeXML( node, may_attributes, required_attributes, may_children, required_children, child_methods, may_params, required_params, node_object, parent_object )
        
        return ok

    def scanSpeciesSwitchLagsXML( self, node, parent_object=None ):
        ok = True
        print( "species::switchLags not scaned yet" )
        return ok

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
