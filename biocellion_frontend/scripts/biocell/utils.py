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

