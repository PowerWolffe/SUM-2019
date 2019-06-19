/*FILE NAME   : UNITS.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */
#include "../DEF.h"
#include "ANIM.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitInit( dd6UNIT *Uni, dd6ANIM *Ani )
{
} /* End of 'DD6_UnitInit' function */
static VOID DD6_UnitClose( dd6UNIT *Uni, dd6ANIM *Ani )
{

} /* End of 'DD6_UnitClose' function */
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitResponse( dd6UNIT *Uni, dd6ANIM *Ani )
{


} /* End of 'DD6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitRender( dd6UNIT *Uni, dd6ANIM *Ani )
{
} /* End of 'DD6_UnitRender' function */


/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dd6UNIT *) pointer to created unit.
 */
dd6UNIT * DD6_AnimUnitCreate( INT Size )
{
  dd6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(dd6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = DD6_UnitInit;
  Uni->Close = DD6_UnitClose;
  Uni->Response = DD6_UnitResponse;
  Uni->Render = DD6_UnitRender;
  return Uni;
} /* End of 'DD6_AnimUnitCreate' function */

/* End of "UNITS.c" function */