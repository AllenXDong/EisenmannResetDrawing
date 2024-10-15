/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Klasse für folgende dpv Files:
*                 - NX SITE DPV XML File (nx120_Site.dpv) ==> en_nx_site_dpv_elements.h
*                 Grund: Auswertung der dpv-Settings zum Benutzen im Programmcode für alte Zeichnungen
***************************************************************************************/

#include "en_nx_site_dpv_elements.h"

en_nx_site_dpv_elements::en_nx_site_dpv_elements(string _dpv_application, string _dpv_category, string _dpv_subcategory, string _dpv_tab, string _dpv_subtab, string _dpv_display_value, string _dpv_mod_date, string _dpv_id_name, string _dpv_title, string _dpv_id_value, string _dpv_comment, string _dpv_id_value_ist)
	: en_dpv_basic_elements(_dpv_application, _dpv_category, _dpv_subcategory, _dpv_tab, _dpv_subtab, _dpv_display_value, _dpv_mod_date, _dpv_id_name, _dpv_title, _dpv_id_value, _dpv_comment, _dpv_id_value_ist)
{
	this->g_nxLog_en_nx_site_dpv_elements = this->set_nxLog();
	//if (this->g_nxLog_en_nx_site_dpv_elements) this->nxLog("--- START - en_nx_site_dpv_elements::en_nx_site_dpv_elements --- \n");
}

/*
Liefert einen Datensatz der nx120_site.dpv-Liste
*/
string en_nx_site_dpv_elements::get_nx_site_dpv_dataset() {
	string datasetline = "";
	string len_or_thickn = "";
	datasetline.assign(
		this->get_dpv_application() + this->separator + this->get_dpv_category() + this->separator + this->get_dpv_subcategory() + this->separator + this->get_dpv_tab() + this->separator + this->get_dpv_subtab() + this->separator + this->get_dpv_display_value() + this->separator +
		this->get_dpv_mod_date() + this->separator + this->get_dpv_id_name() + this->separator + this->get_dpv_title() + this->separator + this->get_dpv_id_value() + this->separator + this->get_dpv_comment() + this->separator + this->get_dpv_id_value_ist() + this->separator
	);
	return datasetline;
}

/*
Ist der default-Wert (dpv_id_value) gleich dem Modell-Wert?
*/
bool en_nx_site_dpv_elements::is_EQU(string _dpv_id_value) {

	if (this->get_dpv_id_value().compare(_dpv_id_value.c_str()) == 0) {
		return true;
	}
	else {
		return false;
	}
}