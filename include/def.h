#ifndef _DEF_H
#define _DEF_H

#ifdef _COBWEB_WIN32
#define MOD_API extern "C" __declspec(dllexport)
#else
#define MOD_API
#endif


#endif