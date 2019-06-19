/* FILE NAME   : MAIN.C
 * PURPOSE     : Entry point.
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

#include <windows.h>
#include<time.h>
#define WND_CLASS_NAME "My window class"
#include "DEF.H"
#include "ANIM/RND/RND.H"
#include"UNITS/UNIT.h"

INT DD6_MouseWheel;

LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShawCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(hInstance, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

  if (!RegisterClass(&wc))

  {
    MessageBox(NULL, "This is a fail", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "SPASITEEEE", WS_OVERLAPPEDWINDOW,
                      0, 0, 1000, 1000, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  /****************UNITS*************************/

  DD6_AnimUnitAdd(DD6_UnitShermanCreate());
  DD6_AnimUnitAdd(DD6_UnitEnviCreate());

  /*****************************************/
  while (TRUE)
   if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
       break;
      DispatchMessage(&msg);  
      }
   else
       SendMessage(hWnd, WM_TIMER, 47, 0);
   return msg.wParam;   
}    


VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;
  
    GetWindowRect(hWnd, &SaveRC);
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, HWND_TOP,
                 rc.left, rc.top,
                 rc.right - rc.left, rc.bottom - rc.top + 201,
                 SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
                 SaveRC.left, SaveRC.top,
                 SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
                 SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}

LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)

{
  HDC hDc;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    DD6_AnimInit(hWnd);
    SetTimer(hWnd, 47, 50, NULL);
    return 0;
  case WM_MOUSEWHEEL:
    DD6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_SIZE:
    DD6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;

  case WM_TIMER:
    DD6_AnimRender();
    hDc = GetDC(hWnd);
    DD6_AnimCopyFrame(hDc);
    ReleaseDC(hWnd, hDc);
    return 0;

  case WM_PAINT:
    hDc = BeginPaint(hWnd, &ps);
    DD6_AnimCopyFrame(hDc);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    if (wParam == VK_INSERT)
      FlipFullScreen(hWnd);
    return 0;

  case WM_DESTROY:
    DD6_AnimClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}