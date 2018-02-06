import biocell
import os

def create_custom_cpp_model( xml_file, cpp_dir, build_dir ):
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
