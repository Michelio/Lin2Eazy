#include "Injector.h"
#include "Glob.h"

#include <QFile>
#include <QMap>

Injector::Injector()
{
    qDebug("Injector()");
    //tempDir = new QDir();
    //tempDir->setPath(tempDir->absolutePath());
}

Injector::~Injector()
{
    stop();
    Sleep(150);
    quit();
    //delete tempDir;
    qDebug("~Injector()");
}

DWORD Injector::GetProcId()
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
            do
            {
                std::wstring ws(procEntry.szExeFile);
                std::string str(ws.begin(), ws.end());
                if (iequals(str, procName.toStdString()))
                {
                    procId = procEntry.th32ProcessID;

                    if (!client.contains(procId))
                        break;
                    else procId = 0;
                }
            } while (Process32Next(hSnap, &procEntry));
    }
    CloseHandle(hSnap);

    return procId;
}

bool Injector::CanInject(DWORD procId)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return false;

    me32.dwSize = sizeof(MODULEENTRY32);

    int count = 0;
    if (!Module32First(hModuleSnap, &me32))
    {
        CloseHandle(hModuleSnap);
        return false;
    }

    do
    {
        count++;
    } while (Module32Next(hModuleSnap, &me32));

    if (count < 100)
    {
        CloseHandle(hModuleSnap);
        return false;
    }

    CloseHandle(hModuleSnap);
    return true;
}

bool Injector::IsInjected(DWORD procId)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return false;

    me32.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(hModuleSnap, &me32))
    {
        CloseHandle(hModuleSnap);
        return false;
    }

    do
    {
        QString moduleName(me32.szModule[0]);
        if (client.value(procId).contains(moduleName, Qt::CaseInsensitive))
            return true;

    } while (Module32Next(hModuleSnap, &me32));

    CloseHandle(hModuleSnap);
    return false;
}

//bool Injector::CreateTempDirectory()
//{
//    if (tempDir->mkpath(tempDir->path() + "\\temp"))
//    {
//        tempDir->setPath(tempDir->path() + "\\temp");
//        return true;
//    }

//    return false;
//}

bool Injector::InjectDll(DWORD &procId)
{
    while (!procId && runInjector)
    {
        procId = GetProcId();
        Sleep(100);
    }

    if (!runInjector)
        return false;

    if (!CanInject(procId))
        return false;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (loc)
        {
            QString newFile = dllPath;
            newFile.replace("\\Hook.dll", "\\temp\\" + QString::number(procId) + "Hook.dll", Qt::CaseSensitive);
            if (QFile::copy(dllPath, newFile))
                client.insert(procId, newFile);

            //else TODO: notification

            WriteProcessMemory(hProc, loc, newFile.toStdString().c_str(), newFile.length(), 0);
        }

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
            CloseHandle(hThread);
    }

    if (hProc)
        CloseHandle(hProc);

    return true;
}

void Injector::run()
{
    while(runInjector)
    {
        DWORD procId = 0;
        if (InjectDll(procId))
        {

            if (IsInjected(procId))
                emit DllInjected(procId);

            else
                client.remove(procId);
        }
    }
}

void Injector::stop()
{
    runInjector = false;
}
