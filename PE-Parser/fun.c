#include "header.h"
BOOL  OpenFileDlg(HWND hwnd)   //打开通用对话框,选择文件,设置文件过滤器,将文件路径及文件名保存在szFilePath变量中.
{
	OPENFILENAME ofn;
	memset(szFilePath,0,MAX_PATH);
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize      =sizeof(ofn);
	ofn.hwndOwner        =hwnd;
	ofn.hInstance        =GetModuleHandle(NULL);
	ofn.nMaxFile         =MAX_PATH;
	ofn.lpstrInitialDir  =".";
	ofn.lpstrFile        =szFilePath;
	ofn.lpstrTitle       ="Open ...";
	ofn.Flags            =OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFilter      ="*.*\0*.*\0";
	if(!GetOpenFileName(&ofn))
		return FALSE;
	return TRUE;

}

BOOL LoadFile(LPTSTR lpFilename)
{
	hFile = CreateFile(lpFilename, GENERIC_READ, \
						FILE_SHARE_READ, NULL, \
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);//根据文件名打开文件,得到文件句柄.
  	if (!hFile) 
		return FALSE;
	hFileMapping = CreateFileMapping(hFile,NULL, PAGE_READONLY, 0, 0, NULL);//创建映像文件

	if (!hFileMapping)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	ImageBase = MapViewOfFile(hFileMapping,FILE_MAP_READ,0,0,0);//得到映像文件的基址,存放在全局变量ImageBase中.
	if (!ImageBase)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	return TRUE;
	
}
BOOL IsPEFile(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER  pDH=NULL;
	PIMAGE_NT_HEADERS  pNtH=NULL;

	if(!ImageBase)
		return FALSE;

	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	if(pDH->e_magic!=IMAGE_DOS_SIGNATURE)
		return FALSE;

	pNtH=(PIMAGE_NT_HEADERS32)((DWORD)pDH+pDH->e_lfanew);
	if (pNtH->Signature != IMAGE_NT_SIGNATURE )
		return FALSE;

	return TRUE;


}
void UnLoadFile(LPVOID ImageBase)
{
	if(ImageBase)
		UnmapViewOfFile(ImageBase);
	if (hFileMapping)
		CloseHandle(hFileMapping);
	if (hFile)
		CloseHandle(hFile);
	
}
void ShowFileHeaderInfo(HWND hWnd)
{    
	char   cBuff[10];
	PIMAGE_DOS_HEADER  pDH = NULL;
	PIMAGE_NT_HEADERS  pNtH = NULL;
	PIMAGE_FILE_HEADER pFH = NULL;

	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
	pFH=&pNtH->FileHeader;
	if(!pFH)
	{
		MessageBox(hWnd,"Can't get File Header ! ","PE-Parser",MB_OK);
		return;
	}
	wsprintf(cBuff, "%04lX", pDH->e_magic);
	SetDlgItemText(hWnd,IDC_EDIT_DH_MAGIC,cBuff); 

	wsprintf(cBuff, "%04lX", pDH->e_ip);
	SetDlgItemText(hWnd,IDC_EDIT_DH_IP,cBuff); 

	wsprintf(cBuff, "%04lX", pDH->e_cs);
	SetDlgItemText(hWnd,IDC_EDIT_DH_CS,cBuff); 

	wsprintf(cBuff, "0x%04lX", sizeof(IMAGE_DOS_HEADER));
	SetDlgItemText(hWnd,IDC_EDIT_DH_SIZE,cBuff); 

	wsprintf(cBuff, "%04lX", pNtH->Signature);
	SetDlgItemText(hWnd,IDC_EDIT_FH_SIG,cBuff);

	wsprintf(cBuff, "%04lX", pFH->Machine);
	SetDlgItemText(hWnd,IDC_EDIT_FH_MACHINE,cBuff);

	wsprintf(cBuff, "%04lX", pFH->NumberOfSections);
	SetDlgItemText(hWnd,IDC_EDIT_FH_NUMOFSECTIONS,cBuff);

	wsprintf(cBuff, "%08lX", pFH->TimeDateStamp);
	SetDlgItemText(hWnd,IDC_EDIT_FH_TDS,cBuff);

	wsprintf(cBuff, "%08lX", pFH->PointerToSymbolTable);
	SetDlgItemText(hWnd,IDC_EDIT_FH_PTSYMBOL,cBuff);

	wsprintf(cBuff, "%08lX", pFH->NumberOfSymbols);
	SetDlgItemText(hWnd,IDC_EDIT_FH_NUMOFSYM,cBuff);

	wsprintf(cBuff, "%04lX", pFH->SizeOfOptionalHeader);
	SetDlgItemText(hWnd,IDC_EDIT_FH_SIZEOFOH,cBuff);

	wsprintf(cBuff, "%04lX", pFH->Characteristics);
	SetDlgItemText(hWnd,IDC_EDIT_FH_CHARACTERISTICS,cBuff);


}



void    ShowOptionHeaderInfo(HWND hWnd)       
{
	char   cBuff[10];
	PIMAGE_DOS_HEADER  pDH=NULL;
	PIMAGE_NT_HEADERS  pNtH=NULL;
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
	pOH=&pNtH->OptionalHeader;
	if(!pOH)
	{
		MessageBox(hWnd,"Can't get Optional Header !:(","PEInfo_Example",MB_OK);
		return;
	}
	wsprintf(cBuff, "%08lX", pOH->AddressOfEntryPoint);
	SetDlgItemText(hWnd,IDC_EDIT_OH_EP,cBuff);

	wsprintf(cBuff, "%08lX", pOH->ImageBase);
	SetDlgItemText(hWnd,IDC_EDIT_OH_IMAGEBASE,cBuff);

	wsprintf(cBuff, "%08lX", pOH->BaseOfCode);
	SetDlgItemText(hWnd,IDC_EDIT_OH_CODEBASE,cBuff);

	wsprintf(cBuff, "%08lX", pOH->BaseOfData);
	SetDlgItemText(hWnd,IDC_EDIT_OH_DATABASE,cBuff);

	wsprintf(cBuff, "%08lX", pOH->SizeOfImage);
	SetDlgItemText(hWnd,IDC_EDIT_OH_IMAGESIZE,cBuff);

	wsprintf(cBuff, "%08lX", pOH->SizeOfHeaders);
	SetDlgItemText(hWnd,IDC_EDIT_OH_HEADERSIZE,cBuff);

	wsprintf(cBuff, "%08lX", pOH->SectionAlignment);
	SetDlgItemText(hWnd,IDC_EDIT_OH_SECTIONALIGN,cBuff);

	wsprintf(cBuff, "%08lX", pOH->FileAlignment);
	SetDlgItemText(hWnd,IDC_EDIT_OH_FILEALIGN,cBuff);

	wsprintf(cBuff, "%08lX", pOH->Subsystem);
	SetDlgItemText(hWnd,IDC_EDIT_OH_SUBSYSTEM,cBuff);

	wsprintf(cBuff, "%04lX", pOH->CheckSum);
	SetDlgItemText(hWnd,IDC_EDIT_OH_CHECKSUM,cBuff);

	wsprintf(cBuff, "%04lX", pOH->DllCharacteristics);
	SetDlgItemText(hWnd,IDC_EDIT_OH_DLLFLAGS,cBuff);
}
LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA)
{	

	return ImageRvaToVa(pNtH,ImageBase,dwRVA,NULL);

}