from biocell import *

class IDynoMiCS( ParamHolder ):

    def __init__( self, biomodel ):
        self.mName = "idynomics"
        self.mBioModel = biomodel
        ParamHolder.__init__(self)

        self.mSimulator = Simulator( )
        self.mWorld = World( self )
        self.mReactions = AllReactions( self )
        self.mSolvers = AllSolvers( self )
        self.mAgentGrid = AgentGrid()
        self.mAgentSpecies = AllAgentSpecies( self )
        self.mSolutes = AllSolutes( self )
        self.mParticles = AllParticles()
        
        return

    def getItemAddIfNeeded( self, tag, name, class_name ):
        if tag == "solute":
            collection = self.getSolutes( )
        elif tag == "particle":
            collection = self.getParticles( )
        elif tag == "reaction":
            collection = self.getReactions( )
        elif tag == "solver":
            collection = self.getSolvers( )
        elif tag == "molecularReactions":
            collection = self.getMolecularReactions( )
        elif tag == "species":
            collection = self.getAgentSpecies( )
        else:
            raise Exception( "Unexpected tag: " + str( tag ) )

        if not collection.hasKey( name ):
            if tag == 'species':
                ok = collection.addSpecies( class_name, name )
            else:
                ok = collection.addItem( name )
                
            if not ok:
                raise Exception( "ERROR: could not add " + str( tag ) + " " + str( name )  )
        return collection.getItem( name )

    def getItem( self, tag, name ):
        if tag == "solute":
            collection = self.getSolutes( )
        elif tag == "particle":
            collection = self.getParticles( )
        elif tag == "reaction":
            collection = self.getReactions( )
        elif tag == "solver":
            collection = self.getSolvers( )
        elif tag == "molecularReactions":
            collection = self.getMolecularReactions( )
        elif tag == "species":
            collection = self.getAgentSpecies( )
        else:
            raise Exception( "Unexpected tag: " + str( tag ) )

        if not collection.hasKey( name ):
            raise Exception( "ERROR: could not get " + str( tag ) + " " + str( name )  )
        return collection.getItem( name )

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.mSimulator.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mWorld.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mReactions.getBioModelH( indent, depth ) )
        lines.append( "" )
        lines.append( self.mSolvers.getBioModelH( indent, depth ) )
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
        lines.append( self.mSolvers.getInitializeBioModel( indent, depth ) )
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
        
    def getSolvers( self ):
        return self.mSolvers
        
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

    def getSolverEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mSolvers, item_key )

    def getAgentSpeciesEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mAgentSpecies, item_key )

    def getSoluteEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mSolutes, item_key )

    def getParticleEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mParticles, item_key )

    def organizeChildren( self ):
        for t in self.mChildren:
            print( "Still have children of type: " + str( t ) )
            raise Exception( "ERROR: children should not be here." )

        print( "FIXME: Warning, *->Reactions need to be connected by enumtoken." )

        ### These must happen before enums are used to like items
        ## Set up AMR related features for the model
        self.mSolutes.calcRefineRatio( )
        ## Set up solute initial concentrations
        self.mSolutes.calcConcentrations( )
        ## Link solutes to solvers
        self.mSolutes.chooseSolvers( )
        
        ### These are linking with enums
        ## Reactions.catalyzedBy->Particles 
        self.linkAttributeToEnumToken( self.mReactions, 'catalyzedBy', self.mParticles )
        ## Reactions.catalyst->AgentSpecies
        self.linkAttributeToEnumToken( self.mReactions, 'catalyst', self.mAgentSpecies )
        ## Solutes->ComputationDomains
        self.linkAttributeToEnumToken( self.mSolutes, 'domain', self.mWorld.getComputationDomains( ) )

        ## Reactions.*->others
        for reaction_key in self.mReactions.getKeys( ):
            reaction = self.mReactions.getItem( reaction_key )

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
                    raise Exception( msg )
        
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
                    raise Exception( msg )
                    
            ## AgentSpeciesReactions need to be connected to Reactions
            for species_reaction_key in species.getReactions( ).getKeys( ):
                species_reaction = species.getReactions( ).getItem( species_reaction_key )
                reaction_key = species_reaction.getAttribute( 'name' ).getValue( )
                if self.mReactions.hasKey( reaction_key ):
                    reaction = self.mReactions.getItem( reaction_key )
                    species_reaction.setReaction( reaction )
                else:
                    msg  = "ERROR:"
                    msg += " species " + species.getAttribute( 'class' ).getValue( ) + "-" + species.getAttribute( 'name' ).getValue( )
                    msg += " has reaction " + species_reaction.getAttribute( 'name' ).getValue( )
                    msg += " but no reaction exists.  Reactions = " + " ".join( self.mReactions.getKeys( ) )
                    raise Exception( msg )
                    
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
        s += str( self.mSolvers ) + "\n"
        s += str( self.mAgentGrid ) + "\n"
        s += str( self.mAgentSpecies ) + "\n"
        s += str( self.mSolutes )  + "\n"
        s += str( self.mParticles )  + "\n"
        s += "</%s>\n" % (self.mName)
        return s

    def __repr__(self):
        return str(self)
        
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
