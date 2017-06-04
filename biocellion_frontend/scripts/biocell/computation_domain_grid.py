from biocell import *

class ComputationDomainGrid( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addAttribute( Param( "nDim", "int", 3, True ) )
        self.addAttribute( Param( "nI", "int", 1, True ) )
        self.addAttribute( Param( "nJ", "int", 1, True ) )
        self.addAttribute( Param( "nK", "int", 1, False ) )
        return

    def __str__(self):
        s  = "<grid" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self ) + "\n"
        s += "</grid>\n"
        return s

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
