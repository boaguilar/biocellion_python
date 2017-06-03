import os, sys, re
from xml.etree.ElementTree import Element, SubElement, Comment
from ElementTree_pretty import prettify

def write_config_xml( biomodel, directory, source_directory ):
    filename = 'config.xml'
    name = directory + '/' + filename
    fout = open(name, 'w')
    if not fout:
        raise Exception( 'ERROR: unable to open ' + name )

    
    top = Element( 'biocellion' )
    txt = '''
      Biocellion has three main components.
      1) simulating individual discrete agent (single-particle agent and multi-particle agent) behavior,
      2) simulating mechanical interactions between discrete agent particles, and
      3) simulating interactions among discrete agents (and the surrounding environment) via diffusible molecules (modelled using PDEs).
      
      1) has two sub-components.
      1-1) simulating agent state changes and
      1-2) simulating birth and death of agents
  '''
    comment = Comment( txt )
    top.append( comment )

    ## required
    required = SubElement( top, 'required' )
    required.append( Comment( 'required, mandatory simulation configuration parameters, required' ) )
    
    ## required.timestep
    required_timestep = SubElement( required, 'time_step' )
    required_timestep.append( Comment( 'time_step, set the number of baseline time steps, required' ) )
    required_timestep.append( Comment( '  num_baseline: number of baseline (to simulate 1-2 and 2) time steps, required, integer, [1,)' ) )
    required_timestep.set( 'num_baseline', str( biomodel.getIDynoMiCS().getSimulator().getNumBaselineSteps() ) )

    ## required.domain
    required_domain = SubElement( required, 'domain' )
    required_domain.append( Comment( 'domain, required, parameters describing the simulation domain' ) )
    required_domain.append( Comment( '  x(, y, or z): simulation grid size in the x(, y, or z) direction' ) )
    required_domain.append( Comment( '  x(, y, or z) should be equal to or larger than 4 (minimum parition size)' ) )
    required_domain.append( Comment( '  x(, y, or z) should be positive integer multiple of refine_ratio^(interface grid level in AMR)' ) )
    required_domain.append( Comment( '  x(, y, or z) should be equal to or larger than refine_ratio^(interface grid level in AMR) * 2 (minimum box size)' ) )
    required_domain.append( Comment( '  x(, y, or z) is integer, [4,)' ) )
    x, y, z = biomodel.getIDynoMiCS().getWorld().getDomainXYZ()
    required_domain.set( 'x', str( x ) )
    required_domain.set( 'y', str( y ) )
    required_domain.set( 'z', str( z ) )

    ## required.init_data
    required_initdata = SubElement( required, 'init_data' )
    required_initdata.append( Comment( 'init_data, required, parameters related to initial cell and grid data, required' ) )
    required_initdata.append( Comment( '  partition_size: partition size, required' ) )
    required_initdata.append( Comment( '    partition_size should be equal to or larger than 4 (minimum parition size)' ) )
    required_initdata.append( Comment( '    partition_size should be positive integer multiple of refine_ratio^(interface grid level in AMR)' ) )
    required_initdata.append( Comment( '    partition_size should be equal to or larger than refine_ratio^(interface grid level in AMR) * 2 (minimum box size)' ) )
    required_initdata.append( Comment( '    partition_size is integer, [4,)' ) )
    required_initdata.append( Comment( '  src: initial cell and grid data source, required' ) )
    required_initdata.append( Comment( '    src should be set to code to initialize inside the model code' ) )
    required_initdata.append( Comment( '    src should be set to file to read from files' ) )
    required_initdata.append( Comment( '    src is code|file' ) )
    required_initdata.append( Comment( '  file_type: valid and required only when src is set to file. input data file type' ) )
    required_initdata.append( Comment( '    file_type should be set to binary if input data files have the Biocellion binary format (currently only this option is supported)' ) )
    required_initdata.append( Comment( '    file_type is binary' ) )
    required_initdata.append( Comment( '  path: valid and required only when src is set to file.' ) )
    required_initdata.append( Comment( '    path is the path to the directory containing input data files' ) )
    required_initdata.append( Comment( '    path is text' ) )
    partition_size = biomodel.getIDynoMiCS().getWorld().getDomainPartitionSize()
    required_initdata.set( 'partition_size', str( partition_size ) )
    required_initdata.set( 'src', "code" )

    ## required.output
    required_output = SubElement( required, 'output' )
    required_output.append( Comment( 'output, parameters related to output data, required' ) )
    required_output.append( Comment( '  path: path to the directory to dump output data files, required, text' ) )
    required_output.append( Comment( '  interval: output interval (0: turn off, unit: baseline time steps), required, integer, [0,)' ) )
    required_output.append( Comment( '  particle: set to pvtp to output particle data using the parallel VTK unstructed grid format (currently only the pvtp format is supported), required, pvtp' ) )
    required_output.append( Comment( '  grid: set to vthb to output data using the VTK hierarchical box data format (currently only the vthb format is supported), required, vthb' ) )
    required_output.append( Comment( '  start_x(, y, or z): output box offset in the x(, y, or z) direction, must be an integer multiple of refine_ratio^(number of AMR levels - 1), required, integer, [1,)' ) )
    required_output.append( Comment( '  size_x(, y, or z): output box size in the x(, y, or z) direction, must be an integer multiple of refine_ratio^(number of AMR levels - 1), required, integer, [1,)' ) )
    
    required_output.set( 'path', "./output" )
    print( "FIXME: output interval should not be hard coded" )
    required_output.set( 'interval', "1" )
    required_output.set( 'particle', "pvtp" )
    required_output.set( 'grid', "vthb" )
    required_output.set( 'start_x', "0" )
    required_output.set( 'start_y', "0" )
    required_output.set( 'start_z', "0" )
    size_x, size_y, size_z = biomodel.getIDynoMiCS().getWorld().getDomainXYZ()
    required_output.set( 'size_x', str( size_x ) )
    required_output.set( 'size_y', str( size_y ) )
    required_output.set( 'size_z', str( size_z ) )
    
    ## optional
    optional= SubElement( top, 'optional' )
    required.append( Comment( 'optional, optional simulation configuration parameters, optional' ) )

    ## optional.model
    optional_model = SubElement( optional, 'model' )
    optional_model.append( Comment( 'model, to set model specific parameters, optional' ) )
    optional_model.append( Comment( '  param: saved to Info::modelParam to be used in Model::init(), optional, default="", text' ) )
    optional_model.set( 'param', "" )

    ## optional.stdout
    optional_stdout = SubElement( optional, 'stdout' )
    optional_stdout.append( Comment( 'stdout, control standard output, optional' ) )
    optional_stdout.append( Comment( '  path: if set, standard output messages are redirected to the output.log (or output.log.[process_id]) file under the specified path, optional, text' ) )
    optional_stdout.append( Comment( '  verbosity: controls the verbosity of standard output messages (5 for the highest level of verbosity), optional, default=1, [0,5]' ) )
    optional_stdout.append( Comment( '  time_stamp: if set to yes, a time stamp is inserted right before every output message, optional, default=no, yes|no' ) )
    optional_stdout.set( 'path', "" )
    optional_stdout.set( 'verbosity', "1" )
    optional_stdout.set( 'time_stamp', "yes" )

    ## optional.system
    optional_system = SubElement( optional, 'system' )
    optional_system.append( Comment( 'system, parameters related to shared and distributed memory parallelism, optional' ) )
    optional_system.append( Comment( '  num_threads: number of threads per MPI process, optional, integer, default=# cores (or hardware threads) in a node, [1,)' ) )
    optional_system.append( Comment( '  num_node_groups: number of node groups in a system, optional, integer, default=1, [1,)' ) )
    optional_system.append( Comment( '  num_nodes_per_group: number of nodes per node group, optional, default=# MPI processes, [1,)' ) )
    optional_system.append( Comment( '  num_sockets_per_node: number of NUMA sockets per node, this information is used for NUMA binding, set to 1 to turn-off NUMA binding, num_node_groups * num_nodes_per_group * num_sockets_per_node should coincide with # MPI processes, optional, default=1, [1,)' ) )
    # optional_system.set( 'num_threads', "16" )
    # optional_system.set( 'num_node_groups', "1" )
    # optional_system.set( 'num_nodes_per_group', "1" )
    # optional_system.set( 'num_sockets_per_node', "1" )

    ## optional.super_partition
    optional_superpartition = SubElement( optional, 'super_partition' )
    optional_superpartition.append( Comment( 'super_partition, sets super partition size' ) )
    optional_superpartition.append( Comment( '  x(, y, or z): super partition size in the direction of x(, y, or z)' ) )
    optional_superpartition.append( Comment( '  x(, y, or z): a larger value reduces the communication requirement between node groups at the expense of lower parallelism' ) )
    optional_superpartition.append( Comment( '  x(, y, or z): this number should be an integer multiple of partition_size' ) )
    optional_superpartition.append( Comment( '  x(, y, or z): optional, default=the smallest integer multiple of partition_size that is equal to or larger than x(, y, or z) (of domain), integer, [1,)' ) )
    supersize_x, supersize_y, supersize_z = biomodel.getIDynoMiCS().getWorld().getDomainXYZ()
    # optional_superpartition.set( 'x', str( supersize_x ) )
    # optional_superpartition.set( 'y', str( supersize_y ) )
    # optional_superpartition.set( 'z', str( supersize_z ) )

    ## optional.interval
    optional_interval = SubElement( optional, 'interval' )
    optional_interval.append( Comment( 'interval, controls the frequency of summary report, load balancing, regridding and checkpointing, optional' ) )
    optional_interval.append( Comment( '  summary: summary report interval (0: turn off, unit: baseline time steps), optional, default=0, integer, [0,)' ) )
    optional_interval.append( Comment( '  load_balance: load balancing interval (unit: baseline time steps), optional, integer, default=100, [1,)' ) )
    optional_interval.append( Comment( '  regridding: regridding interval (unit: baseline time steps), regridding should be equal to or smaller than load_balance and regridding should equally divide load_balance, optional, integer, default=100, [1,)' ) )
    optional_interval.append( Comment( '  checkpoint: checkpoint interval (0: turn off, unit: baseline time steps), optional, default=0, integer, [0,)' ) )
    optional_interval.set( 'summary', "1" )
    optional_interval.set( 'load_balance', "100" )
    optional_interval.set( 'regridding', "100" )
    optional_interval.set( 'checkpoint', "0" )

    ## optional.amr
    optional_amr = SubElement( optional, 'amr' )
    optional_amr.append( Comment( 'amr, controls adaptive mesh refinement' ) )
    optional_amr.append( Comment( 'refine_ratio: adaptive mesh refinement ratio between two consecutive levels, optional, default=2, 2|4|8' ) )
    optional_amr.append( Comment( 'fill_ratio: controls grid efficiency in mesh generation based on model provided tags, large value will generate less filled larger blocks, optional, default=0.5, [0.0,1.0]' ) )
    refine_ratio = biomodel.getIDynoMiCS( ).getSolutes( ).calcRefineRatio( )
    optional_amr.set( 'refine_ratio', str( refine_ratio ) )
    optional_amr.set( 'fill_ratio', "0.5" ) # FIXME: fill_ratio is hard-coded here.  Should add an XML parameter sometime

    
    fout.write( prettify(top) )
    fout.close()
    return


def write_biomodel_h( biomodel, directory, source_directory ):
    filename = 'biomodel.h'
    name = directory + '/' + filename
    fout = open(name, 'w')
    if not fout:
        raise Exception( 'ERROR: unable to open ' + name )
        
    name = source_directory + '/template/' + filename
    fin  = open(name, 'r')
    if not fin:
        raise Exception( 'ERROR: unable to open ' + name )
        
    for line in fin:
        fout.write(line)
        if re.search(r'AGENT SPECIES AUTO BEGIN', line):
            lines = biomodel.getBioModelH( "  ", 0 )
            fout.write(lines)
            fout.write("\n")
            
    fin.close()
    fout.close()
    return

def write_biomodel_cpp( biomodel, directory, source_directory ):
    filename = 'biomodel.cpp'
    name = directory + '/' + filename
    fout = open(name, 'w')
    if not fout:
        raise Exception( 'ERROR: unable to open ' + name )
        
    name = source_directory + '/template/' + filename
    fin  = open(name, 'r')
    if not fin:
        raise Exception( 'ERROR: unable to open ' + name )
        
    for line in fin:
        fout.write(line)
        if re.search(r'AGENT SPECIES AUTO BEGIN', line):
            lines = biomodel.getInitializeBioModel( "  ", 1 )
            fout.write(lines)
            fout.write("\n")
            
    fin.close()
    fout.close()
    return

def copy_file( directory, source_directory, filename ):
    name = directory + '/' + filename
    fout = open(name, 'w')
    if not fout:
        raise Exception( 'ERROR: unable to open ' + name )
        
    name = source_directory + '/template/' + filename
    fin  = open(name, 'r')
    if not fin:
        raise Exception( 'ERROR: unable to open ' + name )
        
    for line in fin:
        fout.write( line )
        
    fin.close()
    fout.close()
    return

def write_biomodel_files( biomodel, directory, source_directory ):
    names = [ 'adhesion.cpp', 'adhesion.h',
              'agar.cpp', 'agar.h',
              'agent_grid.cpp', 'agent_grid.h',
              'agent_species.cpp', 'agent_species.h',
              'bulk.cpp', 'bulk.h',
              'chemotaxis.cpp', 'chemotaxis.h',
              'computation_domain.cpp', 'computation_domain.h',
              'distance_junction.cpp', 'distance_junction.h',
              'init_area.cpp', 'init_area.h',
              'model_define.h',
              'model_mechanisms.cpp', 'model_mechanisms.h',
              'model_routine_agent.cpp', 
              'model_routine_config.cpp', 
              'model_routine_grid.cpp', 
              'model_routine_mech_intrct.cpp', 
              'model_routine_output.cpp', 
              'param_holder.cpp', 'param_holder.h',
              'particle.cpp', 'particle.h',
              'reaction.cpp', 'reaction.h',
              'simulator.cpp', 'simulator.h',
              'solute.cpp', 'solute.h',
              'tight_junction.cpp', 'tight_junction.h',
              'world.cpp', 'world.h',
              'Makefile', 
              'RUN.bash', 
              ]
    for name in names:
        copy_file( directory, source_directory, name )
        
    write_biomodel_h( biomodel, directory, source_directory )
    write_biomodel_cpp( biomodel, directory, source_directory )
    write_config_xml( biomodel, directory, source_directory )
    return
