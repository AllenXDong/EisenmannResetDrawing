#include <ict/ict_userservice.h>
#include <ai/sample_err.h>
#include "libsign_custom_methods.h"
#include <tccore/custom.h>

extern DLLAPI int NANCAL_libsign_register_callbacks()
{
	int stat = ITK_ok;
	printf("\n****************NANCAL_libsign register**************\n");
	CUSTOM_register_exit("NANCAL_libsign", "USERSERVICE_register_methods", (CUSTOM_EXIT_ftn_t)NANCAL_libsign_register_action_handler);

	return stat;
}