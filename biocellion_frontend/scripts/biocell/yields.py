from biocell import *

class Yields( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__( self )
        self.addParam( Param( "*", "g.g-1", 0.0, False ) )
        self.mSolutes = [ ]
        self.mParticles = [ ]
        self.mIdxs = { }
        return

    def setSolute( self, name, idx ):
        self.mSolutes.append( name )
        self.mIdxs[ name ] = idx
        return

    def setParticle( self, name, idx ):
        self.mParticles.append( name )
        self.mIdxs[ name ] = idx
        return
    
    def getKeys( self ):
        return self.getParamKeys( )

    def getItem( self, name ):
        return self.getParam( name )

    def getInitializeBioModel(self, container_name, indent, depth):
        varname = "yield"
        lines = []
        for n in self.mParams:
            if n == '*':
                continue
            
            param = self.mParams[ n ]
            lines.append( (depth*indent) + "{" )
            depth += 1
            lines.append( (depth*indent) + "Reaction::Yield %s;" % (varname, ) )
            if n in self.mSolutes:
                lines.append( (depth*indent) + "%s.setSolute( );" % (varname, ) )
            elif n in self.mParticles:
                lines.append( (depth*indent) + "%s.setParticle( );" % (varname, ) )
            else:
                raise Exception( "ERROR: Yield must be solute or particle" )

            lines.append( (depth*indent) + "%s.setItemIdx( %s );" % ( varname, self.mIdxs[ n ] ) )
            lines.append( (depth*indent) + "%s.setValue( %s );" % ( varname, param.getValue( ) ) )
            
            lines.append( (depth*indent) + "%s.push_back( %s );" % (container_name, varname, ) )
            depth -= 1
            lines.append( (depth*indent) + "}" )
            

        return "\n".join( lines )


def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
