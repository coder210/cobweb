#ifndef _DEF_H
#define _DEF_H

#ifdef _COBWEB_WIN32
#define COBWEB_CMOD_API extern "C" __declspec(dllexport)
#define COBWEB_MOD_API __declspec(dllexport)
#else
#define COBWEB_CMOD_API extern
#define COBWEB_MOD_API extern
#endif


#endif