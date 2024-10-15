#include <tcinit/tcinit.h>
#include <tccore/custom.h>
#include <tccore/grm_msg.h>
#include <ae/dataset_msg.h>
#include <tccore/tctype.h>
#include <sa/sa.h>
#include <tccore/grmtype.h>
#include <property/prop_errors.h>
#include <cfm/cfm.h>
#include <ict/ict_userservice.h>
#include <base_utils/Mem.h>
#include <tc/tc_arguments.h>
#include <sa/sa.h>
#include <tccore/item.h>
#include <tccore/tctype.h>
#include <tc/preferences.h>
#include <user_exits/epm_toolkit_utils.h>
#include <res/res_itk.h>
#include <ps/ps.h>
#include <dispatcher/dispatcher_errors.h>
#include <dispatcher/dispatcher_itk.h>
#include <epm/epm.h>
#include <epm/epm_toolkit_tc_utils.h>
#include <ae/dataset.h>
#include <tccore/grm.h>
#include <tccore/grmtype.h>
#include <tccore/item.h>
#include <ae/datasettype.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <sa/sa.h>
#include <sa/groupmember.h>
#include <epm/cr.h>
#include <epm/signoff.h>
#include <fclasses/tc_date.h>
#include <tc/preferences.h>
#include <stdio.h>
#include <stdlib.h>
#include "libsign_custom_methods.h"
#include "sign_utils.h"
#define NANCAL_CHECK_PROPERTY (EMH_USER_error_base + 10)

#define CR_comment_size_c               240
char temp_info[1024]={'\0'};

// 获取审核信息
int pubSign_get_Inf(tag_t task_tag,char *name,char *cname,char *date);
int getFileName(char *originalName,char new_name[IMF_filename_size_c+1]);
int pubSign_find(int ds_count,tag_t* ds_tags,char* ds_type,char dsname[WSO_name_size_c+1],tag_t *right_tag);
int pubSign_replace_datasetRef(char dstpath[256],char* ref_Type,tag_t right_tag);
int pubSign_checkin(tag_t ds_tag);

int cust_sign(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;

	tag_t	job_tag = NULLTAG;
	tag_t root_task = NULLTAG;
	tag_t task_tag = NULLTAG;

	int	Number = 0;
	int	SingInfCount = 0;
	int	argNumber = 0;
	int	i = 0,j = 0,k = 0,l=0,n=0;
	tag_t	*ftags = NULL;
	tag_t TypeTag = NULLTAG;
	tag_t	SignInfRelType = NULLTAG;
	tag_t	*SignInfs = NULL;  

	char *task_type=NULL;
	char*	argStr = NULL;
	char*	ParaName = NULL;
	char*	ParaValue = NULL;
	tag_t* SignOffs = NULLTAG;
	int SignOffcount=0;
	tag_t member = NULLTAG;
	tag_t user = NULLTAG;
	date_t dicdate;
	SIGNOFF_TYPE_t type;
	char curuserStr[SA_user_size_c+1]={'\0'};
	char curuserCNStr[SA_person_name_size_c+1]={'\0'};
	char dateStr[64]={'\0'};
	char curdateStr[16]={'\0'};
	char str[5]={'\0'};
	char **CurSignInfs=NULL;
	int	CurSignInfNum=0;
	tag_t	curGroupMember=NULLTAG;
	tag_t person_tag = NULLTAG;

	char signName[TCTYPE_class_name_size_c+1]={'\0'};
	char signDate[TCTYPE_class_name_size_c+1]={'\0'};
	char signNameCN[TCTYPE_class_name_size_c+1]={'\0'};
	logical	signNameflag=false;
	logical	signDateflag=false;
	logical	signNameCNflag=false;
	logical	flag=false;
	tag_t	type_tag=NULLTAG;
	tag_t	parent_tag=NULLTAG;
	logical	answer=false;
	logical	is_exist=false;
	logical	sign_flag=false;

	int month=0,m=0,m1=13;
	int day=0,d=0,d1=33;
	int hour=0,h=0,h1=25;
	int minute =0,min=0,min1=61;
	int second = 0,s=0,s1=61;
	int index = 0;
	int index1 = 0;
	int index2 = 1;

	tag_t	*tasks = NULL;  
	int taskCount=0;

	tag_t *reviewtasks = NULL;
	tag_t* cld_reviewtasks=NULL;
	int reviewCount=0,refcount = 0,cld_reviewCount=0;
	char *name=NULL; 

	char cname[TCTYPE_class_name_size_c+1]={'\0'};
	char date[TCTYPE_class_name_size_c+1]={'\0'};
	char  	type_name[TCTYPE_name_size_c+1]	={'\0'};


	char* objectName=NULL;
	char obj_type[WSO_name_size_c+1] = "";
	int datasetCount=0, ds_count=0;

	tag_t* datasets = NULL;	
	tag_t itemRev= NULL;
	tag_t RefRelTag=NULLTAG;
	tag_t relation=NULLTAG;
	char* relation_name = NULL;
	tag_t dataset_type_tag=NULLTAG;
	logical	 ds_answer=false,is_pdf=false;
	char dataset_type[AE_datasettype_name_size_c+1]={'\0'};
	char refname[AE_reference_size_c + 1]={'\0'};
	char dataset_name[WSO_name_size_c+1]={'\0'};
	char* className = NULL;
	tag_t class_id=NULLTAG;		
	char cur_dataset_name[WSO_name_size_c+1]={'\0'};
	AE_reference_type_t reftype;
	tag_t refobj = NULLTAG;
	char OrgFileName[IMF_filename_size_c+1]={'\0'};
	char new_name[IMF_filename_size_c+1]={'\0'};		
	char srcpath[256]="";
	char dstpath[256]="";
	tag_t* ds_tags=NULL;
	tag_t right_tag=NULLTAG;
	char cmd[2048]={'\0'};
	char release_cmd[256]="";
	char* itemID=NULL;
	//char itemRevisionID[ITEM_id_size_c + 1] = "";

	char sign_name[1024]={'\0'};

	/*处理类型*/
	char split[] = ","; 
	char* p; 
	char* pp;
	char words[128];  
	char* PIC_PATH=NULL;//首选项获取图片地址
	char* TEM_PATH=NULL;//首选项获取本地临时目录
	char* TEM_PATH2=NULL;//首选项获取本地临时目录
	char* TEM_PATH3=NULL;//首选项获取本地临时目录
	char* SIGN_TYPE = NULL;//需要进行签字对象父级类型
	int ccc = 0;
	CR_signoff_decision_t decision;                      /**< (O) */
	char comments[CR_comment_size_c + 1];
	char* date_string=NULL;

	tag_t  current_groupmember_tag = NULL;
	tag_t  group_tag=NULL;
	char* groupname;
	/*防止进入循环陷阱*/    
	task_tag = msg.task;
	if( task_tag==NULL_TAG )
		return ITK_ok;
	if ( msg.action > EPM_perform_action )
	{
		return ITK_ok;
	}    
	/*获取首选项*/
	CALL(PREF_ask_char_value("NANCAL_PLM_CUSTOM_TEMP_PATH",0,&TEM_PATH));//获取首选项本地临时目录
	/*printf("定义的本地临时目录:%s\n", TEM_PATH);*/
	WriteLog("定义的本地临时目录:%s\n",TEM_PATH);

	CALL(SA_ask_current_groupmember(&current_groupmember_tag));
	CALL(SA_ask_groupmember_group(current_groupmember_tag,&group_tag));	
	CALL(POM_ask_group_name(group_tag,&groupname));
	/*printf("groupname:%s\n",groupname);*/

	/*获取流程对象*/ 
	CALL(EPM_ask_job( msg.task, &job_tag ));
	CALL(EPM_ask_root_task(job_tag,&root_task));
	CALL(EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags));


	//获取整个流程的子节点
	CALL(AOM_ask_value_tags(root_task,"child_tasks",&taskCount,&tasks));
	printf("***taskCount:%d%\n",taskCount);

	for(index = 0;index < 1024;index ++){
		sign_name[index] = '\0';
	}

	for(k=0; k<taskCount;k++)
	{
		CALL(AOM_ask_value_string(tasks[k],"object_type",&task_type));//类型				
		CALL(AOM_ask_value_string(tasks[k],"object_name",&name));//名称				
		//printf("****type:%s\n",task_type);
		if(tc_strcmp(task_type,"EPMDoTask")==0)//do任务
		{
			tc_strcat(sign_name,name);
			tc_strcat(sign_name,":");
			CALL(pubSign_get_Inf(tasks[k],signName,cname,date));
			tc_strcat(sign_name,cname);			
			tc_strcat(sign_name,",");
			tc_strcat(sign_name,date);
			tc_strcat(sign_name,"$");
			tc_strcat(sign_name,";");
		}
		else if(tc_strcmp(task_type,"EPMReviewTask")==0)//审核任务
		{
			CALL(AOM_ask_value_tags(tasks[k],"child_tasks",&reviewCount,&reviewtasks));
			tc_strcat(sign_name,name);
			tc_strcat(sign_name,":");
			for(j=0; j<reviewCount;j++)
			{
				CALL(AOM_ask_value_string(reviewtasks[j],"object_type",&task_type));
				if(tc_strcmp(task_type,"EPMPerformSignoffTask")==0)
				{
					//获得签审成员
					CALL(EPM_ask_attachments(reviewtasks[j],EPM_signoff_attachment,&SignOffcount,&SignOffs));
					for(l=0;l<SignOffcount;l++)
					{
						//审核人员
						CALL(EPM_ask_signoff_member(SignOffs[l],&member,&type));									      
						if(SIGNOFF_GROUPMEMBER == type)
						{
							CALL(SA_ask_groupmember_user(member,&user));
						}

						/*如果审核人员选择的不是具体的人员，则取当前用户*/
						else 
						{
							CALL(SA_ask_current_groupmember(&curGroupMember));
							CALL(SA_ask_groupmember_user(curGroupMember,&user));
						}
						CALL(SA_ask_user_person(user,&person_tag));
						CALL(SA_ask_person_name2(person_tag,curuserCNStr));
						CALL(SA_ask_user_identifier2(user,curuserStr));

						tc_strcat(sign_name,curuserCNStr);
						tc_strcat(sign_name,",");

						//审核日期
						CALL(EPM_ask_signoff_decision(SignOffs[l],&decision,comments,&dicdate));
						CALL(DATE_date_to_string(dicdate,DATE_TO_STRING_FORMAT,&date_string));
						tc_strcat(sign_name,date_string);
						tc_strcat(sign_name,"$");
					}
				}
			}
			tc_strcat(sign_name,";");
		}
		else if(tc_strcmp(task_type,"EPMRouteTask")==0)//会签任务
		{
			tc_strcat(sign_name,name);
			tc_strcat(sign_name,":");
			CALL(AOM_ask_value_tags(tasks[k],"child_tasks",&reviewCount,&reviewtasks));
			for(j=0; j<reviewCount;j++)
			{
				CALL(AOM_ask_value_tags(reviewtasks[j],"child_tasks",&cld_reviewCount,&cld_reviewtasks));
				for(n=0; n<cld_reviewCount;n++)
				{
					CALL(AOM_ask_value_string(cld_reviewtasks[n],"object_type",&task_type));
					if(tc_strcmp(task_type,"EPMPerformSignoffTask")==0)
					{
						//获得签审成员
						CALL(EPM_ask_attachments(cld_reviewtasks[n],EPM_signoff_attachment,&SignOffcount,&SignOffs));
						for(l=0;l<SignOffcount;l++)
						{
							//审核人员
							CALL(EPM_ask_signoff_member(SignOffs[l],&member,&type));									      
							if(SIGNOFF_GROUPMEMBER == type)
							{
								CALL(SA_ask_groupmember_user(member,&user));
							}

							/*如果审核人员选择的不是具体的人员，则取当前用户*/
							else 
							{
								CALL(SA_ask_current_groupmember(&curGroupMember));
								CALL(SA_ask_groupmember_user(curGroupMember,&user));
							}
							CALL(SA_ask_user_person(user,&person_tag));
							CALL(SA_ask_person_name2(person_tag,curuserCNStr));
							CALL(SA_ask_user_identifier2(user,curuserStr));

							tc_strcat(sign_name,curuserCNStr);
							tc_strcat(sign_name,",");

							//审核日期
							CALL(EPM_ask_signoff_decision(SignOffs[l],&decision,comments,&dicdate));
							CALL(DATE_date_to_string(dicdate,DATE_TO_STRING_FORMAT,&date_string));
							tc_strcat(sign_name,date_string);
							tc_strcat(sign_name,"$");
						}
					}
				}
			}
			tc_strcat(sign_name,";");
		}
	}
	printf("***info:%s\n",sign_name);

	printf("***target_number:%d\n",Number);
	/*循环流程目标对象*/
	for(i=0; i<Number; i++)
	{
		/*刷新元素*/
		CALL(AOM_refresh(ftags[i],0));			
		CALL(AOM_ask_value_string(ftags[i],"object_string",&objectName));
		CALL(WSOM_ask_object_type(ftags[i], obj_type));
		/*获得当前对象的存储类型的父级类型,如果是ItemRevision则继续执行*/
		CALL(TCTYPE_find_type(obj_type,obj_type,&type_tag));
		CALL(TCTYPE_ask_parent_type(type_tag,&parent_tag));//获取当前对象的父级类型对象
		CALL(TCTYPE_ask_name(parent_tag,type_name));//父级类型名称
		//printf("对象名称为:%s,类型为:%s\n",objectName,obj_type);
		WriteLog("对象名称为:%s,类型为:%s\n",objectName,obj_type);
		if(tc_strcmp(type_name,"ItemRevision")==0 || tc_strcmp(obj_type,"ItemRevision")==0
			|| tc_strcmp(type_name,"ChangeNoticeRevision") == 0 || tc_strcmp(obj_type,"ChangeNoticeRevision")==0 
			|| tc_strcmp(type_name,"DocumentRevision")==0 || tc_strcmp(obj_type,"DocumentRevision")==0)
		{
			WriteLog("当前签字对象名称为:%s,类型为:%s\n",objectName,obj_type);
			itemRev = ftags[i];

			tc_strcpy(words,"IMAN_specification,IMAN_manifestation,IMAN_reference");
			pp = words;
			p = strtok(pp,","); 
			while(p!=NULL) { 
				WriteLog("p:%s\n",p);
				datasetCount = 0;
				RefRelTag = NULLTAG;
				datasets = NULL;
				/*得到关系类型对象*/
				CALL(GRM_find_relation_type(p,&RefRelTag));	
				/*得到关系的数据集集合*/
				CALL(GRM_list_secondary_objects_only(itemRev,RefRelTag,&datasetCount,&datasets));	
				WriteLog("dscount:%d\n",datasetCount);

				/*遍历数据集开始*/
				for(j=0;j<datasetCount;j++)
				{
					CALL(TCTYPE_find_type("Dataset","Dataset",&dataset_type_tag));
					CALL(TCTYPE_ask_object_type(datasets[j],&type_tag));
					CALL(TCTYPE_is_type_of(type_tag,dataset_type_tag,&ds_answer));
					if(ds_answer)
					{
						CALL(WSOM_ask_object_type(datasets[j],dataset_type));
						CALL(WSOM_ask_name(datasets[j],dataset_name));
						WriteLog("数据集名称:%s,数据集类型:%s\n",dataset_name,dataset_type);
						if(tc_strcmp(dataset_type,"Bp3_Autocad") == 0)//如果数据集类型为Bp3_Autocad,不做处理
						{
							is_pdf = true;
							continue;
						}
						//sprintf_s(cur_dataset_name,WSO_name_size_c * sizeof( char ),"%s",dataset_name);
						AE_ask_dataset_ref_count(datasets[j],&refcount);
						if (refcount==0)
						{
							WriteLog("* #Warning:no file in the Dataset \"%s\" is 0!\n",dataset_name);
							continue;
						}
						CALL(AE_find_dataset_named_ref2(datasets[j], 0, refname, &reftype, &refobj));
						if (refobj==NULLTAG)
						{
							WriteLog("* #Warning:no File in the Dataset \"%s\" Ref!\n",dataset_name);
							continue;
						}

						CALL(POM_class_of_instance(refobj,&class_id));
						CALL(POM_name_of_class(class_id,&className));
						if(tc_strcmp(className,"ImanFile")!=0)
						{
							continue;
						}
						/*获得源文件的名称*/
						CALL(IMF_ask_original_file_name2(refobj,OrgFileName));
						WriteLog("#info#:源文件的名称:%s\n",OrgFileName);
						for(k=0;k<IMF_filename_size_c+1;k++)
						{
							new_name[k]='\0';	
						}
						CALL(getFileName(OrgFileName,new_name));				

						/*开始处理各类型签字程序*/ 
						if(tc_strcmp(dataset_type,WORD_TYPE)==0)//word类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.doc",TEM_PATH,new_name);
							is_pdf=false;	
						}
						else if(tc_strcmp(dataset_type,WORDX_TYPE)==0)//wordx类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.docx",TEM_PATH,new_name);	
							is_pdf=false;	
						}
						else if(tc_strcmp(dataset_type,EXCEL_TYPE)==0)//excel类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.xls",TEM_PATH,new_name);
							is_pdf=false;	
						}
						else if(tc_strcmp(dataset_type,EXCELX_TYPE)==0)//excelx类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.xlsx",TEM_PATH,new_name);
							is_pdf=false;
						}
						else if(tc_strcmp(dataset_type,PPT_TYPE)==0)//PPT类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.ppt",TEM_PATH,new_name);
							is_pdf=false;
						}
						else if(tc_strcmp(dataset_type,PPTX_TYPE)==0)//PPTX类型
						{
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.pptx",TEM_PATH,new_name);
							is_pdf=false;
						}
						else if(tc_strcmp(dataset_type,PDF_TYPE)==0)//pdf类型
						{
							is_pdf=true;
							sprintf_s(srcpath, 256 * sizeof( char ),"%s\\%s.pdf",TEM_PATH,new_name);	
							if (!access(srcpath,0))//文件是否存在
							{
								remove(srcpath);//删除文件
							}
							CALL(IMF_export_file(refobj ,srcpath));	
							sprintf_s(cmd, 2048 * sizeof( char ), "pdfsign.exe %s %s %s %s", srcpath,sign_name,"$",groupname);
							WriteLog("#info#:运行PDF签字命令:%s\n", cmd);
							if(system(cmd)!= ITK_ok)
							{
								genUserErr("Error! 运行PDF签字程序出错\n");
							}
							else
							{
								/*处理pdf数据集*/        
								ds_count=0;
								ds_tags=NULL;
								right_tag=NULLTAG;               
								GRM_list_secondary_objects_only(itemRev,NULLTAG,&ds_count,&ds_tags);
								WriteLog("#info#:查找当前数据集%s是否存在命名引用:%s\n", dataset_name,dataset_type);
								CALL(pubSign_find(ds_count,ds_tags,dataset_type,dataset_name,&right_tag));
								if(right_tag!=NULLTAG)
								{	
									WriteLog("#info#:源文件地址:%s\n",srcpath);
									WriteLog("#info#:开始替换%s命名引用文件\n",dataset_name);
									pubSign_replace_datasetRef(srcpath,dataset_type,right_tag);
									WriteLog("#info#:完成替换%s命名引用文件\n",dataset_name);
								}
							}
						}


						if(is_pdf == false)//如果类型不为pdf
						{	
							sprintf_s(cmd, 2048 * sizeof(char), "officeSign.exe %s %s %s", srcpath,sign_name,"$");
							if (!access(srcpath,0))//文件是否存在
							{
								remove(srcpath);//删除文件
							}
							CALL(IMF_export_file(refobj ,srcpath));	
							WriteLog("#info#:运行office签字命令:%s\n", cmd);
							if(system(cmd) == ITK_ok)
							{
								/*处理数据集*/           
								ds_count=0;
								ds_tags=NULL;
								right_tag=NULLTAG;      
								GRM_list_secondary_objects_only(itemRev,NULLTAG,&ds_count,&ds_tags);
								WriteLog("#info#:查找当前数据集%s是否存在命名引用:%s\n", dataset_name,dataset_type);
								CALL(pubSign_find(ds_count,ds_tags,dataset_type,dataset_name,&right_tag));
								if(right_tag!=NULLTAG)
								{	
									WriteLog("#info#:源文件地址:%s\n",srcpath);
									WriteLog("#info#:开始替换%s命名引用文件\n",dataset_name);
									pubSign_replace_datasetRef(srcpath,dataset_type,right_tag);
									WriteLog("#info#:完成替换%s命名引用文件\n",dataset_name);
								}
							}
							else
							{
								genUserErr("Error!运行office签字程序出错\n");
							}
						}

						SAFE_SM_FREE(ds_tags);
						WriteLog("#Info:完成签字!\n*\n");
						remove(srcpath);
						remove(dstpath);
					}
				}
				//数据集结束
				p = strtok(NULL,","); 
			}
		}
	}

	SAFE_SM_FREE(SignOffs);
	SAFE_SM_FREE(ftags);
	return ifail;
}
/*****************************************************
*							获得信息												 	 *
******************************************************/
int pubSign_get_Inf(tag_t task_tag,char *name,char *cname,char *date)
{
	int		ifail = ITK_ok;

	char	*err_string=NULL;
	int		err_line = -1;

	char	*personName;// [SA_person_name_size_c + 1] = { '\0' };
	char  *userid;// [SA_user_size_c + 1]; = { '\0' };
	tag_t		person_tag=NULLTAG;
	date_t 	Credate;
	char  	riqi[16]={'\0'};
	tag_t		user_tag=NULLTAG;

	/*获得用户名*/
	AOM_ask_value_tag(task_tag,"last_mod_user",&user_tag);
	SA_ask_user_identifier2(user_tag,&userid);
	SA_ask_user_person(user_tag,&person_tag);
	SA_ask_person_name2(person_tag,&personName);
	tc_strcpy(name,userid);
	tc_strcpy(cname,personName);
	/*获得对象最后一次更新的日期*/
	AOM_ask_value_date(task_tag,"last_mod_date",&Credate);
	/*构建日期的值*/
	if(Credate.month<9&&Credate.day<10)
	{
		sprintf(riqi, "%d.0%d.0%d",Credate.year,(Credate.month+1),Credate.day);
	}
	else if(Credate.month<9&&Credate.day>=10)
	{
		sprintf(riqi, "%d.0%d.%d",Credate.year,(Credate.month+1),Credate.day);
	}
	else if(Credate.month>=9&&Credate.day<10)
	{
		sprintf(riqi, "%d.%d.0%d",Credate.year,(Credate.month+1),Credate.day);
	}
	else
	{
		sprintf(riqi, "%d.%d.%d",Credate.year,(Credate.month+1),Credate.day);
	}
	SAFE_SM_FREE(personName);
	SAFE_SM_FREE(userid);
	tc_strcpy(date,riqi);

	return ifail;
}
/*****************************************************
*							  截取文件名称，不带后缀               *
******************************************************/
int getFileName(char *originalName,char new_name[IMF_filename_size_c+1])
{
	int ifail = ITK_ok;
	int err_line = -1;

	int len=0;
	int i=0;
	int loc=0;
	char chars[]={'!','@','#','$','%','^','\'',',','&','*',',','(',')','_','+','~','`','-','=','[',']','\\',';',',','.','/','{','}','|',':','"','>','<',' ','·','-','=','【','】','、','‘','；','、','。','，','~','！','@','#','￥','%','……','&','*','（','）','——','+','|','}','{','“','：','？','》','《'};
	int chars_len=0;
	int j=0;

	chars_len=tc_strlen(chars);
	len=strlen(originalName);
	for(i=0;i<len;i++)
	{
		if(originalName[i] > '~' || originalName[i] < ' '){
			continue;
		}
		if(originalName[i]=='.')
		{
			loc=i;	
		}
		/*!@#$%^&*()_+~`-=[]\;',./{}|:"?>< ·-=【】、‘；、。，~！@#￥%……&*（）——+|}{“：？》《*/
		for(j=0;j<chars_len;j++)
		{
			if(originalName[i]==chars[j])
			{
				originalName[i]='_';
			}
		}
	}
	for(i=0;i<loc;i++)
	{
		new_name[i]=originalName[i];	
	}
	return ifail;
}
/*****************************************************
*							  寻找同名的数据集		                 *
******************************************************/
int pubSign_find(int ds_count,tag_t* ds_tags,char* ds_type,char dsname[WSO_name_size_c+1],tag_t *right_tag)
{
	int						ifail = ITK_ok;
	int						err_line = -1;

	int 					i=0;
	int 					k=0;
	char 					curname[WSO_name_size_c+1]={'\0'};
	char 					object_type[WSO_name_size_c+1]={'\0'};
	tag_t 				TypeTag=NULLTAG;
	tag_t					type_tag=NULLTAG;
	logical				answer=false;

	char* type=NULL;

	/*遍历数据集*/				
	for(i=0;i<ds_count;i++)
	{
		AOM_refresh(ds_tags[i],0);
		for(k=0;k<WSO_name_size_c+1;k++)
		{
			curname[k]='\0';
			object_type[k]='\0';	
		}
		/*获得数据集类型,并和Dataset类型做比较*/
		TCTYPE_find_type("Dataset","Dataset",&type_tag);
		TCTYPE_ask_object_type(ds_tags[i],&TypeTag);
		TCTYPE_is_type_of(TypeTag,type_tag,&answer);	
		/*如果是Dataset类型，则继续执行，否则进入下次循环*/
		if(answer==true)
		{
			/*获得数据集的type的值和数据集的名称*/
			WSOM_ask_name(ds_tags[i],curname);
			WSOM_ask_object_type(ds_tags[i],object_type);
			type=object_type;
			/*如果type的值是PDF类型且数据集名称和传入的数据集名称一样，则返回当前数据集*/
			if(tc_strcmp(curname,dsname)==0 && tc_strcmp(type,ds_type)==0)
			{
				//printf("pubSign_find:%s\n",ds_type);
				*right_tag=ds_tags[i];
				break;
			}
		}
		pubSign_checkin(ds_tags[i]);	
	}
	return ifail;
}
/*****************************************************
*							  将签出的数据集签入	                 *
******************************************************/
int pubSign_checkin(tag_t ds_tag)
{
	int						ifail = ITK_ok;
	char					*err_string=NULL;
	int						err_line = -1;

	logical				is_checkout=false;
	tag_t					type_tag=NULLTAG;
	char					checkout_user_id[SA_user_size_c+1]={'\0'},current_user_id[SA_user_size_c+1]={'\0'};
	tag_t					checkout_user=NULLTAG,current_groupmember=NULLTAG,current_user=NULLTAG;
	tag_t					form_type_tag=NULLTAG,dataset_type_tag=NULLTAG;
	logical				form_answer=false,ds_answer=false;

	/*判断是否为Form、Dataset的子类型*/
	TCTYPE_find_type("Form","Form",&form_type_tag);
	TCTYPE_find_type("Dataset","Dataset",&dataset_type_tag);
	TCTYPE_ask_object_type(ds_tag,&type_tag);
	TCTYPE_is_type_of(type_tag,form_type_tag,&form_answer);
	TCTYPE_is_type_of(type_tag,dataset_type_tag,&ds_answer);
	/*如果是子类型*/
	if(form_answer==true||ds_answer==true)
	{
		/*判断是否签出*/
		RES_is_checked_out(ds_tag,&is_checkout);
		/*如果签出*/
		if(is_checkout==true)
		{
			/*获得签出的用户，如果是当前用户则签入，否则不执行*/
			AOM_ask_value_tag(ds_tag,"checked_out_user",&checkout_user);
			SA_ask_user_identifier2(checkout_user,checkout_user_id);
			SA_ask_current_groupmember(&current_groupmember);
			SA_ask_groupmember_user(current_groupmember,&current_user);
			SA_ask_user_identifier2(current_user,current_user_id);
			if(tc_strcmp(checkout_user_id,current_user_id)==0)
			{
				RES_checkin(ds_tag);
			}			
		}
	}
	return ifail;
}
/*****************************************************
*							替换数据集的命名引用								 	 *
******************************************************/
int pubSign_replace_datasetRef(char dstpath[256],char* ref_Type,tag_t right_tag)
{
	int						ifail = ITK_ok;
	char					*err_string=NULL;
	int						err_line = -1;

	char       		*generatedFileName = NULL;
	tag_t					file_tag=NULLTAG;
	IMF_file_t  	fileDescriptor;
	int 					right_ref_count=0;
	AE_reference_type_t		pdf_ref_type;
	tag_t					pdf_ref_obj=NULLTAG;

	char 	                reference_name[AE_reference_size_c+1] = {'\0'};

	POM_AM__set_application_bypass(TRUE);

	IMF_import_file(dstpath,generatedFileName,SS_BINARY,&file_tag,&fileDescriptor);

	SAFE_SM_FREE(generatedFileName);
	IMF_close_file(fileDescriptor);

	AOM_save(file_tag);

	AOM_unlock(file_tag);


	/*锁定数据集*/
	AOM_lock(right_tag);
	/*获得命名的引用的个数，如果为0则增加命名的引用，如果不为0则替换命名的引用*/
	AE_ask_dataset_ref_count(right_tag,&right_ref_count);

	//	CALL(AE_find_dataset_named_ref(right_tag,0,PDF_REF,&pdf_ref_type,&pdf_ref_obj));
	AE_find_dataset_named_ref2(right_tag,0,ref_Type,&pdf_ref_type,&pdf_ref_obj);

	if(right_ref_count==0)
	{
		AE_add_dataset_named_ref2(right_tag,ref_Type,pdf_ref_type,file_tag);
	}
	else
	{
		AE_replace_dataset_named_ref2(right_tag,pdf_ref_obj,ref_Type,pdf_ref_type,file_tag);
	}
	/*保存数据集并刷新、签入*/
	AE_save_myself(right_tag);
	AOM_unlock(right_tag);
	AOM_refresh(right_tag,0);
	pubSign_checkin(right_tag);

	POM_AM__set_application_bypass(FALSE);

	return ifail;	
}

int archive_sign(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;
	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;
	tag_t task_tag = NULLTAG;
	int	Number = 0;
	tag_t	*ftags = NULL;
	int	i = 0,j = 0,k = 0,l=0,n=0,m=0;

	char* objectName=NULL;
	char obj_type[WSO_name_size_c+1] = "";
	int datasetCount=0, ds_count=0;
	tag_t* datasets = NULL;	
	tag_t itemRev= NULL;
	tag_t RefRelTag=NULLTAG;
	char sign_name[256]={'\0'};
	char sign_name1[128]={'\0'};
	char* item_id = NULL;
	char* rev_id = NULL;
	char* TEM_PATH1 = NULL;
	char path[128]={'\0'};
	FILE * bodyFile;
	char system_comm[BUFSIZ + 1]={'\0'};

	/*获取流程对象*/ 
	CALL(EPM_ask_job( msg.task, &job_tag ));
	CALL(EPM_ask_root_task(job_tag,&root_task));
	CALL(EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags));

	CALL(PREF_ask_char_value("NANCAL_PLM_CUSTOM_TEMP_PATH",0,&TEM_PATH1));//获取首选项本地临时目录
	tc_strcat(path,TEM_PATH1);
	tc_strcat(path,"\sign_file.txt");
	WriteLog("path:%s\n",path);
	bodyFile = fopen(path, "w");

	for(i=0; i<Number; i++)
	{
		/*刷新元素*/
		CALL(AOM_refresh(ftags[i],0));			
		CALL(AOM_ask_value_string(ftags[i],"object_string",&objectName));
		CALL(WSOM_ask_object_type(ftags[i], obj_type));
		WriteLog("对象名称为:%s,类型为:%s\n",objectName,obj_type);
		if(tc_strcmp(obj_type,"Hm9_MouldPartRevision")==0)
		{
			WriteLog("当前签字对象名称为:%s,类型为:%s\n",objectName,obj_type);
			itemRev = ftags[i];

			for(j = 0;j < 256;j ++){
				sign_name[j] = '\0';
			}
			CALL(AOM_ask_value_string(itemRev,"item_id",&item_id));
			CALL(AOM_ask_value_string(itemRev,"item_revision_id",&rev_id));
			tc_strcat(sign_name,"@DB/");
			tc_strcat(sign_name,item_id);
			tc_strcat(sign_name,"/");
			tc_strcat(sign_name,rev_id);
			tc_strcat(sign_name,"\n");
			datasetCount = 0;
			RefRelTag = NULLTAG;
			datasets = NULL;
			/*得到关系类型对象*/
			CALL(GRM_find_relation_type("IMAN_specification",&RefRelTag));
			/*得到关系的数据集对象*/
			CALL(GRM_list_secondary_objects_only(itemRev,RefRelTag,&datasetCount,&datasets));
			WriteLog("revcount:%d\n",datasetCount);
			for(j=0; j<datasetCount; j++)
			{
				CALL(WSOM_ask_object_type(datasets[j], obj_type));
				if(tc_strcmp(obj_type,"UGPART")==0)
				{
					WriteLog("数据集类型为:%s\n",obj_type);
					CALL(AOM_ask_value_string(datasets[j],"object_string",&objectName));
					for(m = 0;m < 128;m ++){
						sign_name1[m] = '\0';
					}
					tc_strcat(sign_name1,"@DB/");
					tc_strcat(sign_name1,item_id);
					tc_strcat(sign_name1,"/");
					tc_strcat(sign_name1,rev_id);
					tc_strcat(sign_name1,"/specification/");
					tc_strcat(sign_name1,objectName);
					tc_strcat(sign_name1,"\n");
					tc_strcat(sign_name,sign_name1);
				}
			}
			WriteLog("sign_name:%s\n",sign_name);
			fprintf(bodyFile, "%s", sign_name);

		}
	}
	fclose(bodyFile);
	////fwrite(header, sizeof(char), 3, bodyFile);
	tc_strcat(system_comm,"%TC_BIN%\\updateNXattribute.bat");
	WriteLog("system_comm:%s\n",system_comm);
	system("%TC_BIN%\\updateNXattribute.bat");

}

int check_object_class(tag_t target, const char* objtypename, logical *flag)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	tag_t dataset_type_tag;
	tag_t type_tag;
	CALL(TCTYPE_find_type(objtypename, objtypename, &dataset_type_tag));
	CALL(TCTYPE_ask_object_type(target, &type_tag));
	CALL(TCTYPE_is_type_of(type_tag, dataset_type_tag, flag));

	return  ifail;
}

extern int HT6_create_integration_task(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;
	char provider_name[32];
	char service_name[32];
	char task_parameters[256];
	int i = 0;
	char* argKey = NULL;
	char* argValue = NULL;

	tag_t job = NULLTAG;
	tag_t root_task = NULLTAG;
	int attach_object_count = 0;
	tag_t* attach_object_tags = NULL;

	int j = 0;
	logical flag = FALSE;
	tag_t rev_tag = NULLTAG;

	tag_t primary_objs[1];
	char request_arg[32];
	char request_arg1[32];
	char** request_args = NULL;
	tag_t request = NULLTAG;

	char* root_task_uid = NULLTAG;
	char* task_uid = NULLTAG;
	int param_count = 0;
	char** params = NULL;
	char errorMsg[128];
	int rcode = ITK_ok;

	//获取参数
	memset(provider_name, 0, sizeof(provider_name));
	memset(service_name, 0, sizeof(service_name));
	memset(task_parameters, 0, sizeof(task_parameters));

	for (i = 0; i < TC_number_of_arguments(msg.arguments); i++)
	{
		SAFE_SM_FREE(argKey);
		SAFE_SM_FREE(argValue);

		CALL(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argKey, &argValue));

		if (strcmp(argKey, "ProviderName") == 0)
		{
			strcpy(provider_name, argValue);
		}
		else if (strcmp(argKey, "ServiceName") == 0)
		{
			strcpy(service_name, argValue);
		}
		else if (strcmp(argKey, "TaskParameters") == 0)
		{
			strcpy(task_parameters, argValue);
		}

	}

	if (strcmp(provider_name, "") == 0 || strcmp(service_name, "") == 0)
	{
		memset(errorMsg, 0, sizeof(errorMsg));
		sprintf(errorMsg, "参数不正确，请联系系统管理员！");
		EMH_clear_errors();
		rcode = EMH_USER_error_base + 1;
		EMH_store_error_s1(EMH_severity_error, rcode, errorMsg);

		goto CLEANUP;
	}

	CALL(EPM_ask_job(msg.task, &job));
	CALL(EPM_ask_root_task(job, &root_task));
	CALL(EPM_ask_attachments(root_task, EPM_target_attachment, &attach_object_count, &attach_object_tags));

	for (i = 0; i < attach_object_count; i++)
	{
		CALL(check_object_class(attach_object_tags[i], "ItemRevision", &flag));

		if (flag == TRUE)
		{
			rev_tag = attach_object_tags[i];
			break;
		}
	}

	if (rev_tag == NULLTAG)
	{
		memset(errorMsg, 0, sizeof(errorMsg));
		sprintf(errorMsg, "流程目标下未发现零组件版本对象！");
		EMH_clear_errors();
		rcode = EMH_USER_error_base + 1;
		EMH_store_error_s1(EMH_severity_error, rcode, errorMsg);

		goto CLEANUP;
	}

	if (strcmp(task_parameters, "") != 0)
	{
		CALL(EPM__parse_string(task_parameters, ";", &param_count, &params));
	}

	primary_objs[0] = rev_tag;

	request_args = (char**)MEM_alloc(sizeof(char*) *(2 + param_count));

	SAFE_SM_FREE(root_task_uid);
	ITK__convert_tag_to_uid(root_task, &root_task_uid);
	SAFE_SM_FREE(task_uid);
	ITK__convert_tag_to_uid(msg.task, &task_uid);

	memset(request_arg, 0, sizeof(request_arg));
	memset(request_arg1, 0, sizeof(request_arg1));

	sprintf(request_arg, "task_uid=%s", root_task_uid);
	sprintf(request_arg1, "current_task_uid=%s", task_uid);

	request_args[0] = request_arg;
	request_args[1] = request_arg1;

	for (i = 0; i < param_count; i++)
	{
		request_args[i + 2] = params[i];
	}

	CALL(DISPATCHER_create_request(provider_name, service_name, 3, 0, 0, 0, 1, primary_objs, NULL, 2 + param_count, request_args, "WORKFLOW", 0, NULL, NULL, &request));

CLEANUP:
	SAFE_SM_FREE(argKey);
	SAFE_SM_FREE(argValue);
	SAFE_SM_FREE(attach_object_tags);
	SAFE_SM_FREE(root_task_uid);
	SAFE_SM_FREE(task_uid);
	SAFE_SM_FREE(params);
	MEM_free(request_args);

	return rcode;
}

int remove_final(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;
	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;
	tag_t task_tag = NULLTAG;
	int	Number = 0;
	tag_t	*ftags = NULL;
	int	i = 0,j = 0,k = 0,l=0,n=0,m=0;

	char* objectName=NULL;
	char obj_type[WSO_name_size_c+1] = "";
	int datasetCount=0, ds_count=0;
	tag_t* datasets = NULL;	
	tag_t itemRev= NULL;
	tag_t RefRelTag=NULLTAG;
	char sign_name[256]={'\0'};
	char sign_name1[128]={'\0'};
	char* item_id = NULL;
	char* rev_id = NULL;
	char* TEM_PATH1 = NULL;
	char path[128]={'\0'};
	FILE * bodyFile;
	char system_comm[BUFSIZ + 1]={'\0'};
	tag_t target_att[1];
	int n_parents;
	int *levels;
	tag_t *parents;
	tag_t top_parent;
	int  index;

	POM_AM__set_application_bypass(TRUE);

	/*获取流程对象*/ 
	CALL(EPM_ask_job( msg.task, &job_tag ));
	CALL(EPM_ask_root_task(job_tag,&root_task));
	CALL(EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags));

	for(i=0; i<Number; i++)
	{
		/*刷新元素*/
		CALL(AOM_refresh(ftags[i],0));			
		CALL(AOM_ask_value_string(ftags[i],"object_name",&objectName));


		if(strstr(objectName,"Final")!=NULL)
		{
			WriteLog("移除附件:%s,\n",objectName);
			target_att[0] = ftags[i];
			EPM_remove_attachments(root_task,1,target_att);

			PS_where_used_all( ftags[i], 1, &n_parents, &levels, &parents );
			for ( index = 0; index < n_parents; index++ )
			{
				top_parent = parents[index];
				CALL(AOM_ask_value_string(top_parent,"object_name",&objectName));
				WriteLog("父名称:%s,\n",objectName);
				if(strstr(objectName,"Top")!=NULL || strstr(objectName,"TOP")!=NULL || strstr(objectName,"top")!=NULL)
				{
					WriteLog("移除附件:%s,\n",objectName);
					target_att[0] = top_parent;
					EPM_remove_attachments(root_task,1,target_att);
				}
			}
			SAFE_SM_FREE(levels);
			SAFE_SM_FREE(parents);

		}

	}
	SAFE_SM_FREE(objectName);
	SAFE_SM_FREE(ftags);
	POM_AM__set_application_bypass(FALSE);

	return ifail;
}

int node_sign_new(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;

	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;
	tag_t task_tag = NULLTAG;

	int	Number = 0;
	int	SingInfCount = 0;
	int	argNumber = 0;
	int	i = 0,j = 0,k = 0,l=0,n=0;
	tag_t	*ftags = NULL;
	tag_t TypeTag = NULLTAG;
	tag_t	SignInfRelType = NULLTAG;
	tag_t	*SignInfs = NULL;  

	char *task_type=NULL;
	char *temp_task_type=NULL;
	char*	argStr = NULL;
	char*	ParaName = NULL;
	char*	ParaValue = NULL;
	tag_t* SignOffs = NULLTAG;
	int SignOffcount=0;
	tag_t member = NULLTAG;
	tag_t user = NULLTAG;
	date_t dicdate;
	SIGNOFF_TYPE_t type;
	char curuserStr[SA_user_size_c+1]={'\0'};
	char curuserCNStr[SA_person_name_size_c+1]={'\0'};
	char dateStr[64]={'\0'};
	char curdateStr[16]={'\0'};
	char str[5]={'\0'};
	char **CurSignInfs=NULL;
	int	CurSignInfNum=0;
	tag_t	curGroupMember=NULLTAG;
	tag_t person_tag = NULLTAG;

	char signName[TCTYPE_class_name_size_c+1]={'\0'};
	char signDate[TCTYPE_class_name_size_c+1]={'\0'};
	char signNameCN[TCTYPE_class_name_size_c+1]={'\0'};
	logical	signNameflag=false;
	logical	signDateflag=false;
	logical	signNameCNflag=false;
	logical	flag=false;
	tag_t	type_tag=NULLTAG;
	tag_t	parent_tag=NULLTAG;
	logical	answer=false;
	logical	is_exist=false;
	logical	sign_flag=false;

	int month=0,m=0,m1=13;
	int day=0,d=0,d1=33;
	int hour=0,h=0,h1=25;
	int minute =0,min=0,min1=61;
	int second = 0,s=0,s1=61;
	int index = 0;
	int index1 = 0;
	int index2 = 1;

	tag_t	*tasks = NULL;  
	int taskCount=0;

	tag_t *reviewtasks = NULL;
	tag_t* cld_reviewtasks=NULL;
	int reviewCount=0,refcount = 0,cld_reviewCount=0;
	char *name=NULL; 
	char *temp_name=NULL;
	char cname[TCTYPE_class_name_size_c+1]={'\0'};
	char date[TCTYPE_class_name_size_c+1]={'\0'};
	char  	type_name[TCTYPE_name_size_c+1]	={'\0'};

	char* objectName=NULL;
	char obj_type[WSO_name_size_c+1] = "";
	int datasetCount=0, ds_count=0;

	tag_t* datasets = NULL;	
	tag_t itemRev= NULL;
	tag_t RefRelTag=NULLTAG;
	tag_t relation=NULLTAG;
	char* relation_name = NULL;
	tag_t dataset_type_tag=NULLTAG;
	logical	 ds_answer=false,is_pdf=false;
	char dataset_type[AE_datasettype_name_size_c+1]={'\0'};
	char refname[AE_reference_size_c + 1]={'\0'};
	char dataset_name[WSO_name_size_c+1]={'\0'};
	char* className = NULL;
	tag_t class_id=NULLTAG;		
	char cur_dataset_name[WSO_name_size_c+1]={'\0'};
	tag_t refobj = NULLTAG;
	char OrgFileName[IMF_filename_size_c+1]={'\0'};
	char new_name[IMF_filename_size_c+1]={'\0'};		
	char srcpath[256]="";
	char dstpath[256]="";
	tag_t* ds_tags=NULL;
	tag_t right_tag=NULLTAG;
	char cmd[2048]={'\0'};
	char release_cmd[256]="";
	char* itemID=NULL;
	char header[3] = {0xef, 0xbb, 0xbf};
	FILE * bodyFile1;
	FILE * bodyFile2;
	char *tag_string;
	char system_comm[BUFSIZ + 1]={'\0'};

	/*处理类型*/
	char split[] = ","; 
	char* ptr;
	char** TEM_PATH=NULL;//首选项获取本地临时目录
	int nodes_count=0;
	int ccc = 0;
	CR_signoff_decision_t decision;                      /**< (O) */
	char comments[CR_comment_size_c + 1];
	char* date_string=NULL;
	char temp_char2[100];
	char sign_name1[1024]={'\0'};
	char sign_name2[1024]={'\0'};
	char* item_id = NULL;
	char* rev_id = NULL;
	char* TEM_PATH1 = NULL;
	char path1[128]={'\0'};
	char path2[128]={'\0'};
	char file_name[128]={'\0'};
	char* tmpStr;

	tag_t current_groupmember_tag = NULL;
	tag_t  group_tag=NULL;
	char* groupname;
	time_t timep;

	struct tm *p;
	/*防止进入循环陷阱*/    
	task_tag = msg.task;
	if( task_tag==NULL_TAG )
		return ITK_ok;
	if ( msg.action > EPM_perform_action )
	{
		return ITK_ok;
	}    
	printf("++++++++*****node_sign start*******++++++++\n");
	/*获取首选项*/
	CALL(PREF_ask_char_values("NANCAL_PLM_SIGN_NODE_PROPERTY",&nodes_count,&TEM_PATH));//获取签字节点信息
	for(i=0; i<nodes_count; i++)
	{
		char* temp_char = TEM_PATH[i];
		printf("****info:%s\n",temp_char);

	}


	time (&timep);

	p=gmtime(&timep);
	sprintf(file_name,"\%d%d%d%d%d%d.txt",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
	CALL(PREF_ask_char_value("NANCAL_PLM_CUSTOM_TEMP_PATH",0,&TEM_PATH1));//获取首选项本地临时目录
	//tc_strcat(path1,TEM_PATH1);
	//tc_strcat(path1,"\sign_file1.txt");
	//WriteLog("path1:%s\n",path1);
	tc_strcat(path2,TEM_PATH1);
	tc_strcat(path2,file_name);
	//tc_strcat(path2,"\sign_file2.txt");
	WriteLog("path2:%s\n",path2);
	bodyFile1 = fopen(path1, "w");
	bodyFile2 = fopen(path2, "w");

	CALL(SA_ask_current_groupmember(&current_groupmember_tag));
	CALL(SA_ask_groupmember_group(current_groupmember_tag,&group_tag));	
	CALL(POM_ask_group_name(group_tag,&groupname));

	/*获取流程对象*/ 
	CALL(EPM_ask_job( msg.task, &job_tag ));
	CALL(EPM_ask_root_task(job_tag,&root_task));
	CALL(EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags));
	CALL(AOM_ask_value_tags(root_task,"child_tasks",&taskCount,&tasks));
	printf("***taskCount:%d%\n",taskCount);

	CALL(AOM_ask_value_string(msg.task,"object_type",&task_type));//类型				
	CALL(AOM_ask_value_string(msg.task,"object_name",&name));//名称		
	for(i=0; i<Number; i++)
	{
		/*刷新元素*/
		CALL(AOM_refresh(ftags[i],0));			
		CALL(AOM_ask_value_string(ftags[i],"object_string",&objectName));
		CALL(WSOM_ask_object_type(ftags[i], obj_type));
		//WriteLog("对象名称为:%s,类型为:%s\n",objectName,obj_type);
		if(tc_strcmp(obj_type,"Hm9_MouldPartRevision")==0)
		{
			//WriteLog("当前签字对象名称为:%s,类型为:%s\n",objectName,obj_type);
			itemRev = ftags[i];			

			datasetCount = 0;
			RefRelTag = NULLTAG;
			datasets = NULL;
			/*得到关系类型对象*/
			CALL(GRM_find_relation_type("IMAN_master_form",&RefRelTag));	
			/*得到关系的表单对象*/
			CALL(GRM_list_secondary_objects_only(itemRev,RefRelTag,&datasetCount,&datasets));	
			//WriteLog("revcount:%d\n",datasetCount);
			if(datasetCount==0){
				continue;
			}
			CALL(TCTYPE_find_type("Form","Form",&dataset_type_tag));
			CALL(TCTYPE_ask_object_type(datasets[0],&type_tag));
			CALL(TCTYPE_is_type_of(type_tag,dataset_type_tag,&ds_answer));
			//if(ds_answer)
			//{
			//	CALL(AOM_tag_to_string(datasets[0],&tag_string));
			//	for(m = 0;m < 128;m ++){
			//		sign_name2[m] = '\0';
			//	}
			//	tc_strcat(sign_name2,tag_string);
				//tc_strcat(sign_name2,"/");
				//tc_strcat(sign_name2,rev_id);
			//	tc_strcat(sign_name2,";");
			//	fprintf(bodyFile2, "%s", sign_name2);
			//}
			CALL(AOM_tag_to_string(itemRev,&tag_string));
			for(m = 0;m < 128;m ++){
				sign_name2[m] = '\0';
			}
			tc_strcat(sign_name2,tag_string);
			//tc_strcat(sign_name2,"/");
			//tc_strcat(sign_name2,rev_id);
			tc_strcat(sign_name2,";");
			fprintf(bodyFile2, "%s", sign_name2);
		}
	}
	fprintf(bodyFile2, "%s", "\n");
	for(k=0; k<taskCount;k++)
	{
		for(m = 0;m < 28;m ++){
			cname[m] = '\0';
		}
		for(m = 0;m < 28;m ++){
			date[m] = '\0';
		}

		CALL(AOM_ask_value_string(tasks[k],"object_type",&temp_task_type));//类型				
		CALL(AOM_ask_value_string(tasks[k],"object_name",&temp_name));//名称		

		printf("****temp_task_type:%s**temp_name:%s\n",temp_task_type,temp_name);
		if(tc_strcmp(temp_task_type,"EPMDoTask")==0)//do任务
		{
			CALL(pubSign_get_Inf(tasks[k],signName,cname,date));
			printf("***签字人员名称为:%s****签字日期：%s\n",cname, date);
		}
		else if(tc_strcmp(temp_task_type,"EPMReviewTask")==0)//审核任务
		{
			CALL(AOM_ask_value_tags(tasks[k],"child_tasks",&reviewCount,&reviewtasks));
			for(j=0; j<reviewCount;j++)
			{
				CALL(AOM_ask_value_string(reviewtasks[j],"object_type",&temp_task_type));
				if(tc_strcmp(temp_task_type,"EPMPerformSignoffTask")==0)
				{
					//获得签审成员
					CALL(EPM_ask_attachments(reviewtasks[j],EPM_signoff_attachment,&SignOffcount,&SignOffs));
					for(l=0;l<SignOffcount;l++)
					{
						for(m = 0;m < 28;m ++){
							cname[m] = '\0';
						}
						for(m = 0;m < 28;m ++){
							date[m] = '\0';
						}
						//审核人员
						CALL(EPM_ask_signoff_member(SignOffs[l],&member,&type));									      
						if(SIGNOFF_GROUPMEMBER == type)
						{
							CALL(SA_ask_groupmember_user(member,&user));
						}
						/*如果审核人员选择的不是具体的人员，则取当前用户*/
						else 
						{
							CALL(SA_ask_current_groupmember(&curGroupMember));
							CALL(SA_ask_groupmember_user(curGroupMember,&user));
						}
						CALL(SA_ask_user_person(user,&person_tag));
						CALL(SA_ask_person_name2(person_tag,cname));
						CALL(SA_ask_user_identifier2(user,curuserStr));

						//审核日期
						CALL(EPM_ask_signoff_decision(SignOffs[l],&decision,comments,&dicdate));
						CALL(DATE_date_to_string(dicdate,DATE_TO_STRING_FORMAT2,&date_string));
						tc_strcat(date,date_string);
						printf("***签字人员名称为:%s****签字日期：%s\n",cname, date);
					}
				}
			}
		}
		else
		{
			continue;
		}
		for(j=0; j<nodes_count; j++)
		{
			tc_strcpy(temp_char2,TEM_PATH[j]);
			ptr = strtok(temp_char2,"="); 
			if(tc_strcmp(ptr,temp_name)==0){
				ptr = strtok(NULL,"="); 
				tc_strcat(sign_name1,cname);
				tc_strcat(sign_name1,",");
				tc_strcat(sign_name1,ptr);
				tc_strcat(sign_name1,";");
			}else if(tc_strstr(ptr,temp_name)!=0 && tc_strstr(ptr,"日期")!=0){
				ptr = strtok(NULL,"="); 
				tc_strcat(sign_name1,date);
				tc_strcat(sign_name1,",");
				tc_strcat(sign_name1,ptr);
				tc_strcat(sign_name1,";");

			}
		}
		/*
		tc_strcat(sign_name1,cname);
		tc_strcat(sign_name1,",");
		tc_strcat(sign_name1,date);
		tc_strcat(sign_name1,",");
		tc_strcat(sign_name1,ptr);
		tc_strcat(sign_name1,";");*/
	}
	fprintf(bodyFile2, "%s", sign_name1);
	//fclose(bodyFile1);
	fclose(bodyFile2);

	tc_strcat(system_comm,"%TC_BIN%\\updateSignInfo.bat ");
	tc_strcat(system_comm,path2);
	WriteLog("system_comm:%s\n",system_comm);
	system(system_comm);


	tmpStr = tc_strtok(sign_name1, "\n");
	while (tmpStr != NULL)
	{
		printf("***%s****\n",tmpStr);
		tmpStr = tc_strtok(NULL,"\n");
	}
	WriteLog("#Info:完成流程表单签字!\n*\n");
	SAFE_SM_FREE(SignOffs);
	SAFE_SM_FREE(ftags);
	return ifail;
}

int node_sign(EPM_action_message_t msg)
{
	int	ifail = ITK_ok;
	char *err_string = NULL;
	int	err_line = -1;

	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;
	tag_t task_tag = NULLTAG;

	int	Number = 0;
	int	SingInfCount = 0;
	int	argNumber = 0;
	int	i = 0,j = 0,k = 0,l=0,n=0;
	tag_t	*ftags = NULL;
	tag_t TypeTag = NULLTAG;
	tag_t	SignInfRelType = NULLTAG;
	tag_t	*SignInfs = NULL;  

	char *task_type=NULL;
	char*	argStr = NULL;
	char*	ParaName = NULL;
	char*	ParaValue = NULL;
	tag_t* SignOffs = NULLTAG;
	int SignOffcount=0;
	tag_t member = NULLTAG;
	tag_t user = NULLTAG;
	date_t dicdate;
	SIGNOFF_TYPE_t type;
	char *curuserStr=NULL;// [SA_user_size_c + 1] = { '\0' };
	char curuserCNStr[SA_person_name_size_c+1]={'\0'};
	char dateStr[64]={'\0'};
	char curdateStr[16]={'\0'};
	char str[5]={'\0'};
	char **CurSignInfs=NULL;
	int	CurSignInfNum=0;
	tag_t	curGroupMember=NULLTAG;
	tag_t person_tag = NULLTAG;

	char signName[TCTYPE_class_name_size_c+1]={'\0'};
	char signDate[TCTYPE_class_name_size_c+1]={'\0'};
	char signNameCN[TCTYPE_class_name_size_c+1]={'\0'};
	logical	signNameflag=false;
	logical	signDateflag=false;
	logical	signNameCNflag=false;
	logical	flag=false;
	tag_t	type_tag=NULLTAG;
	tag_t	parent_tag=NULLTAG;
	logical	answer=false;
	logical	is_exist=false;
	logical	sign_flag=false;

	int month=0,m=0,m1=13;
	int day=0,d=0,d1=33;
	int hour=0,h=0,h1=25;
	int minute =0,min=0,min1=61;
	int second = 0,s=0,s1=61;
	int index = 0;
	int index1 = 0;
	int index2 = 1;

	tag_t	*tasks = NULL;  
	int taskCount=0;

	tag_t *reviewtasks = NULL;
	tag_t* cld_reviewtasks=NULL;
	int reviewCount=0,refcount = 0,cld_reviewCount=0;
	char *name=NULL; 

	char cname[TCTYPE_class_name_size_c + 1] = { '\0' };
	char date[TCTYPE_class_name_size_c+1]={'\0'};
	char  	type_name[TCTYPE_name_size_c+1]	={'\0'};

	char* objectName=NULL;
	char obj_type[WSO_name_size_c+1] = "";
	int datasetCount=0, ds_count=0;

	tag_t* datasets = NULL;	
	tag_t itemRev= NULL;
	tag_t RefRelTag=NULLTAG;
	tag_t relation=NULLTAG;
	char* relation_name = NULL;
	tag_t dataset_type_tag=NULLTAG;
	logical	 ds_answer=false,is_pdf=false;
	char dataset_type[AE_datasettype_name_size_c+1]={'\0'};
	char refname[AE_reference_size_c + 1]={'\0'};
	char dataset_name[WSO_name_size_c+1]={'\0'};
	char* className = NULL;
	tag_t class_id=NULLTAG;		
	char cur_dataset_name[WSO_name_size_c+1]={'\0'};
	tag_t refobj = NULLTAG;
	char OrgFileName[IMF_filename_size_c+1]={'\0'};
	char new_name[IMF_filename_size_c+1]={'\0'};		
	char srcpath[256]="";
	char dstpath[256]="";
	tag_t* ds_tags=NULL;
	tag_t right_tag=NULLTAG;
	char cmd[2048]={'\0'};
	char release_cmd[256]="";
	char* itemID=NULL;
	char header[3] = {0xef, 0xbb, 0xbf};
	FILE * bodyFile;
	char system_comm[BUFSIZ + 1];

	/*处理类型*/
	char split[] = ","; 
	char* ptr;
	char** TEM_PATH=NULL;//首选项获取本地临时目录
	int nodes_count=0;
	int ccc = 0;
	CR_signoff_decision_t decision;                      /**< (O) */
	char comments[CR_comment_size_c + 1];
	char* date_string=NULL;
	char temp_char2[100];
	char sign_name[1024]={'\0'};
	char sign_name1[1024]={'\0'};
	char* item_id = NULL;
	char* rev_id = NULL;
	char* TEM_PATH1 = NULL;
	char path[128]={'\0'};

	tag_t current_groupmember_tag = NULL;
	tag_t  group_tag=NULL;
	char* groupname;
	/*防止进入循环陷阱*/    
	task_tag = msg.task;
	if( task_tag==NULL_TAG )
		return ITK_ok;
	if ( msg.action > EPM_perform_action )
	{
		return ITK_ok;
	}    
	printf("++++++++*****node_sign start*******++++++++\n");
	/*获取首选项*/
	CALL(PREF_ask_char_values("NANCAL_PLM_SIGN_NODE_PROPERTY",&nodes_count,&TEM_PATH));//获取签字节点信息
	for(i=0; i<nodes_count; i++)
	{
		char* temp_char = TEM_PATH[i];
		printf("****info:%s\n",temp_char);

	}

	CALL(SA_ask_current_groupmember(&current_groupmember_tag));
	CALL(SA_ask_groupmember_group(current_groupmember_tag,&group_tag));	
	CALL(POM_ask_group_name(group_tag,&groupname));

	/*获取流程对象*/ 
	CALL(EPM_ask_job( msg.task, &job_tag ));
	CALL(EPM_ask_root_task(job_tag,&root_task));
	CALL(EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags));
	CALL(AOM_ask_value_tags(root_task,"child_tasks",&taskCount,&tasks));
	printf("***taskCount:%d%\n",taskCount);


	CALL(AOM_ask_value_string(msg.task,"object_type",&task_type));//类型				
	CALL(AOM_ask_value_string(msg.task,"object_name",&name));//名称				
	printf("****type:%s**%s\n",task_type,name);
	if(tc_strcmp(task_type,"EPMDoTask")==0)//do任务
	{
		CALL(pubSign_get_Inf(msg.task,signName,cname,date));
	}
	else if(tc_strcmp(task_type,"EPMReviewTask")==0)//审核任务
	{
		CALL(AOM_ask_value_tags(msg.task,"child_tasks",&reviewCount,&reviewtasks));
		for(j=0; j<reviewCount;j++)
		{
			CALL(AOM_ask_value_string(reviewtasks[j],"object_type",&task_type));
			if(tc_strcmp(task_type,"EPMPerformSignoffTask")==0)
			{
				//获得签审成员
				CALL(EPM_ask_attachments(reviewtasks[j],EPM_signoff_attachment,&SignOffcount,&SignOffs));
				for(l=0;l<SignOffcount;l++)
				{
					//审核人员
					CALL(EPM_ask_signoff_member(SignOffs[l],&member,&type));									      
					if(SIGNOFF_GROUPMEMBER == type)
					{
						CALL(SA_ask_groupmember_user(member,&user));
					}

					/*如果审核人员选择的不是具体的人员，则取当前用户*/
					else 
					{
						CALL(SA_ask_current_groupmember(&curGroupMember));
						CALL(SA_ask_groupmember_user(curGroupMember,&user));
					}
					CALL(SA_ask_user_person(user,&person_tag));
					char *person_name=NULL;
					CALL(SA_ask_person_name2(person_tag, &person_name));
					tc_strcpy(cname, person_name);
					SAFE_SM_FREE(person_name);
					//CALL(SA_ask_user_identifier2(user,&curuserStr));

					//审核日期
					CALL(EPM_ask_signoff_decision(SignOffs[l],&decision,comments,&dicdate));
					CALL(DATE_date_to_string(dicdate,DATE_TO_STRING_FORMAT2,&date_string));
					tc_strcat(date,date_string);
				}
			}
		}
	}
	printf("***签字人员名称为:%s****签字日期：%s\n",cname, date);
	printf("***target_number:%d\n",Number);
	/*循环流程目标对象*/
	for(i=0; i<Number; i++)
	{
		/*刷新元素*/
		CALL(AOM_refresh(ftags[i],0));			
		CALL(AOM_ask_value_string(ftags[i],"object_string",&objectName));
		CALL(WSOM_ask_object_type(ftags[i], obj_type));
		WriteLog("对象名称为:%s,类型为:%s\n",objectName,obj_type);
		if(tc_strcmp(obj_type,"Hm9_MouldPartRevision")==0)
		{
			WriteLog("当前签字对象名称为:%s,类型为:%s\n",objectName,obj_type);
			itemRev = ftags[i];			

			datasetCount = 0;
			RefRelTag = NULLTAG;
			datasets = NULL;
			/*得到关系类型对象*/
			CALL(GRM_find_relation_type("IMAN_master_form",&RefRelTag));	
			/*得到关系的表单对象*/
			CALL(GRM_list_secondary_objects_only(itemRev,RefRelTag,&datasetCount,&datasets));	
			WriteLog("revcount:%d\n",datasetCount);
			if(datasetCount==0){
				continue;
			}
			CALL(TCTYPE_find_type("Form","Form",&dataset_type_tag));
			CALL(TCTYPE_ask_object_type(datasets[0],&type_tag));
			CALL(TCTYPE_is_type_of(type_tag,dataset_type_tag,&ds_answer));
			if(ds_answer)
			{
				POM_AM__set_application_bypass(TRUE);
				for(j=0; j<nodes_count; j++)
				{
					tc_strcpy(temp_char2,TEM_PATH[j]);
					ptr = strtok(temp_char2,"="); 
					if(tc_strcmp(ptr,name)==0){
						ptr = strtok(NULL,"="); 
						CALL(AOM_lock(datasets[0]));
						CALL(AOM_UIF_set_value(datasets[0],ptr,cname));
						CALL(AOM_save(datasets[0]));
						CALL(AOM_unlock(datasets[0]));
						CALL(AOM_refresh(datasets[0],0));

					}else if(tc_strstr(ptr,name)!=0 && tc_strstr(ptr,"Data")!=0){
						ptr = strtok(NULL,"="); 
						CALL(AOM_lock(datasets[0]));
						CALL(AOM_UIF_set_value(datasets[0],ptr,date));
						CALL(AOM_save(datasets[0]));
						CALL(AOM_unlock(datasets[0]));
						CALL(AOM_refresh(datasets[0],0));
					}
				}
				POM_AM__set_application_bypass(FALSE);
				WriteLog("#Info:完成流程节点表单签字!\n*\n");
			}
		}
	}
	//SAFE_SM_FREE(curuserStr);
	SAFE_SM_FREE(SignOffs);
	SAFE_SM_FREE(ftags);
	return ifail;
}



EPM_decision_t nancal_check_property( EPM_rule_message_t msg )
{
	int iNumArg= 0;
	int k=0;
	char* arg= 0;
	char* ptr;
	int ifail = ITK_ok;
	char *err_string = NULL;

	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;

	int	Number = 0,j = 0,l = 0,t=0,index=0;
	tag_t itemRev= NULL;
	tag_t *ftags = NULL;//流程目标
	int m = 0;
	char log_info[2048]={'\0'};
	char* object_type=NULL;
	char* have_value_property = NULL;
	char* property_value = NULL;
	char* no_value_property = NULL;

	iNumArg= TC_number_of_arguments(msg.arguments);
	if(iNumArg!=4){
		EMH_store_error_s1(EMH_severity_error,NANCAL_CHECK_PROPERTY,"参数数量不对！");
		return EPM_nogo;
	}

	for (k=0; k< iNumArg; k++)
	{
		arg= TC_next_argument(msg.arguments);
		ptr = strtok(arg,"="); 
		if(tc_strcmp(ptr,"-object_type") == 0)
		{
			object_type = strtok(NULL,"="); 
		}else if (tc_strcmp(ptr,"-have_value_property") == 0){
			have_value_property = strtok(NULL,"=");
		}else if (tc_strcmp(ptr,"-property_value") == 0){
			property_value = strtok(NULL,"=");
		}else if (tc_strcmp(ptr,"-no_value_property") == 0){
			no_value_property = strtok(NULL,"=");
		}

	}
	if(object_type==NULL || have_value_property==NULL || property_value==NULL || no_value_property==NULL){
		EMH_store_error_s1(EMH_severity_error,NANCAL_CHECK_PROPERTY,"参数值不能为空！");
		return EPM_nogo;
	}

	printf("object_type:%s\n",object_type);
	printf("have_value_property:%s\n",have_value_property);
	printf("property_value:%s\n",property_value);
	printf("no_value_property:%s\n",no_value_property);
	//获得根任务
	CALL_IN_RULE(EPM_ask_job( msg.task, &job_tag ));	
	CALL_IN_RULE(EPM_ask_root_task(job_tag,&root_task));

	EPM_ask_attachments(root_task,EPM_target_attachment,&Number,&ftags);//获取流程目标	
	for(m = 0;m < 2048;m ++){
		temp_info[m] = '\0';
	}
	for(j = 0; j < Number; j++)
	{
		AOM_refresh(ftags[j],0);
		itemRev = ftags[j];
		check_allChild(itemRev,object_type,have_value_property,property_value,no_value_property);
	}
	if(strlen(temp_info)>0){
		temp_info[strlen(temp_info)-1]='\0';
		sprintf_s(log_info, 1024 * sizeof( char ),"规则：%s属性为：%s，%s 属性不能为空!\"\n以下版本对象(%s)不满足:\n\"%s",have_value_property,property_value,no_value_property,object_type,temp_info);
		EMH_store_error_s1(EMH_severity_error,NANCAL_CHECK_PROPERTY,log_info);
		return EPM_nogo;
	}
CLEANUP:
	return EPM_go;
}

EPM_decision_t check_allChild(tag_t parent_item, char* object_type, char* have_value_property, char* property_value, char* no_value_property)
{
	int ifail = ITK_ok;
	char *err_string = NULL;
	int err_line = -1;

	tag_t job_tag = NULLTAG;
	tag_t root_task = NULLTAG;

	char obj_type[WSO_name_size_c+1] = "";  
	int	Number = 0,j = 0,l = 0,t=0,index=0;
	tag_t *ftags = NULL;//流程目标

	tag_t item = NULL;
	char* objectName=NULL;
	char* hm9_MouldType=NULL;
	char* hm9_DrawingN=NULL;
	char* item_id=NULL;
	char log_info[1024]={'\0'};

	int temp_count = 0;
	tag_t *temp_tag = NULL;

	WSOM_ask_object_type(parent_item, obj_type);//获取流程目标的类型
	//printf("type:%s\n",obj_type);
	AOM_ask_value_tag(parent_item,"items_tag",&item);
	AOM_ask_value_tags(parent_item,"ps_children",&temp_count,&temp_tag);
	//判断是否为Hm9_MouldPart版本
	if(tc_strcmp(obj_type,object_type) == 0)
	{
		//判断属性
		AOM_ask_value_string(parent_item,have_value_property,&hm9_MouldType);
		AOM_ask_value_string(parent_item,no_value_property,&hm9_DrawingN);

		//printf("hm9_MouldType:%d\n",hm9_MouldType);
		//printf("hm9_DrawingN:%d\n",hm9_DrawingN);

		if(tc_strcmp(hm9_MouldType,property_value) == 0 && tc_strlen(hm9_DrawingN)==0 )
		{
			AOM_ask_value_string(parent_item,"object_string",&item_id);
			tc_strcat(temp_info,item_id);
			tc_strcat(temp_info,",");
		}

	}
	if(temp_count>0){
		for(j = 0; j < temp_count; j++)
		{
			check_allChild(temp_tag[j],object_type,have_value_property,property_value,no_value_property);
		}
	}
	return EPM_go;
}