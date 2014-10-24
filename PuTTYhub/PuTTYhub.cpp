/*
 * Entrypoint of the application.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#include "StdAfx.h"

#include "PuTTYhub.h"
#include "MainWindow.h"
#include "MessageMulticaster.h"
#include "resource.h"

// Handle to the current instance of the application
HINSTANCE g_hInstance;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int) {
  g_hInstance = hInstance;

  if (!::MessageMulticaster_RegisterClass())
    return ::GetLastError();

  HWND hWnd = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(IDR_MAIN), NULL,
                                  MainWindowProc, 0);
  if (!hWnd)
    return ::GetLastError();

  MSG msg;
  BOOL result;

  while ((result = ::GetMessage(&msg, NULL, 0, 0)) != 0) {
    if (result == -1)
      break;

    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  return msg.wParam;
}
