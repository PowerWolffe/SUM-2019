/* FILE NAME   : U_Envi.C
 * PURPOSE     : WinAPI animation system.
 *               Base definitions.
 * PROGRAMMER  : Konstantin Frolov.
 * LAST UPDATE : 10.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include "UNIT.h"

/* Envi unit type */
typedef struct
{
  DD6_UNIT_BASE_FIELDS;
  dd6PRIM Envi;
} dd6UNIT_Envi;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Envi *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitInit( dd6UNIT_Envi *Uni, dd6ANIM *Ani )
{
  dd6IMAGE img;
  dd6MATERIAL mtl = DD6_RndMaterials[0];

 // DD6_RndPrimLoad(&Uni->Envi, "hf/hf.bmp");
  DD6_RndImgLoad(&img, "HF/hf.bmp");
  DD6_RndImgToHF(&img, &Uni->Envi, 50);
  //strcpy();
  mtl.Tex[0] = DD6_RndTexLoad("HF/hftex.bmp");
  Uni->Envi.Mtlno = DD6_RndMtlAdd(&mtl);
  DD6_RndImgFree(&img);
} /* End of 'DD6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Envi *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitClose( dd6UNIT_Envi *Uni, dd6ANIM *Ani )
{
  DD6_RndPrimFree(&Uni->Envi);
} /* End of 'DD6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Envi *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitResponse( dd6UNIT_Envi *Uni, dd6ANIM *Ani )
{

} /* End of 'DD6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_Envi *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitRender( dd6UNIT_Envi *Uni, dd6ANIM *Ani )
{
  DD6_RndPrimDraw(&Uni->Envi, MatrIdentity());
} /* End of 'DD6_UnitRender' function */

/* Envi unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dd6UNIT *) pointer to created unit.
 */
dd6UNIT * DD6_UnitEnviCreate( VOID )
{
  dd6UNIT *Uni;

  /* Memory allocation */
   if ((Uni = DD6_AnimUnitCreate(sizeof(dd6UNIT_Envi))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DD6_UnitInit;
  Uni->Close = (VOID *)DD6_UnitClose;
  Uni->Response = (VOID *)DD6_UnitResponse;
  Uni->Render = (VOID *)DD6_UnitRender;
  return Uni;
} /* End of 'DD6_AnimUnitCreate' function */
