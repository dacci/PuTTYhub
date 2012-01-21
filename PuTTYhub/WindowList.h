/*
 * Declarations of PuTTY window management.
 *
 * See COPYING for license details.
 * 
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#ifndef PUTTYHUB_WINDOWLIST_H_
#define PUTTYHUB_WINDOWLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

typedef std::vector<HWND> CWindowList;

extern CWindowList *g_windowList;

void UpdateWindowList();
void CreateWindowList();
void DestroyWindowList();
void PostMessageToWindows(UINT uMsg, WPARAM wParam, LPARAM lParam);
void SendMessageToWindows(UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetSelectedWindows(CWindowList* windowList);

#endif  // PUTTYHUB_WINDOWLIST_H_
