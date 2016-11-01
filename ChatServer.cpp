//#include <WinSock2.h>
//#include <stdio.h>
//
//SOCKET* client_socket = NULL;
//int nclient = 0;
//
//DWORD WINAPI ClientThread(LPVOID arg) {
//	SOCKET c = (SOCKET)arg;
//	while (1) {
//		char buffer[1024];
//		memset(buffer, 0, sizeof(buffer));
//		recv(c, buffer, sizeof(buffer), 0);
//
//		// TODO viết hàm đảo xâu
//
//		for (int i = 0; i < nclient; i++) {
//			if (client_socket[i] != c) {
//				send(client_socket[i], buffer, strlen(buffer), 0);
//			}
//		}
//	}
//}
//
//int main(int argc, char* argv[]) {
//	// B1
//	WSADATA wsa;
//	WSAStartup(MAKEWORD(2, 2), &wsa);
//
//	//B2 tạo socket để lắng nghe 
//	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	
//	//B3 khởi tạo địa chỉ
//	SOCKADDR_IN saddr;
//	saddr.sin_family = AF_INET;
//	saddr.sin_port = htons(8888);
//	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//
//	//B3 bind socket với địa chỉ
//	bind(s, (SOCKADDR*)&saddr, sizeof(SOCKADDR));
//
//	//B5 listen
//	listen(s, 10);
//
//	while (true) {
//		SOCKADDR_IN caddr;
//		int clen = sizeof(caddr);
//		SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
//		send(c, "Da ket noi", strlen("Da ket noi"), 0);
//		client_socket = (SOCKET*)realloc(client_socket, (nclient + 1) * sizeof(SOCKET));
//		client_socket[nclient] = c;
//		nclient++;
//		DWORD ID = 0;
//		CreateThread(0, NULL, ClientThread, (LPVOID)c, 0, &ID);
//	}
//
//
//	return 0;
//}