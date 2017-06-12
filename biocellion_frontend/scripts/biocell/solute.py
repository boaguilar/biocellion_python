from biocell import *

class Solute( ParamHolder ):

    def __init__( self, name, model ):
        self.mName = name
        self.mEnumToken = "SOLUTE_%s" % ( name, )
        self.mModel = model
        
        ParamHolder.__init__(self)
        self.setPrefix( "SOLUTE" )
        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "domain", "str", "", True ) )
        self.addParam( Param( "diffusivity", "um2.hour-1", 0.0, True ) )
        self.addParam( Param( "airDiffusivity", "um2.hour-1", 0.0, False ) )
        self.addParam( Param( "decayRate", "float", 0.0, False ) )
        self.addParam( Param( "resolution", "um", -1.0, False ) )
        self.addParam( Param( "concentration", "g.L-1", -1.0, False ) )
        self.addParam( Param( "randomInit", "bool", False, False ) )
        self.addParam( Param( "rndMinConcentration", "g.L-1", 0.0, False ) )
        self.addParam( Param( "rndMaxConcentration", "g.L-1", 0.0, False ) )
        self.addParam( Param( "writeOutput", "bool", False, False ) )
        self.addParam( Param( "AMRLevels", "int", 0, False ) )
        self.addParam( Param( "interfaceAMRLevel", "int", 0, False ) )
        self.addParam( Param( "numTimeSteps", "int", 1, False ) )

        self.mPrivateNumberHiddenParams = [ "AMRLevels", "interfaceAMRLevel", "numTimeSteps", ]
        self.mPrivateBoolHiddenParams = [ ]
        self.mPrivateStringHiddenParams = [ ]
        self.mPrivateHiddenParams = [ "domain" ] + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mDomainReference = None
        self.mSolverReference = None
        return

    def getDomainReference( self ):
        return self.mDomainReference

    def setDomainReference( self, domain ):
        self.mDomainReference = domain
        return

    def getSolverReference( self ):
        return self.mSolverReference

    def setSolverReference( self, solver ):
        self.mSolverReference = solver
        return

    def getAMRLevels( self ):
        amr_levels = self.getParam( 'AMRLevels' ).getValue( )
        if amr_levels == 0:
            amr_levels = self.mSolverReference.getParam( 'AMRLevels' ).getValue( )
        if amr_levels == 0:
            raise Exception( "ERROR: solute needs an AMRLevel specified.  Usually via <solver>." )

        self.getParam( 'AMRLevels' ).setValue( amr_levels )
        return amr_levels
            
    def calcInterfaceAMRLevel( self ):
        self.getResolution( )
        return self.getParam( 'interfaceAMRLevel' ).getValue( )

    def getInterfaceAMRLevel( self ):
        return self.getParam( 'interfaceAMRLevel' ).getValue( )

    def getResolution( self ):
        resolution = self.getParam( 'resolution' ).getValue( )
        if resolution <= 0.0:
            resolution = self.mDomainReference.getParam( 'resolution' ).getValue( )
        if resolution <= 0.0:
            resolution = self.mModel.getAgentGrid( ).getParam( 'resolution' ).getValue( )
            
        unit_box_resolution = self.mModel.getAgentGrid( ).getParam( 'resolution' ).getValue( )
        if resolution > unit_box_resolution:
            resolution = unit_box_resolution

        amr_levels = self.getAMRLevels( )
        refine_ratio = self.mModel.getSolvers( ).getRefineRatio( )

        n = 0
        tmp_resolution = unit_box_resolution * ( refine_ratio ** n )
        while tmp_resolution > resolution:
            n -= 1
            tmp_resolution = unit_box_resolution * ( refine_ratio ** n )

        small_resolution = tmp_resolution
        small_n = n

        while tmp_resolution < resolution:
            n += 1
            tmp_resolution = unit_box_resolution * ( refine_ratio ** n )
        large_resolution = tmp_resolution
        large_n = n

        if abs( large_resolution - resolution ) < abs( resolution - small_resolution ):
            n = large_n
            resolution = large_resolution
        else:
            n = small_n
            resolution = small_resolution

        if n < 0:
            if abs( n ) >= amr_levels:
                amr_levels = abs( n ) + 1
                self.getParam( 'AMRLevels' ).setValue( amr_levels )
            interface_amr_level = amr_levels + n - 1
        elif n > 0:
            # should not happen
            resolution = unit_box_resolution
            interface_amr_level = amr_levels - 1
        else:
            # n == 0
            interface_amr_level = amr_levels - 1
                
        self.getParam( 'interfaceAMRLevel' ).setValue( interface_amr_level )
        self.getParam( 'resolution' ).setValue( resolution )
        return resolution

    def getConcentration( self ):
        concentration = self.getParam( 'concentration' ).getValue( )
        if concentration <= 0.0:
            concentration = self.mModel.getWorld( ).getBulks( ).getSoluteConcentration( self.mName )
        
        self.getParam( 'concentration' ).setValue( concentration )
        return concentration

    def calcConcentration( self ):
        return self.getConcentration( )

    def chooseSolver( self ):
        # find solver that contains reaction that contains this solute as a yield
        self.mSolverReference = None
        for solver_key in self.mModel.getSolvers( ).getKeys( ):
            # global solver
            solver = self.mModel.getSolvers( ).getItem( solver_key )
            for reaction_key in solver.getReactions( ).getKeys( ):
                # global reaction
                reaction = solver.getReactions( ).getItem( reaction_key ).getReference( )
                # is this solute a yield?
                solute = reaction.getYields( ).getSolute( self.mName )
                if solute == self:
                    if ( self.mSolverReference is not None ) and ( self.mSolverReference != solver ):
                        raise Exception( "ERROR: solute has multiple solvers" )
                    self.mSolverReference = solver
                    break
        return
        
    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getInitializeBioModel(self, indent, depth):
        varname = "solute"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Solute *%s = new Solute( \"%s\", %s, %s, %s, %s, %s, %s );" % ( varname, self.mName, self.mEnumToken, self.mDomainReference.getEnumToken( ),
                                                                                                        self.countReal(), self.countInt(), self.countBool(), self.countString(), ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
            
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )

        if self.mSolverReference is not None:
            s = (depth*indent) + "%s->setSolverIdx( %s );" % ( varname, self.mSolverReference.getEnumToken( ), )
            lines.append( s )

        lines.append( (depth*indent) + "gBioModelRW->getSolutes( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<solute" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</solute>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllSolutes( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, Solute )
        self.mModel = model
        return

    def calcInterfaceAMRLevel( self ):
        levels = [ ]
        for name in self.mOrder:
            levels.append( self.mItems[ name ].calcInterfaceAMRLevel( ) )
        if len( levels ) < 1:
            raise Exception( "ERROR: Need at least 1 interface AMR level." )
        return max( levels )

    def getInterfaceAMRLevel( self ):
        levels = [ ]
        for name in self.mOrder:
            levels.append( self.mItems[ name ].getInterfaceAMRLevel( ) )
        if len( levels ) < 1:
            raise Exception( "ERROR: Need at least 1 interface AMR level." )
        return max( levels )

    def calcConcentrations( self ):
        for name in self.mOrder:
            self.mItems[ name ].calcConcentration( )
        return

    def chooseSolvers( self ):
        for name in self.mOrder:
            self.mItems[ name ].chooseSolver( )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSolutesEnum( indent, depth ) )
        return "\n".join( lines )

    def getSolutesEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _solute_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_SOLUTES"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} solute_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = []
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def addItem( self, name, item=None ):
        if item is None:
            item = self.mItemClass( name, self.mModel )
        return ItemHolder.addItem( self, name, item )
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
