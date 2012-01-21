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

// Handle to the current instance of the application
HINSTANCE g_hInstance;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int) {
  g_hInstance = hInstance;

  if (!::MainWindow_RegisterClass()) {
    return ::GetLastError();
  }

  if (!::MessageMulticaster_RegisterClass()) {
    return ::GetLastError();
  }

  if (!::CreateWindowEx(0,
                        WC_MAIN_WINDOW,
                        WC_MAIN_WINDOW,
                        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                        CW_USEDEFAULT, 0,
                        230, 299,
                        NULL,
                        NULL,
                        hInstance,
                        NULL)) {
    return ::GetLastError();
  }

  MSG msg;
  BOOL result;

  while ((result = ::GetMessage(&msg, NULL, 0, 0)) != 0) {
    if (result == -1) {
      break;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  return msg.wParam;
}
