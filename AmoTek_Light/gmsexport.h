#pragma once
/**
This Header file contains useful functions for GameMaker Dll export.
C-Ver.
/* Header Guards*/
#ifndef GMS_H_INCLUDED
#define GMS_H_INCLUDED

#define gmx __declspec(dllexport)

typedef const char* GMSTRTODLL;    // String passed GM --> DLL
typedef char* GMSTRFROMDLL;        // String passed DLL -- > GM
typedef double GMINT;			   // ITs all doubles!
typedef double GMBOOL;

#define GMTRUE (1.0)
#define GMFALSE (0.0)

#define SLOTS_FULL -1
#define DEVICE_INVALID -1.0

#endif // AMOGUS_H_INCLUDED