#!/usr/bin/env python3
import sys, os, getopt
import biocell

COMMAND_PROCESS_XML_TEMPLATE = 'command-process-xml-template'
COMMAND_PROCESS_XML_MODEL = 'command-process-xml-model'
COMMAND_COMPILE_CPP_MODEL = 'command-compile-cpp-model'
COMMAND_RUN_MODEL = 'command-run-model'
COMMAND_ALL_COMMANDS = 'command-all-commands'
TEMPLATE_FILE = 'template-file'
XML_FILE = 'xml-file'
PARMS = 'parms'
MODEL = 'model'
MODEL_DIR = 'model-dir'
BUILD_DIR = 'build-dir'
CPP_DIR   = 'cpp-dir'
CMD_TIMEOUT = 'cmd-timeout'

def reassign_paths( args ):
    args[ TEMPLATE_FILE ] = os.path.join( args[ MODEL_DIR] , args[ MODEL ] + '-template.xml' )
    args[ XML_FILE ] = os.path.join( args[ MODEL_DIR] , args[ MODEL ] + '.xml' )
    return

def usage( argv, args ):
    print( """
%s: run biological models described with XML using the biocellion agent based simulation system.
    The XML model description file may be specified directly via the --xml-file option, or
    it make be created from a template XML file using the --template-file option and repeated use
    of the --parm option.
""" % ( argv[ 0 ], ) )
    print( "usage: %s -A -T -X -C -R -p var=val -t template-file -x xml-file -m model-name -d model-dir -c cpp-dir -b build-dir -o cmd-timeout" % (argv[0], ) )
    print( )
    print( "Choose one or more commands." )
    print( "----------------------------" )
    print( "-A | --all-commands              # process all starting with template xml model file, ending with running model; default: %s" % ( str( args[ COMMAND_ALL_COMMANDS ] ) , ) )
    print( "-T | --process-xml-template      # process template xml model file into xml model file; default: %s" % ( str( args[ COMMAND_PROCESS_XML_TEMPLATE ] ) , ) )
    print( "-X | --process-xml-model         # process xml model file into cpp model files; default: %s" % ( str( args[ COMMAND_PROCESS_XML_MODEL ] ) , ) )
    print( "-C | --compile-cpp-model         # compile cpp model files into model library file; default: %s" % ( str( args[ COMMAND_COMPILE_CPP_MODEL ] ) , ) )
    print( "-R | --run-model                 # run the model; default: %s" % ( str( args[ COMMAND_RUN_MODEL ] ) , ) )
    print( )
    print( "Command options" )
    print( "---------------" )
    print( "XML specifications" )
    print( "-t | --template-file file-name   # name of input model template XML file; default: %s" % (args[ TEMPLATE_FILE ], ) )
    print( "-p | --parm parm=value           # name and value of parameter for the model; may be specified more than once; default: %s" % ( str( args[ PARMS ] ) , ) )
    print( "-x | --xml-file file-name        # name of output model XML file; default: %s" % (args[ XML_FILE ], ) )
    print( "-m | --model-name model-name     # descriptor for model; default: %s" % (args[ MODEL ], ) )
    print( "-d | --model-dir model-dir       # location of template and model files; default: %s" % (args[ MODEL_DIR ], ) )
    print( "Use -t and -x OR -m and -d.  Don't mix them." )
    print( "-c | --cpp-dir cpp-dir           # location of cpp model library files; default: %s" % (args[ CPP_DIR ], ) )
    print( "-b | --build-dir build-dir       # directory to create with c++ code; default: %s" % (args[ BUILD_DIR ], ) )
    print( "-o | --command-timeout seconds   # seconds to let the subprocess run the simulation; default: %s" % (args[ CMD_TIMEOUT ], ) )
    print( "-h | -? | --help                 # show this message and quit" )
    print( )
    print( "Arguments received: " + " ".join( argv ) )
    print( )
    return

def read_args( argv, args ):
    args[ COMMAND_PROCESS_XML_TEMPLATE ] = False
    args[ COMMAND_PROCESS_XML_MODEL ] = False
    args[ COMMAND_COMPILE_CPP_MODEL ] = False
    args[ COMMAND_RUN_MODEL ] = False
    args[ COMMAND_ALL_COMMANDS ] = False
    args[ CMD_TIMEOUT ] = 120
    args[ PARMS ] = { }
    args[ MODEL_DIR ] = "examples"
    args[ MODEL ] = "sorting"
    reassign_paths( args )
    args[ BUILD_DIR ] = "sorting_cpp"

    parts = argv[ 0 ].split( "/" )[ 0 : -1 ]
    parts.append( "biocell_cpp" )
    if argv[ 0 ][ 0 ] == os.sep:
        args[ CPP_DIR ] = os.sep
    else:
        args[ CPP_DIR ] = ""
    for p in parts:
        args[ CPP_DIR ] = os.path.join( args[ CPP_DIR ], p )
    
    
    try:
        short_opts = "ATXCRp:t:x:m:d:c:b:o:h?"
        long_opts = [ "all-commands", "process-xml-template", "process-xml-model", "compile-cpp-model", "run-model", "parm=", "template-file=", "xml-file=", "model-name=", "model-dir=", "cpp-dir=", "build-dir=", "command-timeout=", "help" ]

        do_help = False
        opts, r_args = getopt.getopt( argv[1:], short_opts, long_opts )
        for o, a in opts:
            if o in ('-A', '--all-commands'):
                args[ COMMAND_ALL_COMMANDS ] = True
            elif o in ('-T', '--process-xml-template'):
                args[ COMMAND_PROCESS_XML_TEMPLATE ] = True
            elif o in ('-X', '--process-xml-model'):
                args[ COMMAND_PROCESS_XML_MODEL ] = True
            elif o in ('-C', '--compile-cpp-model'):
                args[ COMMAND_COMPILE_CPP_MODEL ] = True
            elif o in ('-R', '--run-model'):
                args[ COMMAND_RUN_MODEL ] = True
            elif o in ('-t', '--template-file'):
                args[ TEMPLATE_FILE ] = a
            elif o in ('-x', '--xml-file'):
                args[ XML_FILE ] = a
            elif o in ('-m', '--model-name'):
                args[ MODEL ] = a
                reassign_paths( args )
            elif o in ('-d', '--model-dir'):
                args[ MODEL_DIR ] = a
                reassign_paths( args )
            elif o in ('-c', '--cpp-dir'):
                args[ CPP_DIR ] = a
            elif o in ('-b', '--build-dir'):
                args[ BUILD_DIR ] = a
            elif o in ('-o', '--command-timeout'):
                args[ CMD_TIMEOUT ] = int( a )
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
    
    if args[ COMMAND_ALL_COMMANDS ]:
        biocell.create_xml_model_from_xml_model_template( args[ TEMPLATE_FILE ], args[ XML_FILE ], args[ PARMS ] )
        biocell.create_cpp_model_from_xml_model( args[ XML_FILE ], args[ CPP_DIR ], args[ BUILD_DIR ] )
        biocell.compile_cpp_model( args[ BUILD_DIR ] )
        biocell.run_compiled_model( args[ BUILD_DIR ], args[ CMD_TIMEOUT ] )
    else:
        if args[ COMMAND_PROCESS_XML_TEMPLATE ]:
            biocell.create_xml_model_from_xml_model_template( args[ TEMPLATE_FILE ], args[ XML_FILE ], args[ PARMS ] )
        if args[ COMMAND_PROCESS_XML_MODEL ]:
            biocell.create_cpp_model_from_xml_model( args[ XML_FILE ], args[ CPP_DIR ], args[ BUILD_DIR ] )
        if args[ COMMAND_COMPILE_CPP_MODEL ]:
            biocell.compile_cpp_model( args[ BUILD_DIR ] )
        if args[ COMMAND_RUN_MODEL ]:
            biocell.run_compiled_model( args[ BUILD_DIR ], args[ CMD_TIMEOUT ] )
    return

if __name__ == "__main__":
    main( sys.argv )
