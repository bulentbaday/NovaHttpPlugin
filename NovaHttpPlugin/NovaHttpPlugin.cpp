#include <mysql.h>
#include <stdio.h>
#include <WinHttp.h>
#include <string>

#pragma comment(lib, "WinHttp.lib")

using namespace std;

typedef long long my_integer;

extern "C" __declspec(dllexport) bool HttpRequest_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
	if (!(initid->ptr = (char *)malloc(sizeof(char) * 1048576))) {
		const char* s = "Bellek ayrılamadı."; //Function could not be initialized.
		strcpy_s(message, strlen(s) + 1, s);
		return 1;
	}

	memset(initid->ptr, 0, sizeof(char) * 1048576);

	if (args->arg_count != 4)
	{
		const char* s = "Missing parameters."; //Missing parameters.
		strcpy_s(message, strlen(s) + 1, s);
		return 1;
	}

	if (args->arg_type[0] != STRING_RESULT      //URL
		|| args->arg_type[1] != STRING_RESULT   //Method
		|| args->arg_type[2] != STRING_RESULT   //Content Type
		|| args->arg_type[3] != STRING_RESULT)  //Data
	{
		const char* s = "Invalid parameters."; //Wrong parameters.
		strcpy_s(message, strlen(s) + 1, s);
		return 1;
	}

	return 0;
}

extern "C" __declspec(dllexport) void HttpRequest_deinit(UDF_INIT *initid)
{
	if (initid->ptr) {
		delete (char*)initid->ptr;
	}
}

extern "C" __declspec(dllexport) char* HttpRequest(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error)
{
	const char* _result = "";

	char* _url = (char*)args->args[0];
	char* _method = (char*)args->args[1];
	char* _contentType = (char*)args->args[2];

	wstring url(_url, _url + strlen(_url));
	wstring method(_method, _method + strlen(_method));
	wstring contentType(_contentType, _contentType + strlen(_contentType));

	LPSTR data = (char*)args->args[3];

	DWORD dataLength = 0;

	if (data != NULL) {
		dataLength = (DWORD)strlen(data);
	}

	HINTERNET hSession = WinHttpOpen(L"Nova",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0);

	if (!hSession) {
		*error = 1;

		_result = "";
		*length = strlen(_result);
		memcpy(initid->ptr, _result, *length);
		return initid->ptr;
	}

	WinHttpSetTimeouts(hSession,
		60000,
		60000,
		60000,
		60000);

	URL_COMPONENTS urlComp;
	wchar_t szHostName[MAX_PATH] = L"";
	wchar_t szURLPath[MAX_PATH * 5] = L"";
	memset(&urlComp, 0, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);
	urlComp.lpszHostName = szHostName;
	urlComp.dwHostNameLength = MAX_PATH;
	urlComp.lpszUrlPath = szURLPath;
	urlComp.dwUrlPathLength = MAX_PATH * 5;
	urlComp.dwSchemeLength = 1;

	string strResult = "";

	if (WinHttpCrackUrl(url.c_str(), url.size(), 0, &urlComp))
	{
		HINTERNET hConnect = WinHttpConnect(hSession, szHostName, urlComp.nPort, 0);

		if (!hConnect)
		{
			WinHttpCloseHandle(hSession);

			*error = 1;

			_result = "";
			*length = strlen(_result);
			memcpy(initid->ptr, _result, *length);
			return initid->ptr;
		}

		DWORD dwOpenRequestFlag = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;

		HINTERNET hRequest = WinHttpOpenRequest(hConnect,
			method.c_str(),
			urlComp.lpszUrlPath,
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			dwOpenRequestFlag);

		if (!hRequest) {
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);

			*error = 1;

			_result = "";
			*length = strlen(_result);
			memcpy(initid->ptr, _result, *length);
			return initid->ptr;
		}

		if (urlComp.nScheme == INTERNET_SCHEME_HTTPS)
		{
			DWORD options = SECURITY_FLAG_IGNORE_CERT_CN_INVALID
				| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
				| SECURITY_FLAG_IGNORE_UNKNOWN_CA;

			WinHttpSetOption(hRequest,
				WINHTTP_OPTION_SECURITY_FLAGS,
				(LPVOID)&options,
				sizeof(DWORD));
		}

		if (WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			WINHTTP_NO_REQUEST_DATA,
			0,
			dataLength,
			0)) {

			if (dataLength > 0) {
				DWORD dwWritten = 0;

				if (WinHttpWriteData(hRequest,
					data,
					dataLength,
					&dwWritten)) {

				}
			}

			if (WinHttpReceiveResponse(hRequest, NULL)) {

				DWORD dwSize = 0;

				LPSTR pszOutBuffer;

				do
				{
					dwSize = 0;

					DWORD dwDownloaded = 0;

					if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
					{
						break;
					}

					if (!dwSize)
						break;

					pszOutBuffer = new char[dwSize + 1];
					if (!pszOutBuffer)
					{
						break;
					}

					ZeroMemory(pszOutBuffer, dwSize + 1);

					if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
						dwSize, &dwDownloaded))
					{
						break;
					}
					else
					{
						strResult.append(pszOutBuffer);
					}

					delete[] pszOutBuffer;

					if (!dwDownloaded)
						break;

				} while (dwSize > 0);
			}
			else {

				*error = 1;

				_result = "";
				*length = strlen(_result);
				memcpy(initid->ptr, _result, *length);
				return initid->ptr;

			}
		}
		else {

			*error = 1;

			_result = "";
			*length = strlen(_result);
			memcpy(initid->ptr, _result, *length);
			return initid->ptr;

		}

		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
	}

	WinHttpCloseHandle(hSession);

	_result = (char*)strResult.c_str();
	*length = strlen(_result);
	memcpy(initid->ptr, _result, *length);
	return initid->ptr;

}
