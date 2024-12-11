#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define a struct to represent a game process
typedef struct {
    HANDLE handle;
    DWORD pid;
    HWND window;
} GameProcess;

// Define a struct to represent a game module
typedef struct {
    HMODULE handle;
    char name[256];
    DWORD baseAddress;
    DWORD size;
} GameModule;

// Define a struct to represent a game memory address
typedef struct {
    DWORD address;
    DWORD size;
    uint8_t* data;
} GameMemoryAddress;

// Function to get the current game process
GameProcess* getGameProcess(const char* name) {
    GameProcess* process = (GameProcess*)malloc(sizeof(GameProcess));
    process->handle = NULL;
    process->pid = 0;
    process->window = NULL;

    // Get the window handle
    process->window = FindWindowA(NULL, name);
    if (process->window == NULL) {
        free(process);
        return NULL;
    }

    // Get the process ID
    GetWindowThreadProcessId(process->window, &process->pid);

    // Get the process handle
    process->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process->pid);
    if (process->handle == NULL) {
        free(process);
        return NULL;
    }

    return process;
}

// Function to get a game module
GameModule* getGameModule(GameProcess* process, const char* name) {
    GameModule* module = (GameModule*)malloc(sizeof(GameModule));
    module->handle = NULL;
    strcpy(module->name, name);
    module->baseAddress = 0;
    module->size = 0;

    // Get the module handle
    module->handle = GetModuleHandleA(name);
    if (module->handle == NULL) {
        free(module);
        return NULL;
    }

    // Get the module base address and size
    module->baseAddress = (DWORD)module->handle;
    module->size = GetModuleSize(module->handle);

    return module;
}

// Function to read memory from a game process
uint8_t* readMemory(GameProcess* process, DWORD address, DWORD size) {
    uint8_t* buffer = (uint8_t*)malloc(size);
    ReadProcessMemory(process->handle, (void*)address, buffer, size, NULL);
    return buffer;
}

// Function to write memory to a game process
void writeMemory(GameProcess* process, DWORD address, uint8_t* data, DWORD size) {
    WriteProcessMemory(process->handle, (void*)address, data, size, NULL);
}

// Function to get the game's base address
DWORD getBaseAddress(GameProcess* process) {
    return (DWORD)GetModuleHandleA(NULL);
}

// Function to get the game's size
DWORD getSize(GameProcess* process) {
    return GetModuleSize(GetModuleHandleA(NULL));
}

// Function to find a pattern in the game's memory
DWORD findPattern(GameProcess* process, uint8_t* pattern, DWORD size) {
    uint8_t* buffer = readMemory(process, getBaseAddress(process), getSize(process));
    for (DWORD i = 0; i < getSize(process) - size; i++) {
        if (memcmp(buffer + i, pattern, size) == 0) {
            free(buffer);
            return getBaseAddress(process) + i;
        }
    }
    free(buffer);
    return 0;
}

// Function to get the game's window title
char* getWindowTitle(GameProcess* process) {
    char* title = (char*)malloc(256);
    GetWindowTextA(process->window, title, 256);
    return title;
}

// Function to get the game's window rect
RECT* getWindowRect(GameProcess* process) {
    RECT* rect = (RECT*)malloc(sizeof(RECT));
    GetWindowRect(process->window, rect);
    return rect;
}

// Function to get the game's window style
DWORD getWindowStyle(GameProcess* process) {
    return GetWindowLongA(process->window, GWL_STYLE);
}

// Function to get the game's window extended style
DWORD getWindowExStyle(GameProcess* process) {
    return GetWindowLongA(process->window, GWL_EXSTYLE);
}

// Function to get the game's window thread ID
DWORD getWindowThreadId(GameProcess* process) {
    DWORD threadId;
    GetWindowThreadProcessId(process->window, &threadId);
    return threadId;
}

// Function to get the game's process ID
DWORD getProcessId(GameProcess* process) {
    return process->pid;
}

// Function to get the game's handle
HANDLE getHandle(GameProcess* process) {
    return process->handle;
}

// Function to close the game process
void closeProcess(GameProcess* process) {
    CloseHandle(process->handle);
    free(process);
}

// Function to get the game's module base address
DWORD getModuleBaseAddress(GameModule* module) {
    return module->baseAddress;
}

// Function to get the game's module size
DWORD getModuleSize(GameModule* module) {
    return module->size;
}

// Function to get the game's module name
char* getModuleName(GameModule* module) {
    return module->name;
}

// Function to get the game's module handle
HMODULE getModuleHandle(GameModule* module) {
    return module->handle;
}

// Function to free a game module
void freeModule(GameModule* module) {
    free(module);
}

// Function to free a game process
void freeProcess(GameProcess* process) {
    free(process);
}

// Function to free a game memory address
void freeMemoryAddress(GameMemoryAddress* address) {
	free(address);
}

// Function for hooks
void hookFunction(GameProcess* process, DWORD address, void* hook) {
	DWORD oldProtection;
	VirtualProtectEx(process->handle, (LPVOID)address, sizeof(hook), PAGE_EXECUTE_READWRITE, &oldProtection);
	WriteProcessMemory(process->handle, (LPVOID)address, hook, sizeof(hook), NULL);
	VirtualProtectEx(process->handle, (LPVOID)address, sizeof(hook), oldProtection, &oldProtection);
}

// Functions for injecting code
void injectCode(GameProcess* process, DWORD address, void* code, DWORD size) {
	DWORD oldProtection;
	VirtualProtectEx(process->handle, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
	WriteProcessMemory(process->handle, (LPVOID)address, code, size, NULL);
	VirtualProtectEx(process->handle, (LPVOID)address, size, oldProtection, &oldProtection);
}

// Function for finding strings
void findString(GameProcess* process, char* string, int isWide) {
    uint8_t* pattern = isWide ? (uint8_t*)string : (uint8_t*)string;
    DWORD size = isWide ? wcslen((wchar_t*)string) * sizeof(wchar_t) : strlen(string);
    DWORD address = findPattern(process, pattern, size);
    if (address != 0) {
        printf("String found at address: 0x%x\n", address);
    }
}
