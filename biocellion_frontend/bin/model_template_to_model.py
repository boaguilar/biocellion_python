#!/usr/bin/env python3
import sys, os, getopt
import biocell

TEMPLATE_FILE = 'template-file'
XML_FILE = 'xml-file'
PARMS = 'parms'
MODEL = 'model'
MODEL_DIR = 'model-dir'

def translate_template( args ):

    if not os.path.exists( args[ TEMPLATE_FILE ] ):
        raise Exception( "Input file doesn't exist: %s" % ( args[ TEMPLATE_FILE ], ) )
    if os.path.exists( args[ XML_FILE ] ):
        raise Exception( "Output file already exists: %s" % ( args[ XML_FILE ], ) )
    
    # creating the translator, completes the translation
    p = biocell.XMLProcessor( args[ TEMPLATE_FILE ], args[ XML_FILE ], args[ PARMS ] )
    return


def reassign_paths( args ):
    args[ TEMPLATE_FILE ] = os.path.join( args[ MODEL_DIR] , args[ MODEL ] + '-template.xml' )
    args[ XML_FILE ] = os.path.join( args[ MODEL_DIR] , args[ MODEL ] + '.xml' )
    return


def usage( argv, args ):
    print( "usage: %s -p var=val -t template-file -x xml-file -m model-name -d model-dir" % (argv[0], ) )
    print( "-p | --parm parm=value           # name and value of parameter for the model; may be specified more than once; default: %s" % ( str( args[ PARMS ] ) , ) )
    print( "-t | --template-file file-name   # name of input model template XML file; default: %s" % (args[ TEMPLATE_FILE ], ) )
    print( "-x | --xml-file file-name        # name of output model XML file; default: %s" % (args[ XML_FILE ], ) )
    print( "-m | --model-name model-name     # descriptor for model; default: %s" % (args[ MODEL ], ) )
    print( "-d | --model-dir model-dir       # location of template and model files; default: %s" % (args[ MODEL_DIR ], ) )
    print( "-h | -? | --help                 # show this message and quit" )
    print( "Use -t and -x OR -m and -d.  Don't mix them." ) 
    print( argv )
    return

def read_args( argv, args ):
    args[ PARMS ] = { }
    args[ MODEL_DIR ] = "examples"
    args[ MODEL ] = "sorting"
    reassign_paths( args )
    
    try:
        short_opts = "p:t:x:m:d:h?"
        long_opts = [ "parm=", "template-file=", "xml-file=", "model-name=", "model-dir=", "help" ]

        do_help = False
        opts, r_args = getopt.getopt( argv[1:], short_opts, long_opts )
        for o, a in opts:
            if o in ('-t', '--template-file'):
                args[ TEMPLATE_FILE ] = a
            elif o in ('-x', '--xml-file'):
                args[ XML_FILE ] = a
            elif o in ('-m', '--model-name'):
                args[ MODEL ] = a
                reassign_paths( args )
            elif o in ('-d', '--model-dir'):
                args[ MODEL_DIR ] = a
                reassign_paths( args )
            elif o in ('-p', '--parm'):
                var, val = a.split( '=' )
                var = var.strip( )
                val = float( val.strip( ) )
                args[ 'parms' ][ var ] = val
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
        
    except ValueError as err:
        print( "Parameters must have float values" )
        print( str( err ) )
        usage( argv, args )
        sys.exit( 1 )

    return

def main( argv ):

    args = { }
    read_args( argv, args )
    translate_template( args )

    return

if __name__ == "__main__":
    main( sys.argv )

