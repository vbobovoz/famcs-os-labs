#include <iostream>
#include <windows.h>
#include <process.h>
using namespace std;

struct data_info {
	int size;
	int* arr;
};

UINT WINAPI worker(void* input_data) {
	cout << "Thread start to work!" << endl;
	int sum = 0;
	data_info* tmp = (data_info*)input_data;
	for (int i = 0; i < tmp->size; ++i) {
		sum += tmp->arr[i];
		Sleep(20);
	}
	cout << "Sum of elements: " << sum << endl;
	cout << "Thread stop to work!" << endl;
	return 0;
}

int main() {
	int size;
	cout << "Input size of the array: ";
	cin >> size;

	int* arr = new int[size];
	cout << "Input the elements of the array: ";
	for (int i = 0; i < size; ++i) {
		cin >> arr[i];
	}

	int stop_time, start_time;
	cout << "Input stop time: ";
	cin >> stop_time;
	cout << "Input start time: ";
	cin >> start_time;
	cout << endl;

	HANDLE work;
	data_info* arr_t = new data_info();
	arr_t->size = size;
	arr_t->arr = arr;
	work = (HANDLE) _beginthreadex(NULL, 0, worker, (void*)arr_t, 0, NULL);
	SuspendThread(work);
	cout << "-Work stoped!" << endl;
	Sleep(stop_time);

	ResumeThread(work);
	cout << "+Work started!" << endl;

	WaitForSingleObject(work, INFINITE);
	CloseHandle(work);
	return 0;
}