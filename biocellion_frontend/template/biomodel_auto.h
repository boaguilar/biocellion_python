#ifndef _BIOMODEL_AUTO_H_
#define _BIOMODEL_AUTO_H_
extern void initializeBioModelAuto( );


// FIXME: World
const std::string BULK_updateType = "updateType";
const std::string BULK_D = "D";
const std::string BULK_isConstant = "isConstant";

const std::string BULK_SOLUTE_Spulse = "Spulse";
const std::string BULK_SOLUTE_pulseRate = "pulseRate";
const std::string BULK_SOLUTE_isConstant = "isConstant";
const std::string BULK_SOLUTE_Sin = "Sin";
const std::string BULK_SOLUTE_Sbulk = "Sbulk";

typedef enum _bulk_type_e {
  BULK_MyTank,
  NUM_BULKS
} bulk_type_e;


// FIXME: Agars
typedef enum _domain_type_e {
  DOMAIN_MyColony,
  NUM_DOMAINS
} domain_type_e;
// FIXME: ComputationDomain

typedef enum _reaction_type_e {
  REACTION_Growth,
  NUM_REACTIONS
} reaction_type_e;

const std::string SOLVER_postStep = "postStep";
const std::string SOLVER_AMRLevels = "AMRLevels";
const std::string SOLVER_coarseStep = "coarseStep";
const std::string SOLVER_nCycles = "nCycles";
const std::string SOLVER_preStep = "preStep";
const std::string SOLVER_active = "active";
const std::string SOLVER_refineRatio = "refineRatio";

typedef enum _solver_type_e {
  SOLVER_solutes,
  NUM_SOLVERS
} solver_type_e;



const std::string SPECIES_epsColor = "epsColor";
const std::string SPECIES_color = "color";
const std::string SPECIES_shoveLimit = "shoveLimit";
const std::string SPECIES_DoNotRandomizeMassOnCreation = "DoNotRandomizeMassOnCreation";
const std::string SPECIES_attachDestroyFactor = "attachDestroyFactor";
const std::string SPECIES_deathRadius = "deathRadius";
const std::string SPECIES_brownianScale = "brownianScale";
const std::string SPECIES_startingTimeActivationInhibition = "startingTimeActivationInhibition";
const std::string SPECIES_babyMassFrac = "babyMassFrac";
const std::string SPECIES_attachToBoundaryDestroyFactor = "attachToBoundaryDestroyFactor";
const std::string SPECIES_divRadiusCV = "divRadiusCV";
const std::string SPECIES_computationDomain = "computationDomain";
const std::string SPECIES_attachToBoundaryCreateFactor = "attachToBoundaryCreateFactor";
const std::string SPECIES_deathRadiusCV = "deathRadiusCV";
const std::string SPECIES_neighborhoodRadiusCoefficient = "neighborhoodRadiusCoefficient";
const std::string SPECIES_noSkinBottomLayerBoundary = "noSkinBottomLayerBoundary";
const std::string SPECIES_kHyd = "kHyd";
const std::string SPECIES_shoveScale = "shoveScale";
const std::string SPECIES_initialMassCV = "initialMassCV";
const std::string SPECIES_divRadius = "divRadius";
const std::string SPECIES_epsMax = "epsMax";
const std::string SPECIES_visible = "visible";
const std::string SPECIES_useActivationInhibition = "useActivationInhibition";
const std::string SPECIES_tightJunctionToBoundaryStrength = "tightJunctionToBoundaryStrength";
const std::string SPECIES_shoveFactor = "shoveFactor";
const std::string SPECIES_attachCreateFactor = "attachCreateFactor";
const std::string SPECIES_fixed = "fixed";
const std::string SPECIES_babyMassFracCV = "babyMassFracCV";

typedef enum _agent_species_type_e {
  AGENT_SPECIES_Yeast_MyGrowingCells,
  NUM_AGENT_SPECIES
} agent_species_type_e;

typedef enum _AGENT_SPECIES_Yeast_MyGrowingCells_bool_type_e {
  AGENT_SPECIES_Yeast_MyGrowingCells_NUM_BOOLS
} AGENT_SPECIES_Yeast_MyGrowingCells_bool_type_e;
typedef enum _AGENT_SPECIES_Yeast_MyGrowingCells_real_type_e {
  AGENT_SPECIES_Yeast_MyGrowingCells_biomass,
  AGENT_SPECIES_Yeast_MyGrowingCells_inert,
  AGENT_SPECIES_Yeast_MyGrowingCells_NUM_REALS
} AGENT_SPECIES_Yeast_MyGrowingCells_real_type_e;
typedef enum _AGENT_SPECIES_Yeast_MyGrowingCells_int_type_e {
  AGENT_SPECIES_Yeast_MyGrowingCells_NUM_INTS
} AGENT_SPECIES_Yeast_MyGrowingCells_int_type_e;
typedef enum _AGENT_SPECIES_Yeast_MyGrowingCells_mech_force_real_type_e {
  AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_X,
  AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_Y,
  AGENT_SPECIES_Yeast_MyGrowingCells_MECH_REAL_FORCE_Z,
  AGENT_SPECIES_Yeast_MyGrowingCells_NUM_MECH_FORCE_REALS
} AGENT_SPECIES_Yeast_MyGrowingCells_mech_force_real_type_e;

const std::string SOLUTE_airDiffusivity = "airDiffusivity";
const std::string SOLUTE_AMRLevels = "AMRLevels";
const std::string SOLUTE_decayRate = "decayRate";
const std::string SOLUTE_numTimeSteps = "numTimeSteps";
const std::string SOLUTE_interfaceAMRLevel = "interfaceAMRLevel";
const std::string SOLUTE_rndMinConcentration = "rndMinConcentration";
const std::string SOLUTE_randomInit = "randomInit";
const std::string SOLUTE_rndMaxConcentration = "rndMaxConcentration";
const std::string SOLUTE_diffusivity = "diffusivity";
const std::string SOLUTE_concentration = "concentration";
const std::string SOLUTE_resolution = "resolution";
const std::string SOLUTE_writeOutput = "writeOutput";

typedef enum _solute_type_e {
  SOLUTE_GrowthNutrient,
  NUM_SOLUTES
} solute_type_e;

typedef enum _particle_type_e {
  PARTICLE_biomass,
  PARTICLE_inert,
  NUM_PARTICLES
} particle_type_e;

typedef enum _ODE_variables {
  FIRST_VARIABLE,
  NUM_ODE_VARIABLES
} ODE_variables;

#endif /* _BIOMODEL_AUTO_H_ */
