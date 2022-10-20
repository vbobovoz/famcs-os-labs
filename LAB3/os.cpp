#include <windows.h>
#include <iostream>
using namespace std;

CRITICAL_SECTION cs;
HANDLE hOutEvent, hMultEvent;
char* res;
int mult_res = 1;
int k = 0;

struct Information {
	int size;
	char* arr;
	int k;
};

DWORD WINAPI work(void* obj) {
	Information* info = (Information*)obj;
	char* f_arr = info->arr;

	cout << "--- Thread work is started" << endl;
	int time;
	cout << "  Enter time to sleep: ";
	cin >> time;

	int t = 0;
	int ind = 0;
	res = new char[info->size];
	//char* res = new char[info->size];
	for (int i = 0; i < info->size; ++i) {
		if (isdigit(f_arr[i])) {
			int t = int(f_arr[i]) - 48;
			if (t % 2 == 0) {
				res[ind] = f_arr[i];
				ind++;
			}
		}
		Sleep(time);
	}
	for (int i = 0; i < info->size; ++i) {
		if (isdigit(f_arr[i])) {
			int t = int(f_arr[i]) - 48;
			if (t % 2 == 1) {
				res[ind] = f_arr[i];
				ind++;
			}
		}
		Sleep(time);
	}

	cout << "  Result array: ";
	for (int i = 0; i < info->size; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
	cout << "--- Thread work is finished" << endl << endl;
	SetEvent(hOutEvent);
	return 0;
}

DWORD WINAPI MultElement(void* obj) {
	EnterCriticalSection(&cs);
	WaitForSingleObject(hMultEvent, INFINITE);
	cout << "--- Thread MultElement is started" << endl;
	cout << "  ";

	Information* info = (Information*)obj;

	for (int i = 0; i < info->k; ++i) {
		if (isdigit(res[i])) {
			if (res[i] != 0) {
				int t = int(res[i]) - 48;
				mult_res *= t;
			}
		}
	}
	cout << "  Mult: " << mult_res << endl;
	LeaveCriticalSection(&cs);
	cout << "--- Thread MultElement is finished" << endl;
	return 0;
}

int main() {
	// INPUT DATA
	int size;
	cout << "Enter size of array: " << endl << "  ";
	cin >> size;
	cout << endl << "Enter elements of array: " << endl << "  ";
	char* arr = new char[size];
	for (int i = 0; i < size; ++i) {
		cin >> arr[i];
	}

	// PRINT INPUT DATA
	cout << endl << "-----------------------------------" << endl << endl;
	cout << "Input data: " << endl;
	cout << "  Size: " << size << endl;
	cout << "  Elements of array: ";
	for (int i = 0; i < size; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl;

	// INFORMATION
	Information* info = new Information();
	info->arr = arr;
	info->size = size;

	// INITIALIZING
	InitializeCriticalSection(&cs);
	
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hMultEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	HANDLE hThreadWork;
	DWORD IDThreadWork;
	DWORD dwCountWork;
	hThreadWork = CreateThread(NULL, 0, work, (void*)info, CREATE_SUSPENDED, &IDThreadWork);

	HANDLE hThreadMult;
	DWORD IDThreadMult;
	DWORD dwCountMult;
	hThreadMult = CreateThread(NULL, 0, MultElement, (void*)info, NULL, &IDThreadMult);
	
	cout << endl << "Enter k: ";
	cin >> k;
	info->k = k;
	cout << endl;

	dwCountWork = ResumeThread(hThreadWork);
	WaitForSingleObject(hOutEvent, INFINITE);

	cout << "Elements of array: ";
	for (int i = 0; i < info->size; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
	cout << endl;

	SetEvent(hMultEvent);
	EnterCriticalSection(&cs);

	cout << endl << "Result of MultElement: " << mult_res << endl;

	DeleteCriticalSection(&cs);
	WaitForSingleObject(hThreadWork, INFINITE);
	WaitForSingleObject(hThreadMult, INFINITE);
	CloseHandle(hThreadWork);
	CloseHandle(hThreadMult);

	//WaitForSingleObject(hThreadMult, INFINITE);
	//CloseHandle(hThreadMult);
	return 0;
}