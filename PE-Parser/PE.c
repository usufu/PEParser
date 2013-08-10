#include "header.h"



int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	hInst = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DLG, NULL, (DLGPROC)MainDlgProc);
	return(FALSE);
}
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon;
	char * szError = "There is something wrong,please retry.";
	switch (message)
	{
	case WM_INITDIALOG :
		hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON2));
		SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
		return TRUE ;

	case WM_COMMAND :
		switch (LOWORD (wParam))
		{
		case IDEXIT:
			SendMessage(hWnd, WM_CLOSE,0,0);
			return TRUE;
		case IDC_OPEN_BTN:
			if(!OpenFileDlg(hWnd))
				return FALSE;
			SendDlgItemMessage(hWnd,IDC_FPATH,WM_SETTEXT,MAX_PATH,(LPARAM)szFilePath);
			if (!LoadFile(szFilePath))
			{
				MessageBox(hWnd,"Load file failed!","PE-Parse",MB_OK);
				SendDlgItemMessage(hWnd,IDC_FPATH,WM_SETTEXT,MAX_PATH,(LPARAM)szError);
				return FALSE;
			}
			if (!IsPEFile(ImageBase))
			{
				MessageBox(hWnd,"File is not a PE format file!","PE-Parse",MB_OK);
				SendDlgItemMessage(hWnd,IDC_FPATH,WM_SETTEXT,MAX_PATH,(LPARAM)szError);
				
				return FALSE;

			}
			ShowFileHeaderInfo(hWnd);
			ShowOptionHeaderInfo(hWnd);
			EnableWindow(GetDlgItem(hWnd,IDC_SHOWSHT),TRUE);
			EnableWindow(GetDlgItem(hWnd,IDC_SHOWDATADIR),TRUE);
			return TRUE;
		case IDC_SHOWSHT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SHT_DLG), hWnd, (DLGPROC)SectionTableDlgProc);
			return TRUE;
		case IDC_SHOWDATADIR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DATADIR_DLG), hWnd, (DLGPROC)DataDirDlgProc);
			return TRUE;



		}
		break;
	case WM_CLOSE:
		UnLoadFile(ImageBase);
		EndDialog(hWnd, 0);
		break;

	}

	return FALSE ;
}