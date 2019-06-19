/*FILE NAME   : ANIM.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include "ANIM.h"


VOID DD6_AnimInit (HWND hWnd)
{
  DD6_Anim.hWnd = hWnd;
  DD6_RndInit(hWnd);
  TimerInit();
  DD6_InputInit();
}

VOID DD6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DD6_Anim.NumOfUnits; i++)
  {
    DD6_Anim.Units[i]->Close(DD6_Anim.Units[i], &DD6_Anim);
    free(DD6_Anim.Units[i]);
    DD6_Anim.Units[i] = NULL;
  }
  DD6_Anim.NumOfUnits = 0;
  DD6_RndClose();
}

VOID DD6_AnimResize(INT w, INT h)
{
  DD6_Anim.W = w;
  DD6_Anim.H = h;
  DD6_RndResize(w, h);
}

VOID DD6_AnimRender( VOID )
{
  INT i;
  DD6_InputResponse();
  TimerResponse();

  for (i = 0; i < DD6_Anim.NumOfUnits; i++)
    DD6_Anim.Units[i]->Response(DD6_Anim.Units[i], &DD6_Anim);

  DD6_RndStart();
  for (i = 0; i < DD6_Anim.NumOfUnits; i++)
    DD6_Anim.Units[i]->Render (DD6_Anim.Units[i], &DD6_Anim);
  DD6_RndEnd();
 

}

VOID DD6_AnimUnitAdd(dd6UNIT *Uni)
{
  if (DD6_Anim.NumOfUnits < DD6_MAX_UNITS)
    DD6_Anim.Units[DD6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &DD6_Anim); 
}

VOID DD6_AnimCopyFrame(HDC hDC)
{
  DD6_RndCopyFrame(hDC);
}

VOID DD6_AnimEnd( VOID )
{
}

/* End of "ANIM.c" function */
