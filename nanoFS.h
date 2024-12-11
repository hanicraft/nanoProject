#include <Windows.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
// File system code
// note use functions that start with Unix_ for Unix and Win_ for Windows

void Unix_CreateDirectory(const char* path) {
	mkdir(path, 0755);
}

void Win_CreateDirectory(const char* path) {
	CreateDirectoryA(path, NULL);
}

void Unix_DeleteDirectory(const char* path) {
	rmdir(path, 0755);
}

void Win_DeleteDirectory(const char* path) {
	RemoveDirectoryA(path);
}

void Unix_CreateFile(const char* path) {
	FILE* file = fopen(path, "w");
	fclose(file);
}

void Win_CreateFile(const char* path) {
	CreateFileA(path, NULL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
}

void Unix_DeleteFile(const char* path) {
	remove(path);
}

void Win_DeleteFile(const char* path) {
	DeleteFileA(path);
}

void Unix_RenameFile(const char* oldPath, const char* newPath) {
	rename(oldPath, newPath);
}

void Win_RenameFile(const char* oldPath, const char* newPath) {
	rename(oldPath, newPath);
}

void Unix_CopyFile(const char* sourcePath, const char* destinationPath) {
	FILE* sourceFile = fopen(sourcePath, "rb");
	FILE* destinationFile = fopen(destinationPath, "wb");

	char buffer[4096]; // Larger buffer for better performance
	size_t bytesRead;
	while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
		fwrite(buffer, 1, bytesRead, destinationFile);
	}
	fclose(sourceFile);
	fclose(destinationFile);
}

void Win_CopyFile(const char* sourcePath, const char* destinationPath) {
	CopyFileA(sourcePath, destinationPath, FALSE);
}

void Unix_MoveFile(const char* sourcePath, const char* destinationPath) {
	FILE* sourceFile = fopen(sourcePath, "r");
	FILE* destinationFile = fopen(destinationPath, "w");
	char buffer[1024];
	while (fread(buffer, 1, sizeof(buffer), sourceFile) > 0) {
		fwrite(buffer, 1, sizeof(buffer), destinationFile);
	}
	fclose(sourceFile);
	fclose(destinationFile);
	remove(sourcePath);
}

void Win_MoveFile(const char* sourcePath, const char* destinationPath) {
	MoveFileA(sourcePath, destinationPath);
}

void Unix_GetFileAttributes(const char* path, struct stat* attributes) {
	stat(path, attributes);
}

void Win_GetFileAttributes(const char* path, struct stat* attributes) {
	GetFileAttributesExA(path, GetFileExInfoStandard, attributes);
}

void Unix_SetFileAttributes(const char* path, struct stat attributes) {
	chmod(path, attributes.st_mode);
}

void Win_SetFileAttributes(const char* path, struct stat attributes) {
	SetFileAttributesA(path, attributes.st_mode);
}

void Unix_GetDirectoryContents(const char* path, struct dirent** entries) {
	char* directory = opendir(path);
	struct dirent* entry;
	int i = 0;
	while ((entry = readdir(directory)) != NULL) {
		entries[i] = entry;
		i++;
	}
	closedir(directory);
}

void Win_GetDirectoryContents(const char* path, struct dirent** entries) {
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFileA(path, &findData);
	int i = 0;
	while (hFind != INVALID_HANDLE_VALUE) {
		entries[i] = &findData;
		i++;
		hFind = FindNextFileA(hFind, &findData);
	}
	FindClose(hFind);
}

void Unix_GetFileSize(const char* path, off_t* size) {
	struct stat attributes;
	stat(path, &attributes);
	*size = attributes.st_size;
}

void Win_GetFileSize(const char* path, off_t* size) {
	struct _stat attributes;
	_stat(path, &attributes);
	*size = attributes.st_size;
}

#include <stdio.h>
#include <stdlib.h>

// Read Data
void ReadDataFromFile(const char* path, char* buffer, int bufferSize) {
	FILE* file = fopen(path, "rb");
	fread(buffer, 1, bufferSize, file);
	fclose(file);
}

// Write Data
void WriteDataToFile(const char* path, char* buffer, int bufferSize) {
	FILE* file = fopen(path, "wb");
	fwrite(buffer, 1, bufferSize, file);
	fclose(file);
}

// Seek
void SeekFile(const char* path, int offset, int origin) {
	FILE* file = fopen(path, "rb+");
	fseek(file, offset, origin);
	fclose(file);
}
// Tell
void TellFile(const char* path) {
	FILE* file = fopen(path, "rb+");
	fseek(file, 0, SEEK_CUR);
	fclose(file);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Encryption code
void Encrypt(char* message, int key) {
	int length = strlen(message);
	for (int i = 0; i < length; i++) {
		message[i] = message[i] + key;
	}
}
// Decryption code
void Decrypt(char* message, int key) {
	int length = strlen(message);
	for (int i = 0; i < length; i++) {
		message[i] = message[i] - key;
	}
}
// Encryption of file
void EncryptFile(const char* filePath, int key) {
	FILE* file = fopen(filePath, "r+");
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* message = (char*)malloc(fileSize + 1);
	fread(message, 1, fileSize, file);
	fclose(file);
	Encrypt(message, key);
	file = fopen(filePath, "w+");
	fwrite(message, 1, fileSize, file);
	fclose(file);
	free(message);
}
// Decryption of file
void DecryptFile(const char* filePath, int key) {
	FILE* file = fopen(filePath, "r+");
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* message = (char*)malloc(fileSize + 1);
	fread(message, 1, fileSize, file);
	fclose(file);
	Decrypt(message, key);
	file = fopen(filePath, "w+");
	fwrite(message, 1, fileSize, file);
	fclose(file);
	free(message);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void Compress(const char* input, char* output, int* outputLength) {
    int inputLength = strlen(input);
    int outputIndex = 0;

    for (int i = 0; i < inputLength; i++) {
        int count = 1;

        // Count the number of consecutive repeating characters
        while (i + 1 < inputLength && input[i] == input[i + 1]) {
            count++;
            i++;
        }

        // Append the character and its count to the output
        output[outputIndex++] = input[i];
        outputIndex += snprintf(&output[outputIndex], *outputLength - outputIndex, "%d", count);
    }

    output[outputIndex] = '\0'; // Null-terminate the output string
    *outputLength = outputIndex; // Update the output length
}

void CompressFile(const char* inputFilePath, const char* outputFilePath) {
    FILE* inputFile = fopen(inputFilePath, "r");
    FILE* outputFile = fopen(outputFilePath, "w");

    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    char buffer[1024]; // Adjust the buffer size as needed
    char compressedBuffer[2048]; // Ensure this is large enough for compressed data
    int bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
        int compressedLength = sizeof(compressedBuffer);
        Compress(buffer, compressedBuffer, &compressedLength);
        fwrite(compressedBuffer, 1, compressedLength, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

void Decompress(const char* input, char* output, int* outputLength) {
    int inputLength = strlen(input);
    int outputIndex = 0;

    for (int i = 0; i < inputLength; i++) {
        if (isalpha(input[i])) {
            char currentChar = input[i];
            i++;

            // Extract the count
            int count = 0;
            while (i < inputLength && isdigit(input[i])) {
                count = count * 10 + (input[i] - '0');
                i++;
            }
            i--; // Move back one step to avoid skipping the next character

            // Append the character 'count' times to the output
            for (int j = 0; j < count; j++) {
                output[outputIndex++] = currentChar;
            }
        }
    }

    output[outputIndex] = '\0'; // Null-terminate the output string
    *outputLength = outputIndex; // Update the output length
}

void DecompressFile(const char* inputFilePath, const char* outputFilePath) {
    FILE* inputFile = fopen(inputFilePath, "r");
    FILE* outputFile = fopen(outputFilePath, "w");

    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    char buffer[1024]; // Adjust the buffer size as needed
    char decompressedBuffer[2048]; // Ensure this is large enough for decompressed data
    int bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
        int decompressedLength = sizeof(decompressedBuffer);
        Decompress(buffer, decompressedBuffer, &decompressedLength);
        fwrite(decompressedBuffer, 1, decompressedLength, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}