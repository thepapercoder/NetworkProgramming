//Buổi 1
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//#include <stdio.h>
//
//int main() {
//	WSADATA		wsaData;
//	SOCKET		s;
//	SOCKADDR_IN serverAddr;
//	int			port = 2323;
//	char* http_request_vnx = "GET / HTTP/1.1\r\n";
//	char http_request[1024];
//	addrinfo	hints, *result;
//
//	// Nhập tên miền
//	char domainame[1024];
//	ZeroMemory(domainame, 1024);
//	printf("Nhap vao ten mien: \n");
//	fflush(stdin);
//	fgets(domainame, 1024, stdin);
//	// Loại bỏ dấu xuống dòng
//	domainame[strlen(domainame) - 1] = '\0';
//	// Loại bỏ www.
//	if (domainame[0] == 'w' && domainame[1] == 'w' && domainame[2] == 'w' && domainame[3] == '.') {
//		char* tmp = (char*) malloc(sizeof(domainame) - 4);
//		ZeroMemory(tmp, sizeof(domainame) - 4);
//		sscanf(domainame+4, "%s", tmp);
//		ZeroMemory(domainame, 1024);
//		strcpy(domainame, tmp);
//	}
//	ZeroMemory(http_request, 1024);
//	sprintf(http_request, "%s%s%s%s", http_request_vnx, "Host: ", domainame, "\r\n\r\n");
//	printf(http_request);
//
//	// Khởi tạo winsock
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		printf("Failed. Error Code : %d", WSAGetLastError());
//		return 1;
//	};
//	// Khởi tạo socket
//	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
//		printf("Could not create socket : %d", WSAGetLastError());
//	};
//	// Khởi tại cấu trúc hints
//	memset(&hints, 0, sizeof(addrinfo));
//	hints.ai_family = AF_INET;
//	hints.ai_flags = AI_CANONNAME;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	// Truy vấn thông tin tên miền
//	int rc = 0;
//	rc = getaddrinfo(domainame, "80", &hints, &result);
//	if (rc != 0) {
//		printf("getaddrinfo failed : %d", WSAGetLastError());
//		return 1;
//	}
//	if (connect(s, result->ai_addr, sizeof(SOCKADDR)) < 0) {
//		puts("connect error");
//		return 1;
//	}
//	printf("connected \n");
//	// Gửi chuỗi helloo
//	int check = 0;
//	check = send(s, http_request, strlen(http_request), 0);
//	if (check == 0) {
//		printf("Error!!!");
//		return 1;
//	}
//	int recv_size = 0;
//	char server_reply[1001];
//	while ((recv_size = recv(s, server_reply, 1000, 0)) != 0) {
//		server_reply[recv_size] = '\0';
//		printf("%s", server_reply);
//	}
//	printf("Done and closing connection");
//
//	// Đóng kết nối
//	shutdown(s, SD_BOTH);
//	closesocket(s);
//
//	// Giải phóng socket
//	WSACleanup();
//
//	return 0;
//}