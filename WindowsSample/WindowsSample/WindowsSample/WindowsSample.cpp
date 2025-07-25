// WindowsSample.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "WindowsSample.h"
#include <string>
#include <windowsx.h>
#include <format>
#include "Game.h"

// 윈도우 프로그램 생성시, 자동으로 만들어지는 코드

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND gHwnd;
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
    // 메모리 누수 감지 활성화
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSSAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSSAMPLE));

    MSG msg = {};

    // 메모리릭 테스트
    //int* p = new int;

    // 콘솔 게임 만들떄
    // 프리 컴파일 헤더 : 필요한 파일들을 미리 인클루드하는 시스템
    // 이제는 Game 객체를 통해서만 여러가지 작업을 하자.
    Game* game = Game::GetInstance();  // 변수 선언 주소 : 0x01
    game->Init(gHwnd);

    // 아무런 의미가 없다.
    // 이미 Game 객체를 알고 있는 상태에서 GetInstance를 하고 싶은게 아니라.
    // Game 객체가 뭔지 모르지만, GetInstance() 호출하고 싶은거다.
    //game.GetInstance() (X)
    //Game::GetInstance();    // 함수호출에 필요한 객체는 없고, 그냥 이름 영역 :: 함수 이름 호출하면 된다.

    LARGE_INTEGER frequency, now, prev;
    ::QueryPerformanceFrequency(&frequency);
    ::QueryPerformanceCounter(&prev);

    // 1초는 1000ms
    const float targetFrameTime = 1000.0f / 120.f;  // 초당(1000ms) 120 프레임

    // 기본 메시지 루프입니다:
    // 게임은 메세지가 안와도 계속해서 루프를 무한으로 돌아야한다. -> PeekMessage
    // 외우실 필요없고, 구글링 검색하면 다 나오고
    // 한번만 설정할거라서 왜 이렇게 변경되고 있는지만 흐름을 이해하면 됩니다.
    while (msg.message != WM_QUIT)
    {
        // 윈도우 메세지가 있으면, 그 메세지를 처리하고.
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // CPU 를 극한까지 쓸필요가 없다. 고정 프레임 개념 추가
            ::QueryPerformanceCounter(&now);
            float elapsed = (now.QuadPart - prev.QuadPart) / static_cast<float>(frequency.QuadPart) * 1000.0f;

            if (elapsed >= targetFrameTime)
            {
			    // 우리만의 게임 로직을 처리하자.
			    // 여기다가 렌더링 코드를 넣으면?
			    // WM_PAINT 메세지 없이, 그림을 그릴수 있지 않을까
			    game->Update();
			    game->Render();

                prev = now;
            }
        }
    }

    game->Destroy();
    game->DestroyInstance();    // 힙에 할당된 싱글톤 객체 해제

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSSAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSSAMPLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 게임 윈도우의 크기를 정하기 위해, 전체 윈도우 사이즈를 조절한다.
   RECT windowRect = { 0, 0, GWinSizeX, GWinSizeY };
   ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, true);

   // 실제로 윈도우창을 생성한다.
   // PC에 여러개의 윈도우창이 떠있을수 있는데 각 윈도우창을 구분하는 핸들
   gHwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

   if (!gHwnd)
   {
      return FALSE;
   }

   ShowWindow(gHwnd, nCmdShow);
   UpdateWindow(gHwnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
    case WM_MOUSEMOVE:
    {
        // 마우스 좌표를 알았으니,.
        // WM_PAINT 메세지를 만들자
        //::InvalidateRect(gHwnd, nullptr, true);
    }
        break;
    case WM_PAINT:  
        {
            // 그리는 용도
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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
