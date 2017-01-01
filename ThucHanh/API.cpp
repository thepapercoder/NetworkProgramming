//Buổi 2
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//#include <windows.h>
//#include <stdio.h>
//
//int getFileSize(DWORD high, DWORD low) {
//	if (high > 0) {
//		return high*(MAXDWORD + 1) + low;
//	}
//	else 
//		return low;
//}
//
//int main() {
//	char*  welcome = "Chao mung den voi server \n";
//
//	WSAData wsa;
//	WSAStartup(MAKEWORD(2, 2), &wsa);
//	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	SOCKADDR_IN saddr;
//	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	saddr.sin_family = AF_INET;
//	saddr.sin_port = htons(8888);
//
//	bind(s, (SOCKADDR*)&saddr, sizeof(saddr));
//
//	listen(s, 10);
//
//	while (1) {
//		SOCKADDR_IN caddr;
//		int clen = sizeof(caddr);
//		SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
//		send(c, welcome, strlen(welcome), 0);
//		WIN32_FIND_DATAA DATA;
//		HANDLE h = FindFirstFileA("C:\\nc111nt (1)\\*.*", &DATA);
//		do
//		{
//			if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			{
//				send(c, "Thu muc: ", strlen("Thu muc: "), 0);
//				send(c, DATA.cFileName, strlen(DATA.cFileName), 0);
//				send(c, "\n", strlen("\n"), 0);
//			}
//			else
//			{	
//				char* buffer = (char*)malloc(1024);
//				ZeroMemory(buffer, 1024);
//				long fsize = getFileSize(DATA.nFileSizeHigh, DATA.nFileSizeLow);
//				sprintf(buffer, "%s %s %s %d bytes\n", "File: ", DATA.cFileName, " size: ", fsize);
//				send(c, buffer, strlen(buffer), 0);
//			}
//		} while (FindNextFileA(h, &DATA));
//
//	}
//
//
//	return 0;
//}