/*
 * Implementation of the main window.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#include "StdAfx.h"

#include "resource.h"

#include "PuTTYhub.h"
#include "MainWindow.h"
#include "MessageMulticaster.h"
#include "WindowList.h"

HWND g_hMsgMcasterWnd;
HWND g_hListBoxWnd;

/**
 * Registers window class of the main window.
 *
 * @return Registered class atom, or zero if failed.
 */
ATOM MainWindow_RegisterClass() {
  WNDCLASSEX wcex = { sizeof(wcex) };

  wcex.lpfnWndProc = ::MainWindowProc;
  wcex.hInstance = g_hInstance;
  wcex.hIcon = ::LoadIcon(NULL, IDI_ERROR);
  wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
  wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
  wcex.lpszClassName = WC_MAIN_WINDOW;

  return ::RegisterClassEx(&wcex);
}

/**
 * WM_CREATE message handler.
 *
 * @param hWnd
 *     Handle to the window that received the message
 * @param lpCreateStruct
 *     Pointer to a CREATESTRUCT
 */
BOOL MainWindow_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
  g_hMsgMcasterWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE,
                                      WC_MESSAGE_MULTICASTER,
                                      WC_MESSAGE_MULTICASTER,
                                      WS_CHILD |
                                      WS_VISIBLE |
                                      WS_CLIPCHILDREN |
                                      WS_CLIPSIBLINGS,
                                      5, 5,
                                      214, 99,
                                      hWnd,
                                      NULL,
                                      g_hInstance,
                                      NULL);

  g_hListBoxWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE,
                                   WC_LISTBOX,
                                   WC_LISTBOX,
                                   WS_CHILD |
                                   WS_VISIBLE |
                                   WS_CLIPCHILDREN |
                                   WS_CLIPSIBLINGS |
                                   LBS_MULTIPLESEL,
                                   5, 109,
                                   214, 112,
                                   hWnd,
                                   NULL,
                                   g_hInstance,
                                   NULL);
  SetWindowFont(g_hListBoxWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE);

  HWND hButton = CreateWindowEx(0,
                                WC_BUTTON,
                                _T("ç≈êVÇÃèÓïÒÇ…çXêV"),
                                WS_CHILD |
                                WS_VISIBLE |
                                WS_CLIPCHILDREN |
                                WS_CLIPSIBLINGS,
                                5, 226,
                                214, 20,
                                hWnd,
                                reinterpret_cast<HMENU>(IDRETRY),
                                g_hInstance,
                                NULL);
  SetWindowFont(hButton, GetStockFont(DEFAULT_GUI_FONT), FALSE);

  ::CreateWindowList();

  ::ShowWindow(hWnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hWnd);

  return TRUE;
}

/**
 * WM_DESTROY message handler.
 *
 * @param hWnd
 *     Handle to the window that received the message
 */
void MainWindow_OnDestroy(HWND hWnd) {
  ::DestroyWindowList();
  ::PostQuitMessage(0);
}

/**
 * WM_ACTIVATE message handler.
 *
 * @param hWnd
 *     Handle to the window that received the message
 * @param uState
 *     Whether the window is being activated or deactivated
 * @param hWndActDeact
 *     Handle to the window being activated or deactivated
 * @param fMinimized
 *     The minimized state of the window being activated or deactivated
 */
void MainWindow_OnActivate(HWND hWnd, UINT uState, HWND hWndActDeact,
                           BOOL fMinimized) {
  if (uState == WA_ACTIVE)
    ::SetFocus(g_hMsgMcasterWnd);
}

/**
 * WM_COMMAND message handler.
 *
 * @param hWnd
 *     Handle to the window that received the message
 * @param nID
 *     The identifier of the menu item, control, or accelerator
 * @param hWndCtrl
 *     Handle to the control sending the message if the message is from a control
 * @param uCodeNotify
 *     The notification code if the message is from a control
 */
void MainWindow_OnCommand(HWND hWnd, int nID, HWND hWndCtrl, UINT uCodeNotify) {
  switch (nID) {
    case IDRETRY: {
      ::UpdateWindowList();
      break;
    }

    case ID_WINDOW_CASCADE:
    case ID_WINDOW_TILE_HORZ:
    case ID_WINDOW_TILE_VERT: {
      CWindowList windowList;
      ::GetSelectedWindows(&windowList);
      if (windowList.size() == 0)
        break;

      RECT rcScreen;
      ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);

      RECT rcWindow;
      ::GetWindowRect(windowList[windowList.size() - 1], &rcWindow);

      size_t width = (rcScreen.right - rcScreen.left) -
                     (rcWindow.right - rcWindow.left);
      size_t deltaX = windowList.size() > 1 ?
          width / (windowList.size() - 1) : 0;

      if (nID == ID_WINDOW_TILE_HORZ)
        deltaX = 0;

      size_t height = (rcScreen.bottom - rcScreen.top) -
                      (rcWindow.bottom - rcWindow.top);
      size_t deltaY = windowList.size() > 1 ?
          height / (windowList.size() - 1) : 0;

      if (nID == ID_WINDOW_TILE_VERT)
        deltaY = 0;

      DWORD this_thread = ::GetCurrentThreadId();

      for (int i = windowList.size() - 1; 0 <= i; --i) {
        DWORD window_thread = ::GetWindowThreadProcessId(windowList[i], NULL);
        ::AttachThreadInput(this_thread, window_thread, TRUE);

        ::SetForegroundWindow(windowList[i]);
        ::SetWindowPos(windowList[i], HWND_TOP, deltaX * i, deltaY * i, 0, 0,
                       SWP_NOSIZE);

        ::AttachThreadInput(this_thread, window_thread, FALSE);
      }

      ::SetForegroundWindow(hWnd);
      break;
    }

    case ID_WINDOW_NEW: {
      CWindowList windowList;
      ::GetSelectedWindows(&windowList);

      for (auto i = windowList.rbegin(); i != windowList.rend(); ++i) {
        ::ShowWindow(*i, SW_SHOWNORMAL);
        ::SetWindowPos(*i, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }

      ::SetForegroundWindow(hWnd);
      break;
    }
  }
}

/**
 * Window procedure of the main window.
 *
 * @param hWnd
 *     Handle to the window that received the message
 * @param uMsg
 *     The message
 * @param wParam
 *     additional message information
 * @param lParam
 *     additional message information
 */
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) {
  switch (uMsg) {
    HANDLE_MSG(hWnd, WM_CREATE, MainWindow_OnCreate);
    HANDLE_MSG(hWnd, WM_DESTROY, MainWindow_OnDestroy);
    HANDLE_MSG(hWnd, WM_COMMAND, MainWindow_OnCommand);
    HANDLE_MSG(hWnd, WM_ACTIVATE, MainWindow_OnActivate);

    default:
      return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return FALSE;
}
