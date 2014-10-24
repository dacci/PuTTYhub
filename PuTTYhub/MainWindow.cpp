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
 * WM_INITDIALOG message handler.
 *
 * @param hWndFocus
 *     A handle to the control to receive the default keyboard focus
 * @param lParam
 *     Additional initialization data
 */
BOOL MainWindow_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {
  g_hMsgMcasterWnd = ::GetDlgItem(hWnd, IDC_CASTER);
  g_hListBoxWnd = ::GetDlgItem(hWnd, IDC_WINDOWS);

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
    case IDCANCEL:
      ::DestroyWindow(hWnd);
      break;

    case IDRETRY:
      ::UpdateWindowList();
      break;

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

      for (auto i = windowList.rbegin(), l = windowList.rend(); i != l; ++i) {
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
INT_PTR CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) {
  switch (uMsg) {
    HANDLE_MSG(hWnd, WM_INITDIALOG, MainWindow_OnInitDialog);
    HANDLE_MSG(hWnd, WM_DESTROY, MainWindow_OnDestroy);
    HANDLE_MSG(hWnd, WM_COMMAND, MainWindow_OnCommand);
    HANDLE_MSG(hWnd, WM_ACTIVATE, MainWindow_OnActivate);

    default:
      return FALSE;
  }

  return TRUE;
}
