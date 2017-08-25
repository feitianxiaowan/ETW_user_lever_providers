#pragma once

#define ETW_SESSION_NAME_MAX_LENGTH 20 
#define ETW_SESSION_NAME "darpa_detection" // the length of ETW_SESSION_NAME should not larger than ETW_SESSION_NAME_MAX_LENGTH
#define ETW_LOGFILE_NAME_MAX_LENGTH 100
#define ETW_LOGFILE_NAME "E:\Programming\ETW_project\User level providers\logfile\demo.etl"


// Enable ETW provider
void start_etw();
