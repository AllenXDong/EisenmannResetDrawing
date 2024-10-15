/*==============================================================================
Programmierer : J. Klaus
Datum         : 22.02.2006
Last Update   :
Beschreibung  : Definitionen für Export
==============================================================================*/
#ifndef en_exports_H
#define en_exports_H
#pragma once

// #define _CRT_SECURE_NO_DEPRECATE // M.Künne - 24.03.09 - auskommentiert, sonst Neudefinition
// #define _CRT_SECURE_NO_WARNINGS // M.Künne - 24.03.09 - auskommentiert, sonst Neudefinition
// #pragma warning (disable : 4251)

/*****************************************************************/
/*  DllExport                                                    */
/*  This macro is used in function prototypes on Windows NT to   */
/*  indicate that the associated function is to be exported from */
/*  the containing dynamic link library.  On other platforms the */
/*  macro is null.                                               */
/*****************************************************************/
#if defined(_WIN32)
#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)
#else
#define DllExport
#define DllImport
#endif

// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem EN_COMMON_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen
// EN_COMMON_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
#ifdef EN_COMMON_EXPORTS
#define EN_COMMON_API DllExport
#else
#define EN_COMMON_API DllImport
#endif

#endif //en_exports_H

