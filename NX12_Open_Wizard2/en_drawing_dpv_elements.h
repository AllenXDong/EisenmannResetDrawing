/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        08.05.2019
*   Beschreibung: Neue Klasse für folgende dpv Files:
*                 - Drafting DPV XML File (nx_EN_Drafting_Standard_Site.dpv) ==> en_drawing_dpv_elements.h
*                 Grund: Auswertung der dpv-Settings zum Benutzen im Programmcode für alte Zeichnungen
***************************************************************************************/

#pragma once

#ifndef en_drawing_dpv_elements_H
#define en_drawing_dpv_elements_H

#define DRAFTING_DPV_APPLICATION 1
#define DRAFTING_DPV_CATEGORY 2
#define DRAFTING_DPV_SUBCATEGORY 3
#define DRAFTING_DPV_TAB 4
#define DRAFTING_DPV_DISPLAY_VALUE 5
#define DRAFTING_DPV_MOD_DATE 6
#define DRAFTING_DPV_ID_NAME 7
#define DRAFTING_DPV_TITLE 8
#define DRAFTING_DPV_ID_VALUE 9
#define DRAFTING_DPV_COMMENT 10

#include "en_dpv_basic_elements.h"

class en_drawing_dpv_elements : public en_dpv_basic_elements {

public:
	en_drawing_dpv_elements::en_drawing_dpv_elements(string _dpv_application, string _dpv_category, string _dpv_subcategory, string _dpv_tab, string _dpv_subtab, string _dpv_display_value, string _dpv_mod_date, string _dpv_id_name, string _dpv_title, string _dpv_id_value, string _dpv_comment, string _dpv_id_value_ist);
	
	/*
	Liefert einen Datensatz der nx_EN_Drafting_Standard_Site.dpv-Liste
	*/
	string get_drawing_dpv_dataset();

	/*
	Ist der dpv_id_value dieses Datensatzes gleich dem Übergabewert?
	*/
	bool is_EQU(string _dpv_id_vlaue);
private:
	bool g_nxLog_en_drawing_dpv_elements;
};

#endif