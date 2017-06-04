from biocell import *

class ComputationDomainBoundaryCondition( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "class", "str", "", True ) )
        self.addAttribute( Param( "name", "str", "", True ) )
        return
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
