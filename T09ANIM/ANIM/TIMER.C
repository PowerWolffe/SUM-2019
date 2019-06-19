 /*FILE NAME   : TIMER.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 10.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */
#include "ANIM.h"

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

VOID TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  DD6_Anim.IsPause = FALSE;
  DD6_Anim.FPS = 30.0;
  PauseTime = 0;
}

VOID TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  DD6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  DD6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (DD6_Anim.IsPause)
  {
    DD6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    DD6_Anim.DeltaTime = DD6_Anim.GlobalDeltaTime;
    DD6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    DD6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}
