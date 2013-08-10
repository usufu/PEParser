#include "header.h"
void InitExportListCtrl(HWND);
void ShowExportDirInfo(HWND);
void ShowExportFuncsInfo(HWND);
PIMAGE_DOS_HEADER		pDH	=	NULL;
PIMAGE_NT_HEADERS       pNtH=NULL;
PIMAGE_EXPORT_DIRECTORY pExportDir=NULL;
PIMAGE_OPTIONAL_HEADER	pOH	=	NULL;
LRESULT CALLBACK  ExportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		pDH		=	(PIMAGE_DOS_HEADER)ImageBase;
		pNtH	=	(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
		pOH		=	&pNtH->OptionalHeader;
		InitExportListCtrl(hDlg);
		ShowExportDirInfo(hDlg);
		ShowExportFuncsInfo(hDlg);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:  
			SendMessage(hDlg, WM_CLOSE,0,0);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg,0);
		return TRUE;;
	}
	return FALSE;
}

void InitExportListCtrl(HWND hDlg) 
{
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "Ordinal";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "RVA";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 280;
	lvColumn.pszText   = "Function Name";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

}
void   ShowExportDirInfo(HWND hDlg)
{
	char  cBuff[10]; 

	
	
	pExportDir=(PIMAGE_EXPORT_DIRECTORY)pOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	pExportDir	=(PIMAGE_EXPORT_DIRECTORY)RvaToPtr(pNtH,ImageBase,(DWORD)pExportDir);

	if(!pExportDir)
	{
		MessageBox(hDlg,"Can't get Export Directory","PE-Parser",MB_OK);
		return;
	}

	wsprintf(cBuff, "%08lX", pExportDir->Characteristics);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_CHARACTERISTICS,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->TimeDateStamp);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_TDS,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->MajorVersion);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_MJV,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->MinorVersion);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_MIV,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->Base);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_BASE,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->Name);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NAME,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->NumberOfFunctions);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NUMOFFUNCS,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->NumberOfNames);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NUMOFNAMES,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->AddressOfFunctions);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFFUNCS,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->AddressOfNames);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFNAMES,cBuff);

	wsprintf(cBuff, "%08lX", pExportDir->AddressOfNameOrdinals);
	SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFNAMEORD,cBuff);
}
void   ShowExportFuncsInfo(HWND hDlg)
{
	HWND         hList;
	LVITEM       lvItem;
	char         cBuff[10], *szFuncName; 

	UINT                    iNumOfName=0;
	PDWORD                  pdwRvas, pdwNames;
	PWORD                   pwOrds;
	UINT                    i=0,j=0,k=0; 
	BOOL                    bIsByName=FALSE;

	pExportDir= (PIMAGE_EXPORT_DIRECTORY)pOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	pExportDir	=	RvaToPtr(pNtH,ImageBase,(DWORD)pExportDir);

	pwOrds    = (PWORD)RvaToPtr(pNtH, ImageBase,pExportDir->AddressOfNameOrdinals);
	pdwRvas   = (PDWORD)RvaToPtr(pNtH, ImageBase,pExportDir->AddressOfFunctions);
	pdwNames  = (PDWORD)RvaToPtr(pNtH, ImageBase,pExportDir->AddressOfNames);

	hList=GetDlgItem(hDlg,IDC_EXPORT_LIST);
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);


	iNumOfName=pExportDir->NumberOfNames;

	for( i=0;i<pExportDir->NumberOfFunctions;i++)//外部大循环,以输出函数的个数来计数.
	{
		if(*pdwRvas) //如果输出函数的RVA不为0.这里这样处理是为了防止自定义序号的情况.
		{    
			for( j=0;j<iNumOfName;j++) /*查找名称.*/
			{
				if(i==pwOrds[j])
				{  
					bIsByName=TRUE;//若能找到则是以名称作为输出.
					szFuncName=(char*)RvaToPtr(pNtH,ImageBase,pdwNames[j]);//NameOrdinals与Names一一对应
					break;
				}

				bIsByName=FALSE;
			}


			memset(&lvItem, 0, sizeof(lvItem));
			lvItem.mask    = LVIF_TEXT;
			lvItem.iItem   = k;

			lvItem.pszText = cBuff;
			wsprintf(cBuff, "%04lX", (UINT)(pExportDir->Base+i));
			SendDlgItemMessage(hDlg,IDC_EXPORT_LIST,LVM_INSERTITEM,0,(LPARAM)&lvItem);

			lvItem.pszText  = cBuff;
			wsprintf(cBuff, "%08lX", (*pdwRvas));
			lvItem.iSubItem = 1;
			SendDlgItemMessage(hDlg,IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

			if(bIsByName)			
				lvItem.pszText=szFuncName;
			else
				lvItem.pszText  = "-";

			lvItem.iSubItem = 2;
			SendDlgItemMessage(hDlg,IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);


			//
			++k;

		}

		++pdwRvas;

	}


}