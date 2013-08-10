#include "header.h"

void ShowDataDirInfo(HWND);
typedef struct
{
	UINT   ID_RVA;
	UINT   ID_SIZE;
} DataDir_EditID;

DataDir_EditID EditID_Array[]=
{
	{IDC_EDIT_DD_RVA_EXPORT,     IDC_EDIT_DD_SIZE_EXPORT},
	{IDC_EDIT_DD_RVA_IMPORT,     IDC_EDIT_DD_SIZE_IMPORT},
	{IDC_EDIT_DD_RVA_RES,        IDC_EDIT_DD_SIZE_RES},
	{IDC_EDIT_DD_RVA_EXCEPTION,  IDC_EDIT_DD_SIZE_EXCEPTION},
	{IDC_EDIT_DD_RVA_SECURITY,	 IDC_EDIT_DD_SIZE_SECURITY},
	{IDC_EDIT_DD_RVA_RELOC,		 IDC_EDIT_DD_SIZE_RELOC},
	{IDC_EDIT_DD_RVA_DEBUG,		 IDC_EDIT_DD_SIZE_DEBUG},
	{IDC_EDIT_DD_RVA_COPYRIGHT,	 IDC_EDIT_DD_SIZE_COPYRIGHT},
	{IDC_EDIT_DD_RVA_GP,		 IDC_EDIT_DD_SIZE_GP},
	{IDC_EDIT_DD_RVA_TLS,        IDC_EDIT_DD_SIZE_TLS},
	{IDC_EDIT_DD_RVA_LOADCONFIG, IDC_EDIT_DD_SIZE_LOADCONFIG},
	{IDC_EDIT_DD_RVA_BOUND,		 IDC_EDIT_DD_SIZE_BOUND},
	{IDC_EDIT_DD_RVA_IAT,		 IDC_EDIT_DD_SIZE_IAT},
	{IDC_EDIT_DD_RVA_DELAYIMPORT,IDC_EDIT_DD_SIZE_DELAYIMPORT},
	{IDC_EDIT_DD_RVA_COM,		 IDC_EDIT_DD_SIZE_COM},
	{IDC_EDIT_DD_RVA_NOUSE,		 IDC_EDIT_DD_SIZE_NOUSE}
};

LRESULT CALLBACK  DataDirDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message)
	{
	case WM_INITDIALOG:
		ShowDataDirInfo(hDlg);
		return TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{

		case IDC_OK:  
			SendMessage(hDlg, WM_CLOSE,0,0);
			return TRUE;

		case IDC_SHOWEXPORT_BTN: 
				DialogBox(hInst, MAKEINTRESOURCE(IDD_EXPORT_DLG), hDlg, (DLGPROC)ExportTableDlgProc);
			return TRUE;

		case IDC_SHOWIMPORT_BTN: 
				DialogBox(hInst, MAKEINTRESOURCE(IDD_IMPORT_DLG), hDlg, (DLGPROC)ImportTableDlgProc);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return  TRUE;
	}


	return FALSE;

}
void ShowDataDirInfo(HWND hDlg)
{
	char   cBuff[9];
	int i	=	0;
	PIMAGE_OPTIONAL_HEADER	pOH	=	NULL;
	PIMAGE_DOS_HEADER		pDH	=	NULL;
	PIMAGE_NT_HEADERS		pNtH=	NULL;
	PIMAGE_FILE_HEADER		pFH	=	NULL;
	PIMAGE_SECTION_HEADER   pSH	=	NULL;


	pDH		=	(PIMAGE_DOS_HEADER)ImageBase;
	pNtH	=	(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
	pOH		=	&pNtH->OptionalHeader;	
	if(!pOH)
		return;

	for(i=0;i<16;i++)
	{
		wsprintf(cBuff, "%08lX", pOH->DataDirectory[i].VirtualAddress);
		SetDlgItemText(hDlg,EditID_Array[i].ID_RVA,cBuff);

		wsprintf(cBuff, "%08lX", pOH->DataDirectory[i].Size);
		SetDlgItemText(hDlg,EditID_Array[i].ID_SIZE,cBuff);
	}
	if (pOH->DataDirectory[0].Size	!=	0)//如果有导出表,则导出表按钮可用.
		EnableWindow(GetDlgItem(hDlg,IDC_SHOWEXPORT_BTN),TRUE);
	if (pOH->DataDirectory[1].Size	!=	0)//如果有导入表,则导入表按钮可用.
		EnableWindow(GetDlgItem(hDlg,IDC_SHOWIMPORT_BTN),TRUE);
}