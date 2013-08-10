/*定义宏，使得头文件只被包含一次，并对于wsprintf的使用不出现警告提示。告诉编译器使用6.0版的通用控件库。*/
#ifndef _HAVE_INCLUDE_
#define _HAVE_INCLUDE_
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

/*需要的头文件*/
#include <Windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <imagehlp.h>
#include "resource.h"

/*全局变量*/
HINSTANCE hInst;//实例句柄
HANDLE	hFile;//文件句柄
LPVOID	hFileMapping;//内存映射文件句柄
LPVOID ImageBase;//映像文件基址
char szFilePath[MAX_PATH];//得到文件名的路径及其文件名

/*窗口过程回调函数*/
LRESULT CALLBACK	MainDlgProc(HWND, UINT, WPARAM, LPARAM);//主对话框窗口过程函数
LRESULT CALLBACK	SectionTableDlgProc(HWND, UINT, WPARAM, LPARAM);//节表对话框窗口过程
LRESULT CALLBACK	DataDirDlgProc(HWND, UINT, WPARAM, LPARAM);//数据目录对话框窗口过程
LRESULT CALLBACK	ImportTableDlgProc(HWND, UINT, WPARAM, LPARAM);//导入表对话框窗口过程
LRESULT CALLBACK	ExportTableDlgProc(HWND, UINT, WPARAM, LPARAM);//导出表对话框窗口过程

/*自定义函数*/
BOOL    OpenFileDlg(HWND);  //打开文件通用对话框函数
BOOL	LoadFile(LPTSTR);//载入文件
BOOL	IsPEFile(LPVOID);//判断是否为PE文件
void	UnLoadFile(LPVOID);//卸载文件
void    ShowFileHeaderInfo(HWND);//显示文件头信息,将文件头中的信息显示在窗口的对应编辑框中
void    ShowOptionHeaderInfo(HWND);//显示可选文件头信息.
LPVOID	RvaToPtr(PIMAGE_NT_HEADERS,LPVOID ,DWORD);//将RVA转换为offset
#endif



