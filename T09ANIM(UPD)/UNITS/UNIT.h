/*FILE NAME   : UNIT.H
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */


#ifndef __UNITS_H_
#define __UNITS_H_

#include "../ANIM/ANIM.h"

/* Sherman unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dd6UNIT *) pointer to created unit.
 */
dd6UNIT * DD6_UnitShermanCreate( VOID );
VOID FlipFullScreen( HWND hWnd );
VEC DD6_CAM( VEC Pos );


#endif 