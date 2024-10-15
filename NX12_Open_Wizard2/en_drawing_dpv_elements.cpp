/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Klasse für folgende dpv Files:
*                 - Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv) ==> en_drawing_dpv_elements.h
*                 Grund: Auswertung der dpv-Settings zum Benutzen im Programmcode für alte Zeichnungen
***************************************************************************************/

#include "en_drawing_dpv_elements.h"

en_drawing_dpv_elements::en_drawing_dpv_elements(string _dpv_application, string _dpv_category, string _dpv_subcategory, string _dpv_tab, string _dpv_subtab, string _dpv_display_value, string _dpv_mod_date, string _dpv_id_name, string _dpv_title, string _dpv_id_value, string _dpv_comment, string _dpv_id_value_ist)
	: en_dpv_basic_elements(_dpv_application, _dpv_category, _dpv_subcategory, _dpv_tab, _dpv_subtab, _dpv_display_value, _dpv_mod_date, _dpv_id_name, _dpv_title, _dpv_id_value, _dpv_comment, _dpv_id_value_ist)
{
	this->g_nxLog_en_drawing_dpv_elements = this->set_nxLog();
	//if (this->g_nxLog_en_drawing_dpv_elements) this->nxLog("--- START - en_drawing_dpv_elements::en_drawing_dpv_elements --- \n");
}

/*
Liefert einen Datensatz der nx_EN_Drafting_Standard_Site.dpv-Liste
*/
string en_drawing_dpv_elements::get_drawing_dpv_dataset() {
	string datasetline = "";
	datasetline.clear();

	datasetline.assign( 
		this->separator + this->get_dpv_application() + this->separator + this->get_dpv_category() + this->separator + this->get_dpv_subcategory() + this->separator + this->get_dpv_tab() + this->separator + this->get_dpv_display_value() + this->separator +
		this->get_dpv_mod_date() + this->separator + this->get_dpv_id_name() + this->separator + this->get_dpv_title() + this->separator + this->get_dpv_id_value() + this->separator + this->get_dpv_comment() + this->separator + this->get_dpv_id_value_ist() + this->separator
	);
	return datasetline;
}

/*
Ist der dpv_id_value dieses Datensatzes gleich dem Übergabewert?
*/
bool en_drawing_dpv_elements::is_EQU(string _dpv_id_value) {

	if (this->get_dpv_id_value().compare(_dpv_id_value.c_str()) == 0) {
		return true;
	}
	else {
		return false;
	}
}