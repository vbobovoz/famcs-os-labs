#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std;

mutex critical_section;
mutex hOutEvent, hMultEvent;
condition_variable_any cv;

vector<char> res;
int mult_res = 1;
int k = 0;

void Work(vector<char>& arr, int size, int& k) {
	cout << "--- Thread work is started" << endl;
	int time;
	cout << "  Enter time to sleep: ";
	cin >> time;

	int t = 0;
	res = vector<char>(size);
	int ind = 0;
	for (int i = 0; i < size; ++i) {
		if (isdigit(arr[i])) {
			int t = int(arr[i]) - 48;
			if (t % 2 == 0) {
				res[ind] = arr[i];
				ind++;
			}
		}
		this_thread::sleep_for(chrono::milliseconds(time));
	}
	for (int i = 0; i < size; ++i) {
		if (isdigit(arr[i])) {
			int t = int(arr[i]) - 48;
			if (t % 2 == 1) {
				res[ind] = arr[i];
				ind++;
			}
		}
		this_thread::sleep_for(chrono::milliseconds(time));
	}

	cout << "  Result array: ";
	for (int i = 0; i < size; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
	cout << "--- Thread work is finished" << endl << endl;
	cv.notify_one();
	//unique_lock event_locker(hOutEvent);
}

void MultElement(vector<char>& res, int& k) {
	unique_lock critical_section_locker(critical_section);
	unique_lock event_locker(hMultEvent);
	cv.wait(event_locker);
	cout << "--- Thread MultElement is started" << endl;
	cout << "  ";

	for (int i = 0; i < k; ++i) {
		if (isdigit(res[i])) {
			if (res[i] != 0) {
				int t = int(res[i]) - 48;
				mult_res *= t;
			}
		}
	}
	cout << "  Mult: " << mult_res << endl;
	cout << "--- Thread MultElement is finished" << endl;
	cv.notify_one();
	critical_section_locker.unlock();
}

int main() {
	// INPUT DATA
	int size;
	cout << "Enter size of array: ";
	cin >> size;
	cout << endl << "Enter elements of array: " << endl << "  ";
	
	vector<char> arr(size);

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

	// INITIALIZING
	cout << endl << "Enter k: ";
	cin >> k;

	jthread work(Work, ref(arr), size, ref(k));
	jthread multElement(MultElement, ref(res), ref(k));
	this_thread::sleep_for(chrono::milliseconds(2));

	std::unique_lock lock_out(hOutEvent);
	cv.wait(lock_out);

	cout << endl;

	cout << "Elements of array: ";
	for (int i = 0; i < size; ++i) {
		cout << res[i] << " ";
	}
	cout << endl << "Result of MultElement: " << mult_res << endl;

	return 0;
}