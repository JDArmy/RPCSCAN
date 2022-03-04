//===============================================================================================//
// This is a stub for the actuall functionality of the DLL.
//===============================================================================================//
#include "ReflectiveLoader.h"
#include <stdio.h>
#include "../rpc.h"

// Note: REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR and REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN are
// defined in the project properties (Properties->C++->Preprocessor) so as we can specify our own 
// DllMain and use the LoadRemoteLibraryR() API to inject this DLL.

// You can use this value as a pseudo hinstDLL value (defined and set via ReflectiveLoader.c)
extern HINSTANCE hAppInstance;
//===============================================================================================//

//typedef struct {
//	char	processName[40];
//	char	uuid[40];
//} UPB;
//
//typedef struct {
//	char	host[20];
//	int		port;
//	int		lengthUPB;
//	UPB		upbdata[];
//} PBINFO;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved) {
	BOOL bReturnValue = TRUE;
	switch (dwReason) {
	case DLL_QUERY_HMODULE:
		if (lpReserved != NULL)
			*(HMODULE*)lpReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		hAppInstance = hinstDLL;

		/* print some output to the operator */
		// MessageBoxA(NULL, "123", "1123", MB_OK);
		if (lpReserved != NULL) {
			PBINFO* para = (PBINFO*)lpReserved;
			resolve(para);
			//printf("len is %d\n", para->length);
			//printf("len is %d\n", para->thread);
			//printf("port is %d\n", para->port);
			//printf("host is %s\n", para->host);
			////ips(*para);
			//for (int i = 0; i < para->lengthUPB;i++) {
			//	printf("proc is %s\n", para->upbdata[i].processName);
			//	printf("uuid is %s\n", para->upbdata[i].uuid);
			//}

			//MessageBox(NULL, para->host, "test", MB_OK);
			//HANDLE hFile = CreateFile("c:\\users\\public\\1.txt", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			//DWORD dwWrite;

			//WriteFile(hFile, lpReserved, 1024, &dwWrite, NULL);
			//CloseHandle(hFile);
			//printf("Hello from test.dll. Parameter is '%s'\n", (char*)lpReserved);
		}
		else {
			printf("There is no parameter\n");
		}

		/* flush STDOUT */
		fflush(stdout);

		/* we're done, so let's exit */
		ExitProcess(0);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return bReturnValue;
}