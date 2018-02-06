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

def reassign_paths( args ):
    args[ XML_FILE ] = os.path.join( args[ MODEL_DIR] , args[ MODEL ] + '.xml' )
    return


def usage( argv, args ):
    print( "usage: %s -c cpp-dir -x xml-file -m model-name -d model-dir -b build-dir" % (argv[0], ) )
    print( "-c | --cpp-dir cpp-dir           # location of cpp model library files; default: %s" % (args[ CPP_DIR ], ) )
    print( "-x | --xml-file file-name        # name of output model XML file; default: %s" % (args[ XML_FILE ], ) )
    print( "-m | --model-name model-name     # descriptor for model; default: %s" % (args[ MODEL ], ) )
    print( "-d | --model-dir model-dir       # location of template and model files; default: %s" % (args[ MODEL_DIR ], ) )
    print( "-b | --build-dir build-dir       # directory to create with c++ code; default: %s" % (args[ BUILD_DIR ], ) )
    print( "-h | -? | --help                 # show this message and quit" )
    print( "Use -x OR -m and -d.  Don't mix them." ) 
    print( argv )
    return

def read_args( argv, args ):
    args[ MODEL_DIR ] = "examples"
    args[ MODEL ] = "sorting"
    reassign_paths( args )
    args[ BUILD_DIR ] = "sorting_cpp"
    #
    parts = argv[ 0 ].split( "/" )[ 0 : -1 ]
    parts.append( "biocell_cpp" )
    if argv[ 0 ][ 0 ] == os.sep:
        args[ CPP_DIR ] = os.sep
    else:
        args[ CPP_DIR ] = ""
    for p in parts:
        args[ CPP_DIR ] = os.path.join( args[ CPP_DIR ], p )
    
    try:
        short_opts = "c:x:m:d:b:h?"
        long_opts = [ "cpp-dir=", "xml-file=", "model-name=", "model-dir=", "build-dir=", "help" ]

        do_help = False
        opts, r_args = getopt.getopt( argv[1:], short_opts, long_opts )
        for o, a in opts:
            if o in ('-c', '--cpp-dir'):
                args[ CPP_DIR ] = a
            elif o in ('-x', '--xml-file'):
                args[ XML_FILE ] = a
            elif o in ('-m', '--model-name'):
                args[ MODEL ] = a
                reassign_paths( args )
            elif o in ('-d', '--model-dir'):
                args[ MODEL_DIR ] = a
                reassign_paths( args )
            elif o in ('-b', '--build-dir'):
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
    biocell.create_cpp_model_from_xml_model( args[ XML_FILE ], args[ CPP_DIR ], args[ BUILD_DIR ] )

    return

if __name__ == "__main__":
    main( sys.argv )

