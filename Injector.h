#ifndef INJECTOR_H
#define INJECTOR_H

#include <QThread>
#include <QTextStream>
#include <QDir>

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

class Injector : public QThread
{
    Q_OBJECT

private:
    QMap<DWORD, QString> client;
    //QDir *tempDir; // TODO use QTemporaryDir instead
    QString dllPath = "C:\\Users\\michelio\\Desktop\\l2\\Hook.dll";
    QString procName = "L2.exe";
    bool runInjector = true;
    void stop();

public:
    Injector();
    ~Injector();
    bool InjectDll(DWORD&);
    DWORD GetProcId();
    bool CanInject(DWORD);
    bool IsInjected(DWORD);
    //bool CreateTempDirectory();

protected:
    void run();

signals:
    void DllInjected(DWORD);
};

#endif // INJECTOR_H
