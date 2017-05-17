import os
import sys 
from BiocellionParam  import diffusible_solutes, cell_types, domain_parameters, mechanical_parameters, multigrid_solver_parm, basic_simulation_param 

def write_biocell_otherfiles( diffusibles, celltypes, myreactions, myforces, mydomain, mygridsolver, mysimulator, directory, source_directory ):

 # write info of the agents
 #agentf  = open(directory+"/model_routine_agent.cpp", 'w')
 #input_agentf   = open(source_directory+'/template/model_routine_agent.cpp', 'r')
 #for line in input_agentf:
 #    agentf.write(line)
   
 
 # write info of the grid
 #gridf  = open(directory+"/model_routine_grid.cpp", 'w')
 #input_gridf   = open(source_directory+'/template/model_routine_grid.cpp', 'r')
 #for line in input_gridf:
 #    gridf.write(line)
   
  
 # write info of the agents
 outputf  = open(directory+"/model_routine_output.cpp", 'w')
 input_outputf   = open(source_directory+'/template/model_routine_output.cpp', 'r')
 for line in input_outputf:
     outputf.write(line)
   

 # write info of the agents
 mechf  = open(directory+"/model_routine_mech_intrct.cpp", 'w')
 input_mechf   = open(source_directory+'/template/model_routine_mech_intrct.cpp', 'r')
 for line in input_mechf:
     mechf.write(line)
   
 # write info of the agents
 makef  = open(directory+"/Makefile", 'w')
 input_makef   = open(source_directory+'/template/Makefile', 'r')
 for line in input_makef:
     makef.write(line)
   
