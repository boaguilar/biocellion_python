from biocell import *

def get_type_of( a ):
    if type( a ).__name__ == 'instance':
        ta = a.__class__.__name__
    elif type( a ).__name__ == 'classobj':
        ta = a.__name__
    else:
        raise Exception( "ERROR: Unable to find type of object: " + str( a ) )
    return ta

def same_type( a, b ):
    ta = get_type_of( a )
    tb = get_type_of( b )
    return ta == tb

class GenericException( Exception ):
    def __init__( self , msg ):
        msg = "ERROR: " + msg
        Exception.__init__( self, msg )
        return

class BadItemType( Exception ):
    def __init__( self , tag, item_name ):
        msg = "ERROR: Can not find list of " + str( tag ) + " items looking for/adding item " + str( item_name ) + "."
        Exception.__init__( self, msg )
        return

class BadItemName( Exception ):
    def __init__( self , tag, item_name ):
        msg = "ERROR: Can not find " + str( item_name ) + " in list of " + str( tag ) + " items."
        Exception.__init__( self, msg )
        return
