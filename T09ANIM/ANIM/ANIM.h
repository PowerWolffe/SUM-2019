/*FILE NAME   : ANIM.h
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "rnd/rnd.h"

#define DD6_MAX_UNITS 3000

#define DD6_UNIT_BASE_FIELDS \
  VOID (*Init)( dd6UNIT *Uni, dd6ANIM *Ani );      \
  VOID (*Close)( dd6UNIT *Uni, dd6ANIM *Ani );     \
  VOID (*Response)( dd6UNIT *Uni, dd6ANIM *Ani );  \
  VOID (*Render)( dd6UNIT *Uni, dd6ANIM *Ani )


typedef struct tagdd6UNIT dd6UNIT;
typedef struct tagdd6ANIM
{
  HWND hWnd;
  HDC hDC;
  HGLRC hRC;
  INT W, H;
  dd6UNIT *Units[DD6_MAX_UNITS];
  INT NumOfUnits;

  BYTE KeysClick[256], Keys[256];

  BYTE
    JBut[32], JButClick[32];
  INT JPov;

  DBL
    JX, JY, JZ, JR;

  DBL
    GlobalTime, GlobalDeltaTime, 
    Time, DeltaTime, FPS;

  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  BOOL IsPause;
} dd6ANIM;

struct tagdd6UNIT
{
  DD6_UNIT_BASE_FIELDS;
};

typedef struct tagUNIT_COW UNIT_COW;


extern dd6ANIM DD6_Anim;
extern INT DD6_MouseWheel;

VOID DD6_AnimInit (HWND hWnd);
VOID DD6_AnimClose( VOID );

VOID DD6_AnimResize(INT w, INT h);
VOID DD6_AnimRender( VOID );
VOID DD6_AnimCopyFrame(HDC hDC);
VOID DD6_AnimEnd( VOID );

VOID DD6_AnimUnitAdd(dd6UNIT *Uni);

dd6UNIT * DD6_AnimUnitCreate( INT Size );

VOID TimerInit( VOID );

VOID TimerResponse( VOID );

VOID DD6_InputInit( VOID );

VOID DD6_InputResponce( VOID );



#endif
/* End of "ANIM.h" function */