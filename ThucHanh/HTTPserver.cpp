// Buổi 3

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>

int getFileSize(DWORD high, DWORD low) {
	if (high > 0) {
		return high*(MAXDWORD + 1) + low;
	}
	else
		return low;
}

bool isFile(char* link) {
	WIN32_FIND_DATAA DATA;
	HANDLE h = FindFirstFileA(link, &DATA);
	// created by Nguyen Giang Son
	if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return false;
	}
	else
	{
		return true;
	}
}

char* getServerURL(char* link) {
	char* baselink = "C:\\*.*";
	char* newlink = (char*) malloc(4);
	strcpy(newlink, "C:");
	if (strcmp(link, "\\") == 0) {
		return baselink;
	}
	char * pch = (char*) malloc(1024);
	ZeroMemory(pch, sizeof(pch));
	pch = strtok(link, "/");
	while (pch != NULL) {
		newlink = (char*)realloc(newlink, (strlen(newlink) + strlen(pch) + 2) * sizeof(char));
		strcpy(newlink + strlen(newlink), "\\");
		strcpy(newlink + strlen(newlink), pch);
		ZeroMemory(pch, sizeof(pch));
		pch = strtok(NULL, "/");
	}
	return newlink;
}

void sendDirectory(SOCKET c, char* link) {
	char* baselink = "C://*.*";
	char* newlink;
	bool isHomePage = false;

	if (strcmp(link, "/") != 0) // Kiểm tra có phải trang chủ không
	{	
		char* tmp = (char*)malloc(strlen(link));
		ZeroMemory(tmp, sizeof(tmp));
		strcpy(tmp, link);
		newlink = getServerURL(tmp);
	}
	else {
		newlink = baselink;
		isHomePage = true;
	}

	if (isFile(newlink)) // Kiểm tra có phải file không
	{	
		// nếu là file thì đọc file và gửi sang server
		FILE *fp = fopen(newlink, "r");
		if (fp == NULL) {
			send(c, "sida roi", strlen("sida roi"), 0);
			return;
		}
		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);
		rewind(fp);
		char* response = (char*)malloc(fsize + strlen("HTTP/1.1 200 OK\nContent-Type:text/html;charset=utf-8\n\n<html></html>"));
		ZeroMemory(response, sizeof(response));
		strcat(response, "HTTP/1.1 200 OK\nContent-Type:text/html;charset=utf-8\n\n<html>");
		fread(response + strlen("HTTP/1.1 200 OK\nContent-Type:text/html;charset=utf-8\n\n<html>"), 1, fsize, fp);
		strcat(response + strlen(response), "</html>");
		send(c, response, strlen(response), 0);
		return;
	}
	else {
		// nếu là folder thì gửi danh sách
		char* response = (char*)malloc(99999);
		ZeroMemory(response, 99999);
		strcat(response, "HTTP/1.1 200 OK\nContent-Type:text/html;charset=utf-8\n\n<html>");
		if (!isHomePage) {
			int sizeinc = (strlen(newlink) + 6);
			newlink = (char*)realloc(newlink, sizeof(char) * sizeinc);
			strcpy(newlink + strlen(newlink), "//*.*");
		}
		
		WIN32_FIND_DATAA DATA;
		HANDLE h = FindFirstFileA(newlink, &DATA);
		do
		{	
			char* href = (char*) malloc(1024);
			ZeroMemory(href, 1024);
			sprintf(href, "%s/%s");

			if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char* buffer = (char*)malloc(1024);
				ZeroMemory(buffer, 1024);
				long fsize = getFileSize(DATA.nFileSizeHigh, DATA.nFileSizeLow);
				sprintf(buffer, "<p><b><a href='http://127.0.0.1:8888/%s/%s'> %s </a></b></p>", link+1, DATA.cFileName, DATA.cFileName);
				strncat(response, buffer, strlen(buffer));
			}
			else
			{
				char* buffer = (char*)malloc(1024);
				ZeroMemory(buffer, 1024);
				long fsize = getFileSize(DATA.nFileSizeHigh, DATA.nFileSizeLow);
				sprintf(buffer, "<p><i><a href='http://127.0.0.1:8888/%s/%s'>%s size: %d</i></p>", link+1, DATA.cFileName, DATA.cFileName, fsize);
				strncat(response, buffer, strlen(buffer));
			}
		} while (FindNextFileA(h, &DATA));

		strcat(response, "</html>");
		send(c, response, strlen(response), 0);
		return;
	}

	

	
}

int main() {
	char*  welcome = "Chao mung den voi server \n";

	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN saddr;
	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);

	bind(s, (SOCKADDR*)&saddr, sizeof(saddr));

	listen(s, 10);
	// created by Nguyen Giang Son
	while (1) {
		SOCKADDR_IN caddr;
		int clen = sizeof(caddr);
		SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
		char* buffer = (char*)malloc(10240);
		memset(buffer, 0, 10240);
		int recvlen = recv(c, buffer, 2048, 0);
		if (recvlen == 0) {
			printf("No string was send to server");
		}
		char* link = (char*)malloc(1024);
		ZeroMemory(link, 1024);
		sscanf(buffer, "GET %s HTTP/1.1%s", link, buffer);
		sendDirectory(c, link);
		closesocket(c);
	}
	return 0;
}

