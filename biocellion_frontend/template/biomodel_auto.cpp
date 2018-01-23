#include "biomodel.h"

void initializeBioModelAuto( ) {
  {
    gBioModel = gBioModelRW = new BioModel( );
    gBioModelRW->setDistanceJunctionsEnabled( true );
  }
  {

    gBioModelRW->getSimulator( ).setRestartPreviousRun( false );
    gBioModelRW->getSimulator( ).setChemostat( false );
    gBioModelRW->getSimulator( ).setDiffusionReactionOnAgentTime( false );
    gBioModelRW->getSimulator( ).setRandomSeed( 5515 );
    gBioModelRW->getSimulator( ).setOutputPeriod( 0.01 );
    gBioModelRW->getSimulator( ).setAgentTimeStep( 0.01 );
    {

      gBioModelRW->getSimulator( ).getTimeStep().setAdaptive( false );
      gBioModelRW->getSimulator( ).getTimeStep().setTimeStepIni( 0.01 );
      gBioModelRW->getSimulator( ).getTimeStep().setTimeStepMin( 0.01 );
      gBioModelRW->getSimulator( ).getTimeStep().setTimeStepMax( 0.01 );
      gBioModelRW->getSimulator( ).getTimeStep().setEndOfSimulation( 1.0 );
    }
  }
// FIXME: World
  {
    Bulk *bulk = new Bulk(  );
    bulk->setName( "MyTank" );
    bulk->setBulkIdx( BULK_MyTank );
    bulk->setParamBool( bulk->getIdxBool( BULK_isConstant ), true );
    bulk->setParamReal( bulk->getIdxReal( BULK_D ), 0.6 );
    bulk->setParamString( bulk->getIdxString( BULK_updateType ), "reaction" );
    {
      BulkSolute *bulk_solute = new BulkSolute( );
      bulk_solute->setParamReal( bulk_solute->getIdxReal( BULK_SOLUTE_Sbulk ), 0.0 );
      bulk_solute->setParamReal( bulk_solute->getIdxReal( BULK_SOLUTE_Sin ), 0.0 );
      bulk_solute->setParamBool( bulk_solute->getIdxBool( BULK_SOLUTE_isConstant ), true );
      bulk_solute->setParamReal( bulk_solute->getIdxReal( BULK_SOLUTE_Spulse ), 0.0 );
      bulk_solute->setParamReal( bulk_solute->getIdxReal( BULK_SOLUTE_pulseRate ), 0.0 );
      bulk_solute->setSolute( SOLUTE_GrowthNutrient );
      bulk->getSolutes().push_back( bulk_solute );
    }
    gBioModelRW->getWorld( ).getBulks( ).push_back( bulk );
  }
// FIXME: Agars
// FIXME: ComputationDomains
// FIXME: ComputationDomain
  {
    Reaction *reaction = new Reaction(  );
    reaction->setCatalyzedBy( PARTICLE_biomass );
    reaction->setCatalyst( -1 );
    reaction->setMuMax( 3.0 );
    reaction->setName( "Growth" );
    reaction->setClass( "ReactionFactor" );
    {
      Reaction::Yield yield;
      yield.setParticle( );
      yield.setItemIdx( PARTICLE_biomass );
      yield.setValue( 1.0 );
      reaction->getYields().push_back( yield );
    }
    {
      Reaction::Yield yield;
      yield.setSolute( );
      yield.setItemIdx( SOLUTE_GrowthNutrient );
      yield.setValue( -1.0 );
      reaction->getYields().push_back( yield );
    }
    {
      Reaction::KineticFactor *kinetic_factor = new Reaction::MonodKinetic( );

      kinetic_factor->setMolecule( -1 );
      kinetic_factor->setKi( 0.0 );
      kinetic_factor->setKs( 0.1 );
      kinetic_factor->setClass( "MonodKinetic" );
      kinetic_factor->setSolute( SOLUTE_GrowthNutrient );
      reaction->getKineticFactors().push_back( kinetic_factor );
    }
    gBioModelRW->getReactions( ).push_back( reaction );
  }
  {
    Solver *solver = new Solver(  );
    solver->setSolverIdx( SOLVER_solutes );
    solver->setParamBool( solver->getIdxBool( SOLVER_active ), true );
    solver->setParamInt( solver->getIdxInt( SOLVER_preStep ), 150 );
    solver->setParamInt( solver->getIdxInt( SOLVER_postStep ), 150 );
    solver->setParamInt( solver->getIdxInt( SOLVER_coarseStep ), 1500 );
    solver->setParamInt( solver->getIdxInt( SOLVER_nCycles ), 5 );
    solver->setParamInt( solver->getIdxInt( SOLVER_refineRatio ), 2 );
    solver->setParamInt( solver->getIdxInt( SOLVER_AMRLevels ), 3 );
    solver->setName( "solutes" );
    solver->setClass( "SolverSimple" );
    solver->setDomain( "MyColony" );
    solver->getReactions().push_back( REACTION_Growth );
    gBioModelRW->getSolvers( ).push_back( solver );
  }
  {

    gBioModelRW->getAgentGrid( ).setShovingMutual( true );
    gBioModelRW->getAgentGrid( ).setErosionMethod( true );
    gBioModelRW->getAgentGrid( ).setSloughDetachedBiomass( false );
    gBioModelRW->getAgentGrid( ).setResolution( 8.0 );
    gBioModelRW->getAgentGrid( ).setShovingFraction( 0.0025 );
    gBioModelRW->getAgentGrid( ).setShovingMaxIter( 1000 );
    gBioModelRW->getAgentGrid( ).setComputationDomain( "MyColony" );
  }
  {
    AgentSpecies *species = new AgentSpecies( "MyGrowingCells", "Yeast", AGENT_SPECIES_Yeast_MyGrowingCells, 19, 2, 4, 3 );
    species->setParamReal( species->getIdxReal( SPECIES_initialMassCV ), 0.1 );
    species->setParamString( species->getIdxString( SPECIES_color ), "blue" );
    species->setParamReal( species->getIdxReal( SPECIES_tightJunctionToBoundaryStrength ), 0.0 );
    species->setParamReal( species->getIdxReal( SPECIES_attachToBoundaryCreateFactor ), 1.1 );
    species->setParamReal( species->getIdxReal( SPECIES_attachToBoundaryDestroyFactor ), 2.0 );
    species->setParamReal( species->getIdxReal( SPECIES_attachCreateFactor ), 1.0 );
    species->setParamReal( species->getIdxReal( SPECIES_attachDestroyFactor ), 2 );
    species->setParamBool( species->getIdxBool( SPECIES_DoNotRandomizeMassOnCreation ), false );
    species->setParamBool( species->getIdxBool( SPECIES_visible ), true );
    species->setParamString( species->getIdxString( SPECIES_computationDomain ), "MyColony" );
    species->setParamReal( species->getIdxReal( SPECIES_divRadius ), 2.5 );
    species->setParamReal( species->getIdxReal( SPECIES_divRadiusCV ), 0.1 );
    species->setParamReal( species->getIdxReal( SPECIES_deathRadius ), 0.2 );
    species->setParamReal( species->getIdxReal( SPECIES_deathRadiusCV ), 0.1 );
    species->setParamReal( species->getIdxReal( SPECIES_babyMassFrac ), 0.5 );
    species->setParamReal( species->getIdxReal( SPECIES_babyMassFracCV ), 0.1 );
    species->setParamReal( species->getIdxReal( SPECIES_shoveLimit ), 0.0 );
    species->setParamReal( species->getIdxReal( SPECIES_shoveFactor ), 1.15 );
    species->setParamReal( species->getIdxReal( SPECIES_shoveScale ), 1.0 );
    species->setParamReal( species->getIdxReal( SPECIES_brownianScale ), 0.0 );
    species->setParamBool( species->getIdxBool( SPECIES_fixed ), false );
    species->setParamInt( species->getIdxInt( SPECIES_noSkinBottomLayerBoundary ), 0 );
    species->setParamReal( species->getIdxReal( SPECIES_epsMax ), 0.5 );
    species->setParamString( species->getIdxString( SPECIES_epsColor ), "lightGray" );
    species->setParamReal( species->getIdxReal( SPECIES_kHyd ), 0.1 );
    species->setParamBool( species->getIdxBool( SPECIES_useActivationInhibition ), false );
    species->setParamReal( species->getIdxReal( SPECIES_neighborhoodRadiusCoefficient ), 2.5 );
    species->setParamInt( species->getIdxInt( SPECIES_startingTimeActivationInhibition ), 0 );
    species->setDMax( 8.0 );
    species->setNumModelBools( AGENT_SPECIES_Yeast_MyGrowingCells_NUM_BOOLS );
    species->setNumModelReals( AGENT_SPECIES_Yeast_MyGrowingCells_NUM_REALS );
    species->setNumModelInts( AGENT_SPECIES_Yeast_MyGrowingCells_NUM_INTS );
    species->setUseMechForceReals( true );
    species->setIdxMechForceReals( AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_X, AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_Y, AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_Z );
    species->setNumODEVariables( NUM_ODE_VARIABLES );
    {
      InitArea *init_area = new InitArea( );

      init_area->setNumber( 1 );
      init_area->setBirthday( 0.0 );
      init_area->setShape( "default" );
      init_area->setAgentSpecies( species );
      {
        Coordinates *coordinates = new Coordinates( );

        coordinates->setX( 64.0 );
        coordinates->setY( 64.0 );
        coordinates->setZ( 0 );
        coordinates->setR( 0.0 );
        init_area->getCoordinates().push_back( coordinates );
      }
      {
        Coordinates *coordinates = new Coordinates( );

        coordinates->setX( 65.0 );
        coordinates->setY( 65.0 );
        coordinates->setZ( 0 );
        coordinates->setR( 0.0 );
        init_area->getCoordinates().push_back( coordinates );
      }
      species->getInitAreas().push_back( init_area );
    }
    {
      Adhesion *adhesion = new Adhesion( );

      adhesion->setStrength( 0.2 );
      adhesion->setWithSpecies( AGENT_SPECIES_Yeast_MyGrowingCells );
      adhesion->setScale( 1.0 );
      species->getAdhesions().push_back( adhesion );
    }
    {
      DistanceJunction *junction = new DistanceJunction( );

      junction->setEnabled( true );
      junction->setWithSpecies( AGENT_SPECIES_Yeast_MyGrowingCells );
      species->getDistanceJunctions().push_back( junction );
    }
    {
      species->addParticle( PARTICLE_biomass, AGENT_SPECIES_Yeast_MyGrowingCells_biomass, 12000.0 );
      species->addParticle( PARTICLE_inert, AGENT_SPECIES_Yeast_MyGrowingCells_inert, 70.0 );
    }
    {
      species->getReactions( ).push_back( REACTION_Growth );
    }
    gBioModelRW->getAgentSpecies( ).push_back( species );
  }
  {
    Solute *solute = new Solute( "GrowthNutrient", SOLUTE_GrowthNutrient, DOMAIN_MyColony, 7, 3, 2, 0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_diffusivity ), 360.0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_airDiffusivity ), 0.0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_decayRate ), 0.0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_resolution ), 8.0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_concentration ), 300.0 );
    solute->setParamBool( solute->getIdxBool( SOLUTE_randomInit ), false );
    solute->setParamReal( solute->getIdxReal( SOLUTE_rndMinConcentration ), 0.0 );
    solute->setParamReal( solute->getIdxReal( SOLUTE_rndMaxConcentration ), 0.0 );
    solute->setParamBool( solute->getIdxBool( SOLUTE_writeOutput ), true );
    solute->setAMRLevels( 3 );
    solute->setInterfaceAMRLevel( 2 );
    solute->setNumTimeSteps( 1 );
    solute->setSolverIdx( SOLVER_solutes );
    gBioModelRW->getSolutes( ).push_back( solute );
  }
  {
    Particle *particle = new Particle( PARTICLE_biomass, "biomass", 150.0 );
    gBioModelRW->getParticles( ).push_back( particle );
  }
  {
    Particle *particle = new Particle( PARTICLE_inert, "inert", 150.0 );
    gBioModelRW->getParticles( ).push_back( particle );
  }
}
