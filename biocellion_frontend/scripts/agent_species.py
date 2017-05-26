import sys
#####################################################
# Unit
#####################################################
class Unit:

    def __init__( self, standard_unit, conversions={} ):
        self.mStandardUnit = standard_unit
        self.mConversions = { }
        self.mAllUnits = ( )
        self.addConversion( standard_unit, 1.0 )
        for name in conversions:
            self.addConversion( name, conversions[ name ] )
        return

    def getStandardUnit( self ):
        return self.mStandardUnit

    def getAllUnits( self ):
        return self.mAllUnits

    def updateAllUnits( self ):
        self.mAllUnits = tuple( self.mConversions )
        return
    
    def addConversion( self, unit, factor ):
        self.mConversions[ unit ] = factor
        self.updateAllUnits( )
        return

    def contains( self, unit ):
        return unit in self.mAllUnits

    def convertToStandard( self, unit, value ):
        if not self.contains( unit ):
            sys.exit( "ERROR: unknown unit: " + str( unit ) )
            return 0.0
        return value * self.mConversions[ unit ]

class AllUnits:

    def __init__( self ):
        self.mUnits = [ ]
        # density
        self.addUnit( Unit( "pg.um-3", { "pg.um-3": 1.0,
                                         "g.L-1": 1.0e12 / 1.0e15,
        } ) )
        # volume
        self.addUnit( Unit( "um3", { "um3": 1.0,
                                     "m3": 1.0e18,
                                     "L": 1.0e15,
        } ) )
        # mass
        self.addUnit( Unit( "pg", { "pg": 1.0,
                                    "g": 1.0e12,
                                    "gram": 1.0e12,
                                    "mg": 1.0e9,
                                    "ug": 1.0e6,
                                    "ng": 1.0e3,
                                    "fg": 1.0e-3,
                                    "ag": 1.0e-6,
        } ) )
        # time
        self.addUnit( Unit( "hr", { "h": 1.0,
                                    "hr": 1.0,
                                    "hour": 1.0,
                                    "s": 1.0/(60.*60.),
                                    "sec": 1.0/(60.*60.),
                                    "second": 1.0/(60.*60.),
        } ) )
        
        return

    def addUnit( self, unit ):
        self.mUnits.append( unit )
        return

    def getUnits( self ):
        return self.mUnits

    def getUnitStrings( self ):
        units = ( )
        for u in self.mUnits:
            units = units + u.getAllUnits( )
        return units
    
    
#####################################################
# Param
#####################################################
class Param:
    
    mAllUnits = AllUnits()
    # mDensityConvert = { "pg.um-3": 1.0,
    #                     "g.L-1": 1.0e12 / 1.0e15,
    #                   }
    # mDensityStandardUnit = "pg.um-3"
    # mDensityUnits = tuple( mDensityConvert )

    # mVolumeConvert = { "um3": 1.0,
    #                    "m3": 1.0e18,
    #                    "L": 1.0e15,
    #                  }
    # mVolumeStandardUnit = "um3"
    # mVolumeUnits = tuple( mVolumeConvert )

    # mMassConvert = { "pg": 1.0,
    #                  "g": 1.0e12,
    #                  "gram": 1.0e12,
    #                  "mg": 1.0e9,
    #                  "ug": 1.0e6,
    #                  "ng": 1.0e3,
    #                  "fg": 1.0e-3,
    #                  "ag": 1.0e-6,
    #                }
    # mMassStandardUnit = "pg"
    # mMassUnits = tuple( mMassConvert )

    # mTimeConvert = { "h": 1.0,
    #                  "hr": 1.0,
    #                  "hour": 1.0,
    #                  "s": 1.0/(60.*60.),
    #                  "sec": 1.0/(60.*60.),
    #                  "second": 1.0/(60.*60.),
    #                }
    # mTimeStandardUnit = "hr"
    # mTimeUnits = tuple( mTimeConvert )
    
    mStringUnits = ( "str", "string" )
    mStringStandardUnit = "str"
    
    def toStandardUnit( self ):
        for u in self.mAllUnits.getUnits( ):
            if u.contains( self.mUnit ):
                self.mValue = u.convertToStandard( self.mUnit, self.mValue )
                self.mUnit = u.getStandardUnit( )
                
        if self.mUnit in self.mStringUnits:
            self.mUnit = self.mStringStandardUnit
        return
    
    mRealUnits = ( "float", "um", "hr-1", "g.L-1", "m2.m-3" ) + mAllUnits.getUnitStrings( )
    mIntUnits = ( "int" )
    mBoolUnits = ( "bool" )
    
    def __init__( self, name, unit, default_value, required=False ):
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
        self.mRequired = required

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
    def isRequired( self ):
        return self.mRequired
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

    def checkUnit(self, desired_unit):
        if self.getUnit() != desired_unit:
            sys.exit( 'ERROR: ' + str( self.mName ) + ' unit should be ' + desired_unit + ' but is actually ' + str( self.getUnit() ) )
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

    def getMayAttributes( self ):
        names = [ ]
        for n in self.mAttributes:
            names.append( n )
        return names
        
    def getRequiredAttributes( self ):
        names = [ ]
        for n in self.mAttributes:
            if self.mAttributes[ n ].isRequired( ):
                names.append( n )
        return names
        
    def getMayParams( self ):
        names = [ ]
        for n in self.mParams:
            names.append( n )
        return names
        
    def getRequiredParams( self ):
        names = [ ]
        for n in self.mParams:
            if self.mParams[ n ].isRequired( ):
                names.append( n )
        return names
        
    def paramNamesToCpp(self, indent, depth):
        lines = []
        for n in self.mParams:
            if n in self.mHiddenParams:
                continue
            s = (depth*indent) + "const std::string %s = \"%s\";" % (self.mParams[ n ].getConstName(), n, )
            lines.append( s )
        return "\n".join( lines )
        
    def getInitializeBioModel(self, varname, indent, depth):
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

    def getParamOrAttribute( self, name ):
        if name in self.mParams:
            return self.mParams[ name ]
        elif name in self.mAttributes:
            return self.mAttributes[ name ]
        return None

    def getInitializeBioModelSetDataMembers( self, varname, connector, indent, depth, bools, numbers, strings ):
        lines = [ ]

        for name in bools:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( %s );" % (varname, connector, Name, "true" if self.getParamOrAttribute( name ).getValue() else "false", )
            lines.append( s )
            
        for name in numbers:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( %s );" % (varname, connector, Name, self.getParamOrAttribute( name ).getValue(), )
            lines.append( s )
            
        for name in strings:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( \"%s\" );" % (varname, connector, Name, self.getParamOrAttribute( name ).getValue(), )
            lines.append( s )
        
        return "\n".join( lines )

    def getAttributes(self):
        return self.mAttributes

    def getAttribute( self, attr ):
        return self.mAttributes[ attr ]

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

    def getKeys( self ):
        return self.mOrder

    def hasKey( self, name ):
        return name in self.mOrder

    def getItem( self, name ):
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

    def __len__( self ):
        return len( self.mItems )

    def __getitem__( self, idx ):
        return self.mItems[ self.mOrder[ idx ] ]

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
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "mass", "pg", 0.0, True ) )
        self.mParticle = None
        return

    def setParticle( self, particle ):
        self.mParticle = particle

    def getParticleEnumToken( self ):
        if self.mParticle is None:
            sys.exit( "ERROR: <species><particle> must be linked to <idynomics><particle>" )
        return self.mParticle.getEnumToken( )

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
        self.addAttribute( Param( "rows", "int", 0, True ) )
        self.addAttribute( Param( "cols", "int", 0, True ) )
        self.addAttribute( Param( "bars", "int", 0, False ) )
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
        self.addAttribute( Param( "x", "um", 0, True ) )
        self.addAttribute( Param( "y", "um", 0, True ) )
        self.addAttribute( Param( "z", "um", 0, True ) )
        self.addAttribute( Param( "r", "um", 0, False ) )
        self.mPrivateNumberHiddenParams = [ "x", "y", "z", "r" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getInitializeBioModel( self, container_name, indent, depth ):
        varname = "coordinates"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "Coordinates *%s = new Coordinates( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
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
        self.addAttribute( Param( "number", "float", 0, True ) )
        self.addAttribute( Param( "shape", "str", "unfilledBlock", False ) )
        self.addParam( Param( "birthday", "hour", 0.0, True ) )
        self.mCoordinates = ItemHolder( Coordinates )
        self.mBlocks = ItemHolder( Blocks )


        self.mPrivateNumberHiddenParams = [ "number", "birthday" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "shape" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getCoordinates( self ):
        return self.mCoordinates

    def getBlocks( self ):
        return self.mBlocks

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "init_area"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "InitArea *%s = new InitArea( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = (depth*indent) + "%s%sset%s( %s );" % (varname, "->", "AgentSpecies", parent_varname, )
        lines.append( s )
            
        coordinate_container_name = "%s->getCoordinates()" % ( varname, )
        for i in range( len( self.mCoordinates ) ):
            lines.append( self.mCoordinates[ i ].getInitializeBioModel( coordinate_container_name, indent, depth ) )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
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
# Adhesion
#####################################################
class Adhesion( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "strength", "float", 0, True ) )
        self.addAttribute( Param( "withSpecies", "str", "", True ) )
        self.addAttribute( Param( "scale", "float", 1.0, False ) )

        self.mPrivateNumberHiddenParams = [ "strength", "withSpecies", "scale" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "adhesion"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "Adhesion *%s = new Adhesion( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<adhesion" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</adhesion>\n"
        return s

    def __repr__(self):
        return str(self)

#####################################################
# DistanceJunction
#####################################################
class DistanceJunction( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "enabled", "bool", False, True ) )
        self.addAttribute( Param( "withSpecies", "str", "", True ) )

        self.mPrivateNumberHiddenParams = [ "withSpecies", ]
        self.mPrivateBoolHiddenParams = [ "enabled", ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "junction"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "DistanceJunction *%s = new DistanceJunction( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def __str__(self):
        s  = "<distanceJunction" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</distanceJunction>\n"
        return s

    def __repr__(self):
        return str(self)


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

    def createDistanceJunctions( self ):
        # Create all necessary distance junctions
        # If adhesion is in play, contact inhibition may be needed, so create distance junctions
        for key in self.mAdhesions.getKeys( ):
            item = self.getAdhesions( ).getItem( key )

            junction = DistanceJunction( )
            node_attr = Param( "enabled", None, "true" )
            if not junction.validateAttribute( node_attr ):
                sys.exit( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                sys.exit( "ERROR : Failed to create DistanceJunction" )

            node_attr = Param( "withSpecies", None, item.getAttribute( "withSpecies" ).getValue( ) )
            if not junction.validateAttribute( node_attr ):
                sys.exit( "ERROR : Failed to create DistanceJunction" )
            if not junction.updateAttribute( node_attr ):
                sys.exit( "ERROR : Failed to create DistanceJunction" )

            self.mDistanceJunctions.addItem( None, junction )

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
        # Mech Forces used by shove, adhesion, tightJunction
        trigger_names = [ "shoveLimit", "shoveFactor", "shoveScale" ]
        update = False
        for name in trigger_names:
            if self.paramExists( name ) and self.getParam( name ).getValue() > 0.0:
                update = True
                break
        if len( self.mAdhesions ) > 0:
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
        print("FIXME: <reaction name='name' status='active' /> not yet parsed.")
        print("FIXME: <molecules></molecules> not yet parsed.")
        print("FIXME: <agentMolecularReactions></agentMolecularReactions> not yet parsed.")
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
        return "\n".join( lines )

    def getParticles( self ):
        return self.mParticles

    def toString( self, additional ):
        additional += str( self.mParticles )
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
        self.addParam( Param( "brownianScale", "float",  1.0) ) # biocellion-biomodel only
        self.addParam( Param( "fixed", "bool",  False) )
        self.addParam( Param( "noSkinBottomLayerBoundary", "int", 0 ) )
        print("FIXME: <tightJunctions> not yet parsed <tightJunction withSpecies='name' stiffness='value' />")
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
        print("FIXME: <entryConditions> not yet parsed <entryCondition type='type' name='name'><switch/><fromSpecies/> </entryCondition>")
        print("FIXME: <switchingLags> not yet parsed")
        print("FIXME: <chemotaxis> not yet parsed")
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
        lines.append( self.getSpeciesParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSpeciesEnum( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSpeciesBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getSpeciesParamNames(self, indent, depth):
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
            s = (depth*indent) + "const std::string %s = \"%s\";" % (all_params[ n ].getConstName(), n, )
            lines.append( s )
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
    print( "FIXME: no tester" )
    return
    
if __name__ == "__main__":
    main()
