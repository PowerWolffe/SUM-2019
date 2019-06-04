/* Demchenko Dmitriy, 10-6, 1.06.2019 */ 
#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "My window class"



LRESULT CALLBACK MyWindowFunc ( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  wc.style = CS_VREDRAW | CS_HREDRAW;

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox( NULL, "Wait. This is a fail.", "ERROR", MB_OK);
    return 0;
  }

  hWnd =
    CreateWindow( WND_CLASS_NAME, "Big bro is watching for you...",
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
    CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd,SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
VOID DrawEyes( HDC hDC, INT x, INT y, INT r, INT r1, INT mx, INT my)
{
  double Lx, Ly;
  double t; 

  t = sqrt((mx - x) * (mx - x) + (my - y) * (my - y));
  Lx = (int)((mx - x) / t * (r1 - r));
  Ly = (int)((my - y) / t * (r1 - r)); 

  SelectObject(hDC, GetStockObject(GRAY_BRUSH));
  Ellipse(hDC, x - r1, y - r1, x + r1, y + r1);
  if ( t + r < r1)
  {
    SelectObject(hDC, GetStockObject(BLACK_BRUSH));
    Ellipse(hDC, mx - r - 10, my - r - 10, mx + r + 10, my + r + 10);
  }
  else 
    Ellipse(hDC, x + Lx - r, y + Ly - r, x + Lx + r, y + Ly + r); 
    
}
 
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{      
 
  HDC hDC;
  static int w, h;
  int i;
  static HDC hMemDC;
  static HBITMAP hBm;
  POINT pt;
  PAINTSTRUCT ps;

  switch (Msg)
  { 
  case WM_TIMER:
    hDC = GetDC(hWnd);
    Rectangle(hMemDC, 0, 0, 1920, 1080);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(15);
    for ( i = 0; i < 100; i++)
      DrawEyes(hMemDC,rand() % w, rand() % h, 35, 60, pt.x, pt.y);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd,hDC);
    SetTimer(hWnd, 30, 30, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam); 
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    DeleteObject(hBm);
    DeleteObject(hDC);
    return 0;

  case WM_ERASEBKGND:
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);

}
