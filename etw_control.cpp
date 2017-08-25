#include <Windows.h>
#include <conio.h>
#include <strsafe.h>
#include <wmistr.h>
#include <evntrace.h>
#include <tdh.h>

#include "etw_control.h"


using namespace std;

//Microsoft-Windows-Eventlog  {D8909C24-5BE9-4502-98CA-AB7BDC24899D}
static const GUID provider_guid_eventlog = { 0xAE4BD3BE, 0xF36F, 0x45B6,{ 0x8D, 0x21, 0xBD, 0xD6, 0xFB, 0x83, 0x28, 0x53 } };
//Microsoft-Windows-Audio AE4BD3BE-F36F-45B6-8D21-BDD6FB832853
static const GUID provider_guid_audio = { 0xD8909C24, 0x5BE9, 0x4502,{ 0x98, 0xCA, 0xAB, 0x7B, 0xDC, 0x24, 0x89, 0x9D } };
//Microsoft-Windows-Kernel-File EDD08927-9CC4-4E65-B970-C2560FB5C289
static const GUID provider_guid_file = { 0xEDD08927, 0x9CC4, 0x4E65,{ 0xB9, 0x70, 0xC2, 0x56, 0x0F, 0xB5, 0xC2, 0x89 } };

// Enable ETW provider using EnableTraceEx2() and StartTrace()
void start_etw() {
	// [WNODE.Buffersize] Total size of memory allocated, in bytes, for the event tracing session properties. The size of memory must include the room for the EVENT_TRACE_PROPERTIES structure plus the session name string and log file name string that follow the structure in memory.
	ULONG darpa_session_properites_size = sizeof(EVENT_TRACE_PROPERTIES) + ETW_SESSION_NAME_MAX_LENGTH + ETW_LOGFILE_NAME_MAX_LENGTH; 
	// {To-do} For Windows 10, version 1703 and later version, we can pass filtering to StartTrace. To do so, we need to pass in the new "EVENT_TRACE_PROPERTIES_V2". https://msdn.microsoft.com/en-us/library/windows/desktop/aa363689(v=vs.85).aspx
	PEVENT_TRACE_PROPERTIES p2darpa_session_properites = (PEVENT_TRACE_PROPERTIES)malloc(darpa_session_properites_size);


	PTRACEHANDLE darpa_session_handle, &r2darpa_session_handle = darpa_session_handle;
	TDHSTATUS start_trace_status = StartTrace(r2darpa_session_handle, ETW_SESSION_NAME, p2darpa_session_properites);

}

