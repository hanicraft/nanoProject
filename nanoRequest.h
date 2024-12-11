#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

typedef struct {
    SOCKET sock;
    struct sockaddr_in serverAddr;
} Socket;

int initWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Socket* createSocket() {
    Socket* sock = malloc(sizeof(Socket));
    if (sock == NULL) {
        printf("Failed to allocate memory for socket structure.\n");
        return NULL;
    }
    sock->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock->sock == INVALID_SOCKET) {
        printf("Failed to create socket: %d\n", WSAGetLastError());
        free(sock);
        return NULL;
    }
    return sock;
}

int bindSocket(Socket* sock, const char* address, int port) {
    sock->serverAddr.sin_family = AF_INET;
    sock->serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, address, &sock->serverAddr.sin_addr);

    if (bind(sock->sock, (struct sockaddr*)&sock->serverAddr, sizeof(sock->serverAddr)) == SOCKET_ERROR) {
        printf("Failed to bind socket: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int listenSocket(Socket* sock) {
    if (listen(sock->sock, 3) == SOCKET_ERROR) {
        printf("Failed to listen on socket: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

Socket* acceptConnection(Socket* sock) {
    Socket* newSock = malloc(sizeof(Socket));
    if (newSock == NULL) {
        printf("Failed to allocate memory for new socket structure.\n");
        return NULL;
    }
    newSock->sock = accept(sock->sock, NULL, NULL);
    if (newSock->sock == INVALID_SOCKET) {
        printf("Failed to accept connection: %d\n", WSAGetLastError());
        free(newSock);
        return NULL;
    }
    return newSock;
}

int sendData(Socket* sock, const char* message) {
    int bytesSent = send(sock->sock, message, strlen(message), 0);
    if (bytesSent == SOCKET_ERROR) {
        printf("Failed to send data: %d\n", WSAGetLastError());
        return -1;
    }
    return bytesSent;
}

int receiveData(Socket* sock, char* buffer, int bufferSize) {
    int bytesReceived = recv(sock->sock, buffer, bufferSize, 0);
    if (bytesReceived == SOCKET_ERROR) {
        printf("Failed to receive data: %d\n", WSAGetLastError());
        return -1;
    }

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
    }
    return bytesReceived;
}

void closeSocket(Socket* sock) {
    closesocket(sock->sock);
    free(sock);
}

void cleanupWinsock() {
    WSACleanup();
}

typedef struct {
	char* host;
	char* path;
	char* response;
	int response_len;
} http_request;

http_request* makeRequest(char* host, char* path) {
	http_request* req = malloc(sizeof(http_request));
	req->host = host;
	req->path = path;
	req->response = NULL;
	req->response_len = 0;
	return req;
}

void httpRequestFree(http_request* req) {
	free(req->host);
	free(req->path);
	free(req->response);
	free(req);
}

int httpSendRequest(http_request* req) {
    Socket* sock = createSocket();
    if (sock == NULL) {
        printf("Failed to create socket\n");
        return;
    }

    if (connect(sock->sock, req->host, req->path) != 0) {
        printf("Failed to connect to server: %d\n", WSAGetLastError());
        closeSocket(sock);
        return;
    }

    char* request = malloc(strlen(req->host) + strlen(req->path) + 20);
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", req->path, req->host);
    int bytesSent = sendData(sock, request);
    free(request);
    if (bytesSent == -1) {
        printf("Failed to send request: %d\n", WSAGetLastError());
        closeSocket(sock);
        return;
    }

    char buffer[1024];
    int bytesReceived = receiveData(sock, buffer, 1024);
    if (bytesReceived == -1) {
        printf("Failed to receive response: %d\n", WSAGetLastError());
        closeSocket(sock);
        return;
    }

    req->response = malloc(bytesReceived + 1);
    strcpy(req->response, buffer);
    req->response_len = bytesReceived;

    closeSocket(sock);
}