#include <iostream>
#include <Python.h>
using namespace std;

int main(int argc, char* argv[])
{
    Py_Initialize();  //≥ı ºªØ

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
    pFunc = PyObject_GetAttrString(pModule, "main");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        cout << "get func failed!" << endl;
        cout << int(pFunc) << endl;
        exit(0);
    }
    PyObject* Result = PyObject_CallObject(pFunc, NULL);
    int result = PyFloat_AsDouble(Result);
    Py_Finalize();
    cout << result << endl;
    return 0;
}