#!/usr/bin/python
import os
import sys
import getopt

g_short_opts = "m:o:h?"
g_long_opts = [ "--model-file=", "--output-dir=", "--help" ]
g_model_file = './examples/QS_Growth_2D.xml'
g_output_dir = '/home/baguilar/biocellion-user/QS_Growth_2D'
g_source_dir = "/".join(sys.argv[0].split("/")[0:-1])

def usage(argv):
    print( "usage: %s -m model-file -o output-dir" % (argv[0], ) )
    print( "-m | --model-file file-path # location of XML model file; default: %s" % (g_model_file, ) )
    print( "-o | --output-dir dir-path  # location to create the model source code; default: %s" % (g_output_dir, ) )
    print( "-h | -? | --help            # show this message and quit" )
    print(argv)
    sys.exit(1)
    return

def read_args( ):
    global g_model_file, g_output_dir
    
    try:
        do_help = False
        opts, args = getopt.getopt(sys.argv[1:], g_short_opts, g_long_opts)
        for o, a in opts:
            if o in ('-m', '--model-file'):
                g_model_file = a
            elif o in ('-o', '--output-dir'):
                g_output_dir = a
            elif o in ('-h', '-?', '--help'):
                do_help = True
            else:
                print("Unhandled option: '%s:%s'" % (o, a))
                do_help = True
        if do_help:
            usage(sys.argv)
            sys.exit(1)

    except getopt.GetoptError as err:
        print( str(err) )
        usage(sys.argv)
        sys.exit(1)

def main( ):
    read_args( )
    sys.path.insert( 0, g_source_dir + '/scripts' )
    import biocell

    if not os.path.exists( g_output_dir ):
        os.makedirs( g_output_dir )
    if not os.path.exists( g_output_dir + '/output' ):
        os.makedirs( g_output_dir + '/output' )

    biomodel = biocell.BioModel()
    scanner = biocell.ModelScanner( biomodel )
    scanner.parseXML( g_model_file )
    writer = biocell.ModelWriter( )
    writer.write_biomodel_files( biomodel, g_output_dir, g_source_dir )
    return

if __name__ == "__main__":
    main( )
