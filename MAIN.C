/* FILE NAME   : MAIN.C
 * PURPOSE     : Entry point.
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 10.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

#include <windows.h>
#define WND_CLASS_NAME "My window class"

                       
LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShawCmd)

{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
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
   MessageBox(NULL, "Oh no", "ERROR", MB_OK);
   return 0;
 }

    hWnd = CreateWindow(WND_CLASS_NAME, "OKNO", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
       while (TRUE)
         if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

        {

            if (msg.message == WM_QUIT)
              break;
            DispatchMessage(&msg);

        }

        else
          SendMessage(hWnd, WM_TIMER, 47, 0);

}

VOID FlipFullScreen(HWND hWnd)
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

} /* End of 'FlipFullScreen' function */
LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)

{
  HDC hDC;
  static int w, h;
  static HDC hMemDC;
  static HBITMAP hBm;
  PAINTSTRUCT ps;
 
  switch (Msg)
 {

case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;

case WM_CREATE:
  SetTimer(hWnd, 47, 50, NULL);
  hDC = GetDC(hWnd);
  hMemDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  FlipFullScreen(hWnd);
  return 0;

case WM_LBUTTONDBLCLK:
  FlipFullScreen(hWnd);
  return 0;

case WM_SIZE:
  w = LOWORD(lParam);
  h = HIWORD(lParam);
  if (hBm != NULL)
    DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
  SelectObject(hMemDC, hBm);
  SendMessage(hWnd, WM_TIMER, wParam, lParam);
  return 0;

case WM_KEYDOWN:
  if (wParam == VK_ESCAPE)
    DestroyWindow(hWnd);
  if (wParam == VK_INSERT)
    FlipFullScreen(hWnd);
  return 0;
 }
}