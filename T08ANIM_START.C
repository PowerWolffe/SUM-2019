T08ANIM

Планирование и соглашения:
(VG4_)
функции & глобальные переменные
  VG4_FunctionName

константы
  VG4_CONSTANT_NAME

типы
  vg4TYPE_NAME

{IK3, DP3, EV5, KF5, DD6}

!!!
Все функции/глобальные переменные содержат в качестве первого имени название модуля и т.п.:

  VG4_RndFrameW
  VG4_RndPrimLoad
  VG4_RndPrimDraw  --->  C++: vg4::rnd::prim::Draw()
  VG4_AnimInit
  VG4_TimerInit

17.Rendering System (in 3D)

Rendering pipeline (конвейер вывода)

V – vertex (vertices), I – index (indices)

primitive(V:{x,y,z},I:{#1,#2,3}) ->
  -> triangle sampling {triangles (P012)} ->
    -> vertex transform to NDC {Pi} * WorldCSmatrix * ViewCSmatrix * Projectionmatrix ->
      -> map to screen (viewport transform) ->
        -> rasterize triangles

План реализации
  - Двойная буферизация

данные:
HWND VG4_hWndRnd;
hDC VG4_hDCRndFrame;
HBITMAP VG4_hBmRndFrame;
INT VG4_RndFrameW, VG4_RndFrameH;

функционал:
VOID VG4_RndInit( HWND hWnd );
VOID VG4_RndClose( VOID );
VOID VG4_RndResize( INT W, INT H )
{
  hDC hDC;

  if (VG4_hBmRndFrame != NULL)
    DeleteObject(VG4_hBmRndFrame);

  hDC = GetDC(VG4_hWndRnd);
  VG4_hBmRndFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(VG4_hWndRnd, hDC);
  SelectObject(VG4_hDCRndFrame, VG4_hBmRndFrame);

  VG4_RndFrameW = W;
  VG4_RndFrameH = H;
  VG4_RndProjSet();
}

VOID VG4_RndStart( VOID );
VOID VG4_RndEnd( VOID );

VOID VG4_RndCopyFrame( hDC hDC )
{
  BitBlt(hDC, 0, 0, VG4_RndFrameW, VG4_RndFrameH,
    VG4_hDCRndFrame, 0, 0, SRCCOPY);
}

- Камера (+проецирование)  RND.C

данные:
DBL
  VG4_RndProjSize = 0.1,  /* Project plane fit square */
  VG4_RndProjDist = 0.1,  /* Distance to project plane from viewer (near) */
  VG4_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */

MATR
  VG4_RndMatrView, /* View coordinate system matrix */
  VG4_RndMatrProj, /* Projection coordinate system matrix */
  VG4_RndMatrVP;   /* Stored (View * Proj) matrix */

функционал:

VOID VG4_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = VG4_RndProjSize / 2;
  if (VG4_RndFrameW >= VG4_RndFrameH)
    ratio_x *= (DBL)VG4_RndFrameW / VG4_RndFrameH;
  else
    ratio_y *= (DBL)VG4_RndFrameH / VG4_RndFrameW;
  VG4_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y,
    VG4_RndProjDist, VG4_RndProjFarClip);
  VG4_RndMatrVP = MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj);
}

VOID VG4_RndCamSet( VEC Loc, VEC At, VEC Up1 )
{
  VG4_RndMatrView = MatrView(Loc, At, Up1);
  VG4_RndMatrVP = MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj);
}

использование:

VOID ...Draw...( ... , MATR World )
{
  . . .
  MATR M = MatrMulMatr(World, VG4_RndMatrVP);
  POINT *Proj;
  . . .
  if ((Proj = malloc(sizeof(POINT) * Prim.NumOfV)) == NULL)
    return;
  . . .
    VEC p = VecMulMatr(Prim.V[i], M);

    Proj[i].x = (p.X + 1) * VG4_RndFrameW / 2;
    Proj[i].y = (-p.Y + 1) * VG4_RndFrameH / 2;
  . . .
  free(Proj);
}
-------------
V VectorTransform(V, M) -> (x',y',z',0) =(x,y,z,0) * M --> (x',y',z')
V PointTransform(V, M)  -> (x',y',z',1) =(x,y,z,1) * M --> (x',y',z')
V VecMulMatr(V, M)      -> (x',y',z',w')=(x,y,z,1) * M --> (x'/w',y'/w',z'/w')
-------------

Интеграция RENDER в проект WinAPI:

WinMain:
  создание класса окна
  создание окна
  цикл сообщений (PeekMessage):
    SendMessage(hWnd, WM_TIMER, 47, 0);

WinFunc:
  WM_MINMAXINFO:
    ...
  WM_CREATE:
    VG4_RndInit(hWnd);
    SetTimer(hWnd, 47, 2, NULL);
    return 0;
  WM_SIZE:
    VG4_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  WM_TIMER:
    VG4_RndStart();
    отрисовка сцены
    VG4_RndEnd();
    hDC = GetDC(hWnd);      -- или -- InvalidateRect(hWnd, NULL, FALSE);
    VG4_RndCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  WM_ERASEBKGND:
    return 1;
  WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    VG4_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  WM_DESTROY:
    VG4_RndClose();
    KillTimer(hWnd, 47);
    return 0;

По файлам:
T08ANIM\
  ANIM\
    RND\
      RNDDATA.C – все глобальные переменные
      RNDBASE.C – функции базового набора (камера и двойная буферизация)
      RNDPRIM.C - примитивы
      RND.H     - все объявления (типы, ссылки на глобальные переменные,
                  прототипы)
  MTH\
    MTH.H
  DEF.H         - общий файл определений (#include "mth/mth.h")
  MAIN.C        - entry point

По проекту (логическая структура):
Project:
\Source Files
  \Math Support
    * MTH.H
  \Animation System
    \Rendering System
      * RND.H       --> Rendering system depended
      * RNDDATA.C
      * RNDBASE.C
      * RNDPRIM.C
  \Windows Depended
    * MAIN.C        --> WinMain, WinFunc
  * DEF.H           --> mem.leaks detect, incl."mth.h", + later (base types...)
=========================
!!!
***.h <-- #include "def.h"
