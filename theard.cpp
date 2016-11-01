#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

SOCKET* clientsocket = NULL; // chứa client socket kết nối đến
int nclient = 0; // số client đã kết nối đến

char* decode(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] >= 65 && str[i] <= 90) // xét các chữ hoa 
		{
			str[i] = 90 - (str[i] - 65);
		}
		if (str[i] >= 97 && str[i] < 123) // xét chữ thường
		{
			str[i] = 122 - (str[i] - 97);
		}
		if (str[i] >= 48 && str[i] <= 57) // xét số
		{
			str[i] = 57 - (str[i] - 48);
		}
	}
	return str;
}

DWORD WINAPI ClientThread(LPVOID arg) // Luồng của từng client để trao đổi dữ liệu
{
	SOCKET c = (SOCKET)arg;
	char* buffer;
	buffer = (char*)malloc(1024);
	int clientIndex = -1;

	while (1) {
		memset(buffer, 0, 1024);
		recv(c, buffer, 1024, 0);
		buffer = decode(buffer);
		for (int i = 0; i < nclient; i++) // tìm index của socket c trong mảng clientsocket để có clientIndex mà ta dùng ở dòng 54
		{
			if (clientsocket[i] == c) {
				clientIndex = i;
			}
		}
		for (int j = 0; j < nclient; j++) // gửi tin nhắn cho tất cả người dùng khác
		{
			if (clientsocket[j] != c) {
				char tmp[1024];
				memset(tmp, 0, 1024);
				sprintf(tmp, "Tin nhan gui tu %d: %s\0", clientIndex, buffer);
				send(clientsocket[j], tmp, strlen(tmp), 0);
				printf(tmp);
			}
		}

	}
}

DWORD WINAPI AcceptThread(LPVOID arg) // Luồng nhận kết nối từ client đến
{
	SOCKET s = (SOCKET)arg;
	char* welcome = "Chuc mung ban ket noi thanh cong\n";
	while (1) {
		SOCKADDR_IN caddr;
		int clen = sizeof(caddr);
		SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
		clientsocket = (SOCKET*)realloc(clientsocket, sizeof(SOCKET)*(nclient + 1));
		clientsocket[nclient] = c;
		nclient++;
		send(c, welcome, strlen(welcome), 0);
		DWORD ID = nclient;
		CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, &ID);
	}
}

DWORD WINAPI InputThread(LPVOID arg) 
{
	while (1) {
		char str[1024];
		memset(str, 0, 1024);
		char* msg;
		fflush(stdin);
		fgets(str, 1024, stdin);
		msg = (char*)malloc(strlen(str) + strlen("Server: \n"));
		sprintf(msg, "Server: %s\n", str);
		for (int i = 0; i < nclient; i++)
		{
			send(clientsocket[i], msg, strlen(msg), 0);
		}
	}
}

int main() {
	// Khởi tạo WSA 
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// Khởi tạo socket s để lắng nghe
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888); // host to net short 
	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // host to net long 
	bind(s, (SOCKADDR*)&saddr, sizeof(saddr));
	listen(s, 10);
	
	DWORD ID = 0;
	CreateThread(NULL, 0, AcceptThread, (LPVOID)s, 0, &ID); // tạo thread để accecpt kết nối đến của client
	CreateThread(NULL, 0, InputThread, (LPVOID)s, 0, &ID);
	while (1) // vòng lặp để in ra những gì client gửi lên server 
	{	
		Sleep(100);
	}

	return 0;
}