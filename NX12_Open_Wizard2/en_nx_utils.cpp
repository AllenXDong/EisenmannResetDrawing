/*==============================================================================
Programmierer : M. Künne
Datum         : 26.08.2008 - M. Künne
Last Update   :
Beschreibung  : Utility functions
==============================================================================*/
#include "en_nx_utils.h"

#include <cstdarg>
#include <iostream>

int DEBUG;

int reportError(char *file, int line, char *call, int irc) {
    if (irc) {
        char err[133],
             msg[133];

        sprintf_s(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
                  irc, line, file);
        UF_get_fail_message(irc, err);

        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);

        if (!UF_UI_open_listing_window()) {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }

    return(irc);
}

int list_Tags() {
    int i = 1;
    Part *workPart(Session::GetSession()->Parts()->Work());
    char tmp1[128],
         tmp2[128];


//    if (workPart->DrawingSheets()->GetCurrentDrawingSheet()->Tag() == null_tag) {
    if (workPart->DrawingSheets()->CurrentDrawingSheet()->Tag() == null_tag) {
        uc1601("Keine Zeichnung vorhanden!", 1);
        return(-1);
    }

    // Master auf Reference Set "Ganzes Teil" umschalten
    Assemblies::Component *root(workPart->ComponentAssembly()->RootComponent());
    std::vector<Assemblies::Component *> comps(root->GetChildren());
    std::vector<Assemblies::Component *>::iterator iter;

    strcpy_s(tmp2, root->GetStringAttribute("DB_PART_NO").GetText());
    for (iter = comps.begin(); iter != comps.end(); iter++) {
        //if( componentIsReference(*iter) )
        //  continue;
        strcpy_s(tmp1, (*iter)->GetStringAttribute("DB_PART_NO").GetText());
        UG_debug("%d. MASTER: %s MODEL(root): %s", i++, tmp1, tmp2);

        if (!strcmp(tmp1, tmp2)) {
            std::vector<Assemblies::Component *> childs((*iter)->GetChildren());
            UG_debug("Tags der Kinder des MASTER:");
            for (iter = childs.begin(); iter != childs.end(); iter++) {
                UG_debug("%u", (*iter)->Tag());
            }
            break;
        }
    }

    return(0);
}

bool componentIsReference(Assemblies::Component *comp) {
    std::vector<NXOpen::NXObject::AttributeInformation> attrs(comp->GetUserAttributes(true));
    //std::vector<NXOpen::NXObject::AttributeInformation> attrs(comp->GetAttributeTitlesByType(NXObject::AttributeTypeString));
    std::vector<NXOpen::NXObject::AttributeInformation>::iterator attrs_iter;

    for (attrs_iter = attrs.begin(); attrs_iter != attrs.end(); attrs_iter++) {
        if (attrs_iter->Type == NXObject::AttributeTypeString &&
            !strcmp(attrs_iter->Title.GetText(), "PLIST_IGNORE_MEMBER")) {
            return(true);
        }
    }

    return(false);
}

int get_Objects(tag_t part, int uf_object_type, int *num_objects, tag_t * *objects) {
    int error_code = 0;
    int count      = 0;
    tag_t object   = NULL_TAG;

    /* Object Tags */
    do {
        UF_OBJ_cycle_objs_in_part(part, uf_object_type, &object);

        if (object != NULL_TAG) {
            count++;
            (*num_objects)++;
            (*objects) = (tag_t *) UF_reallocate_memory(*objects, ((*num_objects) * sizeof (tag_t)), &error_code);

            if ((*objects == NULL) || (error_code != ERROR_OK)) {
                *num_objects = 0;
                UG_debug("Fehler in get_Objects. Error_code %d.", error_code);
                UG_debug("num_objects %d %d %d", count, *num_objects, *objects);
                return(1);
            }

            (*objects)[(*num_objects) - 1] = object;
        }
    } while (object != NULL_TAG);

    return(ERROR_OK);
}

int get_Object_type(tag_t obj, int * type)
{
    int subtype;

    return UF_OBJ_ask_type_and_subtype(obj, type, &subtype);
}

int get_Object_subtype(tag_t obj, int * subtype)
{
    int type;

    return UF_OBJ_ask_type_and_subtype(obj, &type, subtype);
}

void UG_debug(const char *fmt, ...) {
    va_list ap;
    char *szText = NULL;
    int len;

    va_start(ap, fmt);
    len    = _vscprintf(fmt, ap) + 2; //Länge des Strings ermitteln + '\n\0'
    szText = (char *) calloc(len, sizeof (char));
    vsprintf_s(szText, len, fmt, ap);
    va_end(ap);
    strcat_s(szText, len, "\n");
    UF_print_syslog(szText, FALSE);

    if (is_DEBUG()) {
        UG_write_listing_window(szText);
    }

    if (szText) {
        free(szText);
    }
}

void UG_alert(const char *fmt, ...) {
    va_list ap;
    char *szText = NULL;
    int len;

    va_start(ap, fmt);
    len    = _vscprintf(fmt, ap) + 2; //Länge des Strings ermitteln + '\n\0'
    szText = (char *) calloc(len, sizeof (char));
    vsprintf_s(szText, len, fmt, ap);
    va_end(ap);

    strcat_s(szText, len, "\n");

    UG_write_listing_window(szText);

    if (szText) {
        free(szText);
    }
}

void UG_write_listing_window(char const *text) {
    logical is_open;

    UF_UI_is_listing_window_open(&is_open);
    if (!is_open) {
        UF_UI_open_listing_window();
    }
    UF_UI_write_listing_window(text);
    UF_print_syslog((char *) text, FALSE);
}

void set_DEBUG(const int &val) {
    DEBUG = val;
}

int is_DEBUG() {
    return(DEBUG);
}