#ifndef _BIOMODEL_DEBUG_H_
#define _BIOMODEL_DEBUG_H_


#define BMD_NONE                     ( 0x0000 )
#define BMD_KINETIC_FACTOR           ( 0x0001 )
#define BMD_KINETIC_FACTOR_DETAIL    ( 0x0002 )
#define BMD_SOLUTE_SECRETION         ( 0x0004 )
#define BMD_SOLUTE_SECRETION_SUMMARY ( 0x0008 )
#define BMD_INTERACTION_OUTPUT       ( 0x0010 )
#define BMD_TIGHT_JUNCTION           ( 0x0020 )
#define BMD_CRNODERHS_DETAILS        ( 0x0040 )
#define BMD_AVAILABLE0               ( 0x0080 )
#define BMD_SOLUTE_BETA              ( 0x0100 )
#define BMD_SOLUTE_ALPHA             ( 0x0200 )
#define BMD_SOLUTE                   ( 0x0400 )
#define BMD_AVAILABLE1               ( 0x0800 )
#define BMD_AVAILABLE2               ( 0x1000 )
#define BMD_AVAILABLE3               ( 0x2000 )
#define BMD_AVAILABLE4               ( 0x4000 )
#define BMD_AVAILABLE5               ( 0x8000 )

#ifndef BIOMODEL_DEBUG_LEVEL
#define BIOMODEL_DEBUG_LEVEL ( /* BMD_SOLUTE | BMD_CRNODERHS_DETAILS | */ 0 | 0 )
#endif /* BIOMODEL_DEBUG */

#define BMD_DO_DEBUG( x ) ( ( BIOMODEL_DEBUG_LEVEL & ( x ) ) != BMD_NONE )

#endif /* _BIOMODEL_DEBUG_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
