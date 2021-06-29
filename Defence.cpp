// Defence.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Defence.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFENCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFENCE));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFENCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DEFENCE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   const int width = 1000 + GetSystemMetrics(SM_CXFRAME) * 2 + 8;
   const int height = 1000 + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_DLGFRAME,
      CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static GameManager gm;

	static int idLen;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
	case WM_CREATE:
	{
		idLen = 0;

		GetClientRect(hWnd, &rect);

		// 오브젝트 업데이트 타이머
		SetTimer(hWnd, 1, 16, NULL);

		// 프레임 업데이트 타이머
		SetTimer(hWnd, 2, 32, NULL);

		// 적 생성 타이머
		SetTimer(hWnd, 3, 500, NULL);
	}
	break;
	case WM_TIMER:
	{
		if (gm.state == STATE::GAME_PLAY)
		{
			switch (wParam)
			{
			case 1:
				if (!gm.Update(rect))
				{
					gm.state = STATE::GAME_OVER;
					gm.OpenRecordFile();
				}
				break;
			case 2:
				InvalidateRect(hWnd, NULL, true);
				break;
			case 3:
				gm.CreateEnemy(rect);
				break;
			}
		}
		else if (gm.state == STATE::GAME_OVER)
		{
			switch (wParam)
			{
			case 2:
				InvalidateRect(hWnd, NULL, true);
				break;
			}
		}
	}
	break;
	case WM_CHAR:
	{
		if (gm.state == STATE::GAME_START)
		{
			if (wParam == VK_BACK)
			{
				if (idLen > 0)
				{
					--idLen;
					gm.id[idLen] = 0;
				}
			}
			else if (wParam == VK_RETURN)
			{
				gm.state = STATE::GAME_PLAY;
			}
			else if((wParam >= 'A' && wParam <= 'Z') || (wParam >= 'a' && wParam <= 'z'))
			{
				if (idLen < 3)
				{
					gm.id[idLen] = wParam;
					++idLen;
				}
			}

			InvalidateRect(hWnd, NULL, true);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (gm.state == STATE::GAME_PLAY)
		{
			// 방향키 왼쪽 오른쪽으로 포신을 회전합니다.
			if (wParam == VK_LEFT)
				gm.GetPlayer().MuzzleRotate(-3);
			else if (wParam == VK_RIGHT)
				gm.GetPlayer().MuzzleRotate(3);

			if (wParam == VK_SPACE)
				gm.Fire(rect);
		}
	}
	break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

		if (gm.state == STATE::GAME_START)
		{
			gm.DrawGameStart(hdc);
		}
		else if (gm.state == STATE::GAME_PLAY)
		{
			gm.DrawGamePlay(hdc);
		}
		else if (gm.state == STATE::GAME_OVER)
		{
			gm.DrawGameOver(hdc);
		}

		gm.PrintTest(hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
