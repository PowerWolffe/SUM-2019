/*FILE NAME   : U_CTRL.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include <stdio.h>
#include "UNIT.h"

/* Text unit type */
typedef struct
{
  DD6_UNIT_BASE_FIELDS;
} dd6UNIT_Text;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Text *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitInit( dd6UNIT_Text *Uni, dd6ANIM *Ani )
{
  HFONT hFnt, hOldFnt;

  hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH | FF_SCRIPT, "Bookman Old Style");
  hOldFnt = SelectObject(DD6_Anim.hDC, hFnt);
  wglUseFontBitmaps(DD6_Anim.hDC, 0, 256, 102);
  hOldFnt = SelectObject(DD6_Anim.hDC, hOldFnt);
  DeleteObject(hFnt);
} /* End of 'DD6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Text *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitClose( dd6UNIT_Text *Uni, dd6ANIM *Ani )
{
} /* End of 'DD6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Text *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitResponse( dd6UNIT_Text *Uni, dd6ANIM *Ani )
{
} /* End of 'DD6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Text *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitRender(dd6UNIT_Text *Uni, dd6ANIM *Ani )
{ 
  char Buf[100];
  int len;

  glLoadIdentity();
  glColor3d(1, 1, 1);
  glRasterPos2d(-0.9, 0.9);
  glListBase(102);
  len = sprintf(Buf, "FPS: %.3f, primitives: %i, triangles: %i", Ani->FPS);
  glCallLists(len, GL_UNSIGNED_BYTE, Buf);
} /* End of 'DD6_UnitRender' function */

/* Text unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dd6UNIT *) pointer to created unit.
 */
dd6UNIT * DD6_UnitTextCreate( VOID )
{
  dd6UNIT *Uni;

  /* Memory allocation */
  if ((Uni = DD6_AnimUnitCreate(sizeof(dd6UNIT_Text))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DD6_UnitInit;
  Uni->Close = (VOID *)DD6_UnitClose;
  Uni->Response = (VOID *)DD6_UnitResponse;
  Uni->Render = (VOID *)DD6_UnitRender;
  return Uni;
} /* End of 'DD6_AnimUnitCreate' function */

