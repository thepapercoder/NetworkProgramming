#include <WinSock2.h>
#include <stdio.h>

SOCKET* client_socket = NULL;
int nclient = 0;
bool* login = NULL;
char** id = NULL;

char* welcome = "Yeu cau Hoa sida dang nhap!";
int main(int argc, char* argv[]) {
	// B1
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//B2 tạo socket để lắng nghe 
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//B3 khởi tạo địa chỉ
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//B3 bind socket với địa chỉ
	bind(s, (SOCKADDR*)&saddr, sizeof(SOCKADDR));

	//B5 listen
	listen(s, 10);

	fd_set fread; // một tập hợp chứa tất cả các socket mà hàm select sẽ lắng nghe

	while (true) {
		FD_ZERO(&fread);
		FD_SET(s, &fread);
		for (int i = 0; i < nclient; i++)
		{
			FD_SET(client_socket[i], &fread);
		}
		select(0, &fread, NULL, NULL, NULL);
		if (FD_ISSET(s, &fread)) {
			SOCKADDR_IN caddr;
			int clen = sizeof(caddr);
			SOCKET c = accept(s, (SOCKADDR*)&caddr, &clen);
			send(c, "Da ket noi", strlen("Da ket noi"), 0);
			send(c, welcome, strlen(welcome), 0);
			client_socket = (SOCKET*)realloc(client_socket, (nclient + 1) * sizeof(SOCKET));
			client_socket[nclient] = c;
			login = (bool*)realloc(login, (nclient + 1) * sizeof(bool));
			login[nclient] = false;
			nclient++;
		}
		for (int i = 0; i < nclient; i++)
		{
			if (FD_ISSET(client_socket[i],&fread)) {

				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				recv(client_socket[i], buffer, sizeof(buffer), 0);
				/*char* username;
				char* password;
				sscanf(buffer, "%s %s", username, password);*/
				if (login[i] == false) {
					if (strncmp(buffer, "login ", strlen("login ")) == 0)
					{
						// TODO lưu id lại vào mảng
						login[i] = true;
					}
					else 
					{
						send(client_socket[i], "dm dang nhap sai", strlen("dm dang nhap sai"), 0);
					}
				}
				else
				{
					// TODO viết hàm đảo xâu
					for (int j = 0; j < nclient; j++) {
						if (j != i) {
							send(client_socket[j], buffer, strlen(buffer), 0);
						}
					}
				}
				
			}
		}
	}

	return 0;
}