#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

wchar_t const* mes[4]{ L"R_Exit", L"R_A", L"R_B", L"R_C" };
wchar_t const* EMes[2]{ L"1", L"0" };

int main()
{
    std::string number;
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
    WaitForSingleObject(hSemaphore, INFINITE);
    std::cout << "Process reader is started" << std::endl;
    std::cout << "Process is active" << std::endl;
    HANDLE mesEv[4];
    HANDLE mesEx[2];
    for (int i = 0; i < 4; ++i) { mesEv[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, mes[i]); }
    for (int i = 0; i < 2; ++i) { mesEx[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, EMes[i]); }
    while (true)
    {
        DWORD m = WaitForMultipleObjects(4, mesEv, FALSE, INFINITE);
        std::wcout << mes[m] << L'\n';
        std::cout << "Input 1 to exit or 0 to continue: ";
        std::cin >> number;
        wstring wline(number.begin(), number.end());
        if (wline._Equal(EMes[0])) { SetEvent(mesEx[0]); break; }
        if (wline._Equal(EMes[1])) { SetEvent(mesEx[1]); }
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}