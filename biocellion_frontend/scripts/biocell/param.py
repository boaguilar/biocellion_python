from biocell import *

def guessStringTypeAndConvert( string_value, unit_string ):
    if unit_string is None or unit_string in Param.mIntUnits:
        try:
            value = int(string_value)
            if unit_string is None:
                unit_string = 'int'
        except:
            pass
                
    if unit_string is None or unit_string in Param.mRealUnits:
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

    return value, unit_string
    

def convertToStandardUnit( value, unit ):
    for u in Param.mAllUnits.getUnits( ):
        if u.contains( unit ):
            value = u.convertToStandard( unit, value )
            unit = u.getStandardUnit( )
                
    if unit in Param.mStringUnits:
        unit = Param.mStringStandardUnit
        
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
    
    def __init__( self, name, unit, default_value, required=False, prefix="" ):
        # infer type from format of default_value
        default_value, unit = guessStringTypeAndConvert( default_value, unit )

        self.mName = str( name )
        self.mPrefix = str( prefix )
        self.mUnit = str( unit )
        self.mDefaultValue = default_value
        self.mValue = default_value
        self.mRequired = required

        self.toStandardUnit()
        return

    def copy(self):
        p = Param( self.mName, self.mUnit, self.mDefaultValue, self.mRequired )
        p.setValueFromParam( self )
        p.setPrefix( self.mPrefix )
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

    # other is another Param object
    def setValueFromParam(self, other):
        if self.getUnit() != other.getUnit():
            return False
        return self.setValue( other.getValue() )

    def validateParam(self, other):
        if self.getUnit() != other.getUnit():
            ##
            ## This occurs naturally while scanning XML files, due to unit detection on attributes and unit-less params
            ## print( "Param ( " + str( self.getName( ) ) + " ) unit ( " + str( self.getUnit( ) ) + " ) does not match expected units ( " + str( other.getUnit( ) ) + " )" )
            ##
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
        s = '<param name="%s" unit="%s">%s</param>' % (str(self.mName), str(self.mUnit), str(self.mValue))
        return s

    def __repr__(self):
        return str(self)


def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
