/*
 * Declarations of the message multicaster control.
 *
 * See COPYING for license details.
 *
 * @author Shinya Tsuda <wish.upon.a.planet@gmail.com>
 */

#ifndef PUTTYHUB_MESSAGEMULTICASTER_H_
#define PUTTYHUB_MESSAGEMULTICASTER_H_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#define WC_MESSAGE_MULTICASTER _T("MessageMulticaster")

ATOM MessageMulticaster_RegisterClass();

LRESULT CALLBACK MessageMulticasterWindowProc(HWND hWnd, UINT uMsg,
                                              WPARAM wParam, LPARAM lParam);

#endif  // PUTTYHUB_MESSAGEMULTICASTER_H_
