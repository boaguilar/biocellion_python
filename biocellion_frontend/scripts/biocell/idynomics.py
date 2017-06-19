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
        self.mMolecules = AllMolecules( self )
        self.mParticles = AllParticles()
        
        return

    def getItemAddIfNeeded( self, tag, name, class_name ):
        if tag == "solute":
            collection = self.getSolutes( )
        elif tag == "molecule":
            collection = self.getMolecules( )
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
        elif tag in ( "computationDomain", "domain" ):
            collection = self.getWorld( ).getComputationDomains( )
        elif tag in ( "bulk" ):
            collection = self.getWorld( ).getBulks( )
        else:
            raise BadItemType( tag, name )

        if not collection.hasKey( name ):
            if tag == 'species':
                ok = collection.addSpecies( class_name, name )
            else:
                ok = collection.addItem( name )
                
            if not ok:
                raise GenericException( "Could not add item " + str( name ) + " to the list of " + str( tag ) + "." )
        return collection.getItem( name )

    def getItem( self, tag, name ):
        if tag == "solute":
            collection = self.getSolutes( )
        elif tag == "molecule":
            collection = self.getMolecules( )
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
        elif tag in ( "computationDomain", "domain" ):
            collection = self.getWorld( ).getComputationDomains( )
        elif tag in ( "bulk" ):
            collection = self.getWorld( ).getBulks( )
        else:
            raise BadItemType( tag, name )

        if not collection.hasKey( name ):
            raise BadItemName( tag, name )
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
        lines.append( self.mMolecules.getBioModelH( indent, depth ) )
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
        lines.append( self.mMolecules.getInitializeBioModel( indent, depth ) )
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
        
    def getMolecules( self ):
        return self.mMolecules
        
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

    def getMoleculeEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mMolecules, item_key )

    def getParticleEnumToken( self, item_key ):
        return self.getItemEnumToken( self.mParticles, item_key )

    def organizeChildren( self ):
        for t in self.mChildren:
            print( "Still have children of type: " + str( t ) )
            raise Exception( "ERROR: children should not be here." )

        ### These must happen before enums are used to link items
        ## Link solutes to reactions
        self.mSolutes.chooseReactions( )
        ## Link solutes to solvers
        self.mSolutes.chooseSolvers( )

        ## Link agent species to reactions
        self.mAgentSpecies.chooseReactions( )
        
        ### These are linking with enums
        ## Reactions.catalyzedBy->Particles 
        self.linkAttributeToEnumToken( self.mReactions, 'catalyzedBy', self.mParticles )
        ## Reactions.catalyst->AgentSpecies
        self.linkAttributeToEnumToken( self.mReactions, 'catalyst', self.mAgentSpecies )

        ## Set up AMR related features for the model
        self.mSolvers.getRefineRatio( )
        self.mSolutes.calcInterfaceAMRLevel( )

        for species_key in self.mAgentSpecies.getKeys( ):
            species = self.mAgentSpecies.getItem( species_key )
                    
            ## AgentSpecies.DistanceJunctions need to be created
            species.createDistanceJunctions( )
            if len( species.getDistanceJunctions( ) ) > 0:
                self.mBioModel.setDistanceJunctionsEnabled( True )

        # make sure that all parameter names are defined
        if len( self.mSolutes ) == 0:
            node_object = self.getItemAddIfNeeded( 'solute', 'PlaceHolderSolute', "" )
            node_object.setDomainReference( self.mWorld.getComputationDomains( ).getLastItem( ) )
        if len( self.mMolecules ) == 0:
            node_object = self.getItemAddIfNeeded( 'molecule', 'PlaceHolderMolecule', "" )
            node_object.setDomainReference( self.mWorld.getComputationDomains( ).getLastItem( ) )
                    
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
        s += str( self.mMolecules )  + "\n"
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
