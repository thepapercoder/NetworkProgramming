//#include <stdio.h>
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//
//SOCKET* clientsocket = NULL; // chứa client socket kết nối đến
//int nclient = 0; // số client đã kết nối đến
//
//// hàm giải mã dữ liệu từ client
//char* decode(char* str) {
//	/*for (; *str; ++str) *str = tolower(*str);*/
//	for (int i = 0; i < strlen(str); i++) {
//		if (str[i] >= 65 && str[i] <= 90) // xét các chữ hoa 
//		{
//			str[i] = 90 - (str[i] - 65);
//		}
//		if (str[i] >= 97 && str[i] < 123) // xét chữ thường
//		{
//			str[i] = 122 - (str[i] - 97);
//		}
//		if (str[i] >= 48 && str[i] <= 57) // xét số
//		{
//			str[i] = 57 - (str[i] - 48);
//		}
//	}
//	return str;
//}
//
//int main() {
//	// Khởi tạo WSA 
//	WSAData wsa;
//	WSAStartup(MAKEWORD(2, 2), &wsa);
//
//	// Khởi tạo socket s để lắng nghe
//	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	SOCKADDR_IN saddr;
//	saddr.sin_family = AF_INET;
//	saddr.sin_port = htons(8888); // host to net short 
//	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // host to net long 
//
//	bind(s, (SOCKADDR*)&saddr, sizeof(saddr));
//
//	listen(s, 10);
//
//	fd_set fread; 
//	char* buffer;
//	buffer = (char*)malloc(1024);
//	while (1) {
//		FD_ZERO(&fread);	// xóa trắng tập fread
//		FD_SET(s, &fread);  // đưa s vào tập fread
//		for (int i = 0; i < nclient; i++) {
//			FD_SET(clientsocket[i], &fread); // đưa tất cả client socket đã kết nối đến vào fread để thăm dò
//		}
//		select(0, &fread, NULL, NULL, NULL);
//		// đã đi qua được select => nó đã có đủ thông tin cho accept hoặc recv
//		if (FD_ISSET(s, &fread)) {
//			SOCKADDR_IN caddr;
//			int clen = sizeof(caddr);
//			SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
//			clientsocket = (SOCKET*)realloc(clientsocket, sizeof(SOCKET)*(nclient + 1));
//			clientsocket[nclient] = c;
//			nclient++;
//		}
//		for (int i = 0; i < nclient; i++) {
//			if (FD_ISSET(clientsocket[i], &fread)) {
//				memset(buffer, 0, 1024);
//				recv(clientsocket[i], buffer, 1024, 0);
//				buffer = decode(buffer);
//				printf("%d %s", i, buffer);
//				for (int j = 0; j < nclient; j++) {
//					if (clientsocket[j] != clientsocket[i]) {
//						char tmp[1024];
//						memset(tmp, 0, 1024);
//						sprintf(tmp, "Tin nhan gui tu %d: %s \n \0", i, buffer);
//						send(clientsocket[j], tmp, strlen(tmp), 0);
//					}
//				}
//			}
//		}
//	}
//
//	return 0;
//}