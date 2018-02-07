from biocell import *

class BioModel:

    def __init__( self ):
        self.mModel = Model( self )
        self.mDistanceJunctionsEnabled = False
        return

    def resetModel( self ):
        self.mModel = Model( self )
        return
    
    def getModel( self ):
        return self.mModel

    def getItemAddIfNeeded( self, tag, name, class_name ):
        return self.mModel.getItemAddIfNeeded( tag, name, class_name )

    def getItem( self, tag, name ):
        return self.mModel.getItem( tag, name )

    def getDistanceJunctionsEnabled( self ):
        return self.mDistanceJunctionsEnabled

    def setDistanceJunctionsEnabled( self, value ):
        self.mDistanceJunctionsEnabled = value
        return

    def getBioModelH( self, indent, depth ):
        return self.mModel.getBioModelH( indent, depth )

    def organizeChildren( self ):
        self.mModel.organizeChildren( )
        return

    def getInitializeBioModel( self, indent, depth ):
        varname_ro = "gBioModel"
        varname = "gBioModelRW"
        lines = []
        lines.append( (depth*indent) + "{" )
        depth += 1
        lines.append( (depth*indent) + "%s = %s = new BioModel( );" % ( varname_ro, varname, ) )
        lines.append( (depth*indent) + "%s->setDistanceJunctionsEnabled( %s );" % ( varname, "true" if self.mDistanceJunctionsEnabled else "false", ) )
        depth -= 1;
        lines.append( (depth*indent) + "}" )

        lines.append( self.mModel.getInitializeBioModel( indent, depth ) )
        return "\n".join( lines )

    def __str__( self ):
        s = "<BioModel>\n"
        s += str( self.mModel ) + "\n"
        s += "</BioModel>"
        return s

    def __repr__( self ):
        return str( self )
    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
