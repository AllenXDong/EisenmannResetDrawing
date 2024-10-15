
/****************************************************************************************
*   Author:       Matthias Kreissig
*   Datum:        11.10.2019
*   Beschreibung: IST-SOLL-Abgleich von allen relevanten 2D-DPV-Settings
*                 Grund: Abgleich der 2D-DPV-Settings mit dem Code
***************************************************************************************/

#ifndef en_nx_getset_dpv_prefs_H
#define en_nx_getset_dpv_prefs_H

#pragma once
#include "en_nx_drawing_funcs.h"
#include "en_nx_dpv.h"
#include <xstring>
#include <list>
#include <map>

using namespace std;
using namespace NXOpen;

typedef char * str_c;

/*
Run "Get and set EN-Drafting-Standards"/"Lesen und setzen der EN-Zeichnungs-Standards" (EN_GETSET_DRAW_PREFS_action)
*/
extern "C" DllExport bool run_getset_dpv_values();

/*
get the work part
*/
Part* get_work_part();

/*
write messages to the NX-Log-File
*/
void nxLog(char *_szFormatString, ...);

/*
Asks if the input dpv id is used in the specified drawing routine
*/
//bool dpv_id_is_used_in_routine(string _id, string _routine_ids);
bool dpv_id_is_used_in_routine(string _id, list<string> _routine_ids);

/*
Set all Site Preferences
*/
void set_site_preferences(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
Set all Drawing Preferences
*/
void set_drawing_preferences(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

//Repair drawing elements
void repair_drawing_elements(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
get_env_dir (string _envname)
*/
string get_env_dir(const char * _envname);

/*
string convert_to_string(bool _bool_value)
*/
string convert_to_string(bool _bool_value);

/*
string convert_to_string(int _int_value)
*/
string convert_to_string(int _int_value);

/*
string convert_to_string(double _double_value)
*/
string convert_to_string(double _double_value);

/*
bool convert_to_bool(string _string_value)
*/
bool convert_to_bool(string _string_value);

/*
int convert_to_int(string _string_value)
*/
int convert_to_int(string _string_value);

/*
double convert_to_double(string _string_value)
*/
double convert_to_double(string _string_value);

str_c str_c_allocate_memory(str_c _input, ...);

void escapeXml(std::string & _in);

string retrieveXml(std::string _in);

string retrieveXmlList(std::string _in);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - Standard -> Repair these preferences.
*/
void repair_site_drafting_GeneralSetup_Standard_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - Workflow -> Repair these preferences.
*/
void repair_site_Drafting_GneralSetup_Workflow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - RetainedAnnotations -> Repair these preferences.
*/
void repair_site_Drafting_GneralSetup_RetainedAnnotations_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - WelcomePage -> Repair these preferences.
*/
void repair_site_Drafting_GneralSetup_WelcomePage_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - Miscellaneous -> Repair these preferences.
*/
void repair_site_Drafting_GneralSetup_Miscellaneous_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - General/Setup - TextEditor -> Repair these preferences.
*/
void repair_site_Drafting_GneralSetup_TextEditor_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Flat Pattern View - Lines -> Repair these preferences.
*/
void repair_site_Drafting_FlatPatternView_Lines_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Flat Pattern View - Callouts -> Repair these preferences.
*/
void repair_site_Drafting_FlatPatternView_Callouts_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Compare - Report General -> Repair these preferences.
*/
void repair_site_Drafting_DrawingCompare_ReportGeneral_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Compare - Report Filter -> Repair these preferences.
*/
void repair_site_Drafting_DrawingCompare_ReportFilter_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Compare - Visual Overlay -> Repair these preferences.
*/
void repair_site_Drafting_DrawingCompare_VisualOverlay_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Automation - Booklet -> Repair these preferences.
*/
void repair_site_Drafting_DrawingAutomation_Booklet_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Automation - Rule -> Repair these preferences.
*/
void repair_site_Drafting_DrawingAutomation_Rule_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Drawing Automation - Region -> Repair these preferences.
*/
void repair_site_Drafting_DrawingAutomation_Region_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Drafting - Convert to PMI - General -> Repair these preferences.
*/
void repair_site_Drafting_ConvertToPMI_General_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Gateway - Visualization - Line - Line Widths -> Repair these preferences.
*/
void repair_site_Gateway_Visualization_Line_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Gateway - Visualization - Color Settings -> Repair these preferences.
*/
void repair_site_Gateway_Visualization_ColorSettings_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Gateway - Line Widths - Conversion -> Repair these preferences.
*/
void repair_site_Gateway_LineWidths_Conversion_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Utilities - Customer Defaults - Ship Drafting - Annotation - Reference Lines -> Repair these preferences.
*/
void repair_site_ShipDrafting_Annotation_ReferenceLines_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);


/*
File - Preferences - Drafting - General - Standard - Standard -> Repair these preferences.
*/
void repair_DraftingStandard_General_Standard_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Lettering -> Repair these preferences.
*/
void repair_DraftingStandard_Common_Lettering_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Line/Arrow - Arrowhaead -> Repair these preferences.
*/
void repair_DraftingStandard_Common_LineArrow_Arrowhead_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Line/Arrow - Arrow Line/Leader -> Repair these preferences.
*/
void repair_DraftingStandard_Common_LineArrow_ArrowLineLeader_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Line/Arrow - Extension Line -> Repair these preferences.
*/
void repair_DraftingStandard_Common_LineArrow_ExtensionLine_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Line/Arrow - Break -> Repair these preferences.
*/
void repair_DraftingStandard_Common_LineArrow_Break_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Line/Arrow - Foreshortening Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Common_LineArrow_ForeshorteningSymbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Stacking -> Repair these preferences.
*/
void repair_DraftingStandard_Common_Stacking_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Origin -> Repair these preferences.
*/
void repair_DraftingStandard_Common_Origin_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Prefix/Suffix -> Repair these preferences.
*/
void repair_DraftingStandard_Common_PrefixSuffix_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Common - Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Common_Symbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Sheet - Name and Number -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_Sheet_NameAndNumber_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Sheet - Size and Scale -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_Sheet_SizeAndScale_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Border and Zones - Border Display -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_BordersAndZones_BorderDisplay_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Border and Zones - Border Margins -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_BordersAndZones_BorderMargins_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Border and Zones - Zones -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_BordersAndZones_Zones_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Template -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_Template_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Drawing Format - Title Block -> Repair these preferences.
*/
void repair_DraftingStandard_DrawingFormat_TitleBlock_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Workflow -> Repair these preferences.
*/
void repair_DraftingStandard_View_Workflow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Configuration -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_Configuration_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - General -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_General_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Angle -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_Angle_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Visible Lines -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_VisibleLines_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Hidden Lines -> Repair these preferences.
*/
void  repair_DraftingStandard_View_Common_HiddenLines_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Virtual Intersections -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_VirtualIntersections_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Tracelines -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_Tracelines_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Secondary Geometry -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_SecondaryGeometry_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Threads -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_Threads_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - PMI -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_PMI_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Shading -> Repair these preferences.
*/
void  repair_DraftingStandard_View_Common_Shading_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - Smooth Edges -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_SmoothEdges_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Common - View Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_Common_ViewLabel_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Base/Drawing - Settings -> Repair these preferences.
*/
void repair_DraftingStandard_View_BaseDrawing_Settings_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Base/Drawing - Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_BaseDrawing_Label_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Projected - Settings -> Repair these preferences.
*/
void repair_DraftingStandard_View_Projected_Settings_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Projected - Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_Projected_Label_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Projected - Arrow -> Repair these preferences.
*/
void repair_DraftingStandard_View_Projected_Arrow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section - Settings -> Repair these preferences.
*/
void repair_DraftingStandard_View_Section_Settings_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section - Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_Section_Label_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Detail - Settings -> Repair these preferences.
*/
void repair_DraftingStandard_View_Detail_Settings_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Detail - Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_Detail_Label_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Display and Format -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_DisplayAndFormat_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Arrowhead -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_Arrowhead_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Arrowline -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_Arrowline_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Label -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_Label_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Offset -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_Offset_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line - Hinge Line -> Repair these preferences.
*/
void repair_DraftingStandard_View_SectionLine_HingeLine_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Break - Break -> Repair these preferences.
*/
void repair_DraftingStandard_View_Break_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Workflow - Directed Dimensions -> Repair these preferences.
*/
void  repair_DraftingStandard_Dimension_Workflow_DirectedDimensions_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Workflow - General -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Workflow_General_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Tolerance - Tolerance -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Tolerance_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Dual -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Dual_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Jog -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Jog_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Narrow -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Narrow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Single Sided Dimension-> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_SingleSidedDimension_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Dimension Set -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_DimensionSet_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Chamfer -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Chamfer_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Dimension Line - Format -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_DimensionLine_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Radial -> Repair these preferences.
*/
void  repair_DraftingStandard_Dimension_Radial_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Ordinate -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Ordinate_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Units -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_Units_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Orientation and Location -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_OrientationLocation_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Format -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_Format_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Appended -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_Appended_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Dimension -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_Dimension_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text - Tolerance -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Text_Tolerance_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Reference -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_Reference_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - Lettering -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_Lettering_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - General Hole -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_GeneralHole_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - Drill Size Hole -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_DrillSizeHole_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - Screw Clearance Hole -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_ScrewClearanceHole_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - Threaded Hole -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_ThreadedHole_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Hole Callout - Symbolic Thread -> Repair these preferences.
*/
void repair_DraftingStandard_Dimension_HoleCallout_SymbolicThread_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - GDT -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_GDT_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Feature Control Frame -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_FeatureControlFrame_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Balloon -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_Balloon_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Surface Finish Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_SurfaceFinishSymbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Weld Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_WeldSymbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Target Point Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_TargetPointSymbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Intersection Symbol -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_IntersectionSymbol_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Crosshatch/Area Fill -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_CrosshatchAreaFill_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Centerline -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_Centerline_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Offset Center Point -> Repair these preferences.
*/
void repair_DraftingStandard_Annotation_OffsetCenterPoint_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Symbol - Workflow -> Repair these preferences.
*/
void repair_DraftingStandard_Symbol_Workflow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Common - Cell -> Repair these preferences.
*/
void repair_DraftingStandard_Table_Common_Cell_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Common - Section -> Repair these preferences.
*/
void repair_DraftingStandard_Table_Common_Section_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Parts List -> Repair these preferences.
*/
void repair_DraftingStandard_Table_PartsList_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Tabular Note -> Repair these preferences.
*/
void repair_DraftingStandard_Table_TabularNote_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Bend Table -> Repair these preferences.
*/
void repair_DraftingStandard_Table_BendTable_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Workflow -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_Workflow_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Format -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_Format_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Content -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_Content_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Hole Filters -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_HoleFilters_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Hole Type Symbols -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_HoleTypeSymbols_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Label Format -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_LabelFormat_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Table - Hole Table - Attributes -> Repair these preferences.
*/
void repair_DraftingStandard_Table_HoleTable_Attributes_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - PMI - GD&T General - Standard -> Repair these preferences.
*/
void repair_DraftingStandard_PMI_GDTGeneral_Standard_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - PMI - GD&T General - Region -> Repair these preferences.
*/
void repair_DraftingStandard_PMI_GDTGeneral_Region_prefs(NXOpen::Drafting::PreferencesBuilder *_preferencesBuilder, en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
VersionUpAllDraftingObjects
*/
void versionUpAllDraftingObjects(Part * _workPart);


/*
Display view style parameters
*/
void print_display_view_style_params(UF_DRAW_view_prfs_t &_vdp1, UF_DRAW_view_prfs_t &_vdp2);

/*
File - Preferences - Drafting - Dimension -> Repair existing dimensions.
*/
void repair_existing_dimension_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Symbol -> Repair existing symbols.
*/
void repair_existing_symbol_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Dimension - Text -> Repair existing texts.
*/
void repair_existing_text_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Centerline -> Repair existing centerlines.
File - Preferences - Drafting - Annotation - IntersectionSymbol -> Repair existing intersection symbols.
File - Preferences - Drafting - Annotation - TargetPointSymbol -> Repair existing target point symbols.
*/
void repair_existing_line_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Weld Symbol -> Repair existing target point symbols.
*/
void repair_existing_weld_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - GDT -> Repair existing GDTs.
*/
void repair_existing_gdt_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Surface Finish Symbol -> Repair existing target point symbols.
*/
void repair_existing_SurfaceFinishSymbol_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View - Section Line -> Repair existing section lines.
*/
void repair_existing_sectionLine_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - Annotation - Crosshatch/Area Fill -> Repair existing section lines.
*/
void repair_existing_hatch_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

/*
File - Preferences - Drafting - View -> Repair existing views.
*/
void repair_existing_view_styles(en_nx_dpv* _nx_dpv, Session * _theSession, Part* _workPart);

#endif
