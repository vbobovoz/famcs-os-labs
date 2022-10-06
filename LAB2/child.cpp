#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]) {
	cout << "Child created." << endl;
	
	if (argc > 1) {
		int size = atoi(argv[1]);
		cout << "  Size of array: " << size << endl;
		int* arr = new int[size];

		for (int i = 2, j = 0; i < argc; ++i) {
			arr[j] = atoi(argv[i]);
			++j;
		}
		
		for (int i = 0; i < size - 1; ++i) {
			for (int j = 0; j < size - i - 1; ++j) {
				if (arr[j] > arr[j + 1]) {
					int temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
		}

		cout << "  Array after bubble sort: ";
		for (int i = 0; i < size; ++i) {
			cout << arr[i] << " ";
		}

		delete[] arr;
	}
	else {
		cout << "Error!" << endl << endl;
	}

	system("pause");
	return 0;
}