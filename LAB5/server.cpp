#include <iostream>
#include <windows.h>
using namespace std;

void main(int argc, char** argv) {
	srand(time(0));
	int size;
	cout << "-Server-\n Enter array size: ";
	cin >> size;
	double* arr = new double[size];
	for (int i = 0; i < size; ++i) {
		arr[i] = rand() % 100 + 1;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe, hEvent;
	SECURITY_ATTRIBUTES sa;

	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
	if (!hEvent) {
		cout << "-Server-\n Event was not created";
		return;
	}

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		cout << "-Server-\n Pipe was not created";
		return;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	wchar_t* wstr = new wchar_t[80];
	wsprintf(wstr, L"\"sum.exe\" %d %d %d", (int)hWritePipe, (int)hReadPipe, (int)size);

	if (!CreateProcess(nullptr, wstr, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, FALSE, FALSE, &si, &pi)) {
		cout << "-Server-\n sum creation failed";
		return;
	}

	double n;
	cout << "-Server-\n Enter n: ";
	cin >> n;
	double* info = new double[size + 2];
	info[0] = n; info[1] = (double)size;
	for (int i = 2; i < size + 2; ++i) {
		info[i] = arr[i - 2];
	}

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, info, sizeof(double) * (size + 2), &dwBytesWritten, nullptr)) {
		cout << "-Server-\n info was not written";
		return;
	}

	WaitForSingleObject(hEvent, INFINITE);

	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, arr, sizeof(double) * (size + 2), &dwBytesRead, nullptr)) {
		cout << "-Server-\n arr was not read";
		return;
	}

	cout << "-Server-\n";
	cout << " n: " << info[0] << "  size: " << info[1] << "\n";
	cout << " Random numbers: ";
	for (int i = 0; i < info[1]; ++i) {
		cout << info[i + 2] << " ";
	} cout << "\n\n";

	cout << "-Sum-\n";
	cout << " sum: " << arr[0] << "\n";
	cout << " elements: ";
	for (int i = 2; i < arr[1] + 2; ++i) {
		cout << arr[i] << " ";
	} cout << "\n";

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);
}