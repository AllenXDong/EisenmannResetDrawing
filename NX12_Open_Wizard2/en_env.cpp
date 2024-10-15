/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Basis-Klasse zum Arbeiten mit XML-Dateien (dpv-Dateien in NX)
*                 Grund: Abgleich der DPV-Settings mit dem Code
***************************************************************************************/

#include "en_env.h"
#include <cstringt.h>

ENenv::ENenv(const char *varName) {
	this->_name = varName;
	//if (this->g_nxLog_ENenv) this->nxLog("--- STOPP - ENenv::ENenv --- \n");
}

ENenv::~ENenv() {
	//if (this->g_nxLog_ENenv) this->nxLog("--- START - ENenv::~ENenv --- \n");
	//this->drafting_dpv_list.clear();
	//this->nx_site_dpv_list.clear();
	//this->instance = 0;
	//if (this->g_nxLog_ENenv) this->nxLog("--- STOPP - ENenv::~ENenv --- \n");
}

/*
this->printLog nur dann ausführen, wenn Umgebungsvariable
EN_UG_LOGLEVEL = 0 oder EN_UG_LOGLEVEL = TRUE oder EN_UG_LOGLEVEL = true oder EN_UG_LOGLEVEL = T oder EN_UG_LOGLEVEL = t
Existiert die Umgebungsvariable EN_UG_LOGLEVEL nicht, so this->printLog nicht ausführen. Nicht Case-Sensitiv!
Achtung: Erst nach einem TC-Neustart steht diese Variable zur Verfügnung!!!
Variable this->g_nxLog_ENenv = true / false steuert hier die this->printLog-Ausgabe
*/
bool ENenv::exists() {
	if (getenv(_name.c_str()))
		return true;
	else return false;
}

const std::string &ENenv::valToString()
{
	_value = getenv(_name.c_str());
	return _value;
}