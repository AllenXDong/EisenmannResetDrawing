/*==============================================================================
Programmierer : Klaus
Datum         : 07.09.2005 - Klaus
Last Update   :
Beschreibung  : EN UG Common Header
==============================================================================*/
#ifndef __en_nx_commonH__
#define __en_nx_commonH__

// UG Header Files
#include <uf_part.h>

// EN Header Files
#include "en_exports.h"
#include "en_nx_utils.h"

//// include log4cxx header files.
//#include "log4cxx/logger.h"
//#include "log4cxx/basicconfigurator.h"
//#include "log4cxx/xml/domconfigurator.h"
//#include "log4cxx/propertyconfigurator.h"
//#include "log4cxx/helpers/exception.h"
//
//using namespace log4cxx;
//using namespace log4cxx::xml;
//using namespace log4cxx::helpers;

// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem en_nx_common_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen
// EN_COMMON_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
/*
#ifdef en_nx_common_EXPORTS
#define EN_COMMON_API __declspec(dllexport)
#else
#define EN_COMMON_API __declspec(dllimport)
#endif
*/

//#define UF_CALL(X) (reportError(__FILE__, __LINE__, # X, (X)))

//int reportError(char *file, int line, char *call, int irc);
extern EN_COMMON_API void printLog(char *szFormatString, ...);
extern EN_COMMON_API void printInfo(char *szFormatString, ...);
extern EN_COMMON_API void reportLoadStatus(UF_PART_load_status_t *status);

// Diese Klasse wird aus en_nx_common.dll exportiert
/*
class EN_COMMON_API Cen_nx_common {
public:
 Cen_nx_common(void);
 // TODO: Fügen Sie hier Ihre Methoden ein.
};
*/

#endif