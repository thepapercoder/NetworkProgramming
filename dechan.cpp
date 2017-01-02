#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

SOCKET* clientsocket = NULL; // chứa client socket kết nối đến
int nclient = 0; // số client đã kết nối đến
char** clientname = NULL;

DWORD WINAPI ClientThread(LPVOID arg) {
	int check = (int)arg;
	
	fd_set fread;
	char* buffer;
	buffer = (char*)malloc(1024);
	while (1) {
		FD_ZERO(&fread);	// xóa trắng tập fread
		for (int i = 0; i < nclient; i++) {
			if(i%2 == check)
				FD_SET(clientsocket[i], &fread); // đưa tất cả client socket đã kết nối đến vào fread để thăm dò
		}
		select(0, &fread, NULL, NULL, NULL);
		// đã đi qua được select => nó đã có đủ thông tin cho accept hoặc recv
		for (int i = 0; i < nclient; i++) {
			if (FD_ISSET(clientsocket[i], &fread) && (i % 2 == check)) {
				memset(buffer, 0, 1024);
				recv(clientsocket[i], buffer, 1024, 0);
				if (clientname[i] == NULL) {
					if (strncmp(buffer, "Client: ", strlen("Client: ")) == 0) {
						char name[1024];
						memset(name, 0, 1024);
						strcpy(name, buffer + strlen("Client: "));
						while (name[strlen(name) - 1] == '\n' || name[strlen(name) - 1] == '\r') {
							name[strlen(name) - 1] = '\0';
						}
						clientname[i] = (char*)calloc(1024, 1);
						strcpy(clientname[i], name);
					}
					else {
						send(clientsocket[i], "Loi", strlen("Loi"), 0);
					}
				}
				else {
					char data[1024];
					char gname[1024];
					char msg[1024];
					memset(data, 0, sizeof(data));
					memset(gname, 0, sizeof(gname));
					memset(msg, 0, sizeof(msg));
					sscanf(buffer, "%s %s", gname, msg);
					sprintf(data, "%s: %s", clientname[i], buffer + strlen(gname));
					//Gui cho tat ca cac socket da dang nhap
					for (int j = 0; j < nclient; j++)
					{
						if ((i != j) && strcmp(gname, clientname[j]) == 0) {
							send(clientsocket[j], data, strlen(data), 0);
						}
					}
				}
			}
		}
	}
}

int main() {
	char* welcomemsg = "Dang nhap bang cu phap: Client: Ten \n";

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

	int thread1 = 1;
	int thread2 = 0;
	DWORD ID = 0;
	CreateThread(NULL, 0, ClientThread, (LPVOID)thread1, 0, &ID);
	CreateThread(NULL, 0, ClientThread, (LPVOID)thread2, 0, &ID);

	fd_set fread; 
	char* buffer;
	buffer = (char*)malloc(1024);
	FD_ZERO(&fread);	// xóa trắng tập fread
	FD_SET(s, &fread);  // đưa s vào tập fread
	while (1) {		
		select(0, &fread, NULL, NULL, NULL);
		// đã đi qua được select => nó đã có đủ thông tin cho accept hoặc recv
		if (FD_ISSET(s, &fread)) {
			SOCKADDR_IN caddr;
			int clen = sizeof(caddr);
			SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
			clientsocket = (SOCKET*)realloc(clientsocket, sizeof(SOCKET)*(nclient + 1));
			clientsocket[nclient] = c;
			clientname = (char**)realloc(clientname, sizeof(char*)*(nclient + 1));
			clientname[nclient] = NULL;
			nclient++;
			send(c, welcomemsg, strlen(welcomemsg), 0);
		}
	}

	return 0;
}