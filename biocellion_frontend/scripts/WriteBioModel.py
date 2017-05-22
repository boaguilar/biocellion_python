import os, sys, re

def write_biomodel_h( biomodel, directory, source_directory ):
    fout = open(directory+"/biomodel.h", 'w')
    fin  = open(source_directory+'/template/biomodel.h', 'r')

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
    fout = open(directory+"/biomodel.cpp", 'w')
    fin  = open(source_directory+'/template/biomodel.cpp', 'r')

    for line in fin:
        fout.write(line)
        if re.search(r'AGENT SPECIES AUTO BEGIN', line):
            lines = biomodel.getInitializeBioModel( "  ", 1 )
            fout.write(lines)
            fout.write("\n")
    fin.close()
    fout.close()
    return

def write_agent_species_h( biomodel, directory, source_directory ):
    fout = open(directory+"/agent_species.h", 'w')
    fin  = open(source_directory+'/template/agent_species.h', 'r')

    for line in fin:
        fout.write(line)
        
    fin.close()
    fout.close()
    return

def write_agent_species_cpp( biomodel, directory, source_directory ):
    fout = open(directory+"/agent_species.cpp", 'w')
    fin  = open(source_directory+'/template/agent_species.cpp', 'r')

    for line in fin:
        fout.write(line)
        
    fin.close()
    fout.close()
    return

def write_model_mechanisms_h( biomodel, directory, source_directory ):
    fout = open(directory+"/model_mechanisms.h", 'w')
    fin  = open(source_directory+'/template/model_mechanisms.h', 'r')

    for line in fin:
        fout.write(line)
        
    fin.close()
    fout.close()
    return

def write_model_mechanisms_cpp( biomodel, directory, source_directory ):
    fout = open(directory+"/model_mechanisms.cpp", 'w')
    fin  = open(source_directory+'/template/model_mechanisms.cpp', 'r')

    for line in fin:
        fout.write(line)
        
    fin.close()
    fout.close()
    return

def write_biomodel_files( biomodel, directory, source_directory ):
    write_biomodel_h( biomodel, directory, source_directory )
    write_biomodel_cpp( biomodel, directory, source_directory )
    write_agent_species_h( biomodel, directory, source_directory )
    write_agent_species_cpp( biomodel, directory, source_directory )
    write_model_mechanisms_h( biomodel, directory, source_directory )
    write_model_mechanisms_cpp( biomodel, directory, source_directory )
    return
