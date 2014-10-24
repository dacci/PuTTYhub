/*
 * Declarations of the main window.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#ifndef PUTTYHUB_MAINWINDOW_H_
#define PUTTYHUB_MAINWINDOW_H_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#define WC_MAIN_WINDOW _T("PuTTYhub")

extern HWND g_hMsgMcasterWnd;
extern HWND g_hListBoxWnd;

INT_PTR CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam);

#endif  // PUTTYHUB_MAINWINDOW_H_
