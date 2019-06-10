/* FILE NAME   : RND.H
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 10.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

LONG StartTime;
LONG OldTime; 
LONG PauseTime; 
LONG OldFPSTime; 
LONGFrameCount;
DOUBLE GLB_Time;
DOUBLE GLB_DeltaTime;
DOUBLE GLB_FPS;
BOOL GLB_IsPause;
VOID GLB_TimerInit( VOID );
VOID GLB_TimerResponse( VOID );
HWND VG4_hWndRnd;
hDC VG4_hDCRndFrame;
HBITMAP VG4_hBmRndFrame;
INT VG4_RndFrameW, VG4_RndFrameH;
