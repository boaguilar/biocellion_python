from agent_species import Param, ParamHolder
from bulk import AllBulks
from agar import AllAgars
from computation_domain import AllComputationDomains

class World( ParamHolder ):

    def __init__(self):
        self.mName = "world"
        ParamHolder.__init__( self )
        self.mBulks = AllBulks()
        self.mAgars = AllAgars()
        self.mComputationDomains = AllComputationDomains()
        return

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        lines.append( "// FIXME: World" )
        lines.append( self.mBulks.getBioModelH( indent, depth ) )
        lines.append( self.mAgars.getBioModelH( indent, depth ) )
        lines.append( self.mComputationDomains.getBioModelH( indent, depth ) )
        return "\n".join( lines )

    def getInitializeBioModel( self, indent, depth ):
        lines = [ ]
        lines.append( "// FIXME: World" )
        lines.append( self.mBulks.getInitializeBioModel( indent, depth ) )
        lines.append( self.mAgars.getInitializeBioModel( indent, depth ) )
        lines.append( self.mComputationDomains.getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def getName( self ):
        return self.mName

    def getBulks( self ):
        return self.mBulks
        
    def getAgars( self ):
        return self.mAgars
        
    def getComputationDomains( self ):
        return self.mComputationDomains

    def makeDomainDimensionValid( self, x ):
        refine_ratio = 2 #FIXME this should be pulled from somewhere else
        interface_grid_level = 3 #FIXME this should be pulled from somewhere else
        print( "FIXME: refine_ratio and interface_grid_level should be read from somewhere, not hard-coded" )
        amr_multiple = refine_ratio ** interface_grid_level
        if x < 4:
            x = 4
        if x < 2 * amr_multiple:
            x = 2 * amr_multiple
        if x % amr_multiple != 0:
            x = ( ( x / amr_multiple ) + 1 ) * amr_multiple
        return x

    def getDomainPartitionSize( self ):
        if len( self.mComputationDomains ) == 0:
            return 0
        partition_size = self.makeDomainDimensionValid( 1 )
        if partition_size % 2 == 0:
            partition_size = int( partition_size / 2 )
        return partition_size
    
    def getDomainXYZ( self ):
        if len( self.mComputationDomains ) == 0:
            return 0
        d = self.mComputationDomains[ 0 ]
        nDim = d.getGrid( ).getAttribute( "nDim" ).getValue( )
        nI = d.getGrid( ).getAttribute( "nI" ).getValue( )
        nJ = d.getGrid( ).getAttribute( "nJ" ).getValue( )
        nK = d.getGrid( ).getAttribute( "nK" ).getValue( )
        if nDim < 2:
            nJ = 1
        if nDim < 3:
            nK = 1
        x = self.makeDomainDimensionValid( nI )
        y = self.makeDomainDimensionValid( nJ )
        z = self.makeDomainDimensionValid( nK )
        return ( x, y, z )
    
    def __str__(self):
        s  = "<%s>\n" % (self.mName)
        s += ParamHolder.__str__( self )
        s += str( self.mBulks ) + "\n"
        s += str( self.mAgars ) + "\n"
        s += str( self.mComputationDomains ) + "\n"
        s += "</%s>\n" % (self.mName)
        return s

def main():
    print("UNCHECKED")
    return
    
if __name__ == "__main__":
    main()
        

