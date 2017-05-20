from agent_species import Param, ParamHolder

class AgentGrid( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__(self)
        self.addParam( Param( "computationDomain", "str", "" ) )
        self.addParam( Param( "resolution", "um", 1.0 ) )
        self.addParam( Param( "shovingFraction", "float", 0.0025 ) )
        self.addParam( Param( "shovingMaxIter", "int", 1 ) )
        self.addParam( Param( "shovingMutual", "bool", True ) )
        self.addParam( Param( "erosionMethod", "bool", True ) )
        self.addParam( Param( "sloughDetachedBiomass", "bool", False ) )
        return

    def getName(self):
        return self.mName

    def __str__(self):
        s  = "<agentGrid" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += "</agentGrid>\n"
        return s

    def __repr__(self):
        return str(self)
        
def main():
    print("Untested")
    return
    
if __name__ == "__main__":
    main()
        

