#ifndef PUTTYHUB_TARGETVER_H_
#define PUTTYHUB_TARGETVER_H_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINVER
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS WINVER
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#ifndef _RICHEDIT_VER
#define _RICHEDIT_VER 0x0200
#endif

#endif  // PUTTYHUB_TARGETVER_H_
