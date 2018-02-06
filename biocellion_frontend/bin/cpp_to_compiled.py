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
    biocell.compile_cpp_model( args[ BUILD_DIR ] )

    return

if __name__ == "__main__":
    main( sys.argv )

