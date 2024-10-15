/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Basis-Klasse für folgende dpv Files:
*                 - Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv) ==> en_drawing_dpv_elements.h
*                 - NX SITE DPV XML File (nx120_Site.dpv)                    ==> en_nx_site_dpv_elements.h
*                 Grund: Auswertung der dpv-Settings zum Benutzen im Programmcode für alte Zeichnungen
***************************************************************************************/

#include "en_dpv_basic_elements.h"

en_dpv_basic_elements::en_dpv_basic_elements() {
	this->g_nxLog_en_dpv_basic_elements = this->set_nxLog();
}

en_dpv_basic_elements::en_dpv_basic_elements(string _dpv_application, string _dpv_category, string _dpv_subcategory, string _dpv_tab, string _dpv_subtab, string _dpv_display_value, string _dpv_mod_date, string _dpv_id_name, string _dpv_title, string _dpv_id_value, string _dpv_comment, string _dpv_id_value_ist) {
	this->g_nxLog_en_dpv_basic_elements = this->set_nxLog();
	//if (this->g_nxLog_en_dpv_basic_elements) this->nxLog("--- START - en_dpv_basic_elements::en_dpv_basic_elements --- \n");

	//this->dpv_application.clear();
	//this->dpv_category.clear();
	//this->dpv_subcategory.clear();
	//this->dpv_tab.clear();
	//this->dpv_subtab.clear();
	//this->dpv_display_value.clear();
	//this->dpv_mod_date.clear();
	//this->dpv_id_name.clear();
	//this->dpv_title.clear();
	//this->dpv_id_value.clear();
	//this->dpv_comment.clear();
	//this->dpv_id_value_ist.clear();

	//this->dpv_application.resize(_dpv_application.size()+(size_t)1);
	//this->dpv_category.resize(_dpv_category.size()+(size_t)1);
	//this->dpv_subcategory.resize(_dpv_subcategory.size()+(size_t)1);
	//this->dpv_tab.resize(_dpv_tab.size()+(size_t)1);
	//this->dpv_subtab.resize(_dpv_subtab.size()+(size_t)1);
	//this->dpv_display_value.resize(_dpv_display_value.size()+(size_t)1);
	//this->dpv_mod_date.resize(_dpv_mod_date.size()+(size_t)1);
	//this->dpv_id_name.resize(_dpv_id_name.size()+(size_t)1);
	//this->dpv_title.resize(_dpv_title.size()+(size_t)1);
	//this->dpv_id_value.resize(_dpv_id_value.size()+(size_t)1);
	//this->dpv_comment.resize(_dpv_comment.size()+(size_t)1);
	//this->dpv_id_value_ist.resize(_dpv_id_value_ist.size()+(size_t)1);
	
	this->dpv_application.assign(_dpv_application.c_str());
	this->dpv_category.assign(_dpv_category.c_str());
	this->dpv_subcategory.assign(_dpv_subcategory.c_str());
	this->dpv_tab.assign(_dpv_tab.c_str());
	this->dpv_subtab.assign(_dpv_subtab.c_str());
	this->dpv_display_value.assign(_dpv_display_value.c_str());
	this->dpv_mod_date.assign(_dpv_mod_date.c_str());
	this->dpv_id_name.assign(_dpv_id_name.c_str());
	this->dpv_title.assign(_dpv_title.c_str());
	this->dpv_id_value.assign(_dpv_id_value.c_str());
	this->dpv_comment.assign(_dpv_comment.c_str());
	this->dpv_id_value_ist.assign(_dpv_id_value_ist.c_str());
}

/*
printLog nur dann ausführen, wenn Umgebungsvariable
EN_UG_LOGLEVEL = 0 oder EN_UG_LOGLEVEL = TRUE oder EN_UG_LOGLEVEL = true oder EN_UG_LOGLEVEL = T oder EN_UG_LOGLEVEL = t
Existiert die Umgebungsvariable EN_UG_LOGLEVEL nicht, so printLog nicht ausführen. Nicht Case-Sensitiv!
Achtung: Erst nach einem TC-Neustart steht diese Variable zur Verfügnung!!!
Variable g_nxLog_en_dpv_basic_elements = true / false steuert hier die printLog-Ausgabe
*/
bool en_dpv_basic_elements::set_nxLog() {
	str_c chr_env_var = "EN_UG_LOGLEVEL";
	str_c log = NULL;
	errno_t err;
	size_t len;
	string log_level;
	this->g_nxLog_en_dpv_basic_elements = false;
	err = _dupenv_s(&log, &len, chr_env_var);
	if (err == 0 && len == 0) {
		this->g_nxLog_en_dpv_basic_elements = false;
		return false;
	}
	if (log != NULL) {
		int z = (int) len-1;
		char c;
		while (log[z] != '\0' && z>-1) {
			c = log[z];
			c = tolower(c);
			log[z] = c;
			z--;
		}
	}
	log_level = log;
	if (log_level == "t" || log_level == "true" || log_level == "0") {
		this->g_nxLog_en_dpv_basic_elements = true;
		return true;
	}
	this->g_nxLog_en_dpv_basic_elements = false;
	return false;
}

string en_dpv_basic_elements::get_double_as_string(double _double_value) {
	string returnvalue = std::to_string(_double_value);
	char last_char = returnvalue.back();
	while (last_char == '0') {
		returnvalue.pop_back();
		last_char = returnvalue.back();
	}
	while (last_char == '.') {
		returnvalue.pop_back();
		last_char = returnvalue.back();
	}
	size_t pos = 0;
	while ((pos = returnvalue.find('.')) != string::npos) {
		returnvalue.replace(pos, 1, ",");
	}
	return returnvalue;
}

string en_dpv_basic_elements::get_dpv_application() {
	return this->dpv_application;
}
string en_dpv_basic_elements::get_dpv_category() {
	return this->dpv_category;
}
string en_dpv_basic_elements::get_dpv_subcategory() {
	return this->dpv_subcategory;
}
string en_dpv_basic_elements::get_dpv_tab() {
	return this->dpv_tab;
}
string en_dpv_basic_elements::get_dpv_subtab() {
	return this->dpv_subtab;
}
string en_dpv_basic_elements::get_dpv_display_value() {
	return this->dpv_display_value;
}
string en_dpv_basic_elements::get_dpv_mod_date() {
	return this->dpv_mod_date;
}
string en_dpv_basic_elements::get_dpv_id_name() {
	return this->dpv_id_name;
}
string en_dpv_basic_elements::get_dpv_title() {
	return this->dpv_title;
}
string en_dpv_basic_elements::get_dpv_id_value() {
	return this->dpv_id_value;
}
string en_dpv_basic_elements::get_dpv_comment() {
	return this->dpv_comment;
}

string en_dpv_basic_elements::get_dpv_id_value_ist() {
	return this->dpv_id_value_ist;
}

void en_dpv_basic_elements::set_dpv_id_value_soll(string _dpv_id_value_soll) {
	//if (this->g_nxLog_en_dpv_basic_elements) this->nxLog("set_dpv_id_value_ist - _dpv_id_value_ist = %s \n", _dpv_id_value_ist.c_str());
	this->dpv_id_value.clear();
	this->dpv_id_value.assign(_dpv_id_value_soll.c_str());
	//if (this->g_nxLog_en_dpv_basic_elements) this->nxLog("set_dpv_id_value_ist - dpv_id_value_ist = %s \n", this->dpv_id_value_ist.c_str());
}

void en_dpv_basic_elements::set_dpv_id_value_ist(string _dpv_id_value_ist) {
	//if (this->g_nxLog_en_dpv_basic_elements) this->nxLog("set_dpv_id_value_ist - _dpv_id_value_ist = %s \n", _dpv_id_value_ist.c_str());
	this->dpv_id_value_ist.clear();
	this->dpv_id_value_ist.assign(_dpv_id_value_ist.c_str());
	//if (this->g_nxLog_en_dpv_basic_elements) this->nxLog("set_dpv_id_value_ist - dpv_id_value_ist = %s \n", this->dpv_id_value_ist.c_str());
}


/*==============================================================================
Programmierer : Kreissig
Datum         : 08.05.2019 - Kreissig
Last Update   :
Beschreibung  : LOG Meldung ausgeben
==============================================================================*/
void en_dpv_basic_elements::nxLog(char *_szFormatString, ...) {
	va_list ap;
	int nLogLevel = 0;
	char *szText = NULL;
	//szLogFile[201];
	//FILE *ptFile;

	str_c chr_env_var = "EN_UG_LOGLEVEL";
	str_c log = NULL;
	errno_t err;
	size_t env_var_len;
	string log_level;
	string dir;

	err = _dupenv_s(&log, &env_var_len, chr_env_var);

	if (err == 0 && env_var_len == 0) {
		int z = (int)env_var_len -1;
		char c;
		while (log[z] != '\0' && z>-1) {
			c = log[z];
			c = tolower(c);
			log[z] = c;
			z--;
		}
	}
	log_level = log;

	// Variable String auswerten
	va_start(ap, _szFormatString);
	env_var_len = _vscprintf(_szFormatString, ap) + 1; //Länge des Strings ermitteln + '\0'
	szText = (char *)calloc(env_var_len, sizeof(char));
	if (env_var_len>1) {
		vsprintf_s(szText, env_var_len, _szFormatString, ap);
	}
	
	va_end(ap);

	//str_c chr_env_tmp_var = "UGII_TMP_DIR";
	//str_c tmp_dir = NULL;
	//err = _dupenv_s(&tmp_dir, &len, chr_env_tmp_var);

	//if (err == 0 && len == 0) {
	//	int z = 0;
	//	char c;
	//	while (tmp_dir[z] != '\0') {
	//		c = tmp_dir[z];
	//		c = tolower(c);
	//		tmp_dir[z] = c;
	//		z++;
	//	}
	//}
	//dir = tmp_dir;

	//if ( !dir.empty() ) {
	//	sprintf_s(szLogFile, "%s\\en_nx_main.log", dir.c_str());
	//	if (fopen_s(&ptFile, szLogFile, "a+") == 0) {
	//		fprintf_s(ptFile, "%s", szText);
	//		fclose(ptFile);
	//	}
	//}

	// Ins SyslogFile schreiben
	if (szText != NULL) {
		UF_print_syslog(szText, FALSE);
	}
	

	if (nLogLevel == 0) {
		goto endPrintLog;
	}

	// String ins Listingfenster
	if (UF_UI_write_listing_window(szText) != 0) {
		UF_UI_open_listing_window();
		UF_UI_write_listing_window(szText);
	}

endPrintLog:
	if (szText) {
		free(szText);
	}
}
