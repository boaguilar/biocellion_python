from biocell import *

class ComputationDomain( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.setPrefix( "DOMAIN" )
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "resolution", "um", 0 ) )
        self.addParam( Param( "specificArea", "m2.m-3", 0 ) )
        self.addParam( Param( "hasBulk", "bool", False ) )
        self.addParam( Param( "boundaryLayer", "um", 0 ) )
        self.addParam( Param( "biofilmDiffusivity", "float", 0 ) )

        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [  ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams

        self.mGrid = ComputationDomainGrid( )
        self.mBoundaryConditions = ItemHolder( ComputationDomainBoundaryCondition )
        return

    def getEnumToken(self):
        return "%s_%s" % ( self.getPrefix(), self.getAttribute( 'name' ).getValue( ), )
    
    def getGrid( self ):
        return self.mGrid

    def getBoundaryConditions( self ):
        return self.mBoundaryConditions

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel(self, indent, depth):
        varname = "domain"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "ComputationDomain *%s = new ComputationDomain(  );" % ( varname, ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        container_name = "%s->getGrid()" % ( varname, )
        s = self.mGrid.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )

        container_name = "%s->getBoundaryConditions()" % ( varname, )
        s = self.mBoundaryConditions.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )
        
        lines.append( (depth*indent) + "gBioModelRW->getWorld( ).getComputationDomains().push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<computationDomain" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += str( self.mGrid ) + "\n"
        s += str( self.mBoundaryConditions ) + "\n"
        s += "</computationDomain>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllComputationDomains( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, ComputationDomain )
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getDomainsEnum( indent, depth ) )
        lines.append( "" )
        lines.append( self.getAllBoundaryConditionParamNames( indent, depth ) )
        lines.append( "" )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getAllBoundaryConditionParamNames( self, indent, depth ):
        all_params = { }
        all_order = [ ]
        for domain_name in self.getKeys( ):
            domain = self.getItem( domain_name )
            for bc_name in domain.getBoundaryConditions( ).getKeys( ):
                bc = domain.getBoundaryConditions( ).getItem( bc_name )
                for param_name in bc.getParamKeys( ):
                    if param_name not in all_params:
                        all_params[ param_name ] = bc.getParam( param_name )
                        all_order.append( param_name )
                    
        lines = []
        for n in all_order:
            s = (depth*indent) + "const std::string %s = \"%s\";" % ( all_params[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )
    
    def getDomainsEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _domain_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_DOMAINS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} domain_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )


def main():
    all = AllComputationDomains()
    all.addItem( "a" )
    all.addItem( "b" )
    all.addItem( "c" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
