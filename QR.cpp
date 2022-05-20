#include <iostream>
#include <windows.h>
#include <Python.h>
#include <tchar.h>
using namespace std;

bool IsProcessRunAsAdmin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    BOOL b = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);
    if (b)
    {
        CheckTokenMembership(NULL, AdministratorsGroup, &b);
        FreeSid(AdministratorsGroup);
    }
    return b == TRUE;
}
short GetAdmin(LPCSTR Param, int Showcmd)
{
    if (IsProcessRunAsAdmin())
        return 0;
    TCHAR Path[MAX_PATH];
    ZeroMemory(Path, MAX_PATH);
    ::GetModuleFileName(NULL, Path, MAX_PATH);           //获取程序路径
    HINSTANCE res;
    res = ShellExecute(NULL, "runas", Path, Param, NULL, Showcmd);
    if ((int)res > 32)
        return 1;
    else
        return 0;
}

int main1()
{
    //GetAdmin(L"runas",1);
    Py_Initialize();  //初始化

    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pParam = NULL;
    PyObject* pResult = NULL;
    const char* pBuffer = NULL;
    int iBufferSize = 0;
    PyRun_SimpleString("import requests");
    PyRun_SimpleString("from PIL import Image");
    PyRun_SimpleString("import time");
    PyRun_SimpleString("import re");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    pModule = PyImport_ImportModule("QR");
        if (pModule == NULL)
        {
            cout << "get module failed!" << endl;
            exit(0);
        }
    pFunc = PyObject_GetAttrString(pModule, "start");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        cout << "get func failed!" << endl;
        cout << int(pFunc) << endl;
        exit(0);
    }
    PyObject* Result = PyObject_CallObject(pFunc, NULL);
    int result = PyFloat_AsDouble(Result);
    Py_Finalize();
    std::cout << result << std::endl;
    return result;
}//调用.py或者.pyc程序版

int main() {

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(STARTUPINFOA);

        TCHAR cmd[256] = _T("QR.exe");
        BOOL working = ::CreateProcess(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

        if (working == 0)
        {
            DWORD error = GetLastError();
            cout << "CreateProcess Error : " << error << endl;
            getchar();
            return 0;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        unsigned long Result;
        GetExitCodeProcess(pi.hProcess, &Result);

        cout << "Exit Code : " << Result << endl;
        getchar();

        return Result;
}//调用.exe版