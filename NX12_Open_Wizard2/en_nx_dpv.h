/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Basis-Klasse zum Arbeiten mit XML-Dateien (dpv-Dateien in NX)
*                 Grund: Abgleich der DPV-Settings mit dem Code
***************************************************************************************/
#ifndef en_nx_dpv_H
#define en_nx_dpv_H

#define DPV_ID_NAME_FILE "dpv_id_names"
#define DRAFTING_DPV_FILE_NAME "nx_EN_Drafting_Standard_Site"
#define NX_DPV_SITE_FILE_NAME "nx_site"
#define DRAFTING_compare_file_name "nx_EN_Drafting_standard_compare"
#define NX_SITE_compare_file_name "nx_EN_NX_SITE_standard_compare"

#pragma once

#include "en_drawing_dpv_elements.h"
#include "en_nx_site_dpv_elements.h"

#include "en_nx_common.h"

#include <NXOpen/OpenXml_DocumentData.hxx>
#include <NXOpen/OpenXml_DocumentDataGroup.hxx>
#include <NXOpen/OpenXml_ImageDocumentData.hxx>
#include <NXOpen/OpenXml_ImageGroupDocumentData.hxx>
#include <NXOpen/OpenXml_TableCellData.hxx>
#include <NXOpen/OpenXml_TableDocumentData.hxx>
#include <NXOpen/OpenXml_TextDocumentData.hxx>

// Standard Header Files
//#include <cstring>
//#include <string>
//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <vector>
using namespace std;

#include "en_env.h"

// UG Header Files
#include <uf_defs.h>
#include <uf.h>
#include <uf_ui_ugopen.h>
#include <uf_text_datatypes.h>
#include <uf_text.h>
//#include <UI.hxx>
//#include <uf.h>
#include <uf_defs.h>
//#include <uf_styler.h>

#include <fstream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
//#include <windows.h> 
//#include <fileapi.h>
#include "direct.h"
#include <regex>
#include <iterator>
#include <map>


typedef char * str_c;

class en_nx_dpv {
public:
	/*
	Statischer Einstiegspunkt - die Basistabellen
	- Drafting DPV XML File einlesen
	- NX SITE DPV XML File einlesen
	werden einmalig erzeugt, wenn noch nicht vorhanden.
	*/
	static en_nx_dpv *getInstance();

	en_nx_dpv::~en_nx_dpv();

	/*
	printLog nur dann ausführen, wenn Umgebungsvariable
	EN_UG_LOGLEVEL = 0 oder EN_UG_LOGLEVEL = TRUE oder EN_UG_LOGLEVEL = true oder EN_UG_LOGLEVEL = T oder EN_UG_LOGLEVEL = t
	Existiert die Umgebungsvariable EN_UG_LOGLEVEL nicht, so printLog nicht ausführen. Nicht Case-Sensitiv!
	Achtung: Erst nach einem TC-Neustart steht diese Variable zur Verfügnung!!!
	Variable this->g_nxLog_en_nx_dpv = true / false steuert hier die printLog-Ausgabe
	*/
	bool set_nxLog();

	/*
	drafting_dpv_list: Testausgabe der gefilterten/ungefilterten Liste ins Verzeichnis C:\Temp\NX\<User-ID>\dpv_vgl\
	==> Testausgabe erfolgt nur mit gesetzter Umgebungsvarialbe EN_UG_LOGLEVEL == 0 oder true oder t (Nicht Case-Sensitiv!)
	*/
	bool create_text_file_from_dpv(list<en_drawing_dpv_elements> _list, string _list_name);

	/*
	nx_site_dpv_list: Testausgabe der gefilterten/ungefilterten Liste ins Verzeichnis C:\Temp\NX\<User-ID>\dpv_vgl\
	==> Testausgabe erfolgt nur mit gesetzter Umgebungsvarialbe EN_UG_LOGLEVEL == 0 oder true oder t (Nicht Case-Sensitiv!)
	*/
	bool create_text_file_from_dpv(list<en_nx_site_dpv_elements> _list, string _list_name);

	/*==============================================================================
	Programmierer : Kreissig
	Datum         : 08.05.2019 - Kreissig
	Last Update   :
	Beschreibung  : LOG Meldung ausgeben
	==============================================================================*/
	void en_nx_dpv::nxLog(char *_szFormatString, ...);

	list <en_drawing_dpv_elements>    drafting_dpv_list;
	list <en_nx_site_dpv_elements>    nx_site_dpv_list;

	map <const std::string, std::string> soll_dpv_map;
	map <const std::string, std::string> ist_dpv_map;

	string usedUnitValue;

protected:
	
private:
	en_nx_dpv::en_nx_dpv();

	static en_nx_dpv *instance;

	/*
	Inhalt einer beliebigen globalen Variable ermitteln
	*/
	bool get_global_variable_value(string _nameGlobalVariable, string &_valueGlobalVariable);

	/*
	Verzeichnis folgender DPV-Dateien
	- Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv)
	- NX SITE DPV XML File (nx120_Site.dpv)
	aus der globalen Variablen %UGII_SITE_DIR%\startup ermitteln
	*/
	bool init_startup_dir(string &_startupDir);

	/*
	Verzeichnis fuer temporaere Vergleichsdatei
	- Drafting DPV txt File - drafting_dpv_list (vgl_nx_EN_Drafting_Standard_Site_dpv.txt)
	- NX SITE DPV txt File - nx_site_dpv_list (vgl_nx120_Site_dpv_txt)
	aus der globalen Variablen %NX_TMP_DIR%\dpv_vgl ermitteln
	*/
	bool init_tmp_dir(string &_tmpDir);

	/*
	Initiieren des function_list - bestimmt, welche Einstellungen abgeglichen werden sollen!
	*/
	//bool create_function_list(str_c _tmp_dir);
	bool create_function_list();

	/*
	File exists?
	*/
	bool fileExists(str_c _fileName);

	/*
	File exists?
	*/
	bool fileExists(string _fileName);

	/*
	Initiieren der Ausgangslisten
	- drafting_dpv_list
	- nx_site_dpv_list
	*/
	bool init_lists(string _tmp_dir);

	/*
	Erzeugen von genau einer speziellen Liste
	*/
	bool create_list(string _tmp_dir, string _dpv_list);

	/*
	Erzeugens eines Haystackfiles mit Suchwörtern aus einer Liste
	*/
	//string get_haystack_of(str_c _inputfilename);
	//list <string> get_haystack_of(list <string> _function_list);

	/*
	Prüfen, ob die dpv-Datensatz-Zeile zu den Zeichnungs-Settings gehört (DPV_ID_NAME_FILE)
	*/
	//bool check_data_row(string _id, string _dpv_ids);
	bool check_data_row(string _id, list <string> _dpv_ids);

	/*
	Umwandlung Const Char * in int
	*/
	template <typename T>
	bool getValueFromString(const std::string & value, T & result);

	int dpv_setting_name;
	int dpv_setting_value;
	int model_setting_name;
	int model_setting_value;

	string string_trim_white_spaces(string _inputString);

	str_c str_c_allocate_memory(str_c _input, ...);

	str_c str_c_free(str_c _input);

	bool g_nxLog_en_nx_dpv;

	list <string> function_list;
};


#endif