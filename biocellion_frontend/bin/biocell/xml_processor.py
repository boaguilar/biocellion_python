from biocell import *
import xml.etree.ElementTree as ET
import re
    
class XMLProcessor:
    """
    Translates the XML file in input_filename and stores the resulting
    XML in output_filename.  Any tag value or element text the pattern
    VAR_(.*) is replaced with values[ \1 ].
    The construct does all the work, so just instantiate an object to
    get the desired results.
    """

    def __init__( self, input_filename, output_filename, values ):
        self.mTree = None
        self.mRoot = None
        self.mInputFilename = input_filename
        self.mOutputFilename = output_filename
        self.mValues = values
        self.parseXML( )
        self.translateRoot( )
        self.saveXML( )
        return

    def parseXML( self ):
        ## read parameters from input file.
        self.mTree = ET.parse( self.mInputFilename )
        self.mRoot = self.mTree.getroot( )
        return

    def translateRoot( self ):
        for elem in self.mRoot.findall(".//"):
            # s = str( elem.tag )
            for ( name, value ) in elem.items( ):
                match = re.search( '^VAR_(.*)$', value )
                if match and match.group( 1 ) in self.mValues:
                    elem.set( name, str( self.mValues[ match.group( 1 ) ] ) )
                elif match and match.group( 1 ) not in self.mValues:
                    msg = "ERROR: VAR_" + str( match.group( 1 ) )
                    msg += " found in the template. "
                    msg += str( match.group( 1 ) )
                    msg += " is not in the substitution values."
                    raise Exception( msg )
            if elem.text:
                match = re.search( '^VAR_(.*)$', elem.text )
                if match and match.group( 1 ) in self.mValues:
                    elem.text = str( self.mValues[ match.group( 1 ) ] )
                elif match and match.group( 1 ) not in self.mValues:
                    msg = "ERROR: VAR_" + str( match.group( 1 ) )
                    msg += " found in the template. "
                    msg += str( match.group( 1 ) )
                    msg += " is not in the substitution values."
                    raise Exception( msg )
        return
    
    def saveXML( self ):
        fout = open( self.mOutputFilename, 'w' )
        fout.write( prettify( self.mRoot ) )
        fout.close( )
        return

def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
