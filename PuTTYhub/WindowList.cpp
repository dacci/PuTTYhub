/*
 * Manages PuTTY windows.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#include "StdAfx.h"

#include "MainWindow.h"
#include "WindowList.h"

// Pointer to CWindowList that contains window handles of the PuTTY windows.
CWindowList* g_windowList;

/**
 * Callback function to be passed to EnumWindows.
 *
 * @param hWnd
 *     Handle to the enumerated window
 * @param lParam
 *     Additional parameter that passed to EnumWindows
 */
BOOL CALLBACK UpdateWindowListProc(HWND hWnd, LPARAM lParam) {
  TCHAR szClassName[256];
  TCHAR szWindowText[256];

  if (::GetClassName(hWnd, szClassName, 256)) {
    if (::_tcscmp(szClassName, _T("PuTTY")) == 0) {
      g_windowList->push_back(hWnd);

      ::GetWindowText(hWnd, szWindowText, 256);
      ListBox_AddString(g_hListBoxWnd, szWindowText);
    }
  }

  return TRUE;
}

/**
 * Updates the PuTTY window list.
 */
void UpdateWindowList() {
  g_windowList->clear();

  for (int i = 0, l = ListBox_GetCount(g_hListBoxWnd); i < l; i++)
    ListBox_DeleteString(g_hListBoxWnd, 0);

  ::EnumWindows(UpdateWindowListProc, NULL);
}

/**
 * Creates and initializes the PuTTY window list.
 */
void CreateWindowList() {
  g_windowList = new CWindowList();
  ::UpdateWindowList();
}

/**
 * Destroys the PuTTY window list.
 */
void DestroyWindowList() {
  delete g_windowList;
}

/**
 * Posts specified message to each of the PuTTY window
 * which selected in the window list.
 */
void PostMessageToWindows(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  for (int i = 0, l = ListBox_GetCount(g_hListBoxWnd); i < l; i++) {
    if (ListBox_GetSel(g_hListBoxWnd, i))
      ::PostMessage(g_windowList->at(i), uMsg, wParam, lParam);
  }
}

/**
 * Sends specified message to each of the PuTTY window
 * which selected in the window list.
 */
void SendMessageToWindows(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  for (int i = 0, l = ListBox_GetCount(g_hListBoxWnd); i < l; i++) {
    if (ListBox_GetSel(g_hListBoxWnd, i))
      ::SendMessage(g_windowList->at(i), uMsg, wParam, lParam);
  }
}

/**
 * Returns subset of window list which contains handls to the selected windows.
 */
void GetSelectedWindows(CWindowList* windowList) {
  windowList->clear();

  for (int i = 0, l = ListBox_GetCount(g_hListBoxWnd); i < l; i++) {
    if (ListBox_GetSel(g_hListBoxWnd, i))
      windowList->push_back(g_windowList->at(i));
  }
}
