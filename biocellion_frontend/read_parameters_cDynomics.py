#!/usr/bin/python
import os
import sys
import getopt
import xml.etree.ElementTree as ET

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
    print(str(err))
    usage(sys.argv)
    sys.exit(1)

sys.path.insert(0,g_source_dir+'/scripts')

from xml.etree.ElementTree import Element, SubElement, Comment
from ElementTree_pretty import prettify
from BiocellionParam  import diffusible_solutes, cell_types, domain_parameters, mechanical_parameters, multigrid_solver_parm, basic_simulation_param, create_e_perturbations 
from ReadcDynomics import read_xml
from WriteBcellHeader import write_biocell_header
from WriteBcellConfig import write_biocell_config
from WriteBcellAgent import write_biocell_agent
from WriteBcellGrid import write_biocell_grid
from WriteBcellXML import write_bcell_xml  
from WriteBcellOtherFiles import write_biocell_otherfiles  

from WriteBioModel import write_biomodel_files
from agent_species import AllAgentSpecies

## read parameters from input file.
xmlfilename = g_model_file
dirname = g_output_dir


if not os.path.exists(dirname):
    os.makedirs(dirname)

##if not os.path.exists(dirname+'/model'):
##    os.makedirs( dirname +'/model' )

if not os.path.exists(dirname+'/output'):
    os.makedirs( dirname +'/output' )


diffusibles = dict()  # list of solutes (names)
celltypes = dict() # list of cell types (names)
agent_species = AllAgentSpecies()
myreactions = dict() # list of reactions  
eperturbations = dict() # list of perturbations
myforces = []  # parameters for force computations
mydomain = domain_parameters() # parameters of the domain 
mygridsolver =  multigrid_solver_parm() # parameters of the multigrid solver
mysimulator = basic_simulation_param() # timings and other basic arameters of simulation



# read xml from cDynomics
read_xml(diffusibles, celltypes, agent_species, myreactions, myforces, eperturbations, mydomain, mygridsolver, mysimulator, xmlfilename, dirname )

print " -----  Cell Types -------------- "
print  celltypes
print " -----  Reactions --------------- "
print myreactions
print " -----  Solutes ----------------- "
print diffusibles 
print " -----  Forces ------------------ "
print myforces 
print "------  E Perturbation ---------- "
print eperturbations 

# 2D case
if ( mydomain['nDim'] == 2 ) :
   if ( mydomain['nz'] == 1 ) :
       mydomain['nz'] = 4
       
if ( mydomain['nz'] < 8 ) :
    mydomain['nz'] = 8
   

write_biocell_header(diffusibles, celltypes, myreactions, myforces, eperturbations, mydomain, mygridsolver, mysimulator, dirname, g_source_dir )
write_biocell_config(diffusibles, celltypes, myreactions, myforces, mydomain, mygridsolver, mysimulator, dirname, g_source_dir )
write_biocell_agent(diffusibles, celltypes, myreactions, myforces, mydomain, mygridsolver, mysimulator, dirname, g_source_dir )
write_biocell_grid(diffusibles, celltypes, myreactions, myforces, mydomain, mygridsolver, mysimulator, dirname, g_source_dir )
write_bcell_xml(diffusibles, celltypes, myreactions, myforces, mydomain,mygridsolver, mysimulator, dirname, g_source_dir )
write_biocell_otherfiles(diffusibles, celltypes, myreactions, myforces, mydomain, mygridsolver, mysimulator, dirname, g_source_dir )
write_biomodel_files( agent_species, dirname, g_source_dir )

