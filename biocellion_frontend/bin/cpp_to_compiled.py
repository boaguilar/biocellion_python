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

def cpp_to_compiled( args ):
    """
    Requires: 
    args[ BUILD_DIR ] = directory containing cpp files to be built
    """

    if not os.path.exists( args[ BUILD_DIR ] ):
        raise Exception( "C++ directory does not exist: %s" % ( args[ BUILD_DIR ], ) )

    cwd = os.getcwd( )
    os.chdir( args[ BUILD_DIR ] )
    
    cmd = "make"
    c = biocell.Command( cmd )
    c.set_timeout( 120 )
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
    print( "usage: %s -b build-dir" % (argv[0], ) )
    print( "-b | --build-dir build-dir       # directory containing C++ code to be built; default: %s" % (args[ BUILD_DIR ], ) )
    print( "-h | -? | --help                 # show this message and quit" )
    print( argv )
    return

def read_args( argv, args ):
    args[ BUILD_DIR ] = "sorting_cpp"
    #
    try:
        short_opts = "b:h?"
        long_opts = [ "build-dir=", "help" ]

        do_help = False
        opts, r_args = getopt.getopt( argv[1:], short_opts, long_opts )
        for o, a in opts:
            if o in ('-b', '--build-dir'):
                args[ BUILD_DIR ] = a
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
    cpp_to_compiled( args )

    return

if __name__ == "__main__":
    main( sys.argv )

