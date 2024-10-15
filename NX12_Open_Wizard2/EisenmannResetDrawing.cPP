#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_exit.h>
#include <uf_ugmgr.h>
#include <uf_part.h>
#include <stdarg.h>
#include "en_nx_getset_dpv_prefs.h"
#include <NXSigningResource.cpp>
static void ECHO(char *format, ...)
{
	char msg[UF_UI_MAX_STRING_LEN + 1];
	va_list args;
	va_start(args, format);
	vsprintf(msg, format, args);
	va_end(args);
	UF_UI_open_listing_window();
	UF_UI_write_listing_window(msg);
	UF_print_syslog(msg, FALSE);
}


/*ARGSUSED*/
void ufusr(char *param, int *retcode, int paramLen)
{
	if (UF_CALL(UF_initialize())) return;
	run_getset_dpv_values();
	UF_terminate();
}

int ufusr_ask_unload(void)
{
	return (UF_UNLOAD_SEL_DIALOG);
}