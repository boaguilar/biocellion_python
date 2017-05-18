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

DEP_HEADERS := model_define.h model_mechanisms.h agent_species.h biomodel.h

libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so: interface_config.o interface_agent.o interface_mech_intrct.o interface_grid.o interface_output.o model_routine_config.o model_routine_grid.o model_routine_agent.o model_routine_mech_intrct.o model_routine_output.o interface_check.o model_mechanisms.o agent_species.o biomodel.o
	$(CXX) -shared $(LINKFLAG) -Wl,-soname,libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so -o libmodel${PRECISION}${SUPPORT_MP}${SUPPORT_SP}.so interface_config.o interface_agent.o interface_mech_intrct.o interface_grid.o interface_output.o model_routine_config.o model_routine_grid.o model_routine_agent.o model_routine_mech_intrct.o model_routine_output.o interface_check.o model_mechanisms.o agent_species.o biomodel.o

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

model_mechanisms.o: model_mechanisms.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

agent_species.o: agent_species.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

biomodel.o: biomodel.cpp $(DEP_HEADERS)
	$(CXX) $(CXXFLAG) $(INCLUDE) -c $< -o $@

all:
	make

clean:
	$(RM) *.ilo
	$(RM) *.o
	$(RM) *.so

clean_all:
	$(RM) *.ilo
	$(RM) *.o
	$(RM) *.so

