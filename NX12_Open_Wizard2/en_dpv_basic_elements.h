/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Basis-Klasse für folgende dpv Files:
*                 - Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv) ==> en_drawing_dpv_elements.h
*                 - NX SITE DPV XML File (nx120_Site.dpv)                    ==> en_nx_site_dpv_elements.h
*                 Grund: Auswertung der dpv-Settings zum Benutzen im Programmcode für alte Zeichnungen
***************************************************************************************/
#ifndef en_dpv_basic_elements_H
#define en_dpv_basic_elements_H

#pragma once

#define DPV_APPLICATION 1
#define DPV_CATEGORY 2
#define DPV_SUBCATEGORY 3
#define DPV_TAB 4
#define DPV_DISPLAY_VALUE 5
#define DPV_MOD_DATE 6
#define DPV_ID_NAME 7
#define DPV_TITLE 8
#define DPV_ID_VALUE 9
#define DPV_COMMENT 10

// Standard Header Files
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>


// UG Header Files
#include <uf.h>
//#include <UI.hxx>
#include <uf_ui_ugopen.h>
#include <uf_defs.h>
//#include <uf_styler.h>


//#include "en_env.h"

//int g_nLogLevel;

using namespace std;
typedef char * str_c;


class en_dpv_basic_elements {
public:
	en_dpv_basic_elements::en_dpv_basic_elements();
	en_dpv_basic_elements::en_dpv_basic_elements(string _dpv_application, string _dpv_category, string _dpv_subcategory, string _dpv_tab, string _dpv_subtab, string _dpv_display_value, string _dpv_mod_date, string _dpv_id_name, string _dpv_title, string dpv_id_value, string _dpv_comment, string _dpv_id_value_ist);

	/*
	printLog nur dann ausführen, wenn Umgebungsvariable
	EN_UG_LOGLEVEL = 0 oder EN_UG_LOGLEVEL = TRUE oder EN_UG_LOGLEVEL = true oder EN_UG_LOGLEVEL = T oder EN_UG_LOGLEVEL = t
	Existiert die Umgebungsvariable EN_UG_LOGLEVEL nicht, so printLog nicht ausführen. Nicht Case-Sensitiv!
	Achtung: Erst nach einem TC-Neustart steht diese Variable zur Verfügnung!!!
	Variable g_nxLog_en_dpv_basic_elements = true / false steuert hier die printLog-Ausgabe
	*/
	bool set_nxLog();

	string get_dpv_application();
	string get_dpv_category();
	string get_dpv_subcategory();
	string get_dpv_tab();
	string get_dpv_subtab();
	string get_dpv_display_value();
	string get_dpv_mod_date();
	string get_dpv_id_name();
	string get_dpv_title();
	string get_dpv_id_value();
	string get_dpv_comment();
	string get_dpv_id_value_ist();
	void   set_dpv_id_value_ist(string _dpv_id_value_ist);
	void   set_dpv_id_value_soll(string _dpv_id_value_soll);
	string separator = "#";
	string get_double_as_string(double _double_value);
protected:
	void nxLog(char *_szFormatString, ...);
private:
	string dpv_application;
	string dpv_category;
	string dpv_subcategory;
	string dpv_tab;
	string dpv_subtab;
	string dpv_display_value;
	string dpv_mod_date;
	string dpv_id_name;
	string dpv_title;
	string dpv_id_value;
	string dpv_comment;
	string dpv_id_value_ist;

	bool g_nxLog_en_dpv_basic_elements;
};


#endif


/*
* Vergleichoperatoren aus cmd:
- EQU = gleich               (equal to)
- NEQ = ungleich             (not equal to)
- LSS = kleiner als          (less than or equal to)
- LEQ = kleiner oder gleich  (lower or equal)
- GTR = größer               (greater than)
- GEQ = größer oder gleich   (greator than or equal to)
*/