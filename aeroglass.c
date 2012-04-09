#include <windows.h>
#include <stdio.h>
#include <psapi.h>

static void changeWindowOpacity(HWND, float);
static BOOL CALLBACK onEnumWindow(HWND, LPARAM);
typedef BOOL (WINAPI *SetLayeredWindowAttributes_t) (HWND, COLORREF, BYTE, DWORD);
static SetLayeredWindowAttributes_t SetLayeredWindowAttributes;
const char *format = " %-4d  %-15s %s\n";

int main(int argc, char**argv)
{
	HMODULE User32DLL = LoadLibrary(TEXT("USER32.DLL"));

	if (!User32DLL) {
		printf("Could not load module `USER32.DLL.'\n"); 
		return 1;
	}

	SetLayeredWindowAttributes = (SetLayeredWindowAttributes_t) GetProcAddress(
		User32DLL, "SetLayeredWindowAttributes");

	printf(" PID  Image Name      Window Title\n");
	printf(" ---- --------------- ------------\n");

	if (!EnumWindows(onEnumWindow, 0)) {
		printf("Could not enumerate windows.\n");
		return 2;
	}

	FreeLibrary(User32DLL);

	return 0;
}

static BOOL CALLBACK onEnumWindow(HWND hwnd, LPARAM lparam)
{
	if (!IsWindowVisible(hwnd)) {
		return TRUE;
	}

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	char text[MAX_PATH];
	GetWindowText(hwnd, text, sizeof(text));

	HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE, pid);

	if (!process) {
		return TRUE;
	}

	char processName[MAX_PATH];
	// GetModuleBaseName or GetModuleFileNameEx
	GetModuleBaseName(process, NULL, processName, sizeof(processName));

	//changeWindowOpacity(hwnd, 1.0);

	printf(format, pid, processName, text);

	CloseHandle(process);

	return TRUE;
}

static void changeWindowOpacity(HWND hwnd, float opacity)
{
	SetWindowLong(hwnd, GWL_EXSTYLE,
		GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(128, 128, 128), 255 * opacity, 0x02);
}
