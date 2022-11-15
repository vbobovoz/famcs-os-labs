#include <iostream>
#include <windows.h>

using namespace std;

HANDLE Writer[4];
HANDLE Reader[4];
HANDLE Exit[2];

wchar_t const* WriterMes[4]{ L"W_Exit", L"W_A", L"W_B", L"W_C" };
wchar_t const* ReaderMes[4]{ L"R_Exit", L"R_A", L"R_B", L"R_C" };
wchar_t const* ExitMes[2]{ L"1", L"0" };

int main()
{
    for (int i = 0; i < 4; ++i)
    {
        Writer[i] = CreateEvent(NULL, FALSE, FALSE, WriterMes[i]);
        Reader[i] = CreateEvent(NULL, FALSE, FALSE, ReaderMes[i]);
    }

    for (int i = 0; i < 2; ++i) { Exit[i] = CreateEvent(NULL, FALSE, FALSE, ExitMes[i]); }

    int numberOfProcess;
    cout << "Input number of process you want to start: ";
    cin >> numberOfProcess;

    HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, L"Semaphore");
    HANDLE hMutex = CreateMutex(NULL, 0, L"Mutex");
    STARTUPINFO* Wstp = new STARTUPINFO[numberOfProcess], * Rstp = new STARTUPINFO[numberOfProcess];
    PROCESS_INFORMATION* Wpi = new PROCESS_INFORMATION[numberOfProcess], * Rpi = new PROCESS_INFORMATION[numberOfProcess];

    for (int i = 0; i < numberOfProcess; ++i)
    {
        ZeroMemory(&Wstp[i], sizeof(STARTUPINFO));
        Wstp[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&Rstp[i], sizeof(STARTUPINFO));
        Rstp[i].cb = sizeof(STARTUPINFO);
        wstring writer = L"Writer.exe", reader = L"Reader.exe";
        CreateProcess(NULL, &writer[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Wstp[i], &Wpi[i]);
        CreateProcess(NULL, &reader[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Rstp[i], &Rpi[i]);
    }

    int counter = 1;
    for (int i = 0; i < numberOfProcess; ++i)
    {
        while (true)
        {
            DWORD mes = WaitForMultipleObjects(4, Writer, FALSE, INFINITE);
            SetEvent(Reader[mes]);
            wcout << WriterMes[mes] << endl;
            DWORD mes1 = WaitForMultipleObjects(2, Exit, FALSE, INFINITE);
            if (mes1 == WAIT_OBJECT_0)
            {
                std::cout << "Reader " << counter << " ended his work\n";
                counter += 1;
                break;
            }
        }
        std::cout << "Writer " << i + 1 << " ended his work\n";
    }

    for (int i = 0; i < 4; i++)
    {
        CloseHandle(Writer[i]);
        CloseHandle(Reader[i]);
    }

    for (int i = 0; i < 2; i++) { CloseHandle(Exit[i]); }

    for (int i = 0; i < numberOfProcess; i++)
    {
        CloseHandle(Wpi[i].hThread);
        CloseHandle(Wpi[i].hProcess);
        CloseHandle(Rpi[i].hThread);
        CloseHandle(Rpi[i].hProcess);
    }

    CloseHandle(hSemaphore);
    return 0;
}