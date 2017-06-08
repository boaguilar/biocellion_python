from biocell import *

#####################################################
# ParamHolder
#####################################################
class ParamHolder:

    def __init__(self):
        self.mAttributes = {  }
        self.mAttributeOrder = [ ]
        self.mParams = {  }
        self.mParamOrder = [ ]
        self.mChildren = {  }
        self.mHiddenParams = [ ]
        self.mPrefix = ""
        self.mReference = None
        return

    def getPrefix( self ):
        return self.mPrefix

    def setPrefix( self, prefix ):
        self.mPrefix = prefix
        for n in self.mAttributes:
            self.mAttributes[ n ].setPrefix( prefix )
        for n in self.mParams:
            self.mParams[ n ].setPrefix( prefix )
        return

    def getReference( self ):
        return self.mReference

    def setReference( self, value ):
        self.mReference = value
        return

    def addChild( self, child ):
        t = child.__class__.__name__
        if t not in self.mChildren:
            self.mChildren[ t ] = [  ]
        self.mChildren[ t ].append( child )
        return

    def countUnits(self, units):
        count = 0
        for n in self.mParams:
            param = self.mParams[ n ]
            if param.getUnit() in units:
                count += 1
        return count
        
    def countReal(self):
        return self.countUnits( Param.mRealUnits )
        
    def countInt(self):
        return self.countUnits( Param.mIntUnits )
        
    def countBool(self):
        return self.countUnits( Param.mBoolUnits )
        
    def countString(self):
        return self.countUnits( Param.mStringUnits )

    def getMayAttributes( self ):
        names = [ ]
        for n in self.mAttributes:
            names.append( n )
        return names
        
    def getRequiredAttributes( self ):
        names = [ ]
        for n in self.mAttributes:
            if self.mAttributes[ n ].isRequired( ):
                names.append( n )
        return names
        
    def getMayParams( self ):
        names = [ ]
        for n in self.mParams:
            names.append( n )
        return names
        
    def getRequiredParams( self ):
        names = [ ]
        for n in self.mParams:
            if self.mParams[ n ].isRequired( ):
                names.append( n )
        return names
        
    def paramNamesToCpp(self, indent, depth):
        lines = []
        for n in self.mParamOrder:
            if n in self.mHiddenParams:
                continue
            s = (depth*indent) + "const std::string %s = \"%s\";" % (self.mParams[ n ].getConstName( ), n, )
            lines.append( s )
        return "\n".join( lines )
        
    def getInitializeBioModel(self, varname, indent, depth):
        lines = []
        for n in self.mParamOrder:
            if n in self.mHiddenParams:
                continue
            param = self.mParams[ n ]
            if param.getUnit() in Param.mStringUnits:
                s = (depth*indent) + "%s->setParamString( %s->getIdxString( %s ), \"%s\" );" % (varname, varname, param.getConstName( ), param.getValue( ), )
                lines.append( s )
            elif param.getUnit() in Param.mBoolUnits:
                s = (depth*indent) + "%s->setParamBool( %s->getIdxBool( %s ), %s );" % (varname, varname, param.getConstName( ), "true" if param.getValue( ) else "false", )
                lines.append( s )
            elif param.getUnit() in Param.mIntUnits:
                s = (depth*indent) + "%s->setParamInt( %s->getIdxInt( %s ), %s );" % (varname, varname, param.getConstName( ), param.getValue( ), )
                lines.append( s )
            elif param.getUnit() in Param.mRealUnits:
                s = (depth*indent) + "%s->setParamReal( %s->getIdxReal( %s ), %s );" % (varname, varname, param.getConstName( ), param.getValue( ), )
                lines.append( s )
            else:
                s = "FIXME: Unknown unit: " + str( param.getUnit( ) )
                lines.append( s )
                raise Exception( "ERROR: " + s )
        return "\n".join( lines )

    def getParamOrAttribute( self, name ):
        if name in self.mParams:
            return self.mParams[ name ]
        elif name in self.mAttributes:
            return self.mAttributes[ name ]
        return None

    def getInitializeBioModelSetDataMembers( self, varname, connector, indent, depth, bools, numbers, strings ):
        lines = [ ]

        for name in bools:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( %s );" % (varname, connector, Name, "true" if self.getParamOrAttribute( name ).getValue() else "false", )
            lines.append( s )
            
        for name in numbers:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( %s );" % (varname, connector, Name, self.getParamOrAttribute( name ).getValue(), )
            lines.append( s )
            
        for name in strings:
            Name = name[ 0 ].upper() + name[ 1 : ]
            s = (depth*indent) + "%s%sset%s( \"%s\" );" % (varname, connector, Name, self.getParamOrAttribute( name ).getValue(), )
            lines.append( s )
        
        return "\n".join( lines )

    def getAttributes(self):
        return self.mAttributes

    def getAttribute( self, attr ):
        return self.mAttributes[ attr ]

    def addAttribute(self, attr):
        n = attr.getName()
        if n in self.mAttributes:
            return False
        else:
            self.mAttributeOrder.append( n )
            self.mAttributes[ n ] = attr.copy()
            self.mAttributes[ n ].setPrefix( self.getPrefix( ) )
        return

    def updateAttribute(self, attr):
        n = attr.getName()
        if n in self.mAttributes:
            if self.mAttributes[ n ].setValueFromParam( attr ):
                return True
            elif ( ( self.mAttributes[ n ].getUnit() in Param.mRealUnits ) and 
                   ( ( attr.getUnit() in Param.mIntUnits ) or ( attr.getUnit() == "float" ) ) ):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                # maybe mis-classified as a float, instead of the united type, because it's an attribute.
                attr = attr.copy()
                attr.setUnit( self.mAttributes[ n ].getUnit() )
                return self.mAttributes[ n ].setValueFromParam( attr )
            else:
                return False
        else:
            return False

    def validateAttribute( self, attr ):
        n = attr.getName()
        if n in self.mAttributes:
            if self.mAttributes[ n ].validateParam( attr ):
                return True
            elif ( ( self.mAttributes[ n ].getUnit() in Param.mRealUnits ) and 
                   ( ( attr.getUnit() in Param.mIntUnits ) or ( attr.getUnit() == "float" ) ) ):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                # maybe mis-classified as a float, instead of the united type, because it's an attribute.
                attr = attr.copy()
                attr.setUnit( self.mAttributes[ n ].getUnit() )
                return self.mAttributes[ n ].validateParam( attr )
            else:
                return False
        else:
            return False

    def getParams(self):
        return self.mParams

    def getParamKeys( self ):
        return self.mParamOrder

    def paramExists( self, name ):
        return name in self.mParams
    
    def getParam( self, name ):
        return self.mParams[ name ]
    
    def addParam(self, param):
        n = param.getName()
        if n in self.mParams:
            return False
        else:
            self.mParamOrder.append( n )
            self.mParams[n] = param.copy()
            self.mParams[ n ].setPrefix( self.getPrefix( ) )
        return

    def updateParam(self, param):
        n = param.getName()
        
        if ( n not in self.mParams ) and ( '*' in self.mParams ):
            p = self.mParams[ '*' ].copy( )
            p.setName( param.getName( ) )
            self.addParam( p )

        if n in self.mParams:
            if self.mParams[ n ].setValueFromParam( param ):
                return True
            if ( self.mParams[ n ].getUnit() in Param.mRealUnits ) and ( param.getUnit() in Param.mIntUnits ):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                param = param.copy()
                param.setUnit( self.mParams[ n ].getUnit() )
                return self.mParams[ n ].setValueFromParam( param )
            else:
                return False
        else:
            return False

    def validateParam( self, param ):
        n = param.getName()
        
        if ( n not in self.mParams ) and ( '*' in self.mParams ):
            p = self.mParams[ '*' ].copy( )
            p.setName( param.getName( ) )
            self.addParam( p )

        if n in self.mParams:
            if self.mParams[n].validateParam( param ):
                return True
            elif (self.mParams[n].getUnit() in Param.mRealUnits) and (param.getUnit() in Param.mIntUnits):
                # maybe mis-classified a float type as an int type due to value. Try it as a float type.
                param = param.copy()
                param.setUnit( self.mParams[n].getUnit() )
                return self.mParams[n].validateParam( param )
            else:
                return False
        else:
            return False

    def formatAttributes( self ):
        s = ''
        for a in self.mAttributeOrder:
            s += ' %s="%s"' % ( self.mAttributes[ a ].getName(), self.mAttributes[ a ].getValue() )
        return s

    def __str__(self):
        s = ""
        for p in self.mParams:
            s += "\t" + str( self.mParams[ p ] ) + "\n"
        if len( self.mChildren ) > 0:
            s += "\t<children>\n"
            for t in self.mChildren:
                s += "\t\ttype: " + t + "\n"
                for c in self.mChildren[ t ]:
                    s += "\t\t" + str( c ) + "\n"
            s += "\t</children>"
        return s

    def __repr__(self):
        return str(self)


    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
