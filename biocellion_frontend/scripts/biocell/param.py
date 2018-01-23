from biocell import *

def guessStringTypeAndConvert( string_value, unit_string ):

    if unit_string is None or unit_string in Param.mIntUnits:
        try:
            value = int(string_value)
            if unit_string is None:
                unit_string = 'int'
        except:
            pass
                
    if unit_string is None or unit_string in Param.mRealUnits or Param.mAllUnits.isValidUnit( unit_string ):
        try:
            value = float(string_value)
            if unit_string is None:
                unit_string = 'float'
        except:
            pass
        
    if unit_string is None or unit_string in Param.mBoolUnits:
        try:
            string_value = str(string_value)
            if string_value.lower() == "true":
                value = True
                if unit_string is None:
                    unit_string = 'bool'
            elif string_value.lower() == "false":
                value = False
                if unit_string is None:
                    unit_string = 'bool'
            else:
                pass
        except:
            pass

    if unit_string is None or unit_string in Param.mStringUnits:
        try:
            value = str(string_value)
            if unit_string is None:
                unit_string = 'str'
        except:
            pass

    try:
        return value, unit_string
    except UnboundLocalError as e:
        print( "------------------------" )
        print( "guessStringTypeAndConvert( '" + str( string_value ) + "', '" + str( unit_string ) + "' ) failed to match unit." )
        print( "Perhaps " + str( string_value ) + " is not properly formatted for " + str( unit_string) + "?" )
        print( "Perhaps you need to add " + str( unit_string ) + " to unit.py?" )
        print( "------------------------" )
        print( "" )
        raise e
        return 0, ""

    

def convertToStandardUnit( value, unit ):
    found = False
    for u in Param.mAllUnits.getUnits( ):
        if u.contains( unit ):
            value = u.convertToStandard( unit, value )
            unit = u.getStandardUnit( )
            found = True
            break
                
    if unit in Param.mStringUnits:
        unit = Param.mStringStandardUnit
        found = True
    elif unit in Param.mIntUnits:
        found = True
    elif unit in Param.mBoolUnits:
        found = True
    elif unit in Param.mRealUnits:
        found = True
    elif Param.mAllUnits.isValidUnit( unit ):
        value, unit = Param.mAllUnits.convertToStandardUnit( value, unit )
        found = True
        
    if not found:
        raise BadUnit( unit, value )

    return value, unit

#####################################################
# Param
#####################################################
class Param:
    
    mAllUnits = AllUnits()
    mStringUnits = ( "str", "string" )
    mStringStandardUnit = "str"
    
    def toStandardUnit( self ):
        self.mValue, self.mUnit = convertToStandardUnit( self.mValue, self.mUnit )
        return
    
    mRealUnits = ( "float", "m2.m-3" ) + mAllUnits.getUnitStrings( )
    mIntUnits = ( "int" )
    mBoolUnits = ( "bool" )
    
    def __init__( self, name, unit, default_value, required=False, prefix="", allowed_values=None ):
        # infer type from format of default_value
        default_value, unit = guessStringTypeAndConvert( default_value, unit )

        self.mName = str( name )
        self.mPrefix = str( prefix )
        self.mUnit = str( unit )
        self.mDefaultValue = default_value
        self.mValue = default_value
        self.mRequired = required
        self.mAllowedValues = allowed_values

        self.toStandardUnit()
        return

    def copy(self):
        p = Param( self.mName, self.mUnit, self.mDefaultValue, self.mRequired )
        p.setValueFromParam( self )
        p.setPrefix( self.mPrefix )
        if isinstance( self.mAllowedValues, ( list, tuple ) ):
            p.mAllowedValues = self.mAllowedValues[ : ]
        else:
            p.mAllowedValues = self.mAllowedValues
        return p

    def getName(self):
        return self.mName
    def setName( self, name ):
        self.mName = name
        return
    def getPrefix( self ):
        return self.mPrefix
    def setPrefix( self, prefix ):
        self.mPrefix = prefix
        return
    def getConstName( self ):
        return "%s_%s" % ( self.mPrefix, self.mName )
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
    def isValueAllowed( self, value ):
        if self.mAllowedValues is None:
            return True
        if self.mUnit in self.mStringUnits:
            if isinstance( self.mAllowedValues, ( list, tuple ) ):
                return value in self.mAllowedValues
            elif isinstance( self.mAllowedValues, ( str ) ):
                return value == self.mAllowedValues
        return False

    # other is another Param object
    def setValueFromParam(self, other):
        if self.getUnit() != other.getUnit():
            return False
        return self.setValue( other.getValue() )


    def validateUnit( self, other ):
        return self.getUnit() == other.getUnit()

    def validateAllowedValue( self, other ):
        return self.isValueAllowed( other.getValue( ) )
        
    def validateParam(self, other):
        if not self.validateUnit( other ):
            ##
            ## This occurs naturally while scanning XML files, due to unit detection on attributes and unit-less params
            ## print( "Param ( " + str( self.getName( ) ) + " ) unit ( " + str( self.getUnit( ) ) + " ) does not match expected units ( " + str( other.getUnit( ) ) + " )" )
            ##
            return False
        if not self.validateAllowedValue( other ):
            return False
        return True

    def checkUnit(self, desired_unit):
        if self.getUnit() != desired_unit:
            raise Exception( 'ERROR: ' + str( self.mName ) + ' unit should be ' + desired_unit + ' but is actually ' + str( self.getUnit() ) )
            return False
        return True


    def getMayAttributes( self ):
        names = [ "name", "unit", ]
        return names
        
    def getRequiredAttributes( self ):
        names = [ "name", ]
        return names
        
    def getMayParams( self ):
        names = [ ]
        return names
        
    def getRequiredParams( self ):
        names = [ ]
        return names

    def __str__(self):
        s  = '<param name="%s" unit="%s">%s</param>' % (str(self.mName), str(self.mUnit), str(self.mValue))
        if self.mAllowedValues is not None:
            s += '<!-- '
            s += 'allowed values: [ '
            for v in self.mAllowedValues:
                s += str( v ) + ", "
            s += ' ] -->'
        return s

    def __repr__(self):
        return str(self)


def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
