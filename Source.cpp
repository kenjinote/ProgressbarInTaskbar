#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <shobjidl.h>

#define IDC_STATIC1 1000
TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static ITaskbarList3 *pTaskbarList3 = 0;
	static DWORD dwProgress = 0;
	static HWND hStatic;
	switch (msg)
	{
	case WM_CREATE:
		hStatic = CreateWindow(TEXT("STATIC"), TEXT("0"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATIC1, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		CoInitialize(0);
		if (FAILED(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&pTaskbarList3)))
		{
			return -1;
		}
		pTaskbarList3->SetProgressState(hWnd, TBPF_NORMAL);
		SetTimer(hWnd, 0x1234, 100, 0);
		break;
	case WM_TIMER:
		dwProgress++;
		SetDlgItemInt(hWnd, IDC_STATIC1, dwProgress, 0);
		if (pTaskbarList3)pTaskbarList3->SetProgressValue(hWnd, dwProgress, 100);
		if (dwProgress == 100)dwProgress = 0;
		break;
	case WM_SIZE:
		MoveWindow(hStatic, 10, 10, 64, 32, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0x1234);
		if (pTaskbarList3)
		{
			pTaskbarList3->Release();
			pTaskbarList3 = 0;
		}
		CoUninitialize();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("タスクバーボタンに進行状況バー(プログレスバー)を表示"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
