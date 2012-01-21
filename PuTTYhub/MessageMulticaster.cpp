/*
 * Implementation of the message multicaster control.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#include "StdAfx.h"

#include "PuTTYhub.h"
#include "MessageMulticaster.h"
#include "WindowList.h"

/**
 * Registers window class of the message multicaster control.
 *
 * @return Registered class atom, or zero if failed.
 */
ATOM MessageMulticaster_RegisterClass() {
  WNDCLASSEX wcex = { sizeof(wcex) };

  wcex.style = CS_PARENTDC;
  wcex.lpfnWndProc = ::MessageMulticasterWindowProc;
  wcex.hInstance = g_hInstance;
  wcex.hIcon = ::LoadIcon(NULL, IDI_ERROR);
  wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
  wcex.lpszClassName = WC_MESSAGE_MULTICASTER;

  return ::RegisterClassEx(&wcex);
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
LRESULT CALLBACK MessageMulticasterWindowProc(HWND hWnd, UINT uMsg,
                                              WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CREATE:
    break;

  case WM_DESTROY:
    break;

  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    ::SendMessageToWindows(uMsg, wParam, lParam);
    break;

  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    ::PostMessageToWindows(uMsg, wParam, lParam);
    break;

  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    ::SetFocus(hWnd);
    break;

  default:
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return FALSE;
}
