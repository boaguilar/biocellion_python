#####################################################
# Param
#####################################################
class Param:

    mTimeConvert = { "h": 1.0,
                     "hr": 1.0,
                     "hour": 1.0,
                     "s": 1.0/(60.*60.),
                     "sec": 1.0/(60.*60.),
                     "second": 1.0/(60.*60.),
                   }
    mTimeStandardUnit = "hr"
    mTimeUnits = tuple( mTimeConvert )
    
    mStringUnits = ( "str", "string" )
    mStringStandardUnit = "str"
    
    def toStandardUnit( self ):
        if self.mUnit in self.mTimeUnits:
            self.mValue *= self.mTimeConvert[ self.mUnit ]
            self.mUnit = self.mTimeStandardUnit
        if self.mUnit in self.mStringUnits:
            self.mUnit = self.mStringStandardUnit
        return
    
    mRealUnits = ( "float", "um", "hr-1", "g.L-1" ) + mTimeUnits
    mIntUnits = ( "int" )
    mBoolUnits = ( "bool" )
    
    def __init__(self, name, unit, default_value):
        # infer type from format of default_value
        if unit is None or unit in self.mIntUnits:
            try:
                default_value = int(default_value)
                if unit is None:
                    unit = 'int'
            except:
                pass
                
        if unit is None or unit in self.mRealUnits:
            try:
                default_value = float(default_value)
                if unit is None:
                    unit = 'float'
            except:
                pass
        
        if unit is None or unit in self.mBoolUnits:
            try:
                default_value = str(default_value)
                if default_value.lower() == "true":
                    default_value = True
                    if unit is None:
                        unit = 'bool'
                elif default_value.lower() == "false":
                    default_value = False
                    if unit is None:
                        unit = 'bool'
                else:
                    pass
            except:
                pass

        if unit is None or unit in self.mStringUnits:
            try:
                default_value = str(default_value)
                if unit is None:
                    unit = 'str'
            except:
                pass

        self.mName = str(name)
        self.mConstName = "SP_%s" % (name, )
        self.mUnit = str(unit)
        self.mDefaultValue = default_value
        self.mValue = default_value

        self.toStandardUnit()
        return

    def copy(self):
        p = Param(self.mName, self.mUnit, self.mDefaultValue)
        p.setValue( self.mValue )
        return p

    def getName(self):
        return self.mName
    def getConstName(self):
        return self.mConstName
    def getUnit(self):
        return self.mUnit
    def getDefaultValue(self):
        return self.mDefaultValue
    def getValue(self):
        return self.mValue
    def setUnit(self, unit):
        self.mUnit = unit
        return True
    def setValue(self, value):
        self.mValue = value
        return True

    # other is another Param object
    def setValueFromParam(self, other):
        if self.getUnit() != other.getUnit():
            return False
        return self.setValue( other.getValue() )

    def validateParam(self, other):
        if self.getUnit() != other.getUnit():
            return False
        return True

    def __str__(self):
        s = '<param name="%s" unit="%s">%s</param>' % (str(self.mName), str(self.mUnit), str(self.mValue))
        return s

    def __repr__(self):
        return str(self)


#####################################################
# ParamHolder
#####################################################
class ParamHolder:

    def __init__(self):
        self.mAttributes = {  }
        self.mParams = {  }
        self.mChildren = {  }
        self.mHiddenParams = [ ]
        return

    def addChild( self, child ):
        t = child.__class__.__name__
        if t not in self.mChildren:
            self.mChildren[ t ] = [  ]
        self.mChildren[ t ].append( child )
        return

    def countUnits(self, units):
        count = 0
        for n in self.mParams:
            param = self.mParams[ n ]
            if param.getUnit() in units:
                count += 1
        return count
        
    def countReal(self):
        return self.countUnits( Param.mRealUnits )
        
    def countInt(self):
        return self.countUnits( Param.mIntUnits )
        
    def countBool(self):
        return self.countUnits( Param.mBoolUnits )
        
    def countString(self):
        return self.countUnits( Param.mStringUnits )
        
    def paramNamesToCpp(self, indent, depth):
        lines = []
        for n in self.mParams:
            if n in self.mHiddenParams:
                continue
            s = (depth*indent) + "const std::string %s = \"%s\";" % (self.mParams[ n ].getConstName(), n, )
            lines.append( s )
        return "\n".join( lines )
        
    def toCpp(self, varname, indent, depth):
        lines = []
        for n in self.mParams:
            if n in self.mHiddenParams:
                continue
            param = self.mParams[ n ]
            if param.getUnit() in Param.mStringUnits:
                s = (depth*indent) + "%s->setParamString( %s->getIdxString( %s ), \"%s\" );" % (varname, varname, param.getConstName(), param.getValue(), )
                lines.append( s )
            elif param.getUnit() in Param.mBoolUnits:
                s = (depth*indent) + "%s->setParamBool( %s->getIdxBool( %s ), %s );" % (varname, varname, param.getConstName(), "true" if param.getValue() else "false", )
                lines.append( s )
            elif param.getUnit() in Param.mIntUnits:
                s = (depth*indent) + "%s->setParamInt( %s->getIdxInt( %s ), %s );" % (varname, varname, param.getConstName(), param.getValue(), )
                lines.append( s )
            elif param.getUnit() in Param.mRealUnits:
                s = (depth*indent) + "%s->setParamReal( %s->getIdxReal( %s ), %s );" % (varname, varname, param.getConstName(), param.getValue(), )
                lines.append( s )
            else:
                s = "FIXME: Unknown unit: " + str(param.getUnit())
                lines.append( s )
        return "\n".join( lines )

    def getAttributes(self):
        return self.mAttributes

    def addAttribute(self, attr):
        n = attr.getName()
        if n in self.mAttributes:
            return False
        else:
            self.mAttributes[ n ] = attr.copy()

    def updateAttribute(self, attr):
        n = attr.getName()
        if n in self.mAttributes:
            if self.mAttributes[ n ].setValueFromParam( attr ):
                return True
            if (self.mAttributes[ n ].getUnit() in Param.mRealUnits) and (attr.getUnit() in Param.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                attr = attr.copy()
                attr.setUnit( self.mAttributes[ n ].getUnit() )
                return self.mAttributes[ n ].setValueFromParam( attr )
            else:
                return False
        else:
            return False

    def validateAttribute( self, attr ):
        n = attr.getName()
        if n in self.mAttributes:
            if self.mAttributes[ n ].validateParam( attr ):
                return True
            elif (self.mAttributes[ n ].getUnit() in Param.mRealUnits) and (attr.getUnit() in Param.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                attr = attr.copy()
                attr.setUnit( self.mAttributes[ n ].getUnit() )
                return self.mAttributes[ n ].validateParam( attr )
            else:
                return False
        else:
            return False

    def getParams(self):
        return self.mParams

    def paramExists( self, name ):
        return name in self.mParams
    
    def getParam( self, name ):
        return self.mParams[ name ]
    
    def addParam(self, param):
        n = param.getName()
        if n in self.mParams:
            return False
        else:
            self.mParams[n] = param.copy()

    def updateParam(self, param):
        n = param.getName()
        if n in self.mParams:
            if self.mParams[n].setValueFromParam(param):
                return True
            if (self.mParams[n].getUnit() in Param.mRealUnits) and (param.getUnit() in Param.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                param = param.copy()
                param.setUnit( self.mParams[n].getUnit() )
                return self.mParams[n].setValueFromParam(param)
            else:
                return False
        else:
            return False

    def validateParam( self, param ):
        n = param.getName()
        if n in self.mParams:
            if self.mParams[n].validateParam( param ):
                return True
            elif (self.mParams[n].getUnit() in Param.mRealUnits) and (param.getUnit() in Param.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                param = param.copy()
                param.setUnit( self.mParams[n].getUnit() )
                return self.mParams[n].validateParam( param )
            else:
                return False
        else:
            return False

    def formatAttributes( self ):
        s = ''
        for a in self.mAttributes:
            s += ' %s="%s"' % ( self.mAttributes[ a ].getName(), self.mAttributes[ a ].getValue() )
        return s

    def __str__(self):
        s = ""
        for p in self.mParams:
            s += "\t" + str( self.mParams[ p ] ) + "\n"
        if len( self.mChildren ) > 0:
            s += "\t<children>\n"
            for t in self.mChildren:
                s += "\t\ttype: " + t + "\n"
                for c in self.mChildren[ t ]:
                    s += "\t\t" + str( c ) + "\n"
            s += "\t</children>"
        return s

    def __repr__(self):
        return str(self)


#####################################################
# ItemHolder
#####################################################
class ItemHolder:

    def __init__(self, item_class):
        self.mItems = {}
        self.mOrder = []
        self.mIndex = 0 # used for nameless items
        self.mItemClass = item_class
        return

    def getItems( self ):
        return self.mItems

    def getItem(self, name):
        return self.mItems[ name ]

    def addItem( self, name=None, item=None ):
        if name is None:
            name = self.mIndex
            self.mIndex += 1
        if item is None:
            item = self.mItemClass(  )
        self.mItems[ name ] = item
        self.mOrder.append( name )
        return True

    def getLastItem( self ):
        return self.mItems[ self.mOrder[ len( self.mOrder ) - 1 ] ]

    def __str__( self ):
        s = "<ITEM_HOLDER_ITEMS_" + str( self.mItemClass ) + "\n"

        for name in self.mOrder:
            s += str( self.mItems[ name ] )

        s += "</ITEM_HOLDER_ITEMS_" + str( self.mItemClass ) + "\n"
        return s

    def __repr__( self ):
        return str( self )

#####################################################
# AgentSpeciesParticle
#####################################################
class AgentSpeciesParticle(ParamHolder):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "name", "str", "" ) )
        self.addParam( Param( "mass", "fg", 0.0 ) )
        return

    def __str__(self):
        s  = "<species-particle" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</species-particle>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# Blocks
#####################################################
class Blocks( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "rows", "int", 0 ) )
        self.addAttribute( Param( "cols", "int", 0 ) )
        self.addAttribute( Param( "bars", "int", 0 ) )
        return

    def __str__(self):
        s  = "<blocks" + self.formatAttributes() + "/>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# Coordinates
#####################################################
class Coordinates( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "x", "um", 0 ) )
        self.addAttribute( Param( "y", "um", 0 ) )
        self.addAttribute( Param( "z", "um", 0 ) )
        self.addAttribute( Param( "r", "um", 0 ) )
        return

    def __str__(self):
        s  = "<coordinates" + self.formatAttributes() + "/>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# InitArea
#####################################################
class InitArea( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "number", "float", 0 ) )
        self.addAttribute( Param( "shape", "str", "unfilledBlock" ) )
        self.addParam( Param( "birthday", "hour", 0.0 ) )
        self.mCoordinates = ItemHolder( Coordinates )
        self.mBlocks = ItemHolder( Blocks )
        return

    def getCoordinates( self ):
        return self.mCoordinates

    def getBlocks( self ):
        return self.mBlocks

    def __str__(self):
        s  = "<initArea" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mCoordinates )
        s += str( self.mBlocks )
        s += "</initArea>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# AgentSpecies
#####################################################
class AgentSpecies(ParamHolder):

    def __init__(self, class_name, name):
        self.mClassName = class_name
        self.mName = name
        self.mEnumToken = "AGENT_SPECIES_%s_%s" % (class_name, name)
        self.mUseMechForceReals = False
        self.mMechForceReals = [ ]
        
        ParamHolder.__init__(self)
        self.addAttribute( Param( "class", "str", "" ) )
        self.addAttribute( Param( "name", "str", "" ) )
        
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
        return

    def toCpp(self, indent, depth):
        self.updateUseMechForceReals(  )
        varname = "species"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "AgentSpecies *%s = new AgentSpecies( \"%s\", \"%s\", %s, %d, %d, %d, %d );" % (varname, self.mName, self.mClassName, self.mEnumToken, self.countReal(), self.countInt(), self.countBool(), self.countString()) )
        lines.append( ParamHolder.toCpp( self, varname, indent, depth ) )

        s = (depth*indent) + "%s->setDMax( 2.0 /* FIXME: read from AgentGrid */ );" % (varname, )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelBools( 0 );" % (varname, )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelReals( 0 );" % (varname, )
        lines.append( s )
        s = (depth*indent) + "%s->setNumModelInts( 0 );" % (varname, )
        lines.append( s )
        if self.mUseMechForceReals:
            s = (depth*indent) + "%s->setUseMechForceReals( true );" % (varname, )
            lines.append( s )
            s = (depth*indent) + "%s->setIdxMechForceReals( %s_MECH_REAL_FORCE_X, %s_MECH_REAL_FORCE_Y, %s_MECH_REAL_FORCE_Z );" % (varname, self.mEnumToken, self.mEnumToken, self.mEnumToken, )
            lines.append( s )
        else:
            s = (depth*indent) + "%s->setUseMechForceReals( false );" % (varname, )
            lines.append( s )

        lines.append( (depth*indent) + "gAgentSpecies.push_back( %s );" % (varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getClassName(self):
        return self.mClassName

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitAreas( self ):
        return self.mInitAreas

    def getEnums( self, indent, depth ):
        self.updateUseMechForceReals(  )
        lines = [ ]
        s = self.getMechForceRealsEnum( indent, depth )
        lines.append( s )
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
        if( ( self.paramExists( "shoveLimit" ) and self.getParam( "shoveLimit" ).getValue() > 0.0 ) or
            ( self.paramExists( "shoveFactor" ) and self.getParam( "shoveFactor" ).getValue() > 0.0 ) or          
            ( self.paramExists( "shoveScale" ) and self.getParam( "shoveScale" ).getValue() > 0.0 ) ):
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
        s += additional
        s += "</species>"
        return s
        
    def __str__(self):
        return self.toString( "" )
        return s

    def __repr__(self):
        return str(self)


class AgentSpeciesActive(AgentSpecies):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Active"
        AgentSpecies.__init__(self, class_name, name)
        
        self.mParticles = ItemHolder( AgentSpeciesParticle )
        print("FIXME: <reaction name='name' status='active' /> not yet parsed.")
        print("FIXME: <molecules></molecules> not yet parsed.")
        print("FIXME: <agentMolecularReactions></agentMolecularReactions> not yet parsed.")
        return

    def getParticles( self ):
        return self.mParticles

    def toString( self, additional ):
        additional += str( self.mParticles )
        return AgentSpecies.toString( self, additional )

class AgentSpeciesLocated(AgentSpeciesActive):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Located"
        AgentSpeciesActive.__init__(self, class_name, name)
        self.addParam( Param( "divRadius", "um",  0.97) )
        self.addParam( Param( "divRadiusCV", "float",  0.1) )
        self.addParam( Param( "deathRadius", "um",  0.2) )
        self.addParam( Param( "deathRadiusCV", "float",  0.1) )
        self.addParam( Param( "babyMassFrac", "um",  0.5) )
        self.addParam( Param( "babyMassFracCV", "float",  0.1) )
        self.addParam( Param( "shoveLimit", "um",  0.0) )    # addition to desired radius
        self.addParam( Param( "shoveFactor", "um",  1.15) )  # listed as um/length, but treated as radius scalar
        self.addParam( Param( "shoveScale", "float",  1.0) ) # biocellion-biomodel only
        self.addParam( Param( "fixed", "bool",  False) )
        self.addParam( Param( "noSkinBottomLayerBoundary", "int", 0 ) )
        print("FIXME: <tightJunctions> not yet parsed <tightJunction withSpecies='name' stiffness='value' />")
        print("FIXME: <adhesions> not yet parsed <adhesion withSpecies='name' strength='value' />")
        return

class AgentSpeciesBacterium(AgentSpeciesLocated):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Bacterium"
        AgentSpeciesLocated.__init__(self, class_name, name)
        self.addParam( Param( "epsMax", "float", 0.15 ) )
        self.addParam( Param( "epsColor", "str", "lightGray" ) )
        print("FIXME: <particle name='capsule'> <particle name='inert'> special names not parsed for Bacterium.")
        return

class AgentSpeciesBactEPS(AgentSpeciesBacterium):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "BactEPS"
        AgentSpeciesBacterium.__init__(self, class_name, name)
        self.addParam( Param( "kHyd", "hr-1", 0.007 ) )
        return

class AgentSpeciesYeast(AgentSpeciesBactEPS):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Yeast"
        AgentSpeciesBactEPS.__init__(self, class_name, name)
        self.addParam( Param( "useActivationInhibition", "bool", False ) )
        self.addParam( Param( "neighborhoodRadiusCoefficient", "float", 2.5 ) )
        self.addParam( Param( "startingTimeActivationInhibition", "int", 0 ) )
        print("FIXME: <entryConditions> not yet parsed <entryCondition type='type' name='name'><switch/><fromSpecies/> </entryCondition>")
        print("FIXME: <switchingLags> not yet parsed")
        print("FIXME: <chemotaxis> not yet parsed")
        return


class AllAgentSpecies:

    def __init__(self):
        self.mAgentSpecies = {}
        self.mOrder = []
        return

    def getSpecies( self, name ):
        return self.mAgentSpecies[ name ]

    def addSpecies( self, class_name, name, species=None ):
        if species is None:
            if class_name == "Species":
                species = AgentSpecies(class_name, name)
            elif class_name == "Active":
                species = AgentSpeciesActive(class_name, name)
            elif class_name == "Located":
                species = AgentSpeciesLocated(class_name, name)
            elif class_name == "Bacterium":
                species = AgentSpeciesBacterium(class_name, name)
            elif class_name == "BactEPS":
                species = AgentSpeciesBactEPS(class_name, name)
            elif class_name == "Yeast":
                species = AgentSpeciesYeast(class_name, name)
            else:
                species = None
                print("Unknown species class: %s" % (class_name, ))
                print("Needs to be implemented")
                return False
                
        self.mAgentSpecies[ name ] = species
        self.mOrder.append( name )
        return True

    def getLastSpecies( self ):
        return self.mAgentSpecies[ self.mOrder[ len( self.mOrder ) - 1 ] ]

    def getBioModelH(self, indent, depth):
        s = ""
        s += self.getSpeciesParamNames(indent, depth)
        s += "\n"
        s += "\n"
        s += self.getSpeciesEnum(indent, depth)
        s += "\n"
        s += "\n"
        s += self.getSpeciesSpecificEnums(indent, depth)
        s += "\n"
        return s

    def getSpeciesParamNames(self, indent, depth):
        all_params = { }
        for name in self.mOrder:
            params = self.mAgentSpecies[ name ].getParams()
            for param_name in params:
                all_params[ param_name ] = params[ param_name ]
        lines = []
        for n in all_params:
            s = (depth*indent) + "const std::string %s = \"%s\";" % (all_params[ n ].getConstName(), n, )
            lines.append( s )
        return "\n".join( lines )
        
    def getSpeciesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _agent_species_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mAgentSpecies[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_AGENT_SPECIES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} agent_species_type_e;" )
        return "\n".join( lines )

    def getSpeciesSpecificEnums(self, indent, depth):
        lines = [ ]
        for name in self.mOrder:
            s = self.mAgentSpecies[ name ].getEnums( indent, depth )
            lines.append( s );
        return "\n".join( lines )
    
    def getInitializeBioModel(self, indent, depth):
        s = ""
        for name in self.mOrder:
            s += self.mAgentSpecies[ name ].toCpp( indent, depth )
        return s

    def __str__( self ):
        s = "<ALL_AGENT_SPECIES>\n"

        for name in self.mOrder:
            s += str( self.mAgentSpecies[ name ] )

        s += "</ALL_AGENT_SPECIES>"
        return s

    def __repr__( self ):
        return str( self )


def main():
    print( "FIXME: no tester" )
    return
    
if __name__ == "__main__":
    main()
