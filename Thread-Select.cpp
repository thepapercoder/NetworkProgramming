// Thread-Select.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
int nthread = 0;	//So luong thread da tao ra
int nsocket = 0;	//So luong socket da ket noi
bool* login = NULL;	//Trang thai dang nhap cua cac socket
char** id = NULL;	//Ten cua socket (client) sau khi da dang nhap
int MAX_SELECT = 2;	//So luong socket toi da co the tham do - select
SOCKET* c = NULL;	//Mang chua cac socket da ket noi


DWORD WINAPI ClientThread(LPVOID arg)
{
	int tid = (int)arg;
	int start_index = tid * MAX_SELECT;
	int stop_index = start_index + MAX_SELECT - 1;
	printf("Luong so: %d da duoc tao gom cac socket: ", tid);
	for (int i = start_index; i <= stop_index; i++)
	{
		printf("%d ", c[i]);
	}
	printf("\n");

	fd_set fdread;
	while (true)
	{
		FD_ZERO(&fdread);
		for (int i = start_index; i <= stop_index; i++)
		{
			FD_SET(c[i], &fdread);
		}
		select(0, &fdread, NULL, NULL, NULL);
		for (int i = start_index; i <= stop_index; i++)
		{
			if (FD_ISSET(c[i], &fdread))
			{
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				recv(c[i], buffer, sizeof(buffer), 0);
				printf("Luong so: %d - ", tid);
				printf("%s\n", buffer);

				if (login[i] == false)
				{
					if (strncmp(buffer, "client_id: ", strlen("client_id: ")) == 0)
					{
						//Luu lai trang thai login va ten cua client vao bien id
						login[i] = true;
						id[i] = (char*)calloc(1024, 1);
						strcpy(id[i], buffer + strlen("client_id: "));
					}
					else
					{
						char* invalid = "Sai cu phap dang nhap\n";
						send(c[i], invalid, strlen(invalid), 0);
					}
				}
				else //Socket c[i] la login
				{
					char data[1024];
					memset(data, 0, sizeof(data));
					sprintf(data, "%s: %s\n", id[i], buffer);
					//Gui cho tat ca cac socket da dang nhap
					for (int j = 0; j < nsocket; j++)
					{
						if (login[j] == true && c[j] != c[i])
						{
							send(c[j], data, strlen(data), 0);
						}
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	WSADATA DATA;
	WSAStartup(MAKEWORD(2, 2), &DATA);

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons((short)atoi(argv[1]));
	saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(s, (sockaddr*)&saddr, sizeof(saddr));
	listen(s, 10);

	fd_set fdread;

	while (true)
	{
		FD_ZERO(&fdread);
		FD_SET(s, &fdread);
		for (int i = nthread * MAX_SELECT; i < nsocket; i++)
		{
			FD_SET(c[i], &fdread);
		}
		select(0, &fdread, NULL, NULL, NULL);
		
		if (FD_ISSET(s, &fdread))  //Co ket noi moi can accept
		{
			SOCKADDR_IN caddr;
			int clen = sizeof(caddr);
			SOCKET tmpc = accept(s, (sockaddr*)&caddr, &clen);
			c = (SOCKET*)realloc(c, (nsocket + 1) * sizeof(SOCKET));
			login = (bool*)realloc(login, (nsocket + 1) * sizeof(bool));
			id = (char**)realloc(id, (nsocket + 1) * sizeof(char*));
			c[nsocket] = tmpc;
			login[nsocket] = false;
			nsocket += 1;
			if (nsocket % MAX_SELECT == 0) 
			{
				//So luong la boi so cua MAX_SELECT thi tach luong
				DWORD ID = 0;
				CreateThread(NULL, 0, ClientThread, (LPVOID)nthread, 0, &ID);
				nthread += 1;
			}
		}

		for (int i = nthread * MAX_SELECT; i < nsocket; i++)
		{
			if (FD_ISSET(c[i], &fdread))
			{
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				recv(c[i], buffer, sizeof(buffer), 0);
				printf("Luong main - ");
				printf("%s\n", buffer);
				if (login[i] == false)
				{
					if (strncmp(buffer, "client_id: ", strlen("client_id: ")) == 0)
					{
						//Luu lai trang thai login va ten cua client vao bien id
						login[i] = true;
						id[i] = (char*)calloc(1024, 1);
						strcpy(id[i], buffer + strlen("client_id: "));
					}
					else
					{
						char* invalid = "Sai cu phap dang nhap\n";
						send(c[i], invalid, strlen(invalid), 0);
					}
				}
				else //Socket c[i] la login
				{
					char data[1024];
					memset(data, 0, sizeof(data));
					sprintf(data, "%s: %s\n", id[i], buffer);
					//Gui cho tat ca cac socket da dang nhap
					for (int j = 0; j < nsocket; j++)
					{
						if (login[j] == true && c[j] != c[i])
						{
							send(c[j], data, strlen(data), 0);
						}
					}
				}
			}
		}
	}

	return 0;
}

