class AgentSpeciesParam:

    mRealUnits = ( "float", "um", "hr-1" )
    mIntUnits = ( "int" )
    mBoolUnits = ( "bool" )
    mStringUnits = ( "str" )
    
    def __init__(self, name, unit, default_value):
        # infer type from format of default_value
        if unit is None:
            try:
                int(default_value)
                unit = 'int'
            except:
                pass
                
        if unit is None:
            try:
                float(default_value)
                unit = 'float'
            except:
                pass
        
        if unit is None:
            try:
                str(default_value)
                unit = 'str'
            except:
                pass

        self.mName = str(name)
        self.mConstName = "SP_%s" % (name, )
        self.mUnit = str(unit)
        self.mDefaultValue = default_value
        self.mValue = default_value
        return

    def copy(self):
        p = AgentSpeciesParam(self.mName, self.mUnit, self.mDefaultValue)
        p.setValue(self.mValue)
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

    # other is another AgentSpeciesParam object
    def setValueFromParam(self, other):
        if self.getUnit() != other.getUnit():
            return False
        return self.setValue(other.getValue())

    def __str__(self):
        s = "%s %s %s" % (str(self.mName), str(self.mValue), str(self.mUnit))
        return s

    def __repr__(self):
        return str(self)

class AgentSpecies:

    def __init__(self, class_name, name):
        self.mClassName = class_name
        self.mName = name
        self.mEnumToken = "AGENT_SPECIES_%s_%s" % (class_name, name)
        self.mParams = {}
        self.addParam( AgentSpeciesParam( "initialMassCV", "float", 0.1 ) )
        self.addParam( AgentSpeciesParam( "color", "str", "white" ) )
        self.addParam( AgentSpeciesParam( "tightJunctionToBoundaryStrength", "float", 0.0 ) )
        self.addParam( AgentSpeciesParam( "attachToBoundaryCreateFactor", "float", 1.1 ) )
        self.addParam( AgentSpeciesParam( "attachToBoundaryDestroyFactor", "float" , 2.0 ) )
        self.addParam( AgentSpeciesParam( "attachCreateFactor", "float", 1.1 ) )
        self.addParam( AgentSpeciesParam( "attachDestroyFactor", "float", 2.0 ) )
        self.addParam( AgentSpeciesParam( "DoNotRandomizeMassOnCreation", "bool", False ) )
        self.addParam( AgentSpeciesParam( "visible", "bool", True ) )
        self.addParam( AgentSpeciesParam( "computationDomain", "str", "" ) )
        return

    def countUnits(self, units):
        count = 0
        for n in self.mParams:
            param = self.mParams[ n ]
            if param.getUnit() in units:
                count += 1
        return count
        
    def countReal(self):
        return self.countUnits( AgentSpeciesParam.mRealUnits )
        
    def countInt(self):
        return self.countUnits( AgentSpeciesParam.mIntUnits )
        
    def countBool(self):
        return self.countUnits( AgentSpeciesParam.mBoolUnits )
        
    def countString(self):
        return self.countUnits( AgentSpeciesParam.mStringUnits )
        
    def paramNamesToCpp(self, indent, depth):
        lines = []
        for n in self.mParams:
            s = (depth*indent) + "const std::string %s = \"%s\";" % (self.mParams[ n ].getConstName(), n, )
            lines.append( s )
        return "\n".join( lines )
        
    def toCpp(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "AgentSpecies *species = new AgentSpecies( \"%s\", \"%s\", %s, %d, %d, %d, %d );" % (self.mName, self.mClassName, self.mEnumToken, self.countReal(), self.countInt(), self.countBool(), self.countString()) )
        for n in self.mParams:
            param = self.mParams[ n ]
            if param.getUnit() in AgentSpeciesParam.mStringUnits:
                s = (depth*indent) + "species->setParamString( species->getIdxString( %s ), \"%s\" );" % (param.getConstName(), param.getValue(), )
                lines.append( s )
            elif param.getUnit() in AgentSpeciesParam.mBoolUnits:
                s = (depth*indent) + "species->setParamBool( species->getIdxBool( %s ), %s );" % (param.getConstName(), "true" if param.getValue() else "false", )
                lines.append( s )
            elif param.getUnit() in AgentSpeciesParam.mIntUnits:
                s = (depth*indent) + "species->setParamInt( species->getIdxInt( %s ), %s );" % (param.getConstName(), param.getValue(), )
                lines.append( s )
            elif param.getUnit() in AgentSpeciesParam.mRealUnits:
                s = (depth*indent) + "species->setParamReal( species->getIdxReal( %s ), %s );" % (param.getConstName(), param.getValue(), )
                lines.append( s )
            else:
                s = "FIXME: Unknown unit: " + str(param.getUnit())
                lines.append( s )
        lines.append( (depth*indent) + "gAgentSpecies.push_back( species );" )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getClassName(self):
        return self.mClassName

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getParams(self):
        return self.mParams

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
            if (self.mParams[n].getUnit() in AgentSpeciesParam.mRealUnits) and (param.getUnit() in AgentSpeciesParam.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                param = param.copy()
                param.setUnit( self.mParams[n].getUnit() )
                return self.mParams[n].setValueFromParam(param)
            else:
                return False
        else:
            return False

    def __str__(self):
        s  = "%s %s %s:\n" % (self.mClassName, self.mName, self.mEnumToken)
        for p in self.mParams:
            s += "\t" + str(self.mParams[p]) + "\n"
        return s

    def __repr__(self):
        return str(self)


class AgentSpeciesActive(AgentSpecies):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Active"
        AgentSpecies.__init__(self, class_name, name)
        # FIXME: Need to parse particle information
        print("FIXME: <particle name='name'><param name='mass' unit='fg'>value</param></particle> not yet parsed.")
        print("FIXME: <reaction name='name' status='active' /> not yet parsed.")
        print("FIXME: <molecules></molecules> not yet parsed.")
        print("FIXME: <agentMolecularReactions></agentMolecularReactions> not yet parsed.")

        return

class AgentSpeciesLocated(AgentSpeciesActive):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Located"
        AgentSpeciesActive.__init__(self, class_name, name)
        self.addParam( AgentSpeciesParam( "divRadius", "um",  0.97) )
        self.addParam( AgentSpeciesParam( "divRadiusCV", "float",  0.1) )
        self.addParam( AgentSpeciesParam( "deathRadius", "um",  0.2) )
        self.addParam( AgentSpeciesParam( "deathRadiusCV", "float",  0.1) )
        self.addParam( AgentSpeciesParam( "babyMassFrac", "um",  0.5) )
        self.addParam( AgentSpeciesParam( "babyMassFracCV", "float",  0.1) )
        self.addParam( AgentSpeciesParam( "shoveLimit", "um",  0.0) )
        self.addParam( AgentSpeciesParam( "shoveFactor", "um",  1.15) )
        self.addParam( AgentSpeciesParam( "fixed", "bool",  False) )
        self.addParam( AgentSpeciesParam( "noSkinBottomLayerBoundary", "int", 0 ) )
        print("FIXME: <tightJunctions> not yet parsed <tightJunction withSpecies='name' stiffness='value' />")
        print("FIXME: <adhesions> not yet parsed <adhesion withSpecies='name' strength='value' />")
        return

class AgentSpeciesBacterium(AgentSpeciesLocated):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Bacterium"
        AgentSpeciesLocated.__init__(self, class_name, name)
        self.addParam( AgentSpeciesParam( "epsMax", "float", 0.15 ) )
        self.addParam( AgentSpeciesParam( "epsColor", "str", "lightGray" ) )
        print("FIXME: <particle name='capsule'> <particle name='inert'> special names not parsed for Bacterium.")
        return

class AgentSpeciesBactEPS(AgentSpeciesBacterium):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "BactEPS"
        AgentSpeciesBacterium.__init__(self, class_name, name)
        self.addParam( AgentSpeciesParam( "kHyd", "hr-1", 0.007 ) )
        return

class AgentSpeciesYeast(AgentSpeciesBactEPS):
    
    def __init__(self, class_name="", name=""):
        if not class_name: class_name = "Yeast"
        AgentSpeciesBactEPS.__init__(self, class_name, name)
        self.addParam( AgentSpeciesParam( "useActivationInhibition", "bool", False ) )
        self.addParam( AgentSpeciesParam( "neighborhoodRadiusCoefficient", "float", 2.5 ) )
        self.addParam( AgentSpeciesParam( "startingTimeActivationInhibition", "int", 0 ) )
        print("FIXME: <entryConditions> not yet parsed <entryCondition type='type' name='name'><switch/><fromSpecies/> </entryCondition>")
        print("FIXME: <switchingLags> not yet parsed")
        print("FIXME: <chemotaxis> not yet parsed")
        return


class AllAgentSpecies:

    def __init__(self):
        self.mAgentSpecies = {}
        self.mOrder = []
        return

    def getSpecies(self, name):
        return self.mAgentSpecies[ name ]

    def addSpecies(self, class_name, name):
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

    def getBioModelH(self, indent, depth):
        s = ""
        s += self.getSpeciesParamNames(indent, depth)
        s += "\n"
        s += "\n"
        s += self.getSpeciesEnum(indent, depth)
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

    def getInitializeBioModel(self, indent, depth):
        s = ""
        for name in self.mOrder:
            s += self.mAgentSpecies[ name ].toCpp( indent, depth )
        return s


def main():
    s = create_species("Yeast", "MovingCells")
    print(s)
    return
    
if __name__ == "__main__":
    main()
