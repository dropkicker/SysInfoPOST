#include <windows.h>
#include <Lmcons.h>
#include <WinInet.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
//#include <cstdio>
using namespace std;

#pragma comment( lib, "Wininet.lib")

void InetError(LPCTSTR msg)
{
    DWORD dwErr = GetLastError();
    if (dwErr == ERROR_INTERNET_EXTENDED_ERROR)
    {
        DWORD dwInetErr = 0;
        LPTSTR szResp = NULL;
        DWORD dwLength = 0;

        InternetGetLastResponseInfo(&dwInetErr, NULL, &dwLength);
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            szResp = new TCHAR[dwLength + 1];
            InternetGetLastResponseInfo(&dwInetErr, szResp, &dwLength);
        }

        
        delete[] szResp;
    }
    else
    {
        
    }
}





int main()
{
    HINTERNET hInternet = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;

    hInternet = InternetOpen(TEXT("MyApp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet)
    {
        InetError(TEXT("InternetOpen failed"));
       // goto done;
    }

    hConnect = InternetConnect(hInternet, TEXT("192.168.112.128"), 8081, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        InetError(TEXT("InternetConnect failed"));
       // goto done;
    }

    LPCTSTR rgpszAcceptTypes[] = { TEXT("application/json"), NULL };

    hRequest = HttpOpenRequest(hConnect, TEXT("POST"), TEXT("/xxxBackend/service1.svc/SaveDataVoid"), NULL, NULL, rgpszAcceptTypes, 0, 0);
    if (!hRequest)
    {
        InetError(TEXT("HttpOpenRequest failed"));
       // goto done;
    }

    if (!HttpAddRequestHeaders(hRequest, TEXT("Content-Type: application/json\r\n"), -1, HTTP_ADDREQ_FLAG_ADD))
    {
        InetError(TEXT("HttpAddRequestHeaders failed"));
       // goto done;
    }



    char* JsonData = (char*)"PISSSSS";
  
    if (!HttpSendRequest(hRequest, NULL, 0, JsonData, strlen(JsonData)))
    {
        InetError(TEXT("HttpSendRequest failed"));
        //goto done;
    }

    DWORD StatusCode = 0;
    DWORD StatusCodeLen = sizeof(StatusCode);
    if (!HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &StatusCode, &StatusCodeLen, NULL))
    {
        InetError(TEXT("HttpQueryInfo failed"));
        goto done;
    }

    if (StatusCode == 200)
    {
        BYTE Buffer[2000];
        DWORD dwNumberOfBytes = 0;
        DWORD dwNumberOfBytesRead = 0;

        if (!InternetQueryDataAvailable(hRequest, &dwNumberOfBytes, 0, 0))
        {
            InetError(TEXT("InternetQueryDataAvailable failed"));
            goto done;
        }

        if (!InternetReadFile(hRequest, Buffer, min(dwNumberOfBytes, sizeof(Buffer)), &dwNumberOfBytesRead))
        {
            InetError(TEXT("InternetReadFile failed"));
            goto done;
        }

        //...
    }

done:
    if (hRequest) InternetCloseHandle(hRequest);
    if (hConnect) InternetCloseHandle(hConnect);
    if (hInternet) InternetCloseHandle(hInternet);
}