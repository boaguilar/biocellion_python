from biocell import *

class Solver( ParamHolder ):

    def __init__( self, name ):
        self.mName = name
        self.mEnumToken = "SOLVER_%s" % ( name, )
        
        ParamHolder.__init__(self)
        self.setPrefix( "SOLVER" )
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "domain", "str", "", True ) )
        self.addParam( Param( "active", "bool", False, True ) )
        self.addParam( Param( "preStep", "int", 1, True ) )
        self.addParam( Param( "postStep", "int", 1, True ) )
        self.addParam( Param( "coarseStep", "int", 1, True ) )
        self.addParam( Param( "nCycles", "int", 1, True ) )

        self.mPrivateTotallyHiddenParams = [ ] 
        self.mPrivateNumberHiddenParams = [  ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "name", "class", "domain" ]
        self.mPrivateHiddenParams = self.mPrivateTotallyHiddenParams + self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        
        self.mReactions = AllSolverReactions( )
        return

    def getName(self):
        return self.mName

    def getEnumToken(self):
        return self.mEnumToken

    def getReactions( self ):
        return self.mReactions
    
    def getInitializeBioModel(self, indent, depth):
        varname = "solver"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "Solver *%s = new Solver(  );" % ( varname, ) )
        lines.append( (depth*indent) + "%s->setSolverIdx( %s );" % ( varname, self.getEnumToken( ) ) )
        s = ParamHolder.getInitializeBioModel( self, varname, indent, depth )
        if s:
            lines.append( s )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        container_name = "%s->getReactions()" % ( varname, )
        s = self.mReactions.getInitializeBioModel( container_name, indent, depth )
        if s:
            lines.append( s )
        
        lines.append( (depth*indent) + "gBioModelRW->getSolvers( ).push_back( %s );" % ( varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<solver" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mReactions )
        s += "</solver>\n"
        return s

    def __repr__(self):
        return str(self)


class AllSolvers( ItemHolder ):

    def __init__( self, model ):
        ItemHolder.__init__( self, Solver )
        self.mModel = model
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( self.getAllParamNames( indent, depth ) )
        lines.append( "" )
        lines.append( self.getSolversEnum( indent, depth ) )
        return "\n".join( lines )

    def getSolversEnum(self, indent, depth):
        lines = []
        lines.append( (depth*indent) + "typedef enum _solver_type_e {" )
        depth += 1
        for name in self.mOrder:
            s = (depth*indent) + "%s," % (self.mItems[ name ].getEnumToken(), )
            lines.append( s )
        s = (depth*indent) + "NUM_SOLVERS"
        lines.append( s )
        depth -= 1
        lines.append( (depth*indent) + "} solver_type_e;" )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = []
        for name in self.mOrder:
            lines.append( self.mItems[ name ].getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def addItem( self, name, item=None ):
        if item is None:
            item = self.mItemClass( name )
        return ItemHolder.addItem( self, name, item )

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
