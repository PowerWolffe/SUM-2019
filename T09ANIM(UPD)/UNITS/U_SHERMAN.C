  /*FILE NAME   : U_SHERMAN.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 18.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include "UNIT.h"

/* Sherman unit type */
typedef struct
{
  DD6_UNIT_BASE_FIELDS;
  dd6PRIMS Sherman;
  dd6PRIMS Prim;
  VEC Pos;
  VEC CamLoc;
  VEC CamDir;
  FLT A, B, C, D;
} dd6UNIT_SHERMAN;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_SHERMAN *Uni;
 *   - animation context:
 *      dd6ANIM *Ani;
 * RETURNS: None.
 */

static VOID DD6_UnitInit( dd6UNIT_SHERMAN *Uni, dd6ANIM *Ani )
{
  Uni->Pos = VecSet(1000, 10, 200); 
  DD6_RndPrimsLoad(&Uni->Sherman, "ShermanNew.g3dm");
} /* End of 'DD6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_SHERMAN *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitClose( dd6UNIT_SHERMAN *Uni, dd6ANIM *Ani )
{
  DD6_RndPrimsFree(&Uni->Sherman);
} /* End of 'DD6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:                  
 *       dd6UNIT_SHERMAN *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitResponse( dd6UNIT_SHERMAN *Uni, dd6ANIM *Ani )
{    
  Uni->Sherman.Trans = MatrScale(VecSet(0.9, 0.9, 0.9));
  /* Uni->Sherman.Trans = MatrMulMatr(Uni->Sherman.Trans, MatrRotateY(180 * Ani->JR ));
  Uni->Sherman.Trans = MatrMulMatr(Uni->Sherman.Trans, MatrRotateX(180 * Ani->JY )); */
  
} /* End of 'DD6_UnitResponse' function */

/* Unit render function
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dd6UNIT_SHERMAN *Uni;
 *   - animation context:
 *       dd6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DD6_UnitRender( dd6UNIT_SHERMAN *Uni, dd6ANIM *Ani )
{
  MATR m, m1;

  VEC HeadV = VecSet(0, 0, 1);
  VEC HVec = VecSet(0, 1, 0);
  FLT 
    //Head = Ani->JR * 20, 
    Vel = Ani->JY * -0.2;
  Uni->B += -Ani->JR * 0.5;
  HeadV = VecMulMatr(HeadV, MatrRotateY(Uni->B));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(HeadV, Vel));      

  glColor3d(0.1, 0, 0.5);
  m = MatrMulMatr3(MatrScale(VecSet(2, 2, 2)),
                   MatrRotateY(Uni->B),
                   MatrRotateX(0));

  if (DD6_Anim.JBut[7] && DD6_Anim.JButClick[7])
    Uni->C -= 5;
  if (DD6_Anim.JBut[5] && DD6_Anim.JButClick[5])
    Uni->C += 5;
  DD6_RndCamSet(VecSubVec(VecSubVec(Uni->Pos, VecMulMatr(VecMulNum(HeadV, 80), 
                MatrRotateY(Uni->C ))), VecMulNum(HVec, -40)), 
                Uni->Pos, VecSet(0, 1, 0));

  m1 = MatrMulMatr(m, MatrTranslate(Uni->Pos));
  //if (IK3_Anim.JBut[6]) 
    //IK3_Anim.IsPause = !IK3_Anim.IsPause;
  //if (IK3_Anim.JBut[8])
    //DestroyWindow(IK3_Anim.hWnd);
  //if (IK3_Anim.JBut[9])
    //FlipFullScreen(IK3_Anim.hWnd);
  if (DD6_Anim.Keys['F'] &&DD6_Anim.KeysClick['F'])
    FlipFullScreen(DD6_Anim.hWnd);
  if (DD6_Anim.JBut[3]) 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (DD6_Anim.JBut[0])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //if (IK3_Anim.Keys[VK_LBUTTON])
   // World = MatrScale(VecSet(0.1, 0.1, 0.1));
  //if (IK3_Anim.Keys[VK_RBUTTON])
   // World = MatrScale(VecSet(2, 2, 2));
  DD6_RndPrimsDraw(&Uni->Sherman, m1);
} /* End of 'DD6_UnitRender' function */
/* Cow unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dd6UNIT *) pointer to created unit.
 */

dd6UNIT * DD6_UnitShermanCreate( VOID )
{
  dd6UNIT *Uni;
  /* Memory allocation */
  if ((Uni = DD6_AnimUnitCreate(sizeof(dd6UNIT_SHERMAN))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)DD6_UnitInit;
  Uni->Close = (VOID *)DD6_UnitClose;
  Uni->Response = (VOID *)DD6_UnitResponse;
  Uni->Render = (VOID *)DD6_UnitRender;
  return Uni;
} /* End of 'DD6_AnimUnitCreate' function */