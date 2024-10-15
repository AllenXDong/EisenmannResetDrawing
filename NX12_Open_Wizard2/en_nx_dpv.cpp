/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Basis-Klasse zum Arbeiten mit XML-Dateien (dpv-Dateien in NX)
*                 Grund: Abgleich der DPV-Settings mit dem Code
***************************************************************************************/

#include "en_nx_dpv.h"
#include <cstringt.h>

// Statische Variablen
en_nx_dpv *en_nx_dpv::instance = 0;

/*
Statischer Einstiegspunkt - die Basistabellen
- Drafting DPV XML File einlesen
- NX SITE DPV XML File einlesen
werden einmalig erzeugt, wenn noch nicht vorhanden.
*/
en_nx_dpv *en_nx_dpv::getInstance() {
	if (instance == 0) {
		instance = new en_nx_dpv();
	}
	return(instance);
}

//en_nx_dpv::en_nx_dpv() {
//	this->nxLog("--- START - en_nx_dpv::en_nx_dpv --- \n");
//	//this->set_nxLog();
//	this->g_nxLog_en_nx_dpv = this->set_nxLog();
//	this->nxLog("--- STOPP - en_nx_dpv::en_nx_dpv --- \n");
//}

en_nx_dpv::en_nx_dpv() {
	this->g_nxLog_en_nx_dpv = false;
	this->g_nxLog_en_nx_dpv = this->set_nxLog();
	//this->nxLog("en_nx_dpv::en_nx_dpv - this->g_nxLog_en_nx_dpv = %s \n", (this->g_nxLog_en_nx_dpv ? "true" : "false"));
	//this->g_nxLog_en_nx_dpv = true;
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::en_nx_dpv --- \n");
	//char * default_value;
	//UF_get_customer_default("UG_drawingBackgroundColor", 0, &default_value);
	//this->nxLog("en_nx_dpv::en_nx_dpv - test - default_value = \"%s\" \n", default_value);
	
	string tmp_dir;
	tmp_dir.assign("");
	if (this->init_tmp_dir(tmp_dir)) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::en_nx_dpv - tmp_dir = \"%s\" \n", tmp_dir.c_str());
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::en_nx_dpv - tmp_dir - ERROR \n");
		return;
	}
	
	bool result = false;
	if (tmp_dir != "") {
		result = init_lists(tmp_dir);
	}
	else {
		this->nxLog("en_nx_dpv::en_nx_dpv - tmp_dir = NULL - ERROR occured! \n");
		return;
	}
	
	tmp_dir.clear();
	//tmp_dir=this->str_c_free(tmp_dir);
	if (result == false) {
		this->nxLog("en_nx_dpv::en_nx_dpv::init_lists - ERROR occured! \n");
		return;
	}

	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::en_nx_dpv --- \n");
}

en_nx_dpv::~en_nx_dpv() {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::~en_nx_dpv --- \n");
	this->drafting_dpv_list.clear();
	this->nx_site_dpv_list.clear();
	this->instance = 0;
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::~en_nx_dpv --- \n");
}

/*
this->printLog nur dann ausführen, wenn Umgebungsvariable
EN_UG_LOGLEVEL = 0 oder EN_UG_LOGLEVEL = TRUE oder EN_UG_LOGLEVEL = true oder EN_UG_LOGLEVEL = T oder EN_UG_LOGLEVEL = t
Existiert die Umgebungsvariable EN_UG_LOGLEVEL nicht, so this->printLog nicht ausführen. Nicht Case-Sensitiv!
Achtung: Erst nach einem TC-Neustart steht diese Variable zur Verfügnung!!!
Variable this->g_nxLog_en_nx_dpv = true / false steuert hier die this->printLog-Ausgabe
*/
bool en_nx_dpv::set_nxLog() {
	/*
	//this->nxLog("--- START - en_nx_dpv::set_nxLog --- \n");
	ENenv globalVar("EN_UG_LOGLEVEL");
	string valueGlobalVariable;

	if (globalVar.exists()) {
		valueGlobalVariable.assign(globalVar.valToString());
		
		if ( !valueGlobalVariable.compare("") == 0) {
			int z = 0;
			char c;
			while (z<valueGlobalVariable.size()) {
				c = valueGlobalVariable[z];
				c = tolower(c);
				valueGlobalVariable[z] = c;
				z++;
			}
		}
		
		//this->nxLog("en_nx_dpv::set_nxLog - log_level = \"%s\" \n", log_level.c_str());
		if ( !valueGlobalVariable.compare("t") == 0 || !valueGlobalVariable.compare("true") == 0) {
			this->g_nxLog_en_nx_dpv = true;
			//this->nxLog("--- STOPP - en_nx_dpv::set_nxLog - return true --- \n");
			return true;
		}
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::get_global_variable_value - _valueGlobalVariable - ERROR --- \n");
		return false;
	}
	*/
	//this->nxLog("--- STOPP - en_nx_dpv::set_nxLog - 2 -return false --- \n");
	this->g_nxLog_en_nx_dpv = false;

	return false;
}

/*
Inhalt einer beliebigen globalen Variable ermitteln
*/
bool en_nx_dpv::get_global_variable_value(string _nameGlobalVariable, string &_valueGlobalVariable) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::get_global_variable_value --- \n");
	
	ENenv globalVar(_nameGlobalVariable.c_str());

	if (globalVar.exists()) {
		_valueGlobalVariable.assign(globalVar.valToString());
		return true;
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::get_global_variable_value - _valueGlobalVariable - ERROR --- \n");
		return false;
	}

	this->nxLog("--- STOPP - en_nx_dpv::get_global_variable_value - ERROR --- \n");
	return false;
}

/*
Verzeichnis folgender DPV-Dateien
- Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv)
- NX SITE DPV XML File (nx120_Site.dpv)
aus der globalen Variablen %UGII_SITE_DIR%\startup ermitteln
*/
bool en_nx_dpv::init_startup_dir(string &_startupDir) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::init_startup_dir --- \n");
	string env_dir;
	env_dir.assign("");
	string found_dirs;
	found_dirs.clear();
	if (this->get_global_variable_value("UGII_SITE_DIR", env_dir)) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_startup_dir - UGII_SITE_DIR = \"%s\" \n", env_dir.c_str());
		found_dirs.assign(env_dir);
		found_dirs.append("\\startup");
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_startup_dir - found_dirs = \"%s\" \n", found_dirs.c_str());
		//found_dirs.copy((*_startupDir), found_dirs.length() + 1, 0);
		_startupDir.assign(found_dirs);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_startup_dir --- \n");
		return true;
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_startup_dir - UGII_SITE_DIR - ERROR \n");
		return false;
	}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_startup_dir - ERROR --- \n");
	found_dirs.clear();
	
	return false;
}

/*
Verzeichnis fuer temporaere Vergleichsdatei
- Drafting DPV txt File - drafting_dpv_list (vgl_nx_EN_Drafting_Standard_Site_dpv.txt)
- NX SITE DPV txt File - nx_site_dpv_list (vgl_nx120_Site_dpv_txt)
aus der globalen Variablen %NX_TMP_DIR%\dpv_vgl ermitteln
*/
bool en_nx_dpv::init_tmp_dir(string &_tmpDir) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::init_tmp_dir --- \n");
	string env_dir;
	env_dir.assign("");
	string found_dirs;
	found_dirs.assign("");
	if (this->get_global_variable_value("NX_TMP_DIR", env_dir)) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - NX_TMP_DIR = \"%s\" \n", env_dir.c_str());
		found_dirs.assign(env_dir);
		found_dirs.append("\\dpv_vgl");
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - found_dirs = \"%s\" \n", found_dirs.c_str());
		//found_dirs.copy((*_tmpDir), found_dirs.length() + 1, 0);
		_tmpDir.assign(found_dirs);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_tmp_dir --- \n");
		try {
			char buf[500];
			sprintf_s(buf, "if exist ""%s"" rd /S /Q ""%s"" ", found_dirs.c_str(), found_dirs.c_str());
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - system: %s \n", buf);
			system(buf);

			/*
			int z = 0;
			while (fileExists(chr_dirs) == true && z < 50) {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - vor _sleep \n");
			_sleep(500); // halbe Sekunde warten
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - _sleep done \n");
			}
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - after _sleep done \n");

			if (z == 50) {
			this->nxLog("ERROR - en_nx_dpv::init_tmp_dir - file still exists! - false \n");
			return NULL;
			}
			*/

			//_wmkdir((const wchar_t *)chr_dirs);
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - mkdir(chr_dirs) - vor \n");
			//_mkdir(chr_dirs);
			sprintf_s(buf, "if not exist ""%s"" mkdir ""%s"" ", found_dirs.c_str(), found_dirs.c_str());
			system(buf);
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - mkdir(chr_dirs) - done \n");
		}
		catch (...) {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - mkdir not done! \n");
			return true;
		}
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_tmp_dir - OK --- \n");
		found_dirs.clear();
		return true;
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - NX_TMP_DIR - ERROR \n");
		return false;
	}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_tmp_dir - ERROR --- \n");
	found_dirs.clear();
	return false;
}

/*
Initiieren der Ausgangslisten
- drafting_dpv_list
- nx_site_dpv_list
*/
bool en_nx_dpv::init_lists(string _tmp_dir) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::init_lists --- \n");
	if (_tmp_dir == "") {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_lists - ERROR - _tmp_dir == NULL --- \n");
		return false;
	}
	this->drafting_dpv_list.clear();
	this->nx_site_dpv_list.clear();

	this->drafting_dpv_list.begin();
	this->nx_site_dpv_list.begin();

	bool res_function_list = false;
	res_function_list = this->create_function_list();
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - nach res_function_list \n");
	if (res_function_list == false) {
		this->nxLog("en_nx_dpv::init_lists - res_function_list = false \n");
		this->nxLog("en_nx_dpv::init_lists - ERROR: create_list of \"%s\" erroneus! \n", DPV_ID_NAME_FILE);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_lists --- \n");
		return false;
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) if(this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - res_function_list = true \n");

		//Test-Ausgabe in Text-Files erzeugen ==> Voraussetzung: EN_UG_LOGLEVEL == 0 oder TRUE oder t (Nicht Case-Sensitiv!)
		//this->create_text_file_from_dpv(this->drafting_dpv_list, DRAFTING_DPV_FILE_NAME);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - create_list of \"%s\" done! \n", DPV_ID_NAME_FILE);
	}

	bool result = false;
	string drafting_dpv_file_name;
	drafting_dpv_file_name.assign(DRAFTING_DPV_FILE_NAME);
	result = create_list(_tmp_dir, drafting_dpv_file_name);
	if (result == false) {
		//string tmp;
		//tmp.assign(DRAFTING_DPV_FILE_NAME);
		//tmp = DRAFTING_DPV_FILE_NAME;
		//this->nxLog("en_nx_dpv::init_lists - ERROR: create_list of \"%s\" erroneus! \n", tmp.c_str());
		this->nxLog("en_nx_dpv::init_lists - ERROR: create_list of \"%s\" erroneus! \n", DRAFTING_DPV_FILE_NAME);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_lists --- \n");
		return false;
	}
	else {
		//Test-Ausgabe in Text-Files erzeugen ==> Voraussetzung: EN_UG_LOGLEVEL == 0 oder TRUE oder t (Nicht Case-Sensitiv!)
		//this->create_text_file_from_dpv(this->drafting_dpv_list, DRAFTING_DPV_FILE_NAME);
		//string tmp;
		//tmp.assign(DRAFTING_DPV_FILE_NAME);
		//tmp = DRAFTING_DPV_FILE_NAME;
		////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - create_list of \"%s\" done! \n", tmp.c_str());
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - create_list of \"%s\" done! \n", DRAFTING_DPV_FILE_NAME);
	}
	string site_dpv_file_name;
	site_dpv_file_name.assign(NX_DPV_SITE_FILE_NAME);

	result = create_list(_tmp_dir, site_dpv_file_name);
	if (result == false) {
		//string tmp;
		//tmp.assign(NX_DPV_SITE_FILE_NAME);
		//tmp = NX_DPV_SITE_FILE_NAME;
		//this->nxLog("en_nx_dpv::init_lists - ERROR: create_list of \"%s\" erroneus! \n", tmp.c_str());
		this->nxLog("en_nx_dpv::init_lists - ERROR: create_list of \"%s\" erroneus! \n", NX_DPV_SITE_FILE_NAME);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_lists --- \n");
		return false;
	}
	else {
		//Test-Ausgabe in Text-Files erzeugen ==> Voraussetzung: EN_UG_LOGLEVEL == 0 oder TRUE oder t (Nicht Case-Sensitiv!)
		//this->create_text_file_from_dpv(this->nx_site_dpv_list, NX_DPV_SITE_FILE_NAME);
		//string tmp;
		//tmp.assign(NX_DPV_SITE_FILE_NAME);
		//tmp = NX_DPV_SITE_FILE_NAME;
		////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - create_list of \"%s\" done! \n", tmp.c_str());
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_lists - create_list of \"%s\" done! \n", NX_DPV_SITE_FILE_NAME);
	}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::init_lists --- \n");
	return true;
}

/*
File exists?
*/
bool en_nx_dpv::fileExists(str_c _fileName) {
	if (this->g_nxLog_en_nx_dpv)this->nxLog("--- START - en_nx_dpv::fileExists --- \n");
	string tmp_filename;
	tmp_filename.assign((string)_fileName);
	if (this->g_nxLog_en_nx_dpv)this->nxLog("en_nx_dpv::fileExists - str_c - tmp_filename = \"%s\" \n", tmp_filename.c_str());
	ifstream file(tmp_filename.c_str(), std::ifstream::in);
	if (this->g_nxLog_en_nx_dpv)this->nxLog("en_nx_dpv::fileExists - ifstream file(...) done \n");
	if (file.fail()) {
		if (this->g_nxLog_en_nx_dpv)this->nxLog("--- STOPP - en_nx_dpv::fileExists - false --- \n");
		//tmp_filename.clear();
		return false;
	}
	else {
		file.close();
		if (this->g_nxLog_en_nx_dpv)this->nxLog("--- STOPP - en_nx_dpv::fileExists - true --- \n");
		//tmp_filename.clear();
		return true;
	}
	this->nxLog("--- STOPP - en_nx_dpv::fileExists - ERROR: file.fail() - false --- \n");
	//tmp_filename.clear();
	return false;
}

/*
File exists?
*/
bool en_nx_dpv::fileExists(string _fileName) {
	if (this->g_nxLog_en_nx_dpv)this->nxLog("--- START - en_nx_dpv::fileExists --- \n");
	if (this->g_nxLog_en_nx_dpv)this->nxLog("en_nx_dpv::fileExists - string - tmp_filename = \"%s\" \n", _fileName.c_str());
	ifstream file(_fileName.c_str());

	if (this->g_nxLog_en_nx_dpv)this->nxLog("en_nx_dpv::fileExists - ifstream file(...) done \n");
	if (file.fail()) {
		if (this->g_nxLog_en_nx_dpv)this->nxLog("--- STOPP - en_nx_dpv::fileExists - false --- \n");
		return false;
	}
	else {
		file.close();
		if (this->g_nxLog_en_nx_dpv)this->nxLog("--- STOPP - en_nx_dpv::fileExists - true --- \n");
		return true;
	}
	this->nxLog("--- STOPP - en_nx_dpv::fileExists - ERROR: file.fail() - false --- \n");
}

/*
Erzeugen von genau einer speziellen Liste
*/
bool en_nx_dpv::create_list(string _tmp_dir, string _dpv_list) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::create_list --- \n");
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - _tmp_dir = \"%s\" \n", _tmp_dir.c_str());
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - _dpv_list = \"%s\" \n", _dpv_list.c_str());
	//str_c customer_dpv_file_name = "";
	string customer_dpv_file_name;
	customer_dpv_file_name.assign("");

	if (_dpv_list == DRAFTING_DPV_FILE_NAME) {
		//customer_dpv_file_name = DRAFTING_DPV_FILE_NAME;
		customer_dpv_file_name.assign(DRAFTING_DPV_FILE_NAME);
	}
	else if (_dpv_list == NX_DPV_SITE_FILE_NAME) {
		//customer_dpv_file_name = NX_DPV_SITE_FILE_NAME;
		//customer_dpv_file_name = str_c_allocate_memory(NX_DPV_SITE_FILE_NAME);
		customer_dpv_file_name.assign(NX_DPV_SITE_FILE_NAME);
	}
	if (customer_dpv_file_name == "") {
		this->nxLog("en_nx_dpv::create_list - ERROR - No default dpv-file found! \n");
		this->nxLog("en_nx_dpv::create_list - ERROR - STOPP \n");
		//customer_dpv_file_name=this->str_c_free(customer_dpv_file_name);
		customer_dpv_file_name.clear();
		return false;
	}
	string tmp_file_dir_name;
	string startup_dir;
	startup_dir.assign("");
	//startup_dir = str_c_allocate_memory(this->init_startup_dir());
	//startup_dir = this->init_startup_dir();
	if (this->init_startup_dir(startup_dir)) {
		//startup_dir = str_c_allocate_memory(startup_dir);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - default_dpv_dir_file_ext_name - startup_dir = \"%s\" \n", startup_dir.c_str());
	}
	else {
		this->nxLog("en_nx_dpv::create_list - default_dpv_dir_file_ext_name - startup_dir ERROR occured \n");
		return false;
	}
	
	tmp_file_dir_name.assign(startup_dir);
	tmp_file_dir_name.append("\\");
	tmp_file_dir_name.append(customer_dpv_file_name);
	tmp_file_dir_name.append(".dpv");
	
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - default_dpv_dir_file_ext_name - customer_dpv_file_name = \"%s\" \n", customer_dpv_file_name.c_str());
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - default_dpv_dir_file_ext_name - tmp_file_dir_name = \"%s\" \n", tmp_file_dir_name.c_str());
	str_c default_dpv_dir_file_ext_name = new char[tmp_file_dir_name.size() + 2];
	strcpy_s(default_dpv_dir_file_ext_name, tmp_file_dir_name.size() + 2, tmp_file_dir_name.c_str());

	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - vor ifstream \n");
	ifstream default_dpv_file;
	default_dpv_file.clear();
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - vor default_dpv_file.open - in \n");
	default_dpv_file.open(default_dpv_dir_file_ext_name, std::ifstream::in);
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - nach ifstream - default_dpv_file.open \n");

	bool res = false;

	if (!default_dpv_file.is_open()) {
		this->nxLog("en_nx_dpv::create_list - ERROR - Can not open file - default_dpv_dir_file_ext_name = \"%s\" \n", default_dpv_dir_file_ext_name);
		this->nxLog("en_nx_dpv::create_list - ERROR - STOPP \n");
		//customer_dpv_file_name=this->str_c_free(customer_dpv_file_name);
		//startup_dir=this->str_c_free(startup_dir);
		customer_dpv_file_name.clear();
		tmp_file_dir_name.clear();
		return false;
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - default_dpv_dir_file_ext_name = \"%s\" - is open!!!\n", default_dpv_dir_file_ext_name);

		string dataset_line;
		dataset_line.clear();
		dataset_line.assign("");
		int nr_data_rows = 0;

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

		dpv_application.clear();
		dpv_category.clear();
		dpv_subcategory.clear();
		dpv_tab.clear();
		dpv_subtab.clear();
		dpv_display_value.clear();
		dpv_mod_date.clear();
		dpv_id_name.clear();
		dpv_title.clear();
		dpv_id_value.clear();
		dpv_comment.clear();

		while ( getline(default_dpv_file, dataset_line) ) {
			dpv_application.assign("Application");
			dpv_category.assign("Category");
			dpv_subcategory.assign("SubCategory");
			dpv_tab.assign("Tab");
			dpv_subtab.assign("Sub-Tab");
			dpv_display_value.assign("displayValue");
			dpv_mod_date.assign("modified");
			dpv_id_name.assign("name");
			dpv_title.assign("title");
			dpv_id_value.assign("value");
			dpv_comment.assign("UserComment");
			size_t pos = 0;
			nr_data_rows++;
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dataset_line [%d] = \"%s\" \n", nr_data_rows, dataset_line.c_str());

			//if (!dataset_line.empty() || !dataset_line.compare("") == 0) {
			if (!dataset_line.compare("") == 0) {
				//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dataset_line <> "" \n");
			//if (dataset_line.length() > 0 || !dataset_line.empty() ) {
				while ((pos = dataset_line.find("<Pref ", pos)) != string::npos) {
					//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - pos = %d \n", (int)pos);
					bool in_id_dpv_list = false;
					string tmp_id_name;
					tmp_id_name.clear();
					tmp_id_name.assign("");
					if (!dataset_line.compare("") == 0) {
						//name
						size_t tmp_pos = 0;
						size_t tmp_pos_start = 0;
						size_t tmp_pos_end = 0;
						string tmp_dataset_zell;
						tmp_dataset_zell.clear();
						tmp_dataset_zell.assign("");
						if (dataset_line.find(dpv_id_name.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - tmp - dpv_id_name exists \n");
							tmp_pos_start = dataset_line.find(dpv_id_name.c_str(), (size_t)0) + dpv_id_name.size() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name - tmp_pos_start = %d \n", (int)tmp_pos_start);
							tmp_pos_end = dataset_line.find(char('"'), tmp_pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name - tmp_pos_end = %d \n", (int)tmp_pos_end);
							tmp_dataset_zell.assign(dataset_line.substr(tmp_pos_start, tmp_pos_end - tmp_pos_start + (size_t)1).c_str());
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - tmp - dpv_id_name - tmp_dataset_zell = \"%s\" \n", tmp_dataset_zell.c_str());
							tmp_id_name.assign(tmp_dataset_zell.c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - tmp_id_name = \"%s\" \n", tmp_id_name.c_str());
							//in_id_dpv_list = this->check_data_row(tmp_id_name, this->haystack);
							in_id_dpv_list = this->check_data_row(tmp_id_name, this->function_list);
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - check_data_row is done! \n");
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - in_id_dpv_list = %s \n", (in_id_dpv_list ? "true" : "false"));
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - tmp - dpv_id_name does not exists !!! \n");
							tmp_id_name;
							tmp_id_name.clear();
							tmp_id_name.assign("");
							in_id_dpv_list = false;
						}
					}
					else {
						in_id_dpv_list = false;
					}
					if (in_id_dpv_list == true) {
						//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dataset found and will be added: \n");
						size_t pos_start = 0;
						size_t pos_end = 0;

						string dataset_zell;
						//dataset_zell.clear();

						string application;
						string category;
						string subcategory;
						string tab;
						string subtab;
						string display_value;
						string mod_date;
						string id_name;
						string title;
						string id_value;
						string comment;


						//application.clear();
						//category.clear();
						//subcategory.clear();
						//tab.clear();
						//subtab.clear();
						//display_value.clear();
						//mod_date.clear();
						//id_name.clear();
						//title.clear();
						//id_value.clear();
						//comment.clear();

						// Application
						if (dataset_line.find(dpv_application.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_application exists \n");
							pos_start = dataset_line.find(dpv_application.c_str(), (size_t)0) + dpv_application.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_application - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_application - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_application - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							application.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_application does not exists !!! \n");
							application.clear();
							application.assign("");
						}
						//Category
						if (dataset_line.find(dpv_category.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_category exists \n");
							pos_start = dataset_line.find(dpv_category.c_str(), (size_t)0) + dpv_category.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_category - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_category - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_category - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							category.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_category does not exists !!! \n");
							category.clear();
							category.assign("");
						}
						//Sub-Category
						if (dataset_line.find(dpv_subcategory.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subcategory exists \n");
							pos_start = dataset_line.find(dpv_subcategory.c_str(), (size_t)0) + dpv_subcategory.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subcategory - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subcategory - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subcategory - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							subcategory.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subcategory does not exists !!! \n");
							subcategory.clear();
							subcategory.assign("");
						}
						//Tab
						if (dataset_line.find(dpv_tab.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_tab exists \n");
							pos_start = dataset_line.find(dpv_tab.c_str(), (size_t)0) + dpv_tab.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_tab - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_tab - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_tab - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							tab.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_tab does not exists !!! \n");
							tab.clear();
							tab.assign("");
						}
						//Sub-Tab
						if (dataset_line.find(dpv_subtab.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subtab exists \n");
							pos_start = dataset_line.find(dpv_subtab.c_str(), (size_t)0) + dpv_subtab.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subtab - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subtab - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subtab - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							subtab.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_subtab does not exists !!! \n");
							subtab.clear();
							subtab.assign("");
						}
						//displayValue
						if (dataset_line.find(dpv_display_value.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_display_value exists \n");
							pos_start = dataset_line.find(dpv_display_value.c_str(), (size_t)0) + dpv_display_value.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_display_value - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_display_value - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_display_value - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							display_value.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_display_value does not exists !!! \n");
							display_value.clear();
							display_value.assign("");
						}
						//modified
						if (dataset_line.find(dpv_mod_date.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_mod_date exists \n");
							pos_start = dataset_line.find(dpv_mod_date.c_str(), (size_t)0) + dpv_mod_date.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_mod_date - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_mod_date - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_mod_date - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							mod_date.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_mod_date does not exists !!! \n");
							mod_date.clear();
							mod_date.assign("");
						}
						//name
						if (dataset_line.find(dpv_id_name.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name exists \n");
							pos_start = dataset_line.find(dpv_id_name.c_str(), (size_t)0) + dpv_id_name.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_name - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							id_name.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title does not exists !!! \n");
							id_name.clear();
							id_name.assign("");
						}
						//title
						if (dataset_line.find(dpv_title.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title exists \n");
							pos_start = dataset_line.find(dpv_title.c_str(), (size_t)0) + dpv_title.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							title.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_title does not exists !!! \n");
							title.clear();
							title.assign("");
						}
						//value
						if (dataset_line.find(dpv_id_value.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_value exists \n");
							pos_start = dataset_line.find(dpv_id_value.c_str(), (size_t)0) + dpv_id_value.length() + (size_t)2;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_value - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_value - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//char * id_name = NULL;
							//id_name = (char *) rec.name.c_str();
							//char * default_value = NULL;
							//UF_get_customer_default(id_name, 0, &default_value);
							//dataset_zell = (string) default_value;
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_value - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							id_value.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_id_value does not exists !!! \n");
							id_value.clear();
							id_value.assign("");
						}
						//UserComment
						if (dataset_line.find(dpv_comment.c_str(), (size_t)0) != string::npos) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_comment exists \n");
							pos_start = dataset_line.find(dpv_comment.c_str(), (size_t)0) + dpv_comment.length() + (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_comment - pos_start = %d \n", (int)pos_start);
							pos_end = dataset_line.find(char('<'), pos_start) - (size_t)1;
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_comment - pos_end = %d \n", (int)pos_end);
							dataset_zell.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_comment - dataset_zell = \"%s\" \n", dataset_zell.c_str());
							comment.assign(dataset_zell.c_str());
						}
						else {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - dpv_comment does not exists !!! \n");
							comment.clear();
							comment.assign("");
						}

						//char * search_filename;
						//search_filename = DRAFTING_DPV_FILE_NAME;

						if (!strcmp(_dpv_list.c_str(), DRAFTING_DPV_FILE_NAME)) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - Add drawing_dataset \n");
							en_drawing_dpv_elements drawing_dataset = en_drawing_dpv_elements(application, category, subcategory, tab, subtab, display_value, mod_date, id_name, title, id_value, comment, "");
							this->drafting_dpv_list.push_back(drawing_dataset);
							this->soll_dpv_map.insert({ (const string) drawing_dataset.get_dpv_id_name(), drawing_dataset.get_dpv_id_value() });
							//this->soll_dpv_map[(const string)drawing_dataset.get_dpv_id_name()] = drawing_dataset.get_dpv_id_value();
							//this->soll_dpv_map->insert({ &drawing_dataset.get_dpv_id_name(), &drawing_dataset.get_dpv_id_value() });
							//this->soll_dpv_map->insert({ drawing_dataset.get_dpv_id_name(), drawing_dataset.get_dpv_id_value() });
							//this->soll_dpv_map->insert(pair<std::string, std::string> (drawing_dataset.get_dpv_id_name(), drawing_dataset.get_dpv_id_value()));
							//this->soll_dpv_map[drawing_dataset.get_dpv_id_name()] = drawing_dataset.get_dpv_id_value();
							//this->ist_dpv_map.insert({ drawing_dataset.get_dpv_id_name(), "" });
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - added drawing_dataset: \n%s \n", drawing_dataset.get_drawing_dpv_dataset().c_str());
						}
						//search_filename = NX_DPV_SITE_FILE_NAME;
						if (!strcmp(_dpv_list.c_str(), string(NX_DPV_SITE_FILE_NAME).c_str())) {
							//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - Add nxsite_dataset \n");
							en_nx_site_dpv_elements nxsite_dataset = en_nx_site_dpv_elements(application, category, subcategory, tab, subtab, display_value, mod_date, id_name, title, id_value, comment, "");
							this->nx_site_dpv_list.push_back(nxsite_dataset);
							this->soll_dpv_map.insert({ (const string) nxsite_dataset.get_dpv_id_name(), nxsite_dataset.get_dpv_id_value() });
							//this->soll_dpv_map[(const string)nxsite_dataset.get_dpv_id_name()] = nxsite_dataset.get_dpv_id_value();
							//this->soll_dpv_map->insert({ nxsite_dataset.get_dpv_id_name(), nxsite_dataset.get_dpv_id_value() });
							//this->ist_dpv_map.insert({ nxsite_dataset.get_dpv_id_name(), "" });
							//this->soll_dpv_map->insert(pair<std::string, std::string>(nxsite_dataset.get_dpv_id_name(), nxsite_dataset.get_dpv_id_value()));
							//this->soll_dpv_map[nxsite_dataset.get_dpv_id_name()] = nxsite_dataset.get_dpv_id_value();
							////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - added nxsite_dataset: \n%s \n", nxsite_dataset.get_nx_site_dpv_dataset().c_str());
						}
					}
					pos = dataset_line.length();
					//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_list - new pos = %d \n", pos);
				}
			}
			dataset_line.clear();
			dataset_line.assign( "" );
		}
	}
	default_dpv_file.close();

	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::create_list --- \n");
	//customer_dpv_file_name=this->str_c_free(customer_dpv_file_name);
	customer_dpv_file_name.clear();
	//startup_dir=this->str_c_free(startup_dir);
	startup_dir.clear();
	tmp_file_dir_name.clear();
	return true;
}

/*
drafting_dpv_list: Testausgabe der gefilterten/ungefilterten Liste ins Verzeichnis C:\Temp\NX\<User-ID>\dpv_vgl\
==> Testausgabe erfolgt nur mit gesetzter Umgebungsvarialbe EN_UG_LOGLEVEL == 0 oder true oder t (Nicht Case-Sensitiv!)
*/
bool en_nx_dpv::create_text_file_from_dpv(list<en_drawing_dpv_elements> _list, string _list_name) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::create_text_file_from_dpv --- \n");
	//if (this->g_nxLog_en_nx_dpv) {
		bool result = false;

		string env_var;
		env_var.assign("");
		if (this->get_global_variable_value("UGII_TMP_DIR", env_var)) {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - env_var = \"%s\" \n", env_var.c_str());
		}
		else {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - env_var - ERROR \n");
			return false;
		}

		string tmp_dir;
		tmp_dir.assign(env_var);
		string dpv_list_tmp_dir;
		dpv_list_tmp_dir.assign( tmp_dir );
		dpv_list_tmp_dir.append("\\dpv_vgl");

		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - dpv_list_tmp_dir = \"%s\" \n", dpv_list_tmp_dir.c_str());

		try {
			// _wmkdir((const wchar_t *) dpv_list_tmp_dir.c_str());
			char buf[500];
			sprintf_s(buf, "if not exist ""%s"" mkdir ""%s"" ", dpv_list_tmp_dir.c_str(), dpv_list_tmp_dir.c_str());
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - system: %s \n", buf);
			system(buf);
		}
		catch (...) {
			this->nxLog("en_nx_dpv::create_text_file_from_dpv - Directory ERROR: dpv_list_tmp_dir = \"%s\" \n", dpv_list_tmp_dir.c_str());
			this->nxLog("--- en_nx_dpv::create_text_file_from_dpv - ERROR - STOPP --- \n");
			

			//tmp_dir.clear();
			//dpv_list_tmp_dir.clear();
			return false;
		}

		string b_list_dir_file_ext_name;
		b_list_dir_file_ext_name.assign( dpv_list_tmp_dir + "\\" + _list_name + ".txt" );
		
		/*
		if (this->fileExists(b_list_dir_file_ext_name)) {
			//Delete this file
			char buf[500];
			sprintf_s(buf, "del %s /q", b_list_dir_file_ext_name.c_str());
			system(buf);
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - system: %s \n", buf);
			this->str_c_free(buf);
		}
		*/

		//remove(b_list_dir_file_ext_name.c_str());

		//fstream drafting_dpv_list_file;
		//drafting_dpv_list_file.open(b_list_dir_file_ext_name.c_str(), fstream::app);
		
		//ofstream drafting_dpv_list_file(b_list_dir_file_ext_name.c_str(), std::ofstream::out | std::ofstream::trunc);
		ofstream drafting_dpv_list_file(b_list_dir_file_ext_name.c_str(), std::ofstream::out );
		
		if (!drafting_dpv_list_file.is_open()) {
		//if (drafting_dpv_list_file.fail()) {
			this->nxLog("- en_nx_dpv::create_text_file_from_dpv - File ERROR: b_list_dir_file_ext_name = \"%s\" \n", b_list_dir_file_ext_name.c_str());
			return false;
		}
		else {
			drafting_dpv_list_file << _list_name << ".txt ==> Number of found datasets: " << _list.size() << "\n";
			drafting_dpv_list_file.flush();
			list<en_drawing_dpv_elements>::iterator dataset_iterator = _list.begin();
			for (dataset_iterator = _list.begin(); dataset_iterator != _list.end(); dataset_iterator++) {
				string dataset;
				dataset.clear();
				dataset.assign( dataset_iterator->get_drawing_dpv_dataset() );
				drafting_dpv_list_file << dataset.c_str() << "\n";
				drafting_dpv_list_file.flush();
			}
		}
		drafting_dpv_list_file.close();
		drafting_dpv_list_file.clear();
		
		//tmp_dir.clear();
		//dpv_list_tmp_dir.clear();
	//}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::create_text_file_from_dpv --- \n");
	return true;
}

/*
nx_site_dpv_list: Testausgabe der gefilterten/ungefilterten Liste ins Verzeichnis C:\Temp\NX\<User-ID>\dpv_vgl\
==> Testausgabe erfolgt nur mit gesetzter Umgebungsvarialbe EN_UG_LOGLEVEL == 0 oder true oder t (Nicht Case-Sensitiv!)
*/
bool en_nx_dpv::create_text_file_from_dpv(list<en_nx_site_dpv_elements> _list, string _list_name) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::create_text_file_from_dpv --- \n");
	//if (this->g_nxLog_en_nx_dpv) {
		bool result = false;

		string env_var;
		env_var.assign("");
		if (this->get_global_variable_value("UGII_TMP_DIR", env_var)) {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - env_var = \"%s\" \n", env_var.c_str());
		}
		else {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - env_var - ERROR \n");
			return false;
		}

		string tmp_dir;
		tmp_dir.assign(env_var);
		string dpv_list_tmp_dir;
		dpv_list_tmp_dir.assign(tmp_dir);
		dpv_list_tmp_dir.append("\\dpv_vgl");

		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - dpv_list_tmp_dir = \"%s\" \n", dpv_list_tmp_dir.c_str());

		try {
			//_wmkdir((const wchar_t *)dpv_list_tmp_dir.c_str());
			char buf[500];
			sprintf_s(buf, "if not exist ""%s"" mkdir ""%s"" ", dpv_list_tmp_dir.c_str(), dpv_list_tmp_dir.c_str());
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::init_tmp_dir - system: %s \n", buf);
			system(buf);
		}
		catch (...) {
			this->nxLog("en_nx_dpv::create_text_file_from_dpv - Directory ERROR: dpv_list_tmp_dir = \"%s\" \n", dpv_list_tmp_dir.c_str());
			this->nxLog("--- en_nx_dpv::create_text_file_from_dpv - ERROR - STOPP --- \n");
			//tmp_dir.clear();
			//dpv_list_tmp_dir.clear();
			return false;
		};

		string b_list_dir_file_ext_name;
		b_list_dir_file_ext_name.assign( dpv_list_tmp_dir + "\\" + _list_name + ".txt" );

		/*
		if (this->fileExists(b_list_dir_file_ext_name)) {
			//Delete this file
			char buf[500];
			sprintf_s(buf, "del %s /q", b_list_dir_file_ext_name.c_str());
			system(buf);
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_text_file_from_dpv - system: %s \n", buf);
			this->str_c_free(buf);
		}
		*/

		//remove(b_list_dir_file_ext_name.c_str());

		//fstream nx_site_dpv_list_file;
		//nx_site_dpv_list_file.open(b_list_dir_file_ext_name.c_str(), fstream::app);

		//ofstream nx_site_dpv_list_file(b_list_dir_file_ext_name.c_str(), std::ofstream::out | std::ofstream::trunc);
		ofstream nx_site_dpv_list_file(b_list_dir_file_ext_name.c_str(), std::ofstream::out );

		if (!nx_site_dpv_list_file.is_open()) {
		//if (nx_site_dpv_list_file.fail()) {
			this->nxLog("- en_nx_dpv::create_text_file_from_dpv - File ERROR: b_list_dir_file_ext_name = \"%s\" \n", b_list_dir_file_ext_name.c_str());
			return false;
		}
		else {
			nx_site_dpv_list_file << _list_name << ".txt ==> Number of found datasets: " << _list.size() << "\n";
			nx_site_dpv_list_file.flush();
			list<en_nx_site_dpv_elements>::iterator dataset_iterator = _list.begin();
			for (dataset_iterator = _list.begin(); dataset_iterator != _list.end(); dataset_iterator++) {
				string dataset;
				dataset.assign( dataset_iterator->get_nx_site_dpv_dataset() );
				nx_site_dpv_list_file << dataset.c_str() << "\n";
				nx_site_dpv_list_file.flush();
			};
		};
		nx_site_dpv_list_file.close();
		nx_site_dpv_list_file.clear();
		//tmp_dir.clear();
		//dpv_list_tmp_dir.clear();
	//}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::create_text_file_from_dpv --- \n");
	return true;
};

/*==============================================================================
Programmierer : Kreissig
Datum         : 08.05.2019 - Kreissig
Last Update   :
Beschreibung  : LOG Meldung ausgeben
==============================================================================*/
void en_nx_dpv::nxLog(char *_szFormatString, ...) {
	//UF_print_syslog("nxLog - START \n", FALSE);
	va_list ap;
	char *szText = NULL;
	int len;

	// Variable String auswerten
	//UF_print_syslog("nxLog - Absturzstelle 1 \n", FALSE);
	va_start(ap, _szFormatString);
	//UF_print_syslog("nxLog - Absturzstelle 2 \n", FALSE);
	len = _vscprintf(_szFormatString, ap) + 1; //Länge des Strings ermitteln + '\0'
	//char buf[30];
	//sprintf_s(buf, "nxLog - len = %d \n", (int) len);
	//UF_print_syslog(buf, FALSE);
	//UF_print_syslog("nxLog - Absturzstelle 3 \n", FALSE);
	szText = (char *)calloc(len, sizeof(char));
	//UF_print_syslog("nxLog - calloc \n", FALSE);
	//UF_print_syslog("nxLog - Absturzstelle 4 \n", FALSE);
	if (len > 1) {
		vsprintf_s(szText, len, _szFormatString, ap);
	}
	//UF_print_syslog("nxLog - vsprintf_s \n", FALSE);
	//szText = this->str_c_allocate_memory(szText);
	//UF_print_syslog("nxLog - Absturzstelle 5 \n", FALSE);
	va_end(ap);
	//UF_print_syslog("nxLog - Absturzstelle 6 \n", FALSE);

	//UF_print_syslog("nxLog - vor printf \n", FALSE);
	if (printf("%s", szText)) {
		//UF_print_syslog("nxLog - vor NULL \n", FALSE);
		if (szText != NULL) {
			//UF_print_syslog("nxLog - vor UF_print_syslog \n", FALSE);
			UF_print_syslog(szText, FALSE);
			//UF_print_syslog("nxLog - vor delete \n", FALSE);
			delete(szText);
			//free(szText);
			//_freea(szText); (zuletzt)
			//UF_print_syslog("nxLog - _freea - OK \n", FALSE);
		}		
		//UF_print_syslog("nxLog - nach NULL \n", FALSE);
	}
	//UF_print_syslog("nxLog - OK \n", FALSE);
}

/*
Umwandlung Const Char * in int
*/
template <typename T>
bool en_nx_dpv::getValueFromString(const std::string & value, T & result)
{
	std::istringstream iss(value);
	return !(iss >> result).fail();
	//return !(iss >> result).bad();
}


/*
Initiieren des function_list - bestimmt, welche Einstellungen abgeglichen werden sollen!
*/
bool en_nx_dpv::create_function_list() {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::create_function_list --- \n");

	string env_dir;
	env_dir.assign("");
	if (this->get_global_variable_value("UGII_SITE_DIR", env_dir)) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - UGII_SITE_DIR = %s \n", env_dir.c_str());
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - UGII_SITE_DIR - ERROR \n");
		return false;
	}
	
	string dpv_site_file_name;
	string tmp_file_name;
	tmp_file_name = string(NX_DPV_SITE_FILE_NAME);
	dpv_site_file_name.assign(tmp_file_name.c_str());
	string Standard_Site;
	Standard_Site.assign(env_dir);
	Standard_Site.append("\\startup\\");
	Standard_Site.append(dpv_site_file_name);
	Standard_Site.append(".dpv");
	this->nxLog("en_nx_dpv::create_function_list - Standard_Site = \"%s\" \n", Standard_Site.c_str());
	//UG_initialUnits
	ifstream dpv_site_file;
	dpv_site_file.open(Standard_Site.c_str(), std::ofstream::in);
	if (!dpv_site_file.fail()) {
		if (dpv_site_file.is_open()) {
			dpv_site_file.seekg(0, ios_base::beg);
		}
	}
	else {
		this->nxLog("en_nx_dpv::create_function_list - ERROR - open dpv_site_file \n");
		return false;
	}
	
	string dataset_line;
	string unitValue;
	unitValue.assign("value");

	while (getline(dpv_site_file, dataset_line)) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - dataset_line=%s \n", dataset_line.c_str());
		if (dataset_line.find("UG_initialUnits", (size_t)0) != string::npos) {
			size_t pos_start;
			pos_start = dataset_line.find(unitValue.c_str(), (size_t)0) + unitValue.length() + (size_t)2;
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - pos_start=%d \n", (int)pos_start);
			size_t pos_end = dataset_line.find(char('"'), pos_start) - (size_t)1;
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - pos_end=%d \n", (int)pos_end);
			this->usedUnitValue.assign(dataset_line.substr(pos_start, pos_end - pos_start + (size_t)1).c_str());
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - this->usedUnitValue = %s \n", this->usedUnitValue.c_str());
			break;
		}
		else {
			this->usedUnitValue.assign("1");
		}
	}
	dpv_site_file.close();

	this->function_list.clear();

	//Inhalt nx_EN_Drafting_Standard_Site.dpv

	/*
	********* DraftingStandard_General_Standard_prefs *********
	*/
	//this->function_list.push_back("Drafting_centerLineDisplay");
	this->function_list.push_back("Drafting_SFsymbolStandard");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_weldSymbolStandard_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_weldSymbolStandard_EU");
	//this->function_list.push_back("Drafting_limitsAndFitsStandard");
	//this->function_list.push_back("Drafting_datumFeatureDisplay");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_checkStandard_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_checkStandard_EU");
	//this->function_list.push_back("Gdt_standard");

	/*
	********* DraftingStandard_Common_Lettering_prefs *********
	*/
	this->function_list.push_back("Drafting_entitySite");
	this->function_list.push_back("Drafting_horizontalTextJust");
	this->function_list.push_back("Drafting_draftTextColor");
	this->function_list.push_back("Drafting_draftTextFont");
	this->function_list.push_back("Drafting_draftTextFontStyle");
	this->function_list.push_back("Drafting_draftTextWidth");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_draftTextCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_draftTextCharSize_EU");
	this->function_list.push_back("Drafting_draftTextSpaceFactor");
	this->function_list.push_back("Drafting_standardDraftTextSpaceFactor");
	this->function_list.push_back("Drafting_draftTextAspectRatio");
	this->function_list.push_back("Drafting_draftTextSymbolAspectRatio");
	this->function_list.push_back("Drafting_draftTextLineFactor");
	this->function_list.push_back("Drafting_draftingAidAngle");
	this->function_list.push_back("Drafting_frameSize");
	this->function_list.push_back("Drafting_symbolTextFont");

	/*
	********* DraftingStandard_Common_LineArrow_Arrowhead_prefs *********
	*/
	this->function_list.push_back("Drafting_automaticOrientation");
	this->function_list.push_back("Drafting_arrowhead1Type");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_arrowHead1Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_arrowHead1Color_EU");
	this->function_list.push_back("Drafting_arrowHead1Font");
	this->function_list.push_back("Drafting_arrowHead1Width");
	this->function_list.push_back("Drafting_textArrowPlacementSideOne");
	this->function_list.push_back("Drafting_arrowhead2Type");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_arrowHead2Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_arrowHead2Color_EU");
	this->function_list.push_back("Drafting_arrowHead2Font");
	this->function_list.push_back("Drafting_arrowHead2Width");
	this->function_list.push_back("Drafting_textArrowPlacementSideTwo");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_arrowSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_arrowSize_EU");
	this->function_list.push_back("Drafting_includedAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dotDiameter_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dotDiameter_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_filledDistance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_filledDistance_EU");
	//this->function_list.push_back("Drafting_dimensionArrowOutLengthFactor");
	this->function_list.push_back("Drafting_clipOrdinateDimLine");
	//this->function_list.push_back("Drafting_filledArrowheads");
	//this->function_list.push_back("Drafting_DatumArrowDisplayStyle");

	/*
	********* DraftingStandard_Common_LineArrow_ArrowLineLeader_prefs *********
	*/
	this->function_list.push_back("Drafting_showArrowLineSideOne");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_arrowLine1Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_arrowLine1Color_EU");
	this->function_list.push_back("Drafting_arrowLine1Font");
	this->function_list.push_back("Drafting_arrowLine1Width");
	this->function_list.push_back("Drafting_showArrowLineSideTwo");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_arrowLine2Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_arrowLine2Color_EU");
	this->function_list.push_back("Drafting_arrowLine2Font");
	this->function_list.push_back("Drafting_arrowLine2Width");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_textLine_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_textLine_EU");
	this->function_list.push_back("Drafting_leaderOrientation");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_stubbSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_stubbSize_EU");
	this->function_list.push_back("Drafting_textOverStubFactor");
	this->function_list.push_back("Drafting_stubSymbolType");
	this->function_list.push_back("Drafting_verticalTextJust");
	this->function_list.push_back("Drafting_textOverLeaderGapFactor");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_allAroundSymbolSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_allAroundSymbolSize_EU");
	//this->function_list.push_back("Drafting_textAboveLeader");

	/*
	********* DraftingStandard_Common_LineArrow_ExtensionLine_prefs *********
	*/
	this->function_list.push_back("Drafting_showExtensionLinesSideOne");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_extensionLine1Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_extensionLine1Color_EU");
	this->function_list.push_back("Drafting_extensionLine1Font");
	this->function_list.push_back("Drafting_extensionLine1Width");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_pointLine1_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_pointLine1_EU");
	this->function_list.push_back("Drafting_showExtensionLinesSideTwo");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_extensionLine2Color_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_extensionLine2Color_EU");
	this->function_list.push_back("Drafting_extensionLine2Font");
	this->function_list.push_back("Drafting_extensionLine2Width");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_pointLine2_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_pointLine2_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_lineArrow_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_lineArrow_EU");
	this->function_list.push_back("Drafting_extensionLineAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_datumExtLine_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_datumExtLine_EU");

	/*
	********* DraftingStandard_Common_LineArrow_Break_prefs *********
	*/
	this->function_list.push_back("Drafting_createBreaks");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_breakSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_breakSize_EU");

	/*
	********* DraftingStandard_Common_LineArrow_ForeshorteningSymbol_prefs *********
	*/
	this->function_list.push_back("Drafting_foreshorteningSymbolType");
	this->function_list.push_back("Drafting_foreshorteningSymbolMethod");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_foreshorteningSymbolWidth_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_foreshorteningSymbolWidth_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_foreshorteningSymbolHeight_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_foreshorteningSymbolHeight_EU");
	this->function_list.push_back("Drafting_foreshorteningSymbolAngle");

	/*
	********* DraftingStandard_Common_Stacking_prefs *********
	*/
	this->function_list.push_back("Drafting_StackLeftRightAlignment");
	this->function_list.push_back("Drafting_StackSpacingLeft");
	this->function_list.push_back("Drafting_StackSpacingRight");
	this->function_list.push_back("Drafting_StackAboveBelowAlignment");
	this->function_list.push_back("Drafting_StackSpacingAbove");
	this->function_list.push_back("Drafting_StackSpacingBelow");
	this->function_list.push_back("Drafting_StackAutospace");
	this->function_list.push_back("Drafting_StackInheritAssociatedObjects");

	/*
	********* DraftingStandard_Common_Origin_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_marginOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_marginOffset_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_marginSpacing_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_marginSpacing_EU");

	/*
	********* DraftingStandard_Common_PrefixSuffix_prefs *********
	*/
	this->function_list.push_back("Drafting_diaRadPlacementSite");
	this->function_list.push_back("Drafting_diaRadSymAndDimTextDistance");
	this->function_list.push_back("Drafting_TrueLengthTextPosition");
	this->function_list.push_back("Drafting_TrueLengthText");
	this->function_list.push_back("Drafting_chamferDimensionSymbol");
	this->function_list.push_back("Drafting_chamferDimensionSymbolName");

	/*
	********* DraftingStandard_Common_Symbol_prefs *********
	*/
	this->function_list.push_back("Drafting_userDefSymColor");
	this->function_list.push_back("Drafting_userDefSymFont");
	this->function_list.push_back("Drafting_userDefSymWidth");

	/*
	********* DraftingStandard_DrawingFormat_Sheet_NameAndNumber_prefs *********
	*/
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_drawingName_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_drawingName_EU");
	this->function_list.push_back("Drafting_InitialSheetNumber");
	this->function_list.push_back("Drafting_InitialSecondaryNumber");
	this->function_list.push_back("Drafting_SecondarySheetNumberDelimiter");
	//this->function_list.push_back("Drafting_ExcludedSheetLabels");

	/*
	********* DraftingStandard_DrawingFormat_Sheet_SizeAndScale_prefs *********
	*/
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetSizes_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetSizes_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_drawingHeight_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_drawingHeight_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_drawingWidth_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_drawingWidth_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_drawingScaleNum");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_drawingScaleDen");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_drawingUnits_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_drawingUnits_EU");
	//this->function_list.push_back("Drafting_projectionAngle");

	/*
	********* DraftingStandard_DrawingFormat_BordersAndZones_BorderDisplay_prefs *********
	*/
	this->function_list.push_back("Drafting_ZoneMethod");
	//this->function_list.push_back("Drafting_borderAndZoneStandard");
	this->function_list.push_back("Drafting_borderDisplayCreateBorders");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayWidth_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayWidth_EU");
	this->function_list.push_back("Drafting_borderDisplayInnerLineColor");
	this->function_list.push_back("Drafting_borderDisplayInnerLineFont");
	this->function_list.push_back("Drafting_borderDisplayInnerLineWidth");
	this->function_list.push_back("Drafting_borderDisplayOuterLineColor");
	this->function_list.push_back("Drafting_borderDisplayOuterLineFont");
	this->function_list.push_back("Drafting_borderDisplayOuterLineWidth");
	this->function_list.push_back("Drafting_borderDisplaySheetSizeInBorder");
	this->function_list.push_back("Drafting_boderDisplayCenteringMarksHorizontal");
	this->function_list.push_back("Drafting_boderDisplayCenteringMarksVertical");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayCenteringMarkLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayCenteringMarkLength_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayExtension_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayExtension_EU");
	this->function_list.push_back("Drafting_borderDisplayCenteringMarkColor");
	this->function_list.push_back("Drafting_borderDisplayCenteringMarkWidth");
	this->function_list.push_back("Drafting_boderDisplayArrowStyle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayArrowLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayArrowLength_EU");
	this->function_list.push_back("Drafting_borderDisplayArrowAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayArrowTailLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayArrowTailLength_EU");
	this->function_list.push_back("Drafting_boderDisplayArrowDirection");
	this->function_list.push_back("Drafting_borderDisplayCreateTrimmingMarks");
	this->function_list.push_back("Drafting_boderDisplayTrimmingMarksStyle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayTrimmingMarksLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayTrimmingMarksLength_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderDisplayTrimmingMarksWidth_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderDisplayTrimmingMarksWidth_EU");
	this->function_list.push_back("Drafting_borderDisplayTrimmingMarksColor");

	/*
	********* DraftingStandard_DrawingFormat_BordersAndZones_BorderMargins_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneTopMargin_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneTopMargin_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneBottomMargin_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneBottomMargin_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneLeftMargin_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneLeftMargin_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneRightMargin_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneRightMargin_EU");
	this->function_list.push_back("Drafting_borderMarginsColor");
	this->function_list.push_back("Drafting_borderMarginsFont");
	this->function_list.push_back("Drafting_borderMarginsWidth");
	this->function_list.push_back("Drafting_borderMarginCreateUntrimmedMargins");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderMarginsTopUntrimmed_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderMarginsTopUntrimmed_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderMarginsBottomUntrimmed_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderMarginsBottomUntrimmed_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderMarginsLeftUntrimmed_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderMarginsLeftUntrimmed_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_borderMarginsRightUntrimmed_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_borderMarginsRightUntrimmed_EU");

	/*
	********* DraftingStandard_DrawingFormat_BordersAndZones_Zones_prefs *********
	*/
	this->function_list.push_back("Drafting_zonesCreateZones");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneHorizontalSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneHorizontalSize_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_sheetZoneVerticalSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_sheetZoneVerticalSize_EU");
	this->function_list.push_back("Drafting_ZoneOriginMethod");
	this->function_list.push_back("Drafting_zonesCornerZoneModification");
	this->function_list.push_back("Drafting_zonesContinueZoneIndexingAcrossSheets");
	this->function_list.push_back("Drafting_zonesCreateZoneMarkings");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_zonesMarkingHeight_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_zonesMarkingHeight_EU");
	this->function_list.push_back("Drafting_zoneMarkingColor");
	this->function_list.push_back("Drafting_zoneMarkingWidth");
	this->function_list.push_back("Drafting_zonesCreateZoneLabels");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_zonesLabelHeight_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_zonesLabelHeight_EU");
	this->function_list.push_back("Drafting_zonesLabelColor");
	this->function_list.push_back("Drafting_zonesLabelFont");
	//this->function_list.push_back("Drafting_zonesLabelFontStyle");
	this->function_list.push_back("Drafting_zonesLabelWidth");
	this->function_list.push_back("Drafting_zonesLabelsToSkip");
	

	/*
	********* DraftingStandard_DrawingFormat_Template_prefs *********
	*/
	//this->function_list.push_back("Drafting_annotationTemplateName_MU");
	//this->function_list.push_back("Drafting_annotationTemplateName_EU");

	/*
	********* DraftingStandard_DrawingFormat_TitleBlock_prefs *********
	*/
	this->function_list.push_back("Drafting_TitleBlockAlignPosition");
	this->function_list.push_back("Drafting_TitleBlockAutoUpdate");
	//this->function_list.push_back("Drafting_AutoStartTitleBlockDialogOnTemplateInstantiation");

	/*
	********* DraftingStandard_View_Workflow_prefs *********
	*/
	this->function_list.push_back("Drafting_delayViewUpdate");
	this->function_list.push_back("Drafting_delayUpdateOnCreate");
	this->function_list.push_back("Drafting_borderDisplay");
	this->function_list.push_back("Drafting_borderColor");
	//this->function_list.push_back("Drafting_activeViewDisplay");
	this->function_list.push_back("Drafting_activeViewColor");
	this->function_list.push_back("Drafting_previewStyle");
	this->function_list.push_back("Drafting_cursorTracking");
	this->function_list.push_back("Drafting_associativeAlignment");
	//this->function_list.push_back("Drafting_noArrangementOption");
	this->function_list.push_back("Drafting_extractedEdgeFaceDisplay");
	this->function_list.push_back("Drafting_SelectGeometryProjectedAsArcs");
	this->function_list.push_back("Drafting_updateViewWithoutLwData");
	this->function_list.push_back("Drafting_loadComponentsOnDemand");
	this->function_list.push_back("Drafting_loadComponentOnSelection");
	this->function_list.push_back("Drafting_loadComponentOnUpdate");
	this->function_list.push_back("Drafting_shadingTranslucency");
	this->function_list.push_back("Drafting_shadingLineAntialiasing");
	this->function_list.push_back("Drafting_shadingShowFacetEdges");
	this->function_list.push_back("Drafting_MaxComponentsForLargeAssembly");

	/*
	********* DraftingStandard_View_Common_Configuration_prefs *********
	*/
	//this->function_list.push_back("Drafting_legacy");
	//this->function_list.push_back("Drafting_enableLightweightViews");
	this->function_list.push_back("Drafting_representation");
	this->function_list.push_back("Drafting_viewQuality");
	this->function_list.push_back("Drafting_extractedEdges");
	this->function_list.push_back("Drafting_renderTolerance");
	this->function_list.push_back("Drafting_wireframeColorSource");

	/*
	********* DraftingStandard_View_Common_General_prefs *********
	*/
	//this->function_list.push_back("Drafting_displayIdentifier");
	this->function_list.push_back("Drafting_sil");
	this->function_list.push_back("Drafting_uvhatch");
	//this->function_list.push_back("Drafting_templateInheritSketchFeatureParameters");
	//this->function_list.push_back("Drafting_templateInheritHoleFeatureParameters");
	this->function_list.push_back("Drafting_centerlineAutoCreate");
	this->function_list.push_back("Drafting_autoAnchor");
	this->function_list.push_back("Drafting_autoUpdate");
	this->function_list.push_back("Drafting_checkBoundaryStatus");


	/*
	********* DraftingStandard_View_Common_Angle_prefs *********
	*/
	this->function_list.push_back("Drafting_viewAngleFormat");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_viewAngleDecimalPlaces_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_viewAngleDecimalPlaces_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_viewAngleDecimalPoint_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_viewAngleDecimalPoint_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_viewAngleLeadingZeros_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_viewAngleLeadingZeros_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_viewAngleTrailingZeros_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_viewAngleTrailingZeros_EU");

	/*
	********* DraftingStandard_View_Common_VisibleLines_prefs *********
	*/
	this->function_list.push_back("Drafting_visColor");
	this->function_list.push_back("Drafting_visFont");
	this->function_list.push_back("Drafting_visWidth");

	/*
	********* _DraftingStandard_View_Common_HiddenLines_prefs *********
	*/
	this->function_list.push_back("Drafting_hlr");
	this->function_list.push_back("Drafting_hlColor");
	this->function_list.push_back("Drafting_hlFont");
	this->function_list.push_back("Drafting_hlWidth");
	this->function_list.push_back("Drafting_ehe");
	this->function_list.push_back("Drafting_referencedEdgesOnly");
	this->function_list.push_back("Drafting_selfHiddenEdges");
	this->function_list.push_back("Drafting_includeModelCurvesAndPoints");
	this->function_list.push_back("Drafting_processInterferingSolids");
	this->function_list.push_back("Drafting_displayInterferenceCurvesSolids");
	this->function_list.push_back("Drafting_simplifySmallFeatures");
	this->function_list.push_back("Drafting_smallFeatureTolerancePercent");

	/*
	********* DraftingStandard_View_Common_VirtualIntersections_prefs *********
	*/
	this->function_list.push_back("Drafting_isect");
	this->function_list.push_back("Drafting_isectColor");
	this->function_list.push_back("Drafting_isectFont");
	this->function_list.push_back("Drafting_isectWidth");
	this->function_list.push_back("Drafting_adjacentBlends");
	this->function_list.push_back("Drafting_isectGapping");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_isectGapSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_isectGapSize_EU");

	/*
	********* DraftingStandard_View_Common_Tracelines_prefs *********
	*/
	this->function_list.push_back("Drafting_traceLineVisibleColor");
	this->function_list.push_back("Drafting_traceLineVisibleFont");
	this->function_list.push_back("Drafting_traceLineVisibleWidth");
	this->function_list.push_back("Drafting_traceLineHiddenColor");
	this->function_list.push_back("Drafting_traceLineHiddenFont");
	this->function_list.push_back("Drafting_traceLineHiddenWidth");
	this->function_list.push_back("Drafting_traceLineGapping");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_traceLineGapSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_traceLineGapSize_EU");

	/*
	********* DraftingStandard_View_Common_SecondaryGeometry_prefs *********
	*/
	this->function_list.push_back("Drafting_ProcessSecondaryComponents");
	this->function_list.push_back("Drafting_ProcessSecondaryHiddenByPrimary");
	this->function_list.push_back("Drafting_ProcessPrimaryHiddenBySecondary");
	this->function_list.push_back("Drafting_SecondaryComponentsVisibleLineColor");
	this->function_list.push_back("Drafting_SecondaryComponentsVisibleLineFont");
	this->function_list.push_back("Drafting_SecondaryComponentsVisibleLineWidth");
	this->function_list.push_back("Drafting_SecondaryComponentsHiddenLineColor");
	this->function_list.push_back("Drafting_SecondaryComponentsHiddenLineFont");
	this->function_list.push_back("Drafting_SecondaryComponentsHiddenLineWidth");
	this->function_list.push_back("Drafting_ShowSmoothEdges");
	this->function_list.push_back("Drafting_ShowVirtualIntersections");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryVisibleLineColor");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryVisibleLineFont");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryVisibleLineWidth");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryHiddenLineColor");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryHiddenLineFont");
	this->function_list.push_back("Drafting_SecondaryHiddenByPrimaryHiddenLineWidth");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryVisibleLineColor");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryVisibleLineFont");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryVisibleLineWidth");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryHiddenLineColor");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryHiddenLineFont");
	this->function_list.push_back("Drafting_PrimaryHiddenBySecondaryHiddenLineWidth");
	/*
	********* DraftingStandard_View_Common_Threads_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_threadMethod_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_threadMethod_EU");
	this->function_list.push_back("Drafting_threadMinAppPitch");
	this->function_list.push_back("Drafting_overrideVisibleThreadColor");
	this->function_list.push_back("Drafting_threadTrueHiddenLine");

	/*
	********* DraftingStandard_View_Common_PMI_prefs *********
	*/
	this->function_list.push_back("Drafting_inheritPmiReferenceSetBehavior");
	this->function_list.push_back("Drafting_inheritPmi");
	//this->function_list.push_back("Drafting_inheritGdt");
	this->function_list.push_back("Drafting_inheritPmiOntoDrawing");
	this->function_list.push_back("Drafting_pmiDimensionFromRevolved");
	//this->function_list.push_back("Drafting_inheritPmiRearrange");
	this->function_list.push_back("Drafting_crosshatchInheritedPMILightweightSectionView");
	//this->function_list.push_back("Drafting_compoundBoxLightweightSectionView");

	/*
	********* DraftingStandard_View_Common_Shading_prefs *********
	*/
	this->function_list.push_back("Drafting_viewRenderingStyle");
	this->function_list.push_back("Drafting_overrideVisibleWireframeColor");
	this->function_list.push_back("Drafting_overrideHiddenWireframeColor");
	this->function_list.push_back("Drafting_shadedCutFaceColor");
	this->function_list.push_back("Drafting_viewShadedTwoSidedLight");
	this->function_list.push_back("Drafting_viewShadedShininess");
	this->function_list.push_back("Drafting_viewShadedTolerance");
	this->function_list.push_back("Drafting_viewShadedEdgeTolerance");
	this->function_list.push_back("Drafting_viewShadedFaceTolerance");
	this->function_list.push_back("Drafting_viewShadedAngleTolerance");
	//this->function_list.push_back("Drafting_shadedViewFacetTolerance");

	/*
	********* DraftingStandard_View_Common_SmoothEdges_prefs *********
	*/
	this->function_list.push_back("Drafting_smoothEdge");
	this->function_list.push_back("Drafting_smoothEdgeColor");
	this->function_list.push_back("Drafting_smoothEdgeFont");
	this->function_list.push_back("Drafting_smoothEdgeWidth");
	this->function_list.push_back("Drafting_smoothEdgeGapping");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_smoothEdgeGapSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_smoothEdgeGapSize_EU");
	this->function_list.push_back("Drafting_smoothEdgeTolerance");
	this->function_list.push_back("Drafting_smoothEdgeToleranceValue");

	/*
	********* DraftingStandard_View_Common_ViewLabel_prefs *********
	*/
	//this->function_list.push_back("Drafting_vwLabelInitialLetter");
	this->function_list.push_back("Drafting_vwLabelSecondaryIndexingMode");
	this->function_list.push_back("Drafting_vwLabelSecondaryIndexingAlignment");
	this->function_list.push_back("Drafting_vwLabelSecondaryIndexingSubscrpiptSizeFactor");
	//this->function_list.push_back("Drafting_vwLabelIncludedLetters");
	//this->function_list.push_back("Drafting_vwLabelMaxLetters");
	//this->function_list.push_back("Drafting_vwLabelMinLetters");
	//this->function_list.push_back("Drafting_vwLabelExcludedLetters");
	//this->function_list.push_back("Drafting_vwLabelExcludedLetterstoBegin");
	//this->function_list.push_back("Drafting_vwLabelExcludedLetterstoEnd");

	/*
	********* DraftingStandard_View_BaseDrawing_Settings_prefs *********
	*/

	/*
	********* DraftingStandard_View_BaseDrawing_Label_prefs *********
	*/
	this->function_list.push_back("Drafting_otherVwLabelPosition");
	this->function_list.push_back("Drafting_otherVwLabelIncludeTextLabel");
	this->function_list.push_back("Drafting_otherVwLabelTextLabelType");
	this->function_list.push_back("Drafting_otherVwLabelViewLabelPrefix");
	this->function_list.push_back("Drafting_otherVwLabelLetterFormat");
	this->function_list.push_back("Drafting_otherVwLabelShowReference");
	this->function_list.push_back("Drafting_otherVwLabelLetterSizeFactor");
	this->function_list.push_back("Drafting_otherVwLabelIncludeScaleLabel");
	this->function_list.push_back("Drafting_otherVwLabelViewScalePosition");
	this->function_list.push_back("Drafting_otherVwLabelScaleLabelPrefix");
	this->function_list.push_back("Drafting_otherVwLabelScalePrefixFactor");
	this->function_list.push_back("Drafting_otherVwLabelScaleValueFormat");
	this->function_list.push_back("Drafting_otherVwLabelScaleTextFactor");
	this->function_list.push_back("Drafting_otherVwLabelIncludeParentheses");

	/*
	********* DraftingStandard_View_Projected_Settings_prefs *********
	*/
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowOption");

	/*
	********* DraftingStandard_View_Projected_Label_prefs *********
	*/
	this->function_list.push_back("Drafting_projectedVwLabelPosition");
	this->function_list.push_back("Drafting_projectedVwLabelIncludeTextLabel");
	this->function_list.push_back("Drafting_projectedVwLabelTextLabelType");
	this->function_list.push_back("Drafting_projectedVwLabelViewLabelPrefix");
	this->function_list.push_back("Drafting_projectedVwLabelIncludeRotationSymbol");
	this->function_list.push_back("Drafting_projectedVwLabelIncludeRotationAngle");
	this->function_list.push_back("Drafting_projectedVwLabelLetterFormat");
	this->function_list.push_back("Drafting_projectedVwLabelLetterSizeFactor");
	this->function_list.push_back("Drafting_projectedVwLabelShowReference");
	this->function_list.push_back("Drafting_projectedVwLabelIncludeScaleLabel");
	this->function_list.push_back("Drafting_projectedVwLabelViewScalePosition");
	this->function_list.push_back("Drafting_projectedVwLabelScalePrefixFactor");
	this->function_list.push_back("Drafting_projectedVwLabelScaleLabelPrefix");
	this->function_list.push_back("Drafting_projectedVwLabelScaleValueFormat");
	this->function_list.push_back("Drafting_projectedVwLabelScaleTextFactor");
	this->function_list.push_back("Drafting_projectedVwLabelIncludeParentheses");

	/*
	********* DraftingStandard_View_Projected_Arrow_prefs *********
	*/
	this->function_list.push_back("PV_DisplayLabelonReferenceArrow");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowLabelSizeFactor");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowheadStyle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowheadLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowheadLength_EU");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowheadAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowLength_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowGeometryOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("PV_ViewingDirectionReferenceArrowGeometryOffset_EU");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowColor");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowFont");
	this->function_list.push_back("PV_ViewingDirectionReferenceArrowLineWidth");

	/*
	********* DraftingStandard_View_Section_Settings_prefs *********
	*/
	this->function_list.push_back("Drafting_bgDisplay");
	this->function_list.push_back("Drafting_fgDisplay");
	this->function_list.push_back("Drafting_sxviewSheetBody");
	this->function_list.push_back("Drafting_bendlinesDisplay");
	this->function_list.push_back("Drafting_sxviewCrosshatchDisplay");
	this->function_list.push_back("Drafting_hdCrosshatch");
	this->function_list.push_back("axs_assemblyxhatching");
	this->function_list.push_back("Axs_restrictCrosshatchAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_xhatchAdjacencyToler_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_xhatchAdjacencyToler_EU");

	/*
	********* DraftingStandard_View_Section_Label_prefs *********
	*/
	this->function_list.push_back("Drafting_sectionVwLabelPosition");
	this->function_list.push_back("Drafting_sectionVwLabelIncludeTextLabel");
	this->function_list.push_back("Drafting_sectionVwLabelTextLabelType");
	this->function_list.push_back("Drafting_sectionVwLabelViewLabelPrefix");
	this->function_list.push_back("Drafting_sectionVwLabelLetterFormat");
	this->function_list.push_back("Drafting_sectionVwLabelIncludeRotationSymbol");
	this->function_list.push_back("Drafting_sectionVwLabelIncludeRotationAngle");
	this->function_list.push_back("Drafting_sectionVwLabelLetterSizeFactor");
	this->function_list.push_back("Drafting_sectionVwLabelShowReference");
	this->function_list.push_back("Drafting_sectionVwLabelIncludeScaleLabel");
	this->function_list.push_back("Drafting_sectionVwLabelViewScalePosition");
	this->function_list.push_back("Drafting_sectionVwLabelScaleLabelPrefix");
	this->function_list.push_back("Drafting_sectionVwLabelScalePrefixFactor");
	this->function_list.push_back("Drafting_sectionVwLabelScaleValueFormat");
	this->function_list.push_back("Drafting_sectionVwLabelScaleTextFactor");
	this->function_list.push_back("Drafting_sectionVwLabelIncludeParentheses");

	/*
	********* DraftingStandard_View_Detail_Settings_prefs *********
	*/
	this->function_list.push_back("Drafting_detailViewBoundaryLineColor");
	this->function_list.push_back("Drafting_detailViewBoundaryLineFont");
	this->function_list.push_back("Drafting_detailViewBoundaryLineWidth");
	this->function_list.push_back("Drafting_detailViewClipBoundary");
	this->function_list.push_back("Drafting_detailViewCreateIndependent");

	/*
	********* DraftingStandard_View_Detail_Label_prefs *********
	*/
	this->function_list.push_back("Drafting_detailVwLabelPosition");
	this->function_list.push_back("Drafting_detailVwLabelIncludeTextLabel");
	this->function_list.push_back("Drafting_detailVwLabelTextLabelType");
	this->function_list.push_back("Drafting_detailVwLabelViewLabelPrefix");
	this->function_list.push_back("Drafting_detailVwLabelLetterFormat");
	this->function_list.push_back("Drafting_detailVwLabelShowReference");
	this->function_list.push_back("Drafting_detailVwLabelLetterSizeFactor");
	this->function_list.push_back("Drafting_detailVwLabelParentType");
	this->function_list.push_back("Drafting_detailVwLabelTextAboveLeader");
	this->function_list.push_back("Drafting_detailVwLabelGapFactor");
	this->function_list.push_back("Drafting_circularDetailVwLabelPrefix");
	this->function_list.push_back("Drafting_detailVwLabelIncludeScaleLabel");
	this->function_list.push_back("Drafting_detailVwLabelViewScalePosition");
	this->function_list.push_back("Drafting_detailVwLabelScaleLabelPrefix");
	this->function_list.push_back("Drafting_detailVwLabelScalePrefixFactor");
	this->function_list.push_back("Drafting_detailVwLabelScaleValueFormat");
	this->function_list.push_back("Drafting_detailVwLabelScaleTextFactor");
	this->function_list.push_back("Drafting_detailVwLabelIncludeParentheses");

	/*
	********* DraftingStandard_View_SectionLine_DisplayAndFormat_prefs *********
	*/
	this->function_list.push_back("Drafting_sectionLineWithView");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_sxlDisplay_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_sxlDisplay_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_sxlColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_sxlColor_EU");
	this->function_list.push_back("Axs_sxlFont");
	this->function_list.push_back("Axs_sxlWidth");
	this->function_list.push_back("Drafting_sectionLineBendAndEndSegmentWidthFactor");

	/*
	********* DraftingStandard_View_SectionLine_Arrowhead_prefs *********
	*/
	this->function_list.push_back("Axs_arrowheadType");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_arrowSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_arrowSize_EU");
	this->function_list.push_back("Axs_arrowIncludedAngle");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_filledArrowheadMode_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_filledArrowheadMode_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_arrowFillDistance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_arrowFillDistance_EU");

	/*
	********* DraftingStandard_View_SectionLine_Arrowline_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_arrowLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_arrowLength_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_distPastPart_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_distPastPart_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_stubbLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_stubbLength_EU");
	this->function_list.push_back("Axs_useLineLength");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_lineLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_lineLength_EU");

	/*
	********* DraftingStandard_View_SectionLine_Label_prefs *********
	*/
	this->function_list.push_back("Axs_labelLocation");
	this->function_list.push_back("Drafting_SectionLineDisplayLetterToggle");
	this->function_list.push_back("Axs_sxlDisplayLettersOnBend");
	this->function_list.push_back("Axs_sxlDisplayJISRotationLetter");
	//this->function_list.push_back("Drafting_sxLineRotationLetter");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_sectionLabelDist_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_sectionLabelDist_EU");
	//this->function_list.push_back("Axs_sectionLabelAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Axs_sxletterCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Axs_sxletterCharSize_EU");

	/*
	********* DraftingStandard_View_SectionLine_Offset_prefs *********
	*/
	this->function_list.push_back("Drafting_offsetSectionLineToggle");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_offsetSectionLineBackground_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_offsetSectionLineBackground_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_offsetSectionLineForeground_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_offsetSectionLineForeground_EU");

	/*
	********* DraftingStandard_View_SectionLine_HingeLine_prefs *********
	*/
	//this->function_list.push_back("Drafting_sxlAssocHinge");

	/*
	********* DraftingStandard_View_Break_prefs *********
	*/
	this->function_list.push_back("Drafting_breakLineVisibilty");
	this->function_list.push_back("Drafting_BreakLineStyle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_BreakLineAmplitude_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_BreakLineAmplitude_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_BreakLineExtension_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_BreakLineExtension_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_BreakGap_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_BreakGap_EU");
	this->function_list.push_back("Drafting_BreakColor");
	this->function_list.push_back("Drafting_breakLineWidth");
	this->function_list.push_back("Drafting_ProbagateViewBreak");
	this->function_list.push_back("Drafting_AddForeshorteningSymbol");

	/*
	********* DraftingStandard_Dimension_Workflow_DirectedDimensions_prefs *********
	*/
	this->function_list.push_back("Drafting_EnableDirectedDimensions");

	/*
	********* DraftingStandard_Dimension_Workflow_General_prefs *********
	*/
	this->function_list.push_back("Drafting_AutomaticCreationIntersectionSymbol");

	/*
	********* DraftingStandard_Dimension_Tolerance_prefs *********
	*/
	this->function_list.push_back("Drafting_toleranceType");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_toleranceDecimalPlaces_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_toleranceDecimalPlaces_EU");
	this->function_list.push_back("Drafting_toleranceUpperValue_SI");
	this->function_list.push_back("Drafting_toleranceLowerValue_SI");
	this->function_list.push_back("Drafting_toleranceUpperValue_IN");
	this->function_list.push_back("Drafting_toleranceLowerValue_IN");
	this->function_list.push_back("Drafting_limitsAndFitsTypeANSI");
	this->function_list.push_back("Drafting_toleranceDeviation");
	this->function_list.push_back("Drafting_toleranceGrade");
	this->function_list.push_back("Drafting_shaftToleranceDeviation");
	this->function_list.push_back("Drafting_shaftToleranceGrade");
	this->function_list.push_back("Drafting_limitsAndFitsDisplay");
	this->function_list.push_back("Drafting_AssemblylimitsAndFitsDisplay");
	this->function_list.push_back("Drafting_LimitsAndFitsSplitByDimline");
	this->function_list.push_back("Drafting_LimitsAndFitsAlignment");
	this->function_list.push_back("Drafting_LimitsandFitsSeparator");
	this->function_list.push_back("Drafting_LimitsandFitsToleranceParenthesis");
	this->function_list.push_back("Drafting_tolerancePlacementSite");
	this->function_list.push_back("Drafting_angularToleranceUnits");
	this->function_list.push_back("Drafting_zeroToleranceDisplayStyle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tolLeadingZeros_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tolLeadingZeros_EU");
	this->function_list.push_back("Drafting_dimToleranceConversion");

	/*
	********* DraftingStandard_Dimension_Dual_prefs *********
	*/
	this->function_list.push_back("Drafting_showDualDimension");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dualDimensionsUnits_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dualDimensionsUnits_EU");
	this->function_list.push_back("Drafting_dualDimensionsFractions");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dualDimensionDecimalPlaces_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dualDimensionDecimalPlaces_EU");
	this->function_list.push_back("Drafting_dualFractionDenominator");
	this->function_list.push_back("Drafting_dualDimensionsMethod");
	this->function_list.push_back("Drafting_dualCenterDimensionText");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dualToleranceDecimalPlaces_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dualToleranceDecimalPlaces_EU");

	/*
	********* DraftingStandard_Dimension_Jog_prefs *********
	*/
	this->function_list.push_back("Drafting_ordinateCreateDoglegMode");
	this->function_list.push_back("Drafting_ordinateDoglegCreationSettings");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_ordinateDoglegStartOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_ordinateDoglegStartOffset_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_ordinateDoglegEndOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_ordinateDoglegEndOffset_EU");
	this->function_list.push_back("Drafting_ordinateDoglegAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_linearSide1JogStartOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_linearSide1JogStartOffset_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_linearSide1JogEndOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_linearSide1JogEndOffset_EU");
	this->function_list.push_back("Drafting_linearSide1JogAngle");
	this->function_list.push_back("Drafting_linearSide1JogOrientation");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_linearSide2JogStartOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_linearSide2JogStartOffset_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_linearSide2JogEndOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_linearSide2JogEndOffset_EU");
	this->function_list.push_back("Drafting_linearSide2JogAngle");
	this->function_list.push_back("Drafting_linearSide2JogOrientation");

	/*
	********* DraftingStandard_Dimension_Narrow_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_narrowDimensionDisplayType_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_narrowDimensionDisplayType_EU");
	this->function_list.push_back("Drafting_narrowDimensionTextOrientation");
	this->function_list.push_back("Drafting_narrowDimensionArrowheadType");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_narrowDimensionTextOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_narrowDimensionTextOffset_EU");
	this->function_list.push_back("Drafting_narrowDimensionLeaderAngle");

	/*
	********* DraftingStandard_Dimension_SingleSidedDimension_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_singleSidedDimensionArrowLength_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_singleSidedDimensionArrowLength_EU");

	/*
	********* DraftingStandard_Dimension_DimensionSet_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_chainDimensionOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_chainDimensionOffset_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_baselineDimensionOffset_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_baselineDimensionOffset_EU");

	/*
	********* DraftingStandard_Dimension_Chamfer_prefs *********
	*/
	this->function_list.push_back("Drafting_chamferDimensionSpaceFactor");
	this->function_list.push_back("Drafting_chamferDimensionStub");
	this->function_list.push_back("Drafting_chamferDimensionLeader");
	this->function_list.push_back("Drafting_chamferDimensionForm");
	this->function_list.push_back("Drafting_chamferDimensionSeperator");

	/*
	********* DraftingStandard_Dimension_DimensionLine_prefs *********
	*/
	this->function_list.push_back("Drafting_dimLineBetweenExtLine");
	this->function_list.push_back("Drafting_trimDimensionLine");

	/*
	********* DraftingStandard_Dimension_Radial_prefs *********
	*/
	//this->function_list.push_back("Drafting_radiusDisplay");
	//this->function_list.push_back("Drafting_useDiameterForInferredCircles");

	/*
	********* DraftingStandard_Dimension_Ordinate_prefs *********
	*/
	this->function_list.push_back("Drafting_ordinatePositiveDirection");
	this->function_list.push_back("Drafting_ordinateShowDimensionLine");
	this->function_list.push_back("Drafting_ordinateDisplayZeroAtBaseline");
	this->function_list.push_back("Drafting_ordinateDisplayNameStyle");
	this->function_list.push_back("Drafting_ordinateUserText");
	this->function_list.push_back("Drafting_ordinateAlignSymbol");
	this->function_list.push_back("Drafting_ordinateSymbolAngle");
	this->function_list.push_back("Drafting_marginNumber");

	/*
	********* DraftingStandard_Dimension_Text_Units_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_linearUnits_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_linearUnits_EU");
	//this->function_list.push_back("Drafting_fractionType");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dimensionDecimalPlaces_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dimensionDecimalPlaces_EU");
	this->function_list.push_back("Drafting_fractionDenominator");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_decimalPoint_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_decimalPoint_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dimLeadingZeros_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dimLeadingZeros_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dimTrailingZeros_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dimTrailingZeros_EU");
	this->function_list.push_back("Drafting_angularUnits");
	this->function_list.push_back("Drafting_angularSuppressZeros");

	/*
	********* DraftingStandard_Dimension_Text_OrientationLocation_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_textOrientation_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_textOrientation_EU");
	this->function_list.push_back("Drafting_textAngle");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_ordinateTextOrientation_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_ordinateTextOrientation_EU");
	this->function_list.push_back("Drafting_ordinateTextAngle");
	this->function_list.push_back("Drafting_DimensionTextPosition");

	/*
	********* DraftingStandard_Dimension_Text_Format_prefs *********
	*/
	this->function_list.push_back("Drafting_inspectionDisplayType");
	this->function_list.push_back("Drafting_inspectionSymbolAroundAppendedText");
	this->function_list.push_back("Drafting_nottoScaleToleranceType");

	/*
	********* DraftingStandard_Dimension_Text_Appended_prefs *********
	*/
	this->function_list.push_back("Drafting_appTextColor");
	this->function_list.push_back("Drafting_appTextFont");
	//this->function_list.push_back("Drafting_appTextFontStyle");
	this->function_list.push_back("Drafting_appTextWidth");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_appTextCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_appTextCharSize_EU");
	this->function_list.push_back("Drafting_appTextSpaceFactor");
	this->function_list.push_back("Drafting_standardAppTextSpaceFactor");
	this->function_list.push_back("Drafting_appTextAspectRatio");
	this->function_list.push_back("Drafting_appSymbolAspectRatio");
	this->function_list.push_back("Drafting_appTextLineFactor");
	this->function_list.push_back("Drafting_dimAppTextSpaceFactor");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_appendedTextSite_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_appendedTextSite_EU");

	/*
	********* DraftingStandard_Dimension_Text_Dimension_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dimTextColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dimTextColor_EU");
	this->function_list.push_back("Drafting_dimTextFont");
	//this->function_list.push_back("Drafting_dimTextFontStyle");
	this->function_list.push_back("Drafting_dimTextWidth");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_dimTextCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_dimTextCharSize_EU");
	this->function_list.push_back("Drafting_dimTextSpaceFactor");
	this->function_list.push_back("Drafting_standardDimTextSpaceFactor");
	this->function_list.push_back("Drafting_dimTextAspectRatio");
	this->function_list.push_back("Drafting_dimSymbolAspectRatio");
	this->function_list.push_back("Drafting_dimTextLineFactor");
	this->function_list.push_back("Drafting_dimDimLineSpaceFactor");

	/*
	********* DraftingStandard_Dimension_Text_Tolerance_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tolTextColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tolTextColor_EU");
	this->function_list.push_back("Drafting_tolTextFont");
	//this->function_list.push_back("Drafting_tolTextFontStyle");
	this->function_list.push_back("Drafting_tolTextWidth");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tolTextCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tolTextCharSize_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_twoLineTolTextCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_twoLineTolTextCharSize_EU");
	this->function_list.push_back("Drafting_tolTextSpaceFactor");
	this->function_list.push_back("Drafting_standardTolTextSpaceFactor");
	this->function_list.push_back("Drafting_tolTextAspectRatio");
	this->function_list.push_back("Drafting_tolSymbolAspectRatio");
	this->function_list.push_back("Drafting_tolTextLineFactor");
	this->function_list.push_back("Drafting_dimTolTextSpaceFactor");

	/*
	********* DraftingStandard_Dimension_Reference_prefs *********
	*/
	this->function_list.push_back("Drafting_showReferenceDimension");
	//this->function_list.push_back("Drafting_referncetoleranceType");
	this->function_list.push_back("Drafting_PrefixSuffixReference");
	this->function_list.push_back("Drafting_ReferenceDimensionPrefix");
	this->function_list.push_back("Drafting_ReferenceDimensionSuffix");

	/*
	********* DraftingStandard_Dimension_HoleCallout_Lettering_prefs *********
	*/
	this->function_list.push_back("Drafting_CalloutGeneral_DisplayOption");
	this->function_list.push_back("Drafting_CalloutGeneral_TextSpaceFactor");
	this->function_list.push_back("Drafting_CalloutGeneral_LineSpaceFactor");

	/*
	********* DraftingStandard_Dimension_HoleCallout_GeneralHole_prefs *********
	*/
	this->function_list.push_back("Drafting_GeneralHole_PatternCount_Prefix");//0
	this->function_list.push_back("Drafting_GeneralHole_PatternCount_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_Diameter_Prefix");//1
	this->function_list.push_back("Drafting_GeneralHole_Diameter_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_Depth_Prefix");//2
	this->function_list.push_back("Drafting_GeneralHole_Depth_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_CounterboreDiameter_Prefix");//4
	this->function_list.push_back("Drafting_GeneralHole_CounterboreDiameter_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_CounterboreDepth_Prefix");//5
	this->function_list.push_back("Drafting_GeneralHole_CounterboreDepth_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_CountersinkDiameter_Prefix");//6
	this->function_list.push_back("Drafting_GeneralHole_CountersinkDiameter_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_CountersinkAngle_Prefix");//7
	this->function_list.push_back("Drafting_GeneralHole_CountersinkAngle_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_TaperAngle_Prefix");//8
	this->function_list.push_back("Drafting_GeneralHole_TaperAngle_Suffix");
	this->function_list.push_back("Drafting_GeneralHole_Parameter_Order");

	/*
	********* DraftingStandard_Dimension_HoleCallout_DrillSizeHole_prefs *********
	*/
	this->function_list.push_back("Drafting_DrillSizeHole_PatternCount_Prefix");//0
	this->function_list.push_back("Drafting_DrillSizeHole_PatternCount_Suffix");
	this->function_list.push_back("Drafting_DrillSizeHole_Diameter_Prefix");//1
	this->function_list.push_back("Drafting_DrillSizeHole_Diameter_Suffix");
	this->function_list.push_back("Drafting_DrillSizeHole_Depth_Prefix");//2
	this->function_list.push_back("Drafting_DrillSizeHole_Depth_Suffix");
	//this->function_list.push_back("Drafting_DrillSizeHole_Parameter_Order");

	/*
	********* DraftingStandard_Dimension_HoleCallout_ScrewClearanceHole_prefs *********
	*/
	this->function_list.push_back("Drafting_ScrewClearanceHole_PatternCount_Prefix");//0
	this->function_list.push_back("Drafting_ScrewClearanceHole_PatternCount_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_ScrewSize_Prefix");//1
	this->function_list.push_back("Drafting_ScrewClearanceHole_ScrewSize_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_Diameter_Prefix");//2
	this->function_list.push_back("Drafting_ScrewClearanceHole_Diameter_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_Fit_Prefix"); //3
	this->function_list.push_back("Drafting_ScrewClearanceHole_Fit_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_Depth_Prefix");//4
	this->function_list.push_back("Drafting_ScrewClearanceHole_Depth_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_CounterboreDiameter_Prefix");//6
	this->function_list.push_back("Drafting_ScrewClearanceHole_CounterboreDiameter_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_CounterboreDepth_Prefix");//7
	this->function_list.push_back("Drafting_ScrewClearanceHole_CounterboreDepth_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_CountersinkDiameter_Prefix");//8
	this->function_list.push_back("Drafting_ScrewClearanceHole_CountersinkDiameter_Suffix");
	this->function_list.push_back("Drafting_ScrewClearanceHole_CountersinkAngle_Prefix");//9
	this->function_list.push_back("Drafting_ScrewClearanceHole_CountersinkAngle_Suffix");
	//this->function_list.push_back("Drafting_ScrewClearanceHole_Parameter_Order");

	/*
	********* DraftingStandard_Dimension_HoleCallout_ThreadedHole_prefs *********
	*/
	this->function_list.push_back("Drafting_ThreadedHole_PatternCount_Prefix");//0
	this->function_list.push_back("Drafting_ThreadedHole_PatternCount_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_ThreadSize_Prefix");//1
	this->function_list.push_back("Drafting_ThreadedHole_ThreadSize_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_ThreadDepth_Prefix");//2
	this->function_list.push_back("Drafting_ThreadedHole_ThreadDepth_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_Depth_Prefix");//3
	this->function_list.push_back("Drafting_ThreadedHole_Depth_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_Pitch_Prefix");//5
	this->function_list.push_back("Drafting_ThreadedHole_Pitch_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_Angle_Prefix");//6
	this->function_list.push_back("Drafting_ThreadedHole_Angle_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_MinorDiameter_Prefix");//8
	this->function_list.push_back("Drafting_ThreadedHole_MinorDiameter_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_MajorDiameter_Prefix");//10
	this->function_list.push_back("Drafting_ThreadedHole_MajorDiameter_Suffix");
	this->function_list.push_back("Drafting_ThreadedHole_TapDrillDiameter_Prefix");//11
	this->function_list.push_back("Drafting_ThreadedHole_TapDrillDiameter_Suffix");
	//this->function_list.push_back("Drafting_ThreadedHole_Parameter_Order");

	/*
	********* DraftingStandard_Dimension_HoleCallout_SymbolicThread_prefs *********
	*/
	this->function_list.push_back("Drafting_SymbolicThread_PatternCount_Prefix");//0
	this->function_list.push_back("Drafting_SymbolicThread_PatternCount_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_Callout_Prefix");//1
	this->function_list.push_back("Drafting_SymbolicThread_Callout_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_Length_Prefix");//2
	this->function_list.push_back("Drafting_SymbolicThread_Length_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_Pitch_Prefix");//4
	this->function_list.push_back("Drafting_SymbolicThread_Pitch_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_Angle_Prefix");//5
	this->function_list.push_back("Drafting_SymbolicThread_Angle_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_MinorDiameter_Prefix");//7
	this->function_list.push_back("Drafting_SymbolicThread_MinorDiameter_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_MajorDiameter_Prefix");//9
	this->function_list.push_back("Drafting_SymbolicThread_MajorDiameter_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_TapDrillSize_Prefix");//10
	this->function_list.push_back("Drafting_SymbolicThread_TapDrillSize_Suffix");
	this->function_list.push_back("Drafting_SymbolicThread_ShaftSize_Prefix");//11
	this->function_list.push_back("Drafting_SymbolicThread_ShaftSize_Suffix");
	//this->function_list.push_back("Drafting_SymbolicThread_Parameter_Order");

	/*
	********* DraftingStandard_Annotation_GDT_prefs *********
	*/
	this->function_list.push_back("Drafting_gdtColor");
	this->function_list.push_back("Drafting_gdtFont");
	this->function_list.push_back("Drafting_gdtWidth");
	//this->function_list.push_back("Drafting_runoutArrowFilled");
	//this->function_list.push_back("Drafting_filledDatumTerminator");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_runoutArrowFilledDistance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_runoutArrowFilledDistance_EU");
	this->function_list.push_back("Drafting_underlineAdditionalText");

	/*
	********* DraftingStandard_Annotation_FeatureControlFrame_prefs *********
	*/
	//this->function_list.push_back("Drafting_DisplayIndicatorGroup");

	/*
	********* DraftingStandard_Annotation_Balloon_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_idSymbolColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_idSymbolColor_EU");
	this->function_list.push_back("Drafting_idSymbolFont");
	this->function_list.push_back("Drafting_idSymbolWidth");
	this->function_list.push_back("Drafting_attachToIdSymbolVertex");
	//this->function_list.push_back("Drafting_balloonType");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_idSymbolSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_idSymbolSize_EU");

	/*
	********* DraftingStandard_Annotation_SurfaceFinishSymbol_prefs *********
	*/
	this->function_list.push_back("Drafting_SFsymbolColor");
	this->function_list.push_back("Drafting_SFsymbolLineFont");
	this->function_list.push_back("Drafting_SFsymbolLineWidth");
	//this->function_list.push_back("Drafting_SFsymbol_Roughness");
	//this->function_list.push_back("Drafting_SFsymbol_Roughness2");
	//this->function_list.push_back("Drafting_SFsymbol_UpperText");
	//this->function_list.push_back("Drafting_SFsymbol_LowerText");
	//this->function_list.push_back("Drafting_SFsymbol_Waviness");
	//this->function_list.push_back("Drafting_SFsymbol_ProductionProcess");
	//this->function_list.push_back("Drafting_SFsymbol_Cutoff");
	//this->function_list.push_back("Drafting_SFsymbol_LaySymbol");
	//this->function_list.push_back("Drafting_SFsymbol_Machining");
	//this->function_list.push_back("Drafting_SFsymbol_SecondryRoughness");
	//this->function_list.push_back("Drafting_SFsymbol_TertiaryRoughness");
	//this->function_list.push_back("Drafting_SFraUnitsOption");
	//this->function_list.push_back("Drafting_SFsymbol_Material_Removal_Basic");
	//this->function_list.push_back("Drafting_SFsymbol_Modifier");
	//this->function_list.push_back("Drafting_SFsymbol_Material_Removal_Required");
	//this->function_list.push_back("Drafting_SFsymbol_Material_Removal_Prohibited");
	//this->function_list.push_back("Drafting_SFsymbol_All_Around");
	//this->function_list.push_back("Drafting_SFroughnessList_N");
	//this->function_list.push_back("Drafting_SFwavinessHeightList");
	//this->function_list.push_back("Drafting_SFroughnessList_Ra");
	//this->function_list.push_back("Drafting_SFsurfacePatternList");
	//this->function_list.push_back("Drafting_SFmachiningAllowanceList");
	//this->function_list.push_back("Drafting_SFCutoffList");
	//this->function_list.push_back("Drafting_SFproductionList");
	//this->function_list.push_back("Drafting_SFotherRoughnessValuesList");

	/*
	********* DraftingStandard_Annotation_WeldSymbol_prefs *********
	*/
	this->function_list.push_back("Drafting_weldSymbolColor");
	this->function_list.push_back("Drafting_weldSymbolFont");
	this->function_list.push_back("Drafting_weldSymbolWidth");
	this->function_list.push_back("Drafting_weldSymbolStandard_weldSpaceFactor");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_weldSymbolStandard_weldLineGap_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_weldSymbolStandard_weldLineGap_EU");
	this->function_list.push_back("Drafting_weldSymbolStandard_weldSymbolSizeFactor");
	//this->function_list.push_back("Weld_DraftArcWeldGridTopDisp");
	//this->function_list.push_back("Weld_DraftArcWeldGridEndCapDisp");
	//this->function_list.push_back("Weld_DraftFractionalDisplayValues");
	//this->function_list.push_back("Weld_DraftFractionType");
	//this->function_list.push_back("Weld_DraftFractionDenominator");
	//this->function_list.push_back("Weld_SymbolDecimalPlaces");
	//this->function_list.push_back("Weld_DraftDashedLinePlacement");
	//this->function_list.push_back("Weld_DraftDisplayDepthInSymbol");
	//this->function_list.push_back("Weld_DraftIsLengthInSymbol");
	//this->function_list.push_back("Weld_DraftIsSymbolTail");
	//this->function_list.push_back("Weld_DraftSymbolTailButtAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailFilletAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailSpotAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailPlugSlotAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailUserDefinedAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailBeadAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailSurfaceAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailCornerAttrTitle");
	//this->function_list.push_back("Weld_DraftSymbolTailLapAttrTitle");

	/*
	********* DraftingStandard_Annotation_TargetPointSymbol_prefs *********
	*/
	this->function_list.push_back("Drafting_targetPointColor");
	this->function_list.push_back("Drafting_targetPointFont");
	this->function_list.push_back("Drafting_targetPointWidth");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_targetPointSize_EU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_targetPointSize_MU");
	//this->function_list.push_back("Drafting_targetPointAngle");

	/*
	********* DraftingStandard_Annotation_IntersectionSymbol_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_intersectionColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_intersectionColor_EU");
	this->function_list.push_back("Drafting_intersectionFont");
	this->function_list.push_back("Drafting_intersectionWidth");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_intersectExtension_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_intersectExtension_EU");

	/*
	********* DraftingStandard_Annotation_CrosshatchAreaFill_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_crosshatchSpacing_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_crosshatchSpacing_EU");
	this->function_list.push_back("Drafting_crosshatchAngle");
	this->function_list.push_back("Drafting_areafillPattern");
	this->function_list.push_back("Drafting_areafillAngle");
	this->function_list.push_back("Drafting_areafillScale");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_solidfillDistance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_solidfillDistance_EU");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_crosshatchColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_crosshatchColor_EU");
	this->function_list.push_back("Drafting_crosshatchWidth");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_crosshatchTolerance_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_crosshatchTolerance_EU");
	this->function_list.push_back("Drafting_hatchApparentMode");
	this->function_list.push_back("Drafting_hatchMargin");
	this->function_list.push_back("Drafting_autoTextIsland");
	//this->function_list.push_back("Drafting_CrosshatchCurveSIRule");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_distanceTolerance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_distanceTolerance_EU");

	/*
	********* DraftingStandard_Annotation_Centerline_prefs *********
	*/
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_centerlineColor_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_centerlineColor_EU");
	this->function_list.push_back("Drafting_centerlineWidth");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_cylCenterlineExtension_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_cylCenterlineExtension_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_centerlineGap_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_centerlineGap_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_centerlineXsize_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_centerlineXsize_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_centerlineExtension_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_centerlineExtension_EU");
	//this->function_list.push_back("Drafting_centerlineAngle");
	//this->function_list.push_back("Drafting_centerlineInheritFromView");

	/*
	********* DraftingStandard_Annotation_OffsetCenterPoint_prefs *********
	*/
	//this->function_list.push_back("Drafting_offsetCenterPointDisplay");

	/*
	********* DraftingStandard_Symbol_Workflow_prefs *********
	*/
	this->function_list.push_back("Drafting_symbolAutoUpdate");
	this->function_list.push_back("Drafting_SymbolSmashToSketch");
	//this->function_list.push_back("Drafting_symbolInstanceDialogSymbolColor");
	//this->function_list.push_back("Drafting_SymbolAssociation");
	//this->function_list.push_back("Drafting_symbolInstanceDialogSymbolAnnotColor");
	//this->function_list.push_back("Drafting_AECustomSymbolPreferences");
	//this->function_list.push_back("Drafting_CustomSymbolAlignment");
	//this->function_list.push_back("Drafting_symbolAllowSLWViewCurves");
	//this->function_list.push_back("Drafting_symbolIconSize");

	/*
	********* DraftingStandard_Table_Common_Cell_prefs *********
	*/
	this->function_list.push_back("Drafting_tabnoteCellFormat");
	this->function_list.push_back("Drafting_tabnoteCellDateFormat");
	this->function_list.push_back("Drafting_tabnoteCellTimeFormat");
	this->function_list.push_back("Drafting_tabnoteCellTextAlignment");
	//this->function_list.push_back("Drafting_tabnoteCellAngularUnits");
	this->function_list.push_back("Drafting_tabnoteCellPrecision");
	this->function_list.push_back("Drafting_tabnoteCellTextSlant");
	this->function_list.push_back("Drafting_tabnoteCellZeroDisplay");
	//this->function_list.push_back("Drafting_tabnoteCellTextIsVertical");
	this->function_list.push_back("Drafting_tabnoteCellFitMethods");

	/*
	********* DraftingStandard_Table_Common_Section_prefs *********
	*/
	this->function_list.push_back("Drafting_tabnoteSectionHeaderLocation");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteSectionMaxHeight_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteSectionMaxHeight_EU");
	this->function_list.push_back("Drafting_tabnoteSectionAttachPoint");
	this->function_list.push_back("Drafting_tabnoteSectionUseDoubleWidthBorder");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteSectionBorderWidth_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteSectionBorderWidth_EU");
	this->function_list.push_back("Drafting_tabnoteSectionOverflowDirection");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteSectionOverflowSpacing_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteSectionOverflowSpacing_EU");
	this->function_list.push_back("Drafting_tabnoteSectionDisplayContinuationNote");
	this->function_list.push_back("Drafting_tabnoteSectionContinuationNote");
	//this->function_list.push_back("Drafting_tabnoteInitialNumberOfRows");
	//this->function_list.push_back("Drafting_tabnoteInitialNumberOfColumns");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteInitialColumnWidth_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteInitialColumnWidth_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteAutoRoundingDistance_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteAutoRoundingDistance_EU");
	//if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_tabnoteInitialRowHeight_MU");
	//if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_tabnoteInitialRowHeight_EU");

	/*
	********* DraftingStandard_Table_PartsList_prefs *********
	*/
	this->function_list.push_back("Drafting_plistGrowDirection");
	this->function_list.push_back("Drafting_plistAutoUpdate");
	this->function_list.push_back("Drafting_plistSortOnUpdate");
	this->function_list.push_back("Drafting_plistCreateNewRowsAsLocked");
	this->function_list.push_back("Drafting_plistAllowManualRows");
	this->function_list.push_back("Drafting_plistShowLockedDeletedRowsMethod");
	this->function_list.push_back("Drafting_plistCalloutType");
	//this->function_list.push_back("Drafting_plistMainSymbolText");
	//this->function_list.push_back("Drafting_plistReferenceSymbolText");
	this->function_list.push_back("Drafting_plistInitialCallout");
	this->function_list.push_back("Drafting_plistCalloutIncrement");
	this->function_list.push_back("Drafting_plistLettersToSkip");
	this->function_list.push_back("Drafting_pVerticalCalloutLeaderAttachment");
	//this->function_list.push_back("Drafting_plistCalloutLeaderPlacement");
	this->function_list.push_back("Drafting_plistHighlightManualText");
	this->function_list.push_back("Drafting_plistManualTextHighlightColor");
	//this->function_list.push_back("Drafting_plistNXDefaultPartsListName");
	//this->function_list.push_back("Drafting_plistTCINDefaultPartsListName");
	//this->function_list.push_back("Drafting_plistForceAutoTextToUpperCase");
	//this->function_list.push_back("Drafting_plistDontForcePartNameToUpperCase");
	//this->function_list.push_back("Drafting_plistMasterModel");
	//this->function_list.push_back("Drafting_plistTopLevelOnly");
	//this->function_list.push_back("Drafting_plistLeavesOnly");
	//this->function_list.push_back("Drafting_plistExpandSubordinateDEs");

	/*
	********* DraftingStandard_Table_TabularNote_prefs *********
	*/
	this->function_list.push_back("Drafting_tabnoteLockFormat");
	this->function_list.push_back("Drafting_tabnoteLockContents");
	this->function_list.push_back("Drafting_tabnoteAutoUpdate");
	this->function_list.push_back("Drafting_tabnoteShowLockedDeletedRowsMethod");

	/*
	********* DraftingStandard_Table_BendTable_prefs *********
	*/
	this->function_list.push_back("DR_BEND_TABLE_AUTOMATIC_UPDATE");
	this->function_list.push_back("DR_BEND_TABLE_SORT_ON_UPDATE");
	this->function_list.push_back("DR_BEND_TABLE_COLUMNS");
	this->function_list.push_back("DR_BEND_TABLE_SORT_COLUMN");

	/*
	********* DraftingStandard_Table_HoleTable_Workflow_prefs *********
	*/
	this->function_list.push_back("DR_HOLEREPORT_LOCK_FORMAT");
	this->function_list.push_back("DR_HOLEREPORT_LOCK_CONTENTS");
	this->function_list.push_back("DR_HOLEREPORT_AUTO_UPDATE");
	this->function_list.push_back("DR_HOLEREPORT_LOCKED_DELETED_ROWS_METHOD");

	/*
	********* DraftingStandard_Table_HoleTable_Format_prefs *********
	*/
	this->function_list.push_back("DR_HOLEREPORT_DISPLAY_PART_BODY_NAME");
	this->function_list.push_back("DR_HOLEREPORT_DISPLAY_COLUMNS");
	this->function_list.push_back("DR_HOLEREPORT_SIZE_FORMAT");
	this->function_list.push_back("DR_HOLEREPORT_MERGE_CELLS");
	this->function_list.push_back("DR_HOLEREPORT_ALL_CAPS");
	this->function_list.push_back("DR_HOLEREPORT_TAP_HOLE");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("DR_HOLEREPORT_PRECISION_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("DR_HOLEREPORT_PRECISION_EU");
	this->function_list.push_back("DR_HOLEREPORT_DELETED_HOLES_DISPLAY");
	this->function_list.push_back("DR_HOLEREPORT_DEPTH_DATUM");

	/*
	********* DraftingStandard_Table_HoleTable_Content_prefs *********
	*/
	this->function_list.push_back("DR_HOLEREPORT_DETAIL");

	/*
	********* DraftingStandard_Table_HoleTable_HoleFilters_prefs *********
	*/
	this->function_list.push_back("DR_HOLEREPORT_TYPE1");
	this->function_list.push_back("DR_HOLEREPORT_TYPE2");
	this->function_list.push_back("DR_HOLEREPORT_TYPE3");
	this->function_list.push_back("DR_HOLEREPORT_TYPE4");
	this->function_list.push_back("DR_HOLEREPORT_TYPE5");
	this->function_list.push_back("DR_HOLEREPORT_TYPE6");
	this->function_list.push_back("DR_HOLEREPORT_TYPE7");
	this->function_list.push_back("DR_HOLEREPORT_TYPE8");
	this->function_list.push_back("DR_HOLEREPORT_TYPE9");
	this->function_list.push_back("DR_HOLEREPORT_ONESIDE");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("DR_HOLEREPORT_COUNTERSINK_ANGLE_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("DR_HOLEREPORT_COUNTERSINK_ANGLE_EU");
	this->function_list.push_back("DR_HOLEREPORT_INCLUDE_PARTIAL_HOLES");
	this->function_list.push_back("DR_HOLEREPORT_PARTIAL_ANGLE");

	/*
	********* DraftingStandard_Table_HoleTable_HoleTypeSymbols_prefs *********
	*/
	//this->function_list.push_back("DR_HOLEREPORT_TYPE1_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE2_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE3_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE4_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE5_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE6_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE7_SYMBOL");
	//this->function_list.push_back("DR_HOLEREPORT_TYPE8_SYMBOL");

	/*
	********* DraftingStandard_Table_HoleTable_LabelFormat_prefs *********
	*/
	this->function_list.push_back("DR_HOLEREPORT_LABEL");
	this->function_list.push_back("DR_HOLEREPORT_LABEL_WITH_LEADER");
	this->function_list.push_back("DR_HOLEREPORT_RELATIVE_LOCATION");
	this->function_list.push_back("DR_HOLEREPORT_CHARACTERS_TO_SKIP");

	/*
	********* DraftingStandard_Table_HoleTable_Attributes_prefs *********
	*/
	//this->function_list.push_back("DR_HOLEREPORT_DIA_FIT");
	//this->function_list.push_back("DR_HOLEREPORT_DEP_FIT");
	//this->function_list.push_back("DR_HOLEREPORT_DRILLING");
	//this->function_list.push_back("DR_HOLEREPORT_WIREEDM");
	//this->function_list.push_back("DR_HOLEREPORT_IGNORED_HOLE");

	/*
	********* DraftingStandard_PMI_GDTGeneral_Standard_prefs *********
	*/
	//this->function_list.push_back("Gdt_defaultPartGage");
	//this->function_list.push_back("Gdt_partRestraintCondition");
	//this->function_list.push_back("Gdt_allowCircleU");
	//this->function_list.push_back("Gdt_invalidDatumChar");

	/*
	********* DraftingStandard_PMI_GDTGeneral_Region_prefs *********
	*/
	//this->function_list.push_back("Gdt_xhatchAngle");
	//this->function_list.push_back("Gdt_xhatchDistance_EU");
	//this->function_list.push_back("Gdt_xhatchDistance_MU");



	//Inhalt nx120_site.dpv
	/*
	********* site_Gateway_Visualization_Line_prefs *********
	*/
	this->function_list.push_back("UG_tolerance");
	this->function_list.push_back("UG_fontDisplayMode");
	this->function_list.push_back("UG_dashSize_MU");
	this->function_list.push_back("UG_dashSize_EU");
	this->function_list.push_back("UG_spaceSize_MU");
	this->function_list.push_back("UG_spaceSize_EU");
	this->function_list.push_back("UG_symbolSize_MU");
	this->function_list.push_back("UG_symbolSize_EU");
	this->function_list.push_back("UG_widthDisplay");
	this->function_list.push_back("UG_pixelWidth1");
	this->function_list.push_back("UG_pixelWidth2");
	this->function_list.push_back("UG_pixelWidth3");
	this->function_list.push_back("UG_pixelWidth4");
	this->function_list.push_back("UG_pixelWidth5");
	this->function_list.push_back("UG_pixelWidth6");
	this->function_list.push_back("UG_pixelWidth7");
	this->function_list.push_back("UG_pixelWidth8");
	this->function_list.push_back("UG_pixelWidth9");

	/*
	********* site_Gateway_Visualization_ColorSettings_prefs *********
	*/
	this->function_list.push_back("UG_drawingBackgroundColor");

	/*
	********* site_Gateway_LineWidths_Conversion_prefs *********
	*/
	this->function_list.push_back("UG_thinLineWidthToNewLineWidth");
	this->function_list.push_back("UG_normalLineWidthToNewLineWidth");
	this->function_list.push_back("UG_thickLineWidthToNewLineWidth");

	/*
	********* site_ShipDrafting_Annotation_ReferenceLines_prefs *********
	*/
	this->function_list.push_back("ShipDrafting_CreateReferenceLines");
	this->function_list.push_back("ShipDrafting_Reference_Lines_CenterLineColor");
	this->function_list.push_back("ShipDrafting_Reference_Lines_CenterLineFont");
	this->function_list.push_back("ShipDrafting_Reference_Lines_CenterLineWidth");
	this->function_list.push_back("ShipDrafting_Reference_Lines_BaseLineColor");
	this->function_list.push_back("ShipDrafting_Reference_Lines_BaseLineFont");
	this->function_list.push_back("ShipDrafting_Reference_Lines_BaseLineWidth");

	/*
	********* site_drafting_GeneralSetup_Standard_prefs *********
	*/
	this->function_list.push_back("Drafting_Standard");

	/*
	********* site_Drafting_GneralSetup_Workflow_prefs *********
	*/
	this->function_list.push_back("Drafting_AutoStartInsertSheetDialog");
	this->function_list.push_back("Drafting_StandaloneAutoStartDrawingViewCreation");
	this->function_list.push_back("Drafting_StandaloneAutoStartProjectedView");
	this->function_list.push_back("Drafting_AutoStartProjectToView");
	this->function_list.push_back("Drafting_showNewDrawingDialog");
	this->function_list.push_back("Drafting_BaseViewOrWizard");
	this->function_list.push_back("Drafting_AutoStartProjectedView");
	this->function_list.push_back("Drafting_CreateComponentOfMasterModelPart");
	this->function_list.push_back("Drafting_DrawingSettings");
	this->function_list.push_back("UG_gridInDrafting");
	this->function_list.push_back("Drafting_AllowPMIBidirectionalEdits");
	this->function_list.push_back("Drafting_PMICustomSymbolSettings");
	this->function_list.push_back("Drafting_EditSceneDialogDelay");
	
	/*
	********* site_Drafting_GneralSetup_RetainedAnnotations_prefs *********
	*/
	this->function_list.push_back("Drafting_retainAnnotationObjects");
	this->function_list.push_back("Drafting_retainColor");
	this->function_list.push_back("Drafting_retainFont");
	this->function_list.push_back("Drafting_retainWidth");

	/*
	********* site_Drafting_GneralSetup_WelcomePage_prefs *********
	*/
	this->function_list.push_back("Drafting_WelcomePageSetup");
	this->function_list.push_back("Drafting_WelcomeSessionCount");

	/*
	********* site_Drafting_GneralSetup_Miscellaneous_prefs *********
	*/
	this->function_list.push_back("Drafting_AllowExpressions");
	this->function_list.push_back("Drafting_saveCGMImageWithPart");
	this->function_list.push_back("Drafting_fontListOption");

	/*
	********* site_Drafting_GneralSetup_TextEditor_prefs *********
	*/
	this->function_list.push_back("Drafting_AEcharSizeMenu");
	this->function_list.push_back("Drafting_AEbold");
	this->function_list.push_back("Drafting_AEitalic");
	this->function_list.push_back("Drafting_AErows");
	this->function_list.push_back("Drafting_AEcolumns");
	this->function_list.push_back("Drafting_AEpreviewSize");
	this->function_list.push_back("Drafting_AEpreviewScale");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drafting_AEpreviewCharSize_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drafting_AEpreviewCharSize_EU");

	/*
	********* site_Drafting_FlatPatternView_Lines_prefs *********
	*/
	this->function_list.push_back("NXSM_bendUpCenterLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_bendUpCenterLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_bendUpCenterLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_bendUpCenterLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_bendDownCenterLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_bendDownCenterLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_bendDownCenterLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_bendDownCenterLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_bendTangentLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_bendTangentLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_bendTangentLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_bendTangentLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_bendInnerMoldLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_bendInnerMoldLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_bendInnerMoldLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_bendInnerMoldLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_bendOuterMoldLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_bendOuterMoldLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_bendOuterMoldLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_bendOuterMoldLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_exteriorCurves_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_exteriorCurves_FlatPatternView_Color");
	this->function_list.push_back("NXSM_exteriorCurves_FlatPatternView_Font");
	this->function_list.push_back("NXSM_exteriorCurves_FlatPatternView_Width");

	this->function_list.push_back("NXSM_interiorCutoutCurves_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_interiorCutoutCurves_FlatPatternView_Color");
	this->function_list.push_back("NXSM_interiorCutoutCurves_FlatPatternView_Font");
	this->function_list.push_back("NXSM_interiorCutoutCurves_FlatPatternView_Width");

	this->function_list.push_back("NXSM_interiorFeatureCurves_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_interiorFeatureCurves_FlatPatternView_Color");
	this->function_list.push_back("NXSM_interiorFeatureCurves_FlatPatternView_Font");
	this->function_list.push_back("NXSM_interiorFeatureCurves_FlatPatternView_Width");

	this->function_list.push_back("NXSM_holeCenter_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_holeCenter_FlatPatternView_Color");
	this->function_list.push_back("NXSM_holeCenter_FlatPatternView_Font");
	this->function_list.push_back("NXSM_holeCenter_FlatPatternView_Width");

	this->function_list.push_back("NXSM_joggleLine_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_joggleLine_FlatPatternView_Color");
	this->function_list.push_back("NXSM_joggleLine_FlatPatternView_Font");
	this->function_list.push_back("NXSM_joggleLine_FlatPatternView_Width");

	this->function_list.push_back("NXSM_addedTop_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_addedTop_FlatPatternView_Color");
	this->function_list.push_back("NXSM_addedTop_FlatPatternView_Font");
	this->function_list.push_back("NXSM_addedTop_FlatPatternView_Width");

	this->function_list.push_back("NXSM_addedBottom_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_addedBottom_FlatPatternView_Color");
	this->function_list.push_back("NXSM_addedBottom_FlatPatternView_Font");
	this->function_list.push_back("NXSM_addedBottom_FlatPatternView_Width");

	this->function_list.push_back("NXSM_toolMarker_FlatPatternView_Enabled");
	this->function_list.push_back("NXSM_toolMarker_FlatPatternView_Color");
	this->function_list.push_back("NXSM_toolMarker_FlatPatternView_Font");
	this->function_list.push_back("NXSM_toolMarker_FlatPatternView_Width");
	
	/*
	********* site_Drafting_FlatPatternView_Callouts_prefs *********
	*/
	this->function_list.push_back("NXCustomCallout_FlatPatternView_1_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_2_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_3_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_4_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_5_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_6_Available");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_7_Available");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_8_Available");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_1_Enabled");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_2_Enabled");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_4_Enabled");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_5_Enabled");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_6_Enabled");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_7_Enabled");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_8_Enabled");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_1_Name");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_1_AutoAttachTypes");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_1_Content");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_2_Content");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_3_Content");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_4_Content");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_5_Content");
	this->function_list.push_back("NXCustomCallout_FlatPatternView_6_Content");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_7_Content");
	//this->function_list.push_back("NXCustomCallout_FlatPatternView_8_Content");
	
	/*
	********* site_Drafting_DrawingCompare_ReportGeneral_prefs *********
	*/
	this->function_list.push_back("Drawing_Compare_Method");
	this->function_list.push_back("Drawing_Compare_Baseline");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drawing_Compare_Tolerance_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drawing_Compare_Tolerance_EU");
	this->function_list.push_back("Drawing_Compare_Restart_ReportId");
	this->function_list.push_back("Drawing_Compare_Increment_ReportId");
	this->function_list.push_back("Drawing_Comapre_Create_Overlay_Data");
	this->function_list.push_back("Drawing_SnapshotData_To_Use");
	this->function_list.push_back("Drawing_Compare_Disp_CS");
	this->function_list.push_back("Drawing_Compare_Preserve_CS_Disp");
	this->function_list.push_back("Drawing_Compare_CS_Type");
	if (this->usedUnitValue.compare("1") == 0) this->function_list.push_back("Drawing_Compare_CS_Size_MU");
	if (this->usedUnitValue.compare("2") == 0) this->function_list.push_back("Drawing_Compare_CS_Size_EU");
	this->function_list.push_back("Drawing_Compare_CS_Color");
	this->function_list.push_back("Drawing_Compare_CS_Font");
	this->function_list.push_back("Drawing_Compare_CS_Width");

	/*
	********* site_Drafting_DrawingCompare_ReportFilter_prefs *********
	*/
	this->function_list.push_back("Drawing_Compare_Sheet_Toggle");
	this->function_list.push_back("Drawing_Compare_View_Toggle");
	this->function_list.push_back("Drawing_Compare_Dim_Toggle");
	this->function_list.push_back("Drawing_Compare_Dim_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Dim_Size");
	this->function_list.push_back("Drawing_Compare_Dim_Origin");
	this->function_list.push_back("Drawing_Compare_Notes_Toggle");
	this->function_list.push_back("Drawing_Compare_Notes_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Notes_Text");
	this->function_list.push_back("Drawing_Compare_Notes_Origin");
	this->function_list.push_back("Drawing_Compare_Notes_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_FCF_Toggle");
	this->function_list.push_back("Drawing_Compare_FCF_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_FCF_Text");
	this->function_list.push_back("Drawing_Compare_FCF_Origin");
	this->function_list.push_back("Drawing_Compare_FCF_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_DFS_Toggle");
	this->function_list.push_back("Drawing_Compare_DFS_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_DFS_Text");
	this->function_list.push_back("Drawing_Compare_DFS_Origin");
	this->function_list.push_back("Drawing_Compare_DFS_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_Datum_Targets_Toggle");
	this->function_list.push_back("Drawing_Compare_Datum_Targets_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Datum_Targets_Text");
	this->function_list.push_back("Drawing_Compare_Datum_Targets_Origin");
	this->function_list.push_back("Drawing_Compare_Datum_Targets_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_Balloons_Toggle");
	this->function_list.push_back("Drawing_Compare_Balloons_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Balloons_Text");
	this->function_list.push_back("Drawing_Compare_Balloons_Origin");
	this->function_list.push_back("Drawing_Compare_Balloons_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_SFS_Toggle");
	this->function_list.push_back("Drawing_Compare_SFS_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_SFS_Text");
	this->function_list.push_back("Drawing_Compare_SFS_Origin");
	this->function_list.push_back("Drawing_Compare_SFS_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_Weld_Symbols_Toggle");
	this->function_list.push_back("Drawing_Compare_Weld_Symbols_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Weld_Symbols_Text");
	this->function_list.push_back("Drawing_Compare_Weld_Symbols_Origin");
	this->function_list.push_back("Drawing_Compare_Weld_Symbols_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_TPS_Toggle");
	this->function_list.push_back("Drawing_Compare_Intersection_Symbols_Toggle");
	this->function_list.push_back("Drawing_Compare_Symbols_Toggle");
	this->function_list.push_back("Drawing_Compare_Symbols_Retained_Toggle");
	this->function_list.push_back("Drawing_Compare_Symbols_Text");
	this->function_list.push_back("Drawing_Compare_Symbols_Origin");
	this->function_list.push_back("Drawing_Compare_Symbols_LeaderTerminator");
	this->function_list.push_back("Drawing_Compare_Tables_Toggle");
	this->function_list.push_back("Drawing_Compare_PartList_Toggle");
	this->function_list.push_back("Drawing_Compare_HoleTable_Toggle");
	this->function_list.push_back("Drawing_Compare_Centerlines_Toggle");
	this->function_list.push_back("Drawing_Compare_XHatch_Toggle");
	this->function_list.push_back("Drawing_Compare_Lines_Toggle");
	this->function_list.push_back("Drawing_Compare_Arcs_Toggle");
	this->function_list.push_back("Drawing_Compare_Circles_Toggle");
	this->function_list.push_back("Drawing_Compare_Conics_Toggle");
	this->function_list.push_back("Drawing_Compare_Splines_Toggle");

	/*
	********* site_Drafting_DrawingCompare_VisualOverlay_prefs *********
	*/
	this->function_list.push_back("Visual_Overlay_Compare_Method");
	this->function_list.push_back("Visual_Overlay_Color");
	this->function_list.push_back("Visual_Overlay_Data_To_Use");
	
	/*
	********* site_Drafting_DrawingAutomation_Booklet_prefs *********
	*/
	this->function_list.push_back("Drafting_wizardShowTemplateStep");
	this->function_list.push_back("Drafting_wizardShowReferencesStep");
	this->function_list.push_back("Drafting_wizardShowAttributesStep");
	this->function_list.push_back("Drafting_bookletItemType");
	this->function_list.push_back("Drafting_bookletDrawingItemType");
	this->function_list.push_back("Drafting_bookletRuleItemType");
	this->function_list.push_back("Drafting_SecondaryContentVisibleColor");
	this->function_list.push_back("Drafting_SecondaryContentVisibleFont");
	this->function_list.push_back("Drafting_SecondaryContentVisibleWidth");
	this->function_list.push_back("Drafting_SecondaryContentHiddenColor");
	this->function_list.push_back("Drafting_SecondaryContentHiddenFont");
	this->function_list.push_back("Drafting_SecondaryContentHiddenWidth");
	
	/*
	********* site_Drafting_DrawingAutomation_Rule_prefs *********
	*/
	this->function_list.push_back("Distribution_Rules_AnnotationInsideGeometry");
	this->function_list.push_back("Distribution_Rules_MinGeomDistance_Real_MM");
	this->function_list.push_back("Distribution_Rules_MinGeomDistance_Real_IN");
	this->function_list.push_back("Distribution_Rules_MaxGeomDistance_Real_MM");
	this->function_list.push_back("Distribution_Rules_MaxGeomDistance_Real_IN");
	this->function_list.push_back("Distribution_Rules_MinAnnDistance_Real_MM");
	this->function_list.push_back("Distribution_Rules_MinAnnDistance_Real_IN");
	//this->function_list.push_back("Distribution_Rules_Order");
	this->function_list.push_back("Dimension_Rules_HideFeetInch");
	this->function_list.push_back("Dimension_Rules_AllowDisplayInInchFeetGreaterThan");
	this->function_list.push_back("Dimension_Rules_DisplayInInchFeetGreaterThan");
	this->function_list.push_back("Dimension_Rules_AllowDisplayInInchFeetNearest");
	this->function_list.push_back("Dimension_Rules_DisplayInInchFeetNearest");
	this->function_list.push_back("Dimension_Rules_ComparsionTol_MM");
	this->function_list.push_back("Dimension_Rules_ComparisonTol_IN");
	this->function_list.push_back("Dimension_Rules_ReferenceGeomDistance_MM");
	this->function_list.push_back("Dimension_Rules_ReferenceGeomDistance_IN");
	
	/*
	********* site_Drafting_DrawingAutomation_Region_prefs *********
	*/
	this->function_list.push_back("Drafting_regionDisplayObject");
	this->function_list.push_back("Drafting_regionDisplayLabel");
	//this->function_list.push_back("Drafting_regionContentGap_MU");
	//this->function_list.push_back("Drafting_regionContentGap_EU");
	this->function_list.push_back("Drafting_regionColor");
	//this->function_list.push_back("Drafting_regionFont");
	//this->function_list.push_back("Drafting_regionWidth");
	
	/*
	********* site_Drafting_ConvertToPMI_General_prefs *********
	*/
	this->function_list.push_back("Drafting_ConvertToPMI_SettingsFile_WIN");
	this->function_list.push_back("Drafting_ConvertToPMI_SettingsFile_UNX");

	//this->haystack.assign(this->get_haystack_of(list_dir_file_ext_name));
	//this->haystack = this->get_haystack_of(function_list);
	////if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::create_function_list - haystack = \n%s\n", this->haystack.c_str());
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::create_function_list --- \n");
	return true;
}

/*
Prüfen, ob die dpv-Datensatz-Zeile zu den Zeichnungs-Settings gehört (DPV_ID_NAME_FILE)
*/
bool en_nx_dpv::check_data_row(string _id, list <string> _dpv_ids) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::check_data_row --- \n");
	
	if (_id.compare("") == 0) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::check_data_row - false --- \n");
		return false;
	}
	else if (_dpv_ids.size() > 0) {
		//this->nxLog("en_nx_dpv::check_data_row - _id = %s found!!! \n", _id.c_str());
		list <string>::iterator id_iterator;
		for (id_iterator = _dpv_ids.begin(); id_iterator != _dpv_ids.end(); id_iterator++) {
			string found_id;
			found_id = *id_iterator;
			if (found_id.compare(_id) == 0) {
				//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::check_data_row - true --- \n");
				_dpv_ids.begin();
				//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::check_data_row - _dpv_ids.begin() done \n");
				return true;
			}
		}
	}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::check_data_row - NOT NULL - false --- \n");
	return false;
}

/*
Leerzeichen vorne und hinten entfernen
*/
string en_nx_dpv::string_trim_white_spaces(string _inputString) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::string_trim_white_spaces --- \n");
	char a = char(32);
	char b = char(9);
	char c = char(13);
	char d = char(10);
	char chr_delim[4];
	chr_delim[0] = a;
	chr_delim[1] = b;
	chr_delim[2] = c;
	chr_delim[3] = d;
	const string& delim_white_space = string(chr_delim);
	_inputString.erase(0, _inputString.find_first_not_of(delim_white_space.c_str()));
	std::string::size_type last = _inputString.find_last_not_of(delim_white_space.c_str());
	_inputString.erase(last + 1);
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::string_trim_white_spaces --- \n");
	return _inputString;
}

str_c en_nx_dpv::str_c_allocate_memory(str_c _input, ...) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::str_c_allocate_memory --- \n");
	va_list ap;
	char *szText = NULL;
	int len;

	va_start(ap, _input);
	len = _vscprintf(_input, ap) + 1; //Länge des Strings ermitteln + '\0'
	szText = (char *)calloc(len, sizeof(char));
	if (len > 1) {
		vsprintf_s(szText, len, _input, ap);
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::str_c_allocate_memory - szText=\"%s\" \n", szText);
	}
	
	va_end(ap);

	if (printf("%s", szText)) {
		if (szText != NULL) {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::str_c_allocate_memory --- \n");
			return szText;
		}
		else {
			//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::str_c_allocate_memory - NULL --- \n");
			return NULL;
		}
	}
	else {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::str_c_allocate_memory - printf - NULL --- \n");
		return NULL;
	}

	return NULL;
}

str_c en_nx_dpv::str_c_free(str_c _input) {
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- START - en_nx_dpv::str_c_free --- \n");
	if (_input != NULL) {
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::str_c_free - _freea(...) before \n");
		//_freea(_input);
		//free(_input);
		//delete[] _input; (bisher)
		try {
			delete(_input);
		}
		catch (...) {
			this->nxLog("en_nx_dpv::str_c_free - delete ERROR \n");
			try {
				_freea(_input);
			}
			catch (...) {
				this->nxLog("en_nx_dpv::str_c_free - _freea ERROR \n");
			}

			this->nxLog("en_nx_dpv::str_c_free - _freea done \n");
		}

		//_input = NULL;
		//if (this->g_nxLog_en_nx_dpv) this->nxLog("en_nx_dpv::str_c_free - _freea(...) done \n");
		return NULL;
		//_input = NULL;

	}
	//if (this->g_nxLog_en_nx_dpv) this->nxLog("--- STOPP - en_nx_dpv::str_c_free --- \n");
	return NULL;
}