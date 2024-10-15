//#include <io.h>
#include <time.h>
#include <tc/tc_macros.h>
#include <tccore/aom_prop.h>
#include <sa/tcfile.h>
#include <ae/ae.h>
#include <tccore/aom.h>
#include <stdio.h>
#include <sa/user.h>
#include <epm/epm.h>
#include <epm/signoff.h>
#include <tccore/grm.h>
#include <user_exits/user_exits.h>
#include <tccore/custom.h>
#include <tc/folder.h>
#include <user_exits/user_exits.h>
#include <itk/bmf.h>
#include <tccore/custom.h>
#include <tccore/project.h>
#include <ai/appinterface.h>
#include <user_exits/user_exit_msg.h>
#include <user_exits/user_exits.h>
#include <tccore/tctype.h>
#include <tc/aliaslist.h>
#include <epm/cr_action_handlers.h>
//#include <epm/cr_errors.h>
#include <tc/distributionlist.h>
#include <tc/emh.h>
#include <tc/preferences.h>
#include <tccore/aom.h>
#include <tccore/item.h>
#include <epm/epm.h>
#include <epm/epm_errors.h>
#include <epm/epm_toolkit_tc_utils.h>
#include <sa/sa_errors.h>
#include <property/prop.h>
#include <tc/folder.h>
#include <tccore/aom_prop.h>
#include <tccore/grmtype.h>
#include <ae/ae_types.h>
#include <ae/dataset.h>
#include <tccore/grm.h>
#include <tccore/workspaceobject.h>

#include <stdio.h> 
#include<malloc.h>
#include <string.h> 
#include <server_exits/user_server_exits.h>
#include <user_exits/user_exits.h>
#include <lov/lov_msg.h>
#include <tccore/tctype.h>
#include <sa/group.h>
#include <tc/folder.h>
#include <fclasses/tc_date.h>
#include <ict/ict_userservice.h>
//#include <tc/iman.h>
#include <epm/epm_toolkit_tc_utils.h>
#include <pom/pom/pom.h>
#include <sa/tcfile.h>



#ifdef _cplusplus
extern "C"{
#endif
	extern DLLAPI int NANCAL_libsign_register_action_handler(int *decision, va_list args);
	extern DLLAPI int cust_sign(EPM_action_message_t msg);
	extern DLLAPI int node_sign(EPM_action_message_t msg);
	extern DLLAPI int node_sign_new(EPM_action_message_t msg);
	extern DLLAPI int archive_sign(EPM_action_message_t msg);
	extern DLLAPI int remove_final(EPM_action_message_t msg);
	extern DLLAPI int HT6_create_integration_task(EPM_action_message_t msg);
	extern DLLAPI EPM_decision_t nancal_check_property( EPM_rule_message_t msg );
	extern DLLAPI EPM_decision_t check_allChild(tag_t parent_item, char* object_type, char* have_value_property, char* property_value, char* no_value_property);
	extern DLLAPI int NANCAL_libsign_register_callbacks();
	extern DLLAPI int get_current_user_info( 
		tag_t *user_tag,   
		counted_tag_list_t *role_tags,                               
		tag_t *group_tag                                              
		);

#ifdef _cplusplus
}
#endif

#define CALL(function)	\
	if ( ifail = function ){\
		EMH_ask_error_text( ifail, &err_string );										\
		if(ifail!=ITK_ok) EMH_store_error( EMH_severity_error,ifail);                   \
		printf ("\nERROR: %d ERROR MSG: %s.\n", ifail, err_string);						\
		printf ("Function: %s, FILE: %s, LINE: %d\n", #function, __FILE__, __LINE__);	\
		MEM_free (err_string);\
		return ifail;\
	}\

#define CALL_IN_RULE(function) \
if ( ifail = function ){\
	EMH_ask_error_text( ifail, &err_string );\
	printf ("\nERROR: %d ERROR MSG: %s.\n", ifail, err_string);						\
	printf ("Function: %s, FILE: %s, LINE: %d\n", #function, __FILE__, __LINE__);	\
	MEM_free (err_string);\
	EMH_store_error( EMH_severity_error, ifail ); \
	goto CLEANUP;\
}\

#define DATE_TO_STRING_FORMAT "%Y%m%d"

//关系名称
#define IMAN_rel "IMAN_specification,IMAN_manifestation"//关系名称
//PDF数据集类型名称
#define PDF_TYPE "PDF"
//PDF引用类型名称
#define PDF_REF "PDF_Reference"
//CAD数据集类型名称
#define DWG_TYPE "CD4_ACADDWG"
//CAD引用类型名称
#define DWG_REF "CD4_DWG"
//WORD数据集类型名称
#define WORD_TYPE "MSWord"
//WORD引用类型名称
#define WORD_REF "word"
//WORDX数据集类型名称
#define WORDX_TYPE "MSWordX"
//WORDX引用类型名称
#define WORDX_REF "word"
//EXCEL数据集类型名称
#define EXCEL_TYPE "MSExcel"
//EXCEL引用类型名称
#define EXCEL_REF "excel"
//EXCELX数据集类型名称
#define EXCELX_TYPE "MSExcelX"
//EXCELX引用类型名称
#define EXCELX_REF "excel"


//PPT数据集类型名称
#define PPT_TYPE "MSPowerPoint"
//PPT引用类型名称
#define PPT_REF "powerpoint"
//PPTX数据集类型名称
#define PPTX_TYPE "MSPowerPointX"
//PPTX引用类型名称
#define PPTX_REF "powerpoint"


