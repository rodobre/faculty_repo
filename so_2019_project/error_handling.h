#ifndef _ERROR_HANDLING_CUSTOM
#define _ERROR_HANDLING_CUSTOM
#include <stdio.h>

#define ERROR_CHECK(condition, message) { if(condition != 0) { perror(message); } }
#define ERROR_CHECK_CND(condition, equal, message) { if(condition == equal) { perror(message); } }
#endif