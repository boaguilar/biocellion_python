from biocell import *
from init_area import Coordinates
#####################################################
#      <entryConditions>
#         <entryCondition type="location" molecule="biomass">
#            <coordinates x="0" y="157.5" z="0"/>
#            <coordinates x="10" y="202.5" z="180"/>
#            <param name="value"> 0.1 </param> 
#            <param name="time" unit="hour">5</param>
#         </entryCondition>
#      </entryConditions>
#####################################################
#####################################################
# entryCondition
#####################################################
class EntryCondition( ParamHolder ):

    def __init__( self ):
        ParamHolder.__init__( self )
        self.addAttribute( Param( "type", "str", "location", True ) )
        self.addAttribute( Param( "molecule", "str", "", True ) )
        self.addParam( Param( "value", "float", 0.0, True ) )
        self.addParam( Param( "timestep","int", 0, True ) )
        self.mCoordinates = ItemHolder( Coordinates )


        self.mPrivateNumberHiddenParams = [ "value", "timestep" ]
        self.mPrivateBoolHiddenParams = [  ]
        self.mPrivateStringHiddenParams = [ "type", "molecule" ]
        self.mPrivateHiddenParams = self.mPrivateNumberHiddenParams + self.mPrivateBoolHiddenParams + self.mPrivateStringHiddenParams
        self.mHiddenParams = self.mHiddenParams + self.mPrivateHiddenParams
        return

    def getCoordinates( self ):
        return self.mCoordinates

    def getBioModelH( self, indent, depth ):
        lines = [ ]
        return "\n".join( lines )

    def getInitializeBioModel( self, parent_varname, container_name, indent, depth ):
        varname = "entry_condition"
        lines = [ ]
        lines.append( (depth*indent) + "{" )
        depth += 1

        lines.append( (depth*indent) + "EntryCondition *%s = new EntryCondition( );" % (varname, ) )
        lines.append( ParamHolder.getInitializeBioModel( self, varname, indent, depth ) )
        s = self.getInitializeBioModelSetDataMembers( varname, "->", indent, depth,
                                                      self.mPrivateBoolHiddenParams,
                                                      self.mPrivateNumberHiddenParams,
                                                      self.mPrivateStringHiddenParams )
        lines.append( s )
        
        s = (depth*indent) + "%s%sset%s( %s );" % (varname, "->", "AgentSpecies", parent_varname, )
        lines.append( s )
            
        coordinate_container_name = "%s->getCoordinates()" % ( varname, )
        for i in range( len( self.mCoordinates ) ):
            lines.append( self.mCoordinates[ i ].getInitializeBioModel( coordinate_container_name, indent, depth ) )
        
        lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )
        return "\n".join( lines )
    
    def __str__(self):
        s  = "<entryCondition" + self.formatAttributes() + ">\n"
        s += ParamHolder.__str__( self )
        s += str( self.mCoordinates )
        s += "</entryCondition>\n"
        return s

    def __repr__(self):
        return str(self)

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
