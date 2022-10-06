#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
using namespace std;

int main() {
	int size;
	cout << "Enter size of array: ";
	cin >> size;
	cout << "Enter " << size << " elements of array: ";
	int* array = new int[size];
	for (int i = 0; i < size; ++i) {
		cin >> array[i];
	}

	wstring cmd_info = L"\"child.exe\" ";
	cmd_info += to_wstring(size);
	cmd_info += L" ";
	for (int i = 0; i < size; i++) {
		cmd_info += to_wstring(array[i]);
		if (i != size - 1) {
			cmd_info += L" ";
		}
	}

	wchar_t* stemp = new wchar_t[cmd_info.length()];
	wcscpy_s(stemp, cmd_info.length() + 1, cmd_info.c_str());

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USECOUNTCHARS;
	si.dwYCountChars = 1000;

	if (!CreateProcess(nullptr, stemp, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &piApp)) {
		cout << "New process is not created" << endl;
		return 0;
	}

	cout << "New process is created" << endl;
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[]array;
	return 0;
}