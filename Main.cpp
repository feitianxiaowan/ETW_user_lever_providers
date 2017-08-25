#include <Windows.h>
#include <conio.h>
#include <strsafe.h>
#include <wmistr.h>
#include <evntrace.h>
#include <tdh.h>

#include <stdio.h>
#include <iostream>

// Attention here!!
#pragma comment(lib,"tdh.lib")

using namespace std;

void enumerate_providers();

int main(int argc, char *argv[]) {

}



// Enumerate all(include manifest-based & MOF registered on computer) ETW providers and output:)
void enumerate_providers() {
	ULONG provider_info_size = 10, *p2provider_info_size = &provider_info_size;
	PROVIDER_ENUMERATION_INFO *p2provider_info = (PROVIDER_ENUMERATION_INFO *)malloc(provider_info_size);
	TDHSTATUS enumerate_status = TdhEnumerateProviders(p2provider_info, p2provider_info_size);
	if (ERROR_INSUFFICIENT_BUFFER == enumerate_status) {
		// Malloc enough space for provider information.
		free(p2provider_info);
		p2provider_info = (PROVIDER_ENUMERATION_INFO *)malloc(provider_info_size);
	}
	else if (ERROR_INVALID_PARAMETER == enumerate_status) {
		printf("Enumerate Providers failed! Invalid parameter.\n");
		return;
	}
	enumerate_status = TdhEnumerateProviders(p2provider_info, p2provider_info_size);
	if (ERROR_SUCCESS != enumerate_status) {
		printf("Enumerate Providers failed! Unkown problem.\n");
		return;
	}
	for (ULONG i = p2provider_info->NumberOfProviders - 1; i >= 0; i--) {
		// output format is worng, but who cares:]
		cout << p2provider_info->TraceProviderInfoArray[i].ProviderGuid.Data1 << '-' << p2provider_info->TraceProviderInfoArray[i].ProviderGuid.Data2 << '-' << p2provider_info->TraceProviderInfoArray[i].ProviderGuid.Data3 << '-' << p2provider_info->TraceProviderInfoArray[i].ProviderGuid.Data4 << endl;
	}
	free(p2provider_info);
}