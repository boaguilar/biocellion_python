from biocell import *

#####################################################
# Param
#####################################################
class Param:
    
    mAllUnits = AllUnits()
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
    
    mRealUnits = ( "float", "m2.m-3" ) + mAllUnits.getUnitStrings( )
    mIntUnits = ( "int" )
    mBoolUnits = ( "bool" )
    
    def __init__( self, name, unit, default_value, required=False, prefix="" ):
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
