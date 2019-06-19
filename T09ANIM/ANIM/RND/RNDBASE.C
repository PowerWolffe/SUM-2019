/* FILE NAME   : RNDBASE.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include "RND.h"
#include "../ANIM.h"
dd6ANIM DD6_Anim;

VOID DD6_RndStart( VOID )
{
  DD6_RndShdDelete(DD6_RndProgId);
  DD6_RndProgId = DD6_RndShdLoad("DEFAULT");

  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID DD6_RndEnd( VOID )
{
  glFinish();
}

VOID DD6_RndCamSet(VEC Loc, VEC At, VEC Up1)
{
  DD6_RndMatrView = MatrView(Loc, At, Up1);
  DD6_RndMatrVP = MatrMulMatr(DD6_RndMatrView, DD6_RndMatrProj);
}

VOID DD6_RndInit(HWND hWnd)
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  DD6_Anim.hWnd = hWnd;
  DD6_Anim.hDC = GetDC(DD6_Anim.hWnd);
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DD6_Anim.hDC, &pfd);
  DescribePixelFormat(DD6_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(DD6_Anim.hDC, i, &pfd);
  /* OpenGL init: setup rendering context */
  DD6_Anim.hRC = wglCreateContext(DD6_Anim.hDC);
  wglMakeCurrent(DD6_Anim.hDC, DD6_Anim.hRC);
  /* Initialize GLEW library */
  if (glewInit() != GLEW_OK)
    return;
  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    return;
  /* Set default render parameters */
  glClearColor(0.30, 0.50, 0.8, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  DD6_Anim.W = 30;
  DD6_Anim.H = 30;

  DD6_RndCamSet(VecSet(100, 60, 130), VecSet(0, 0, 0), VecSet(0, 1, 0));

  DD6_RndShdInit();
  DD6_RndMtlInit();
  DD6_RndTexInit();
}

VOID DD6_RndClose(VOID)
{
  DD6_RndShdDelete(DD6_RndProgId);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DD6_Anim.hRC);
  ReleaseDC(DD6_Anim.hWnd, DD6_Anim.hDC);
  PostQuitMessage(0);
}

VOID DD6_RndCopyFrame(HDC hDC)
{
  SwapBuffers(DD6_Anim.hDC);
}

VOID DD6_RndResize(INT W, INT H)
{
  glViewport(0, 0, W, H);
  DD6_RndProjSet(); 
}

VOID DD6_RndProjSet(VOID)
{
  DBL ratio_x, ratio_y;
  ratio_x = ratio_y = DD6_RndProjSize / 2;

  if (DD6_Anim.W >= DD6_Anim.H)
    ratio_x *= (DBL)DD6_Anim.W / DD6_Anim.H;
  else
    ratio_y *= (DBL)DD6_Anim.H / DD6_Anim.W;
  DD6_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y,
                                DD6_RndProjDist, DD6_RndProjFarClip);
  DD6_RndMatrVP = MatrMulMatr(DD6_RndMatrView, DD6_RndMatrProj);
}
