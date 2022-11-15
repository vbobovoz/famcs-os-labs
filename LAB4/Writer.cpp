#include <iostream>
#include <windows.h>

using namespace std;

wchar_t const* mes[4]{ L"W_Exit", L"W_A", L"W_B", L"W_C"};

int main()
{
    std::string line;
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
    WaitForSingleObject(hMutex, INFINITE);

    std::cout << "Process writer is started" << std::endl;
    std::cout << "Process is active" << std::endl;

    HANDLE mesEv[4];
    for (int i = 0; i < 4; ++i) { mesEv[i] = OpenEvent(EVENT_MODIFY_STATE, FALSE, mes[i]); }
    while (true)
    {
        std::cin >> line;
        line = "W_" + line;
        wstring wline(line.begin(), line.end());
        if (wline._Equal(mes[0])) { SetEvent(mesEv[0]); break; }
        if (wline._Equal(mes[1])) { SetEvent(mesEv[1]); }
        if (wline._Equal(mes[2])) { SetEvent(mesEv[2]); }
        if (wline._Equal(mes[3])) { SetEvent(mesEv[3]); }
    }
    ReleaseMutex(hMutex);
    return 0;
}