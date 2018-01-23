from biocell import *
import sys

def get_type_of( a ):
    if sys.version_info < ( 3, 0 ):
        if type( a ).__name__ == 'instance':
            ta = a.__class__.__name__
        elif type( a ).__name__ == 'classobj':
            ta = a.__name__
        else:
            msg  = "ERROR: Unable to find type of object: " + str( a ) + "\n"
            msg += "type( a ).__name__ = %s\n" % ( type( a ).__name__, )
            msg += "type( a ) = %s" % ( str( type( a ) ), )
            raise Exception( msg )
    else:
        import inspect
        if inspect.isclass( a ):
            ta = a.__name__
        else: # how do I specify a generic instance in Python3?
            ta = type( a ).__name__
        # else:
        #     msg  = "ERROR: Unable to find type of object: " + str( a ) + "\n"
        #     msg += "type( a ).__name__ = %s\n" % ( type( a ).__name__, )
        #     msg += "type( a ) = %s\n" % ( str( type( a ) ), )
        #     for m in inspect.getmembers( type( a ) ):
        #         msg += str( m ) + "\n"
        #     raise Exception( msg )

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
        msg = "ERROR: Can not find '" + str( item_name ) + "' in list of " + str( tag ) + " items."
        Exception.__init__( self, msg )
        return

class BadUnit( Exception ):
    def __init__( self , unit, value ):
        msg = "ERROR: Unknown unit specified '" + str( unit ) + "' for value '" + str( value ) + "'."
        Exception.__init__( self, msg )
        return
