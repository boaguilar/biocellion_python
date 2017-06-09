from biocell import *

class Yields( ParamHolder ):
    
    def __init__( self ):
        ParamHolder.__init__( self )
        self.addParam( Param( "*", "g.g-1", 0.0, False ) )
        self.mSolutes = { }
        self.mParticles = { }
        self.mItems = { }
        return

    def isSolute( self, name ):
        return name in self.mSolutes

    def getSolute( self, name ):
        if self.isSolute( name ):
            return self.mSolutes[ name ]
        return None

    def setSolute( self, name, solute ):
        self.mSolutes[ name ] = solute
        self.mItems[ name ] = solute
        return

    def setParticle( self, name, particle ):
        self.mParticles[ name ] = particle
        self.mItems[ name ] = particle
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

            lines.append( (depth*indent) + "%s.setItemIdx( %s );" % ( varname, self.mItems[ n ].getEnumToken( ) ) )
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
