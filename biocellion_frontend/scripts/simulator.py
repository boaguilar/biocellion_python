import sys
from agent_species import Param, ParamHolder
from agent_species import AllAgentSpecies
from particle import AllParticles
from solute import AllSolutes
from agent_grid import AgentGrid
from world import World
from reaction import AllReactions

class Simulator( ParamHolder ):

    def __init__(self):
        self.mName = "simulator"
        ParamHolder.__init__(self)
        self.addParam( Param( "restartPreviousRun", "bool", False, False ) )
        self.addParam( Param( "randomSeed", "int", 75321, False ) )
        self.addParam( Param( "outputPeriod", "hr", 0.01, True ) )
        self.addParam( Param( "chemostat", "bool", False, False ) )
        self.addParam( Param( "diffusionReactionOnAgentTime", "bool", False, False ) )
        self.addParam( Param( "agentTimeStep", "hr", 0.01, True ) )
        self.mPrivateNumberHiddenParams = [ "randomSeed", "outputPeriod", "agentTimeStep" ]
        self.mPrivateBoolHiddenParams = [ "restartPreviousRun", "chemostat", "diffusionReactionOnAgentTime" ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mTimeStep = TimeStep()
        return

    def getNumBaselineSteps( self ):
        self.getTimeStep().getParam( 'endOfSimulation' ).checkUnit( 'hr' )
        self.getParam( 'agentTimeStep' ).checkUnit( 'hr' )

        end_of_simulation = self.getTimeStep().getParam( 'endOfSimulation' ).getValue()
        agent_time_step = self.getParam( 'agentTimeStep' ).getValue()
        number_steps = int( end_of_simulation / agent_time_step )

        return number_steps

    def getBioModelH( self, indent, depth ):
        lines  = "// FIXME: Simulator \n"
        return lines

    def getInitializeBioModel( self, indent, depth ):
        varname = "gSimulator"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "%s = new Simulator( %s, %s, %s, %s, %s, %s );" % (varname, "false", "false", "true", "75321", "0.01", "0.01", ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )

        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = self.mTimeStep.getInitializeBioModel( "%s->getTimeStep()" % ( varname, ), indent, depth )
        lines.append( s )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )

    def getTimeStep( self ):
        return self.mTimeStep
        
    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += str( self.mTimeStep ) + "\n"
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)
        
class TimeStep( ParamHolder ):

    def __init__(self):
        self.mName = "timeStep"
        ParamHolder.__init__(self)
        self.addParam( Param( "adaptive", "bool", False, False ) )
        self.addParam( Param( "timeStepIni", "hr", 0.01, False ) )
        self.addParam( Param( "timeStepMin", "hr", 0.01, False ) )
        self.addParam( Param( "timeStepMax", "hr", 0.01, False ) )
        self.addParam( Param( "endOfSimulation", "hr", 0.01, True ) )

        self.mPrivateNumberHiddenParams = [ "timeStepIni", "timeStepMin", "timeStepMax", "endOfSimulation" ]
        self.mPrivateBoolHiddenParams = [ "adaptive" ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        return

    def getInitializeBioModel( self, varname, indent, depth ):
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )

        s = self.getInitializeBioModelSetDataMembers( varname, ".", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    

    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)


class IDynoMiCS( ParamHolder ):

    def __init__( self, biomodel ):
        self.mName = "idynomics"
        self.mBioModel = biomodel
        ParamHolder.__init__(self)

        self.mSimulator = Simulator( )
        self.mWorld = World( self )
        self.mReactions = AllReactions( self )
        self.mAgentGrid = AgentGrid()
        self.mAgentSpecies = AllAgentSpecies( self )
        self.mSolutes = AllSolutes( self )
        self.mParticles = AllParticles()
        
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.mSimulator.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mWorld.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mReactions.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mAgentGrid.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mAgentSpecies.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mSolutes.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mParticles.getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines  = [ ]
        lines.append( self.mSimulator.getInitializeBioModel( indent, depth ) )
        lines.append( self.mWorld.getInitializeBioModel( indent, depth ) )
        lines.append( self.mReactions.getInitializeBioModel( indent, depth ) )
        lines.append( self.mAgentGrid.getInitializeBioModel( indent, depth ) )
        lines.append( self.mAgentSpecies.getInitializeBioModel( indent, depth ) )
        lines.append( self.mSolutes.getInitializeBioModel( indent, depth ) )
        lines.append( self.mParticles.getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def getSimulator( self ):
        return self.mSimulator
        
    def getWorld( self ):
        return self.mWorld
        
    def getReactions( self ):
        return self.mReactions
        
    def getAgentGrid( self ):
        return self.mAgentGrid
        
    def getAgentSpecies( self ):
        return self.mAgentSpecies
        
    def getSolutes( self ):
        return self.mSolutes
        
    def getParticles( self ):
        return self.mParticles
        
    def getName(self):
        return self.mName

    def getItemEnumToken( self, item_holder, item_key ):
        if not item_holder.hasKey( item_key ):
            return -1
        target = item_holder.getItem( item_key )
        return target.getEnumToken( )

    def linkAttributeToEnumToken( self, src_item_holder, attribute_name, target_item_holder ):
        for key in src_item_holder.getKeys( ):
            item = src_item_holder.getItem( key )
            target_key = item.getAttribute( attribute_name ).getValue( )
            token = self.getItemEnumToken( target_item_holder, target_key )
            item.getAttribute( attribute_name ).setValue( token )
        return
    
    def getComputationDomainEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mWorld.getComputationDomains( ), item_key )

    def getReactionEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mReactions, item_key )

    def getAgentSpeciesEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mAgentSpecies, item_key )

    def getSoluteEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mSolutes, item_key )

    def getParticleEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mParticles, item_key )

    def organizeChildren( self ):
        for t in self.mChildren:
            print( "Still have children of type: " + str( t ) )
            sys.exit( "ERROR: children should not be here." )

        print( "FIXME: Warning, *->Solutes need to be connected by enumtoken." )
        print( "FIXME: Warning, *->Reactions need to be connected by enumtoken." )

        ## Set up AMR related features for the model
        self.mSolutes.calcRefineRatio( )
        
        ## Reactions.catalyzedBy->Particles 
        self.linkAttributeToEnumToken( self.mReactions, 'catalyzedBy', self.mParticles )
        ## Reactions.catalyst->AgentSpecies
        self.linkAttributeToEnumToken( self.mReactions, 'catalyst', self.mAgentSpecies )
        ## Solutes->ComputationDomains
        self.linkAttributeToEnumToken( self.mSolutes, 'domain', self.mWorld.getComputationDomains( ) )


        ## Reactions.*->others
        for reaction_key in self.mReactions.getKeys( ):
            reaction = self.mReactions.getItem( reaction_key )
            ## Reactions.yield->AgentSpecies/Solutes
            yields = reaction.getYields( )
            for yield_key in yields.getKeys( ):
                if yield_key == '*': continue
                yield_name = yields.getItem( yield_key ).getName( )
                if self.mParticles.hasKey( yield_name ):
                    yields.setParticle( yield_name, self.mParticles.getItem( yield_name ).getEnumToken( ) )
                elif self.mSolutes.hasKey( yield_name ):
                    yields.setSolute( yield_name, self.mSolutes.getItem( yield_name ).getEnumToken( ) )
                else:
                    msg  = "ERROR: Reaction.Yield.Param should name a solute or a particle. " + str( yield_name ) + " is not in either list.\n"
                    msg += "ERROR: Known solutes: " + ", ".join( self.mSolutes.getKeys( ) ) + ".\n"
                    msg += "ERROR: Known particles: " + ", ".join( self.mParticles.getKeys( ) ) + ".\n"
                    sys.exit( msg )

            ## Reactions.KineticFactors->Solutes
            kinetic_factors = reaction.getKineticFactors()
            for kinetic_factor_key in kinetic_factors.getKeys( ):
                kinetic_factor = kinetic_factors.getItem( kinetic_factor_key )
                solute_key = kinetic_factor.getAttribute( 'solute' ).getValue( )
                if self.mSolutes.hasKey( solute_key ):
                    kinetic_factor.getAttribute( 'solute' ).setValue( self.mSolutes.getItem( solute_key).getEnumToken( ) )
                elif solute_key == "":
                    # no solute is allowed
                    kinetic_factor.getAttribute( 'solute' ).setValue( -1 )
                else:
                    msg  = "ERROR: Reaction.KineticFactor.solute should name a solute. " + str( solute_key ) + " is not in the list.\n"
                    msg += "ERROR: Known solutes: " + ", ".join( self.mSolutes.getKeys( ) ) + ".\n"
                    sys.exit( msg )

            ## Reactions.KineticFactors->Molecules
            kinetic_factors = reaction.getKineticFactors()
            for kinetic_factor_key in kinetic_factors.getKeys( ):
                kinetic_factor = kinetic_factors.getItem( kinetic_factor_key )
                molecule_key = kinetic_factor.getAttribute( 'molecule' ).getValue( )
                if molecule_key == "":
                    # no molecule is allowed
                    kinetic_factor.getAttribute( 'molecule' ).setValue( -1 )
                else:
                    msg  = "ERROR: Reaction.KineticFactor.molecule should name a molecule. " + str( molecule_key ) + " is not in the list.\n"
                    msg += "ERROR: Known molecules: NONE.\n"
                    sys.exit( msg )
        
        for species_key in self.mAgentSpecies.getKeys( ):
            species = self.mAgentSpecies.getItem( species_key )
            ## AgentSpeciesParticles need to be connected to Particles
            for species_particle_key in species.getParticles( ).getKeys( ):
                species_particle = species.getParticles( ).getItem( species_particle_key )
                particle_key = species_particle.getAttribute( 'name' ).getValue( )
                if self.mParticles.hasKey( particle_key ):
                    particle = self.mParticles.getItem( particle_key )
                    species_particle.setParticle( particle )
                else:
                    msg  = "ERROR:"
                    msg += " species " + species.getAttribute( 'class' ).getValue( ) + "-" + species.getAttribute( 'name' ).getValue( )
                    msg += " has particle " + species_particle.getAttribute( 'name' ).getValue( )
                    msg += " but no particle exists.  Particles = " + " ".join( self.mParticles.getKeys( ) )
                    sys.exit( msg )
                    
            ## AgentSpecies.Adhesions->AgentSpecies
            self.linkAttributeToEnumToken( species.getAdhesions( ), 'withSpecies', self.mAgentSpecies )
                
            ## AgentSpecies.TightJunctions->AgentSpecies
            self.linkAttributeToEnumToken( species.getTightJunctions( ), 'withSpecies', self.mAgentSpecies )
                
            ## AgentSpecies.DistanaceJuctions need to be created
            species.createDistanceJunctions( )
            if len( species.getDistanceJunctions( ) ) > 0:
                self.mBioModel.setDistanceJunctionsEnabled( True )
            # THIS SECTION SHOULD NOT BE NECESSARY, as long as adhesions and tight junctions, etc. are updated before creation
            # ## AgentSpecies.DistanaceJuctions need to be connected to AgentSpecies
            # for junction_key in species.getDistanceJunctions( ).getKeys( ):
            #     junction = species.getDistanceJunctions( ).getItem( junction_key )
            #     target_species_key = junction.getAttribute( "withSpecies" ).getValue( )
            #     target_species = self.mAgentSpecies.getItem( target_species_key )
            #     target_species_token = target_species.getEnumToken( )
            #     junction.getAttribute( "withSpecies" ).setValue( target_species_token )
                    
                    
        return

    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += str( self.mSimulator ) + "\n"
        s += str( self.mWorld ) + "\n"
        s += str( self.mReactions ) + "\n"
        s += str( self.mAgentGrid ) + "\n"
        s += str( self.mAgentSpecies ) + "\n"
        s += str( self.mSolutes )  + "\n"
        s += str( self.mParticles )  + "\n"
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)
        

def main():
    print("UNCHECKED")
    return
    
if __name__ == "__main__":
    main()
        

