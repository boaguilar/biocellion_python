#####################################################
# ItemHolder
#####################################################
class ItemHolder:

    def __init__(self, item_class):
        self.mItems = {}
        self.mOrder = []
        self.mIndex = 0 # used for nameless items
        self.mItemClass = item_class
        return

    def getItems( self ):
        return self.mItems

    def getKeys( self ):
        return self.mOrder

    def hasKey( self, name ):
        return name in self.mOrder

    def getItem( self, name ):
        return self.mItems[ name ]

    def addItem( self, name=None, item=None ):
        if name is None:
            name = self.mIndex
            self.mIndex += 1
        if item is None:
            item = self.mItemClass(  )
        self.mItems[ name ] = item
        self.mOrder.append( name )
        return True

    def getInitializeBioModel( self, container_name, indent, depth ):
        lines = [ ]
        for idx in self.mOrder:
            s = self.mItems[ idx ].getInitializeBioModel( container_name, indent, depth )
            if s:
                lines.append( s )
        return "\n".join( lines )

    def getLastItem( self ):
        return self.mItems[ self.mOrder[ len( self.mOrder ) - 1 ] ]

    def __len__( self ):
        return len( self.mItems )

    def __getitem__( self, idx ):
        return self.mItems[ self.mOrder[ idx ] ]

    def __str__( self ):
        s = "<ITEM_HOLDER_ITEMS_" + str( self.mItemClass ) + "\n"

        for name in self.mOrder:
            s += str( self.mItems[ name ] )

        s += "</ITEM_HOLDER_ITEMS_" + str( self.mItemClass ) + "\n"
        return s

    def __repr__( self ):
        return str( self )

    
def main():
    print( "FIXME: no tester for " + str( __file__ ) )
    return
    
if __name__ == "__main__":
    main()
