#!/usr/bin/env python3
import sys, os, getopt
import biocell

TEMPLATE_FILE = 'template-file'
XML_FILE = 'xml-file'
PARMS = 'parms'
MODEL = 'model'
MODEL_DIR = 'model-dir'
BUILD_DIR = 'build-dir'
CPP_DIR   = 'cpp-dir'
CMD_TIMEOUT = 'cmd-timeout'

def compiled_to_data( args ):
    """
    Requires: 
    args[ BUILD_DIR ] = directory containing compiled library
    args[ CMD_TIMEOUT ] = maximum amount of time for sub process to complete
    """

    if not os.path.exists( args[ BUILD_DIR ] ):
        raise Exception( "Build directory does not exist: %s" % ( args[ BUILD_DIR ], ) )
    if not os.path.exists( os.path.join( args[ BUILD_DIR ], "libmodel.DP.SPAGENT.so" )  ):
        raise Exception( "Built library does not exist: %s" % ( args[ BUILD_DIR ], ) )

    cwd = os.getcwd( )
    os.chdir( args[ BUILD_DIR ] )
    
    cmd = "make -j run"
    c = biocell.Command( cmd )
    c.set_timeout( int( args[ CMD_TIMEOUT ] ) )
    c.run_command( )
    if c.get_exit_code() != 0:
        os.chdir( cwd )
        print( "-----------------------" )
        for line in c.get_lines( ):
            print( line )
        print( "-----------------------" )
        raise Exception( cmd + " failed in " + args[ BUILD_DIR ] )

    os.chdir( cwd )

    return

def usage( argv, args ):
    print( "usage: %s -b build-dir -t cmd-timeout" % (argv[0], ) )
    print( "-b | --build-dir build-dir       # directory containing compiled library to be run; default: %s" % (args[ BUILD_DIR ], ) )
    print( "-t | --command-timeout seconds   # seconds to let the subprocess run the simulation; default: %s" % (args[ CMD_TIMEOUT ], ) )
    print( "-h | -? | --help                 # show this message and quit" )
    print( argv )
    return

def read_args( argv, args ):
    args[ BUILD_DIR ] = "sorting_cpp"
    args[ CMD_TIMEOUT ] = 120
    #
    try:
        short_opts = "b:t:h?"
        long_opts = [ "build-dir=", "command-timeout=", "help" ]

        do_help = False
        opts, r_args = getopt.getopt( argv[1:], short_opts, long_opts )
        for o, a in opts:
            if o in ('-b', '--build-dir'):
                args[ BUILD_DIR ] = a
            elif o in ('-t', '--command-timeout'):
                args[ CMD_TIMEOUT ] = int( a )
            elif o in ('-h', '-?', '--help'):
                do_help = True
            else:
                print("Unhandled option: '%s:%s'" % (o, a))
                do_help = True
        if do_help:
            usage( argv, args )
            sys.exit( 1 )

    except getopt.GetoptError as err:
        print( str( err ) )
        usage( argv, args )
        sys.exit( 1 )
        
    return

def main( argv ):

    args = { }
    read_args( argv, args )
    compiled_to_data( args )

    return

if __name__ == "__main__":
    main( sys.argv )

