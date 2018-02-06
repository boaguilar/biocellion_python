import biocell
import os

def create_xml_model_from_xml_model_template( template_file, xml_file, parms ):
    """
    Requires: 
    template_file = input template xml file name
    xml_file = output xml file name
    parms = dictionary where key == VAR_key pattern to replace and value = replacement value.
    """

    if not os.path.exists( template_file ):
        raise Exception( "Input file doesn't exist: %s" % ( template_file, ) )
    if os.path.exists( xml_file ):
        raise Exception( "Output file already exists: %s" % ( xml_file, ) )
    
    # creating the translator completes the translation
    p = biocell.XMLProcessor( template_file, xml_file, parms )
    return



def create_cpp_model_from_xml_model( xml_file, cpp_dir, build_dir ):
    """
    Requires: 
    xml_file = input xml model file name
    cpp_dir = input directory for cpp files
    build_dir = output model directory for cpp files
    """

    if not os.path.exists( xml_file ):
        raise Exception( "Input file doesn't exist: %s" % ( xml_file, ) )
    if not os.path.exists( cpp_dir ):
        raise Exception( "Input directory doesn't exist: %s" % ( cpp_dir, ) )
    if os.path.exists( build_dir ):
        raise Exception( "Output dir already exists: %s" % ( build_dir, ) )
    else:
        os.makedirs( build_dir )

    biomodel = biocell.BioModel( )
    scanner = biocell.ModelScanner( biomodel )
    scanner.parseXML( xml_file )
    writer = biocell.ModelWriter( )
    writer.write_biomodel_files( biomodel, build_dir, cpp_dir )
    
    return

def compile_cpp_model( build_dir ):
    """
    Requires: 
    build_dir = directory containing cpp files to be built
    """

    if not os.path.exists( build_dir ):
        raise Exception( "C++ directory does not exist: %s" % ( build_dir, ) )

    cwd = os.getcwd( )
    os.chdir( build_dir )
    
    cmd = "make -j"
    c = biocell.Command( cmd )
    c.set_timeout( 120 )
    c.run_command( )
    if c.get_exit_code() != 0:
        os.chdir( cwd )
        print( "-----------------------" )
        for line in c.get_lines( ):
            print( line )
        print( "-----------------------" )
        raise Exception( cmd + " failed in " + build_dir )

    os.chdir( cwd )

    return

def run_compiled_model( build_dir, cmd_timeout ):
    """
    Requires: 
    build_dir = directory containing compiled library
    cmd_timeout = maximum amount of time for sub process to complete
    """

    if not os.path.exists( build_dir ):
        raise Exception( "Build directory does not exist: %s" % ( build_dir, ) )
    if not os.path.exists( os.path.join( build_dir, "libmodel.DP.SPAGENT.so" )  ):
        raise Exception( "Built library does not exist: %s" % ( build_dir, ) )

    cwd = os.getcwd( )
    os.chdir( build_dir )
    
    cmd = "make -j run"
    c = biocell.Command( cmd )
    c.set_timeout( int( cmd_timeout ) )
    c.run_command( )
    if c.get_exit_code() != 0:
        os.chdir( cwd )
        print( "-----------------------" )
        for line in c.get_lines( ):
            print( line )
        print( "-----------------------" )
        raise Exception( cmd + " failed in " + build_dir )

    os.chdir( cwd )

    return
