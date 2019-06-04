/* Demchenko Dmitriy, 10-6, 1.06.2019 */ 

#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WND_CLASS_NAME "My winow class"
#define PI 3.14159265358979323846
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
    GetMonitorInfo(hmon, (MONITORINFO *) &moninfo);
    rc = moninfo.rcMonitor;
    
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top, 
      rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }

  }
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static HDC hMemDC, hMemDCClock;
  BITMAP bm;
  static HBITMAP hBm, hBmClock;
  static INT w, h, H;
  SYSTEMTIME st;
  HPEN hPen, hOldPen;
  double mas[3], a, b, c;
    
  POINT pnts1[4]; 
  POINT pnts2[4];                    
  POINT pnts3[4]; 
  switch (Msg)
  {                                                                  
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCClock = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBmClock = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCClock, hBmClock);
    SetTimer(hWnd, 1, 30, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBm != NULL)                              
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    SelectObject(hMemDC, hBm);
    ReleaseDC(hWnd, hDC);
    SendMessage(hWnd, WM_TIMER, 1, 0);
    return 0;

  case WM_LBUTTONDBLCLK:
    FlipFullScreen(hWnd);
  case WM_TIMER:                                                                                                      
    hDC = GetDC(hWnd);
    Rectangle(hMemDC, 0, 0, 1920, 1080);
    GetLocalTime(&st);
                            
    GetObject(hBmClock, sizeof(bm), &bm);                      
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCClock, 0, 0, SRCCOPY);
    hPen = CreatePen(PS_SOLID, 5, RGB(100, 100, 0));
    hOldPen = SelectObject(hDC, hPen);
    SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
    /*
    MoveToEx(hMemDC, 368, 368, NULL);
    LineTo(hMemDC, cos((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368,
                   sin((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368);

    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, 368, 368, NULL);
    LineTo(hMemDC, cos((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368,
                   sin((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368);
      
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, 368, 368, NULL);
    LineTo(hMemDC, cos((((2 * PI) / 12) * (st.wHour % 12 + st.wMinute / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368,
                     sin((((2 * PI) / 12) * (st.wHour % 12  + st.wMinute / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368);
    
    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
    */                                                                                                         
    
    a = st.wDay;
    b = st.wMonth;
    c = st.wYear;
    mas[0] = a;
    mas[1] = b;
    mas[2] = c;
    
    pnts1[1].x = 368;
    pnts1[0].x = cos((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2) - PI / 45) * 80 + 368; 
    pnts1[2].x = cos((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2) + PI / 45) * 80 + 368; 
    pnts1[3].x = cos((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368;
      
    pnts1[1].y = 368;
    pnts1[0].y = sin((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2) - PI / 45) * 80 + 368; 
    pnts1[2].y = sin((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2) + PI / 45) * 80 + 368; 
    pnts1[3].y = sin((((2 * PI) / 60) * (st.wSecond + st.wMilliseconds / 1000.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368;

    pnts2[1].x = 368;

    pnts2[0].x = cos((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2) - PI / 45) * 100 + 368;
    pnts2[2].x = cos((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2) + PI / 45) * 100 + 368;
    pnts2[3].x = cos((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368;
      
    pnts2[1].y = 368;
    pnts2[0].y = sin((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2) - PI / 45) * 100 + 368;
    pnts2[2].y = sin((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2) + PI / 45) * 100 + 368;
    pnts2[3].y = sin((((2 * PI) / 60) * (st.wMinute + st.wSecond / 60.0)) - (PI / 2)) * (bm.bmWidth / 2) + 368;
  

    pnts3[1].x = 368;
    pnts3[0].x = cos((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2) - PI / 45) * 120 + 368;
    pnts3[2].x = cos((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2) + PI / 45) * 120 + 368;
    pnts3[3].x = cos((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2)) * (bm.bmWidth / 2) + 368;
      
    pnts3[1].y = 368;
    pnts3[0].y = sin((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2) - PI / 45) * 120 + 368;
    pnts3[2].y = sin((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2) + PI / 45) * 120 + 368;
    pnts3[3].y = sin((((2 * PI) / 60) * (st.wHour % 12 + st.wMinute / 60.0 + 20)) - (PI / 2)) * (bm.bmWidth / 2) + 368;

    SelectObject(hMemDC, hPen);
    Polygon(hMemDC, pnts1, 4);
    SelectObject(hMemDC, hPen);
    Polygon(hMemDC, pnts2, 4);
    SelectObject(hMemDC, hPen);
    Polygon(hMemDC, pnts3, 4);
    
    BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_CLOSE:
    if (MessageBox(NULL, "Revolver Ocelot", "Revolver Ocelot", MB_YESNO | MB_ICONQUESTION) == IDYES)
    { 
      KillTimer(hWnd, 1);
      PostQuitMessage(0);
      DeleteObject(hMemDC);
      DeleteDC(hMemDC);
      return 0;
    }
    else
      Msg = WM_ACTIVATE;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Wait.This is a fail", "ERROR", MB_OK);
    return 0;
  }
  hWnd = CreateWindow(WND_CLASS_NAME,"CHASIKI TIK TAK", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}