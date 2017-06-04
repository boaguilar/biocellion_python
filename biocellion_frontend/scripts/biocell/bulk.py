from biocell import *

class Bulk( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.setPrefix( "BULK" )
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "isConstant", "bool", True, True ) )
        self.addParam( Param( "D", "h-1", 0.0, True ) )
        self.addParam( Param( "updateType", "str", "reaction", False ) )
        
        self.mSolutes = AllBulkSolutes( )
        return

    def getSolutes( self ):
        return self.mSolutes
        
    def getEnumToken(self):
        return "%s_%s" % ( self.getPrefix( ), self.getAttribute( 'name' ).getValue( ), )

    def getSoluteConcentration( self, solute_name ):
        # returns the highest concentration specified for this solute in this bulk
        concentration = -1.0
        if self.mSolutes.hasKey( solute_name ):
            concentration = self.mSolutes.getItem( solute_name ).getParam( 'Sbulk' ).getValue( )
        return concentration

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel(self, indent, depth):
        varname = "bulk"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Bulk *%s = new Bulk(  );" % ( varname, ) )
        lines.append( (depth*indent) + "%s->setName( \"%s\" );" % ( varname, self.getAttribute( 'name' ).getValue( ) ) )
        lines.append( (depth*indent) + "%s->setBulkIdx( %s );" % ( varname, self.getEnumToken( ) ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
        
        container_name = "%s->getSolutes()" % ( varname, )
        s = self.mSolutes.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )

        lines.append( (depth*indent) + "gBioModel->getWorld( ).getBulks( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<bulk" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mSolutes )
        s += "</bulk>\n"
        return s

    def __repr__(self):
        return str(self)
        
class AllBulks( ItemHolder ):

    def __init__(self):
        ItemHolder.__init__( self, Bulk )
        return

    def getSoluteConcentration( self, solute_name ):
        # returns the highest concentration specified for this solute in any bulk
        concentration = -1.0
        for name in self.mOrder:
            concentration = max( concentration,
                                 self.mItems[ name ].getSoluteConcentration( solute_name ) )
        return concentration

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getBulkParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getBulkSoluteParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getBulksEnum( indent, depth ) )
        lines.append( "" )
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getBulkParamNames(self, indent, depth):
        all_params = { }
        all_order = [ ]
        for name in self.mOrder:
            params = self.mItems[ name ].getParams()
            for param_name in params:
                if param_name not in all_params:
                    all_params[ param_name ] = params[ param_name ]
                    all_order.append( param_name )
        lines = []
        for n in all_order:
            s = (depth*indent) + "const std::string %s = \"%s\";" % ( all_params[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )
 
    def getBulkSoluteParamNames(self, indent, depth):
        all_params = { }
        all_order = [ ]
        for name in self.mOrder:
            solutes = self.mItems[ name ].getSolutes( )
            for sname in solutes.getKeys( ):
                params = solutes.getItem( sname ).getParams( )
                for param_name in params:
                    if param_name not in all_params:
                        all_params[ param_name ] = params[ param_name ]
                        all_order.append( param_name )
        lines = []
        for n in all_order:
            s = (depth*indent) + "const std::string %s = \"%s\";" % ( all_params[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )
 
    def getBulksEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _bulk_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_BULKS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} bulk_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )


def main():
    all = AllBulks()
    all.addItem( "a" )
    all.addItem( "b" )
    all.addItem( "c" )
    print(all)
    return
    
if __name__ == "__main__":
    main()
        

