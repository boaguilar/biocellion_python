from biocell import *

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
            raise Exception( "ERROR: unknown unit: " + str( unit ) )
            return 0.0
        return value * self.mConversions[ unit ]

class AllUnits:

    def __init__( self ):
        self.mUnits = [ ]
        # yield mass/mass
        self.addUnit( Unit( "g.g-1", { "g.g-1": 1.0,
        } ) )
        # diffusion area/time
        self.addUnit( Unit( "um2.hr-1", { "um2.hr-1": 1.0,
                                          "um2.hour-1": 1.0,
                                          "m2.day-1": 1.0e6*1.0e6/24.0,
        } ) )
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
        # length
        self.addUnit( Unit( "um", { "um": 1.0,
                                    "pm": 1e6,
                                    "nm": 1e3,
                                    "mm": 1e-3,
                                    "cm": 1e-4,
                                    "m": 1e-6,
        } ) )
        # time
        self.addUnit( Unit( "hr", { "h": 1.0,
                                    "hr": 1.0,
                                    "hour": 1.0,
                                    "s": 1.0/(60.*60.),
                                    "sec": 1.0/(60.*60.),
                                    "second": 1.0/(60.*60.),
        } ) )
        # rate
        self.addUnit( Unit( "hr-1", { "h-1": 1.0,
                                      "hr-1": 1.0,
                                      "hour-1": 1.0,
                                      "s-1": (60.*60.),
                                      "sec-1": (60.*60.),
                                      "second-1": (60.*60.),
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
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
