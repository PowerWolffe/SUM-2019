/*FILE NAME   : INPUT.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include "ANIM.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define DD6_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

BYTE KeysOld[256],JButOld[32]; 


static VOID DD6_JoystickRespons( VOID );
static VOID KeyboardRespons( VOID );   

/*-------------------------------*/

VOID KeyboardInit( VOID )
{
  memset(DD6_Anim.Keys, 0, 256);
  memset(KeysOld, 0, 256);
  memset(DD6_Anim.KeysClick, 0, 256);
}

/*------------------------------*/


VOID MouseInit( VOID )
{
  DD6_Anim.Mx = 0;
  DD6_Anim.My = 0;
  DD6_Anim.Mz = 0;
  DD6_Anim.Mdx = 0;
  DD6_Anim.Mdy = 0; 
  DD6_Anim.Mdz = 0;
}

/*---------------------------*/

VOID DD6_InputInit( VOID )
{
  MouseInit();
  KeyboardInit();
  //JoystickInit();
}  

/*=========================*/

VOID DD6_InputResponse( VOID )
{
  DD6_JoystickRespons();
  KeyboardRespons();
  //JoystickInit();
}

/*===========================*/

static VOID KeyboardRespons( VOID )
{
  INT i;

  GetKeyboardState(DD6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {                                                                
    DD6_Anim.Keys[i] >>= 7;
    DD6_Anim.KeysClick[i] = DD6_Anim.Keys[i] && !KeysOld[i];  
  }
  memcpy(KeysOld, DD6_Anim.Keys, 256);
}

/*==================================*/
static VOID DD6_JoystickRespons( VOID )
{
  INT i;
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          DD6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          DD6_Anim.JButClick[i] = DD6_Anim.JBut[i] && !JButOld[i];
          JButOld[i] = DD6_Anim.JBut[i];
        }

        /* Axes */
        DD6_Anim.JX = DD6_GET_JOYSTIC_AXIS(X);
        DD6_Anim.JY = DD6_GET_JOYSTIC_AXIS(Y);
        DD6_Anim.JZ = DD6_GET_JOYSTIC_AXIS(Z);
        DD6_Anim.JR = DD6_GET_JOYSTIC_AXIS(R);

        /* Point of view */
        DD6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}
/*=========================================*/

static VOID MouseRespons( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(DD6_Anim.hWnd, &pt);

  DD6_Anim.Mdx = pt.x - DD6_Anim.Mx;
  DD6_Anim.Mdy = pt.y - DD6_Anim.My;
  DD6_Anim.Mdz = DD6_MouseWheel;
  DD6_Anim.Mz += DD6_MouseWheel;
  DD6_MouseWheel = 0;
  DD6_Anim.Mx = pt.x;
  DD6_Anim.My = pt.y;

}


