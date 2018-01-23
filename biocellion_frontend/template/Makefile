include ../Makefile.model

ifeq ("$(REAL)", "float")
PRECISION = .SP
else
PRECISION = .DP
endif
ifeq ("$(MPAGENT)", "yes")
SUPPORT_MP = .MPAGENT
else
SUPPORT_MP =
endif
ifeq ("$(SPAGENT)", "yes")
SUPPORT_SP = .SPAGENT
else
SUPPORT_SP =
endif

INCLUDE += -I $(BIOCELLION_ROOT)/include
INCLUDE += -I $(BIOCELLION_ROOT)/libmodel/include

CXXFLAG += -fPIC

DEP_HEADERS := model_define.h model_mechanisms.h agent_species.h biomodel.h particle.h reaction.h simulator.h agent_grid.h tight_junction.h init_area.h adhesion.h distance_junction.h solute.h param_holder.h computation_domain.h bulk.h agar.h world.h chemotaxis.h biomodel_auto.h entry_condition.h solver.h molecule.h ode_network.h interaction.h biomodel_debug.h
BIOMODEL_OBJECTS := biomodel_auto.o agent_species.o biomodel.o model_mechanisms.o particle.o reaction.o simulator.o agent_grid.o tight_junction.o init_area.o adhesion.o distance_junction.o solute.o param_holder.o computation_domain.o bulk.o agar.o world.o chemotaxis.o entry_condition.o solver.o molecule.o ode_network.o interaction.o

libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so: interface_config.o interface_agent.o interface_mech_intrct.o interface_grid.o interface_output.o model_routine_config.o model_routine_grid.o model_routine_agent.o model_routine_mech_intrct.o model_routine_output.o interface_check.o $(BIOMODEL_OBJECTS)
	$(CXX) -shared $(LINKFLAG) -Wl,-soname,libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so -o libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so interface_config.o interface_agent.o interface_mech_intrct.o interface_grid.o interface_output.o model_routine_config.o model_routine_grid.o model_routine_agent.o model_routine_mech_intrct.o model_routine_output.o interface_check.o $(BIOMODEL_OBJECTS)

interface_config.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_config.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_config.cpp -o interface_config.o

interface_agent.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_agent.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_agent.cpp -o interface_agent.o

interface_mech_intrct.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_mech_intrct.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_mech_intrct.cpp -o interface_mech_intrct.o

interface_grid.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_grid.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_grid.cpp -o interface_grid.o

interface_output.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_output.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_output.cpp -o interface_output.o

interface_check.o: $(BIOCELLION_ROOT)/libmodel/interface/interface_check.cpp
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $(BIOCELLION_ROOT)/libmodel/interface/interface_check.cpp -o interface_check.o

model_routine_config.o: model_routine_config.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

model_routine_grid.o: model_routine_grid.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

model_routine_agent.o: model_routine_agent.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

model_routine_mech_intrct.o: model_routine_mech_intrct.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

model_routine_output.o: model_routine_output.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

$(BIOMODEL_OBJECTS) : %.o : %.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

all:
	make

clean:
	$(RM) *.ilo
	$(RM) *.o
	$(RM) *.so
	$(RM) *~

clean_all:
	$(RM) *.ilo
	$(RM) *.o
	$(RM) *.so

run:
	make all
	chmod +x ./RUN.bash
	./RUN.bash "$(BIOCELLION_ROOT)" "$(DEBUG)"

