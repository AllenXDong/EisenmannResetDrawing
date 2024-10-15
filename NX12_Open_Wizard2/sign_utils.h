#pragma warning(once:4819)
#pragma warning(once:4996)

#define USER_ERROR_NORMAL (EMH_USER_error_base)

#define DEBUG "-debug"
#define DEBUG_EQUALS "-debug="
#define MAX_FIELD_LENGTH 255
#define MAX_ICO_ARRAY_ATTRIBUTE_TOTAL_LENGTH 5000
#define MAX_PRINTLINE_LENGTH 2000
#define MAX_READLINE_LENGTH 1024

#define MAX_ARGUMENT_LENGTH 400
#define MAX_PARAMNAME_LENGTH 50

#define MAX_PATH_LENGTH 2000
#define MAX_FILE_NAME_LENGTH 255
#define MAX_FILE_EXT_LENGTH 10
#define MAX_COMMAND_LINE_LENGTH 2000

#define DATE_TO_STRING_FORMAT "%Y.%m.%d"
#define DATE_TO_STRING_FORMAT2 "%Y.%m.%d"

#define DETAILLOG true
#define TEXT_FORMAT "TEXT"
#define BINARY_FORMAT "BINARY"
#define ATTRIBUTE_SPLITTER ","

#define SPLIT_SIGN "|"
#define ERROR_SIGN "FALSE"
#define DONE_SIGN "TRUE"



logical debugFlag = false;
/*=============================================================================*
* FUNCTION: DebugLog
* PURPOSE : write debug log, write log message to log File with the [debug] flag
*
* DO NOT use this function directly, Call macro WRITEDEBUGLOGX instead
*
* INPUT:
*     const char* format // debug message string
*
* RETURN:
*     void
*============================================================================*/
void DebugLog(const char* format, ...)
{
	va_list arg;
	char tmp[MAX_PRINTLINE_LENGTH];



	if (!debugFlag)
	{
		return;
	}

	//get the message
	memset(tmp, 0, sizeof(tmp));
	va_start(arg, format);
	vsnprintf(tmp, MAX_PRINTLINE_LENGTH, format, arg);
	va_end(arg);

	//print message to back ground screen
	printf("[debug]%s\n", tmp);

	//print message to log file
	TC_write_syslog("[debug]%s\n", tmp);
}

/*=============================================================================*
* FUNCTION: WriteLog
* PURPOSE : write log, if debug log File not null, write log message to log File
* INPUT:
*     const char* format // debug message string
*
* RETURN:
*     void
*============================================================================*/
void WriteLog(const char* format, ...)
{
	va_list arg;
	char tmp[MAX_PRINTLINE_LENGTH];



	//get the message
	memset(tmp, 0, sizeof(tmp));
	va_start(arg, format);
	vsnprintf(tmp, MAX_PRINTLINE_LENGTH, format, arg);
	va_end(arg);

	//----------print to command window for trace--------//
	if (tmp[sizeof(tmp) - 1] != 0)
	{
		tmp[sizeof(tmp) - 1] = 0;
	}

	//print message to back ground screen
	printf("%s\n", tmp);

	//print message to log file
	TC_write_syslog("%s\n", tmp);
}
/*=============================================================================*
* FUNCTION: genUserErr
* PURPOSE : generate user error message info
* INPUT:
*     const char* format // message string
*
* RETURN:
*     void
*============================================================================*/
int genUserErr(const char* format, ...)
{
	va_list arg;
	char tmp[MAX_PRINTLINE_LENGTH];

	//get the message
	memset(tmp, 0, sizeof(tmp));
	va_start(arg, format);
	vsnprintf(tmp, MAX_PRINTLINE_LENGTH, format, arg);
	va_end(arg);

	WriteLog("[ERROR!]%s\n", tmp);

	EMH_store_error_s1(EMH_severity_user_error,
		USER_ERROR_NORMAL,
		tmp);

	return USER_ERROR_NORMAL;
}