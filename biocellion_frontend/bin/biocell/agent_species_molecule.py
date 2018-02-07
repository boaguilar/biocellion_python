from biocell import *

#####################################################
# AgentSpeciesMolecule
#####################################################
class AgentSpeciesMolecule( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )        
        self.addAttribute( Param( "name", "str", "", True ) )
        self.addParam( Param( "concentration", "g.L-1", 0.0, True ) )
        return

    def getMoleculeEnumToken( self ):
        if self.mReference is None:
            raise Exception( "ERROR: <species><molecule> must be linked to <model><molecule>" )
        return self.mReference.getEnumToken( )

    def __str__(self):
        s  = "<species-molecule" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</species-molecule>\n"
        return s

    def __repr__(self):
        return str(self)
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
