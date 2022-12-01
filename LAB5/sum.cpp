#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
using namespace std;

void main(int argc, char** argv) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);
	int size = atoi(argv[3]);
	cout << size << "\n";
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
	if (!hEvent) {
		cout << "-Sum-\n event was not read";
	}

	double* info = new double[size + 2];

	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, info, sizeof(double) * (size + 2), &dwBytesRead, NULL)) {
		cout << "-Sum-\n info was not read";
	}

	double* arr = new double[info[1]];
	int n = (int)info[0];
	
	vector<double> v;
	double total = 0, count = 0;
	cout << "-Sum-\n";
	for (int i = 2; i < size + 2; ++i) {
		if (info[i] > n) {
			cout << info[i] << " ";
			v.push_back(info[i] * info[i]);
			total += info[i] * info[i];
			count++;
		}
	}

	double* res = new double[v.size() + 1];
	res[0] = total;
	res[1] = count;
	for (int i = 2; i < v.size() + 2; ++i) {
		res[i] = v[i - 2];
	}

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, res, sizeof(double) * (v.size() + 2), &dwBytesWritten, NULL)) {
		cout << "-Sum-\n res was not written";
	}

	SetEvent(hEvent);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	while(_getch() != '0'){}
}