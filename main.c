#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <strsafe.h>
#include <wmistr.h>
#include <evntrace.h>

#define LOGFILE_PATH L"C:\\Users\\admin\\Desktop\\test2.etl"
#define LOGSESSION_NAME L"Darpa ETW Session"
#define EVENT_ENABLE_PROPERTY_SID 0x1;

// GUID that identifies your trace session.
// Remember to create your own session GUID.

// {AE44CB98-BD11-4069-8093-770EC9258A12}
static const GUID SessionGuid =
{ 0xae44cb98, 0xbd11, 0x4069, { 0x80, 0x93, 0x77, 0xe, 0xc9, 0x25, 0x8a, 0x12 } };

// GUID that identifies the provider that you want
// to enable to your session.
//Microsoft-Windows-Kernel-File EDD08927-9CC4-4E65-B970-C2560FB5C289
static const GUID ProviderGuid =
{ 0xEDD08927, 0x9CC4, 0x4E65, { 0xB9, 0x70, 0xC2, 0x56, 0x0F, 0xB5, 0xC2, 0x89 } };



void wmain(void)
{
	ULONG status = ERROR_SUCCESS;
	TRACEHANDLE SessionHandle = 0;
	EVENT_TRACE_PROPERTIES* pSessionProperties = NULL;
	ULONG BufferSize = 0;
	BOOL TraceOn = TRUE;

	// Allocate memory for the session properties. The memory must
	// be large enough to include the log file name and session name,
	// which get appended to the end of the session properties structure.

	BufferSize = sizeof(EVENT_TRACE_PROPERTIES)+sizeof(LOGFILE_PATH)+sizeof(LOGSESSION_NAME);
	pSessionProperties = (EVENT_TRACE_PROPERTIES*)malloc(BufferSize);
	if (NULL == pSessionProperties)
	{
		wprintf(L"Unable to allocate %d bytes for properties structure.\n", BufferSize);
		goto cleanup;
	}

	// Set the session properties. You only append the log file name
	// to the properties structure; the StartTrace function appends
	// the session name for you.

	ZeroMemory(pSessionProperties, BufferSize);
	pSessionProperties->Wnode.BufferSize = BufferSize;
	pSessionProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	pSessionProperties->Wnode.ClientContext = 1; //QPC clock resolution
	pSessionProperties->Wnode.Guid = SessionGuid;
	pSessionProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
	pSessionProperties->MaximumFileSize = 1;  // 1 MB
	pSessionProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
	pSessionProperties->LogFileNameOffset = sizeof(EVENT_TRACE_PROPERTIES)+sizeof(LOGSESSION_NAME);
	StringCbCopy((LPWSTR)((char*)pSessionProperties + pSessionProperties->LogFileNameOffset), sizeof(LOGFILE_PATH), LOGFILE_PATH);

	// Create the trace session.

	status = StartTrace((PTRACEHANDLE)&SessionHandle, LOGSESSION_NAME, pSessionProperties);
	if (ERROR_SUCCESS != status)
	{
		wprintf(L"StartTrace() failed with %lu\n", status);
		goto cleanup;
	}

	// Enable the providers that you want to log events to your session.
	status = EnableTraceEx(
		//(LPCGUID)&ProviderGuid_file,
		(LPCGUID)&ProviderGuid,
		0,
		SessionHandle,
		1,
		5,
		0,
		0,
		0,
		0
		);

	/*status = EnableTraceEx2(
		SessionHandle,
		(LPCGUID)&ProviderGuid,
		EVENT_CONTROL_CODE_ENABLE_PROVIDER,
		TRACE_LEVEL_INFORMATION,
		0,
		0,
		0,
		NULL
		);
*/
	if (ERROR_SUCCESS != status)
	{
		wprintf(L"EnableTrace() failed with %lu\n", status);
		TraceOn = FALSE;
		goto cleanup;
	}
	wprintf(L"Run the provider application. Then hit any key to stop the session.\n");
	_getch();

cleanup:
	if (SessionHandle)
	{
		if (TraceOn)
		{
			status = EnableTraceEx2(
				SessionHandle,
				(LPCGUID)&ProviderGuid,
				EVENT_CONTROL_CODE_DISABLE_PROVIDER,
				TRACE_LEVEL_INFORMATION,
				0,
				0,
				0,
				NULL
				);
		}

		status = ControlTrace(SessionHandle, LOGSESSION_NAME, pSessionProperties, EVENT_TRACE_CONTROL_STOP);

		if (ERROR_SUCCESS != status)
		{
			wprintf(L"ControlTrace(stop) failed with %lu\n", status);
		}
	}

	if (pSessionProperties)
	{
		free(pSessionProperties);
		pSessionProperties = NULL;
	}
	_getch();
}
