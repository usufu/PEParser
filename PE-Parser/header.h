/*����꣬ʹ��ͷ�ļ�ֻ������һ�Σ�������wsprintf��ʹ�ò����־�����ʾ�����߱�����ʹ��6.0���ͨ�ÿؼ��⡣*/
#ifndef _HAVE_INCLUDE_
#define _HAVE_INCLUDE_
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

/*��Ҫ��ͷ�ļ�*/
#include <Windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <imagehlp.h>
#include "resource.h"

/*ȫ�ֱ���*/
HINSTANCE hInst;//ʵ�����
HANDLE	hFile;//�ļ����
LPVOID	hFileMapping;//�ڴ�ӳ���ļ����
LPVOID ImageBase;//ӳ���ļ���ַ
char szFilePath[MAX_PATH];//�õ��ļ�����·�������ļ���

/*���ڹ��̻ص�����*/
LRESULT CALLBACK	MainDlgProc(HWND, UINT, WPARAM, LPARAM);//���Ի��򴰿ڹ��̺���
LRESULT CALLBACK	SectionTableDlgProc(HWND, UINT, WPARAM, LPARAM);//�ڱ�Ի��򴰿ڹ���
LRESULT CALLBACK	DataDirDlgProc(HWND, UINT, WPARAM, LPARAM);//����Ŀ¼�Ի��򴰿ڹ���
LRESULT CALLBACK	ImportTableDlgProc(HWND, UINT, WPARAM, LPARAM);//�����Ի��򴰿ڹ���
LRESULT CALLBACK	ExportTableDlgProc(HWND, UINT, WPARAM, LPARAM);//������Ի��򴰿ڹ���

/*�Զ��庯��*/
BOOL    OpenFileDlg(HWND);  //���ļ�ͨ�öԻ�����
BOOL	LoadFile(LPTSTR);//�����ļ�
BOOL	IsPEFile(LPVOID);//�ж��Ƿ�ΪPE�ļ�
void	UnLoadFile(LPVOID);//ж���ļ�
void    ShowFileHeaderInfo(HWND);//��ʾ�ļ�ͷ��Ϣ,���ļ�ͷ�е���Ϣ��ʾ�ڴ��ڵĶ�Ӧ�༭����
void    ShowOptionHeaderInfo(HWND);//��ʾ��ѡ�ļ�ͷ��Ϣ.
LPVOID	RvaToPtr(PIMAGE_NT_HEADERS,LPVOID ,DWORD);//��RVAת��Ϊoffset
#endif



