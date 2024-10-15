/*==============================================================================
Programmierer : J. Klaus
Datum         : 09.09.2005 - J. Klaus
Last Update   :
Beschreibung  : Definiert den Einstiegspunkt für die DLL-Anwendung.
==============================================================================*/
#pragma once

// UG Header Files
#include <uf.h>
#include <uf_obj.h>
#include <uf_plist.h>
#include <uf_mb.h>
#include <uf_part.h>
#include <uf_assem.h>
#include <uf_ugmgr.h>

/*
// EN Header Files
#include "ActionHandler.h"
#include "NxTeil.h"
#include "NxTeilCreator.h"
#include "NxModell.h"
#include "NxZeichnung.h"
#include "NxManifestation.h"
#include "NxRahmen.h"
#include "NxFont.h"
#include "NxUtils.h"
#include "NxSimplify.h"
#include "NxException.h"
#include "NxComponent.h"
#include "NxUtils.h"
*/
#include "en_nx_common.h"
#include "en_env.h"

// Standard Header Files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Funktionsdeklarationen
//void initLog4cxx(void);

//LoggerPtr mainLogger(Logger::getLogger("en_nx_common_main"));

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // Init log4cxx
            //initLog4cxx();
            break;
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return(TRUE);
}

///*==============================================================================
//Programmierer : Silier
//Datum         : 26.02.10 - Silier
//Last Update   :
//Beschreibung  : Log4cxx initialisieren
//==============================================================================*/
//void initLog4cxx(void) {
//    static int bInitLog4cxx = false;
//
//    if (!bInitLog4cxx) {
//        char *configFileName = NULL;
//        size_t len;
//        errno_t err;
//
//        // Determine configuration filename
//        err = _dupenv_s(&configFileName, &len, "EN_LOG4CXX_NX_CONFIG_FILE");
//        if (configFileName != NULL) {
//            // Load and watch configuration file
//            DOMConfigurator::configureAndWatch(configFileName);
//            // Now we will see if it works
//            LOG4CXX_DEBUG(mainLogger, "Log4cxx initialized.");
//            // Remember that log4cxx is now initialized
//            bInitLog4cxx = true;
//            // Cleanup memory
//            free(configFileName);
//        } else {
//            // Using basic configuration
//            BasicConfigurator::configure();
//            LOG4CXX_WARN(mainLogger, "Environment variable EN_LOG4CXX_NX_CONFIG_FILE not set!");
//            LOG4CXX_WARN(mainLogger, "Using basic configuration.");
//        }
//    }
//}

// Dies ist das Beispiel einer exportierten Variablen
extern "C" DllExport  int nen_nx_common = 0;

// Dies ist das Beispiel einer exportierten Funktion
extern "C" DllExport  int fnen_nx_common(void) {
    return(42);
}

// Dies ist der Konstruktor einer Klasse, die exportiert wurde.
// Siehe en_nx_common.h für die Klassendefinition
/*
Cen_nx_common::Cen_nx_common()
{
 return;
}
*/

/*==============================================================================
Programmierer : Klaus
Datum         : 09.03.2004 - Klaus
Last Update   :
Beschreibung  : Fehlermeldung ausgeben
==============================================================================*/
//int reportError(char *file, int line, char *call, int irc) {
//    if (irc) {
//        char err[133],
//             msg[133];
//
//        sprintf_s(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
//                  irc, line, file);
//        UF_get_fail_message(irc, err);
//
//        UF_print_syslog(msg, FALSE);
//        UF_print_syslog(err, FALSE);
//        UF_print_syslog("\n", FALSE);
//        UF_print_syslog(call, FALSE);
//        UF_print_syslog(";\n", FALSE);
//
//        if (!UF_UI_open_listing_window()) {
//            UF_UI_write_listing_window(msg);
//            UF_UI_write_listing_window(err);
//            UF_UI_write_listing_window("\n");
//            UF_UI_write_listing_window(call);
//            UF_UI_write_listing_window(";\n");
//        }
//        throw NxException(irc, call, file, line);
//    }
//
//    return(irc);
//}

/*==============================================================================
Programmierer : Klaus
Datum         : 19.07.04 - Klaus
Last Update   :
Beschreibung  : User Function sofort entladen
==============================================================================*/
extern "C" DllExport int ufusr_ask_unload(void) {
    return(UF_UNLOAD_IMMEDIATELY);
}

/*==============================================================================
Programmierer : Klaus
Datum         : 19.07.04 - Klaus
Last Update   :
Beschreibung  : User Function zum testen der DLL
==============================================================================*/
extern "C" DllExport void ufusr(char *param, int *retcode, int paramLen) {
    if (!UF_CALL(UF_initialize())) {
        //ActionHandler* actionHandler = ActionHandler::getInstance();
        // actionHandler->preSavePart();
        // UF_CALL(UF_terminate());
    }
}

