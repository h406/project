//==============================================================================
//
// window[window.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "window.h"

//==============================================================================
// constant
//------------------------------------------------------------------------------
namespace {
  static const char* kClassName = "ShaderTest";
  static const char* kDefaultTitle = "ShaderTest";
};

//==============================================================================
// system
//------------------------------------------------------------------------------
Window::Window(int width,int height)
  : _hInst(GetModuleHandle(NULL))
  ,_hWnd(nullptr)
  ,_WindowSize({width,height})
  ,_bExitApplication(false)
  ,_nCountFPS(0) {

  // メモリーク検出フラグ
  //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  //ウィンドウクラス作成
  WNDCLASSEX wcex =
  {
    sizeof(WNDCLASSEX),           // ウィンドウの容量を伝える
    CS_CLASSDC,                   // クラスのスタイル
    winProc,                      // カスタムウィンドウプロシージャ設定
    0,                            // 使わない
    0,                            // 使わない
    _hInst,                       // メイン関数の引数であるインスタンスハンドル
    NULL,                         // ラージアイコンを指定
    LoadCursor(NULL,IDC_ARROW),   //カーソルの種類を指定
    (HBRUSH)(COLOR_WINDOW + 1),   //背景色
    NULL,
    kClassName,                   // このウィンドウクラスの名前を指定
    NULL                          // スモールアイコンを指定
  };

  // ウィンドウサイズの計算(非クライアント領域考慮)
  RECT rcAdjustWindow = {0,0,(LONG)width,(LONG)height};	// クライアント領域サイズを指定
  DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);			// ウィンドウのスタイル
  AdjustWindowRect(&rcAdjustWindow,dwStyle,NULL);	// ウィンドウサイズを自動調整

  // ウィンドウクラスの登録
  RegisterClassEx(&wcex);

  // ウィンドウの生成
  _hWnd = CreateWindowEx(
    0,                                          // 拡張ウィンドウクラス
    kClassName,                                 // ウィンドウクラスの名前
    kDefaultTitle,                              // ウィンドウ枠に表示されるタイトル
    dwStyle,                                    // ウィンドウのスタイル、全面に表示
    CW_USEDEFAULT,                              // ウィンドウの左角のX座標
    CW_USEDEFAULT,                              // ウィンドウの左角のY座標
    rcAdjustWindow.right - rcAdjustWindow.left, // ウィンドウの幅
    rcAdjustWindow.bottom - rcAdjustWindow.top, // ウィンドウの高さ
    NULL,                                       // 親ウィンドウのハンドル(親ルートなのでNULL)
    NULL,                                       // メニュー、子ウィンドウのID
    _hInst,                                     // メイン関数の引数のインスタンスハンドル
    NULL                                        // 追加引数(なし)
    );

  timeBeginPeriod(1); // 分解能1msに設定
}

//==============================================================================
// system
//------------------------------------------------------------------------------
Window::~Window() {
  timeEndPeriod(1);
}

//==============================================================================
// run
//------------------------------------------------------------------------------
int Window::run() {
  // メッセージ変数を用意
  MSG msg;

  // FPSのため変数
  DWORD dwExecLastTime = 0; // 任意のFPS制御するために
  DWORD dwFPSLastTime = 0;  // 現在のFPS計測するために
  DWORD dwCurrentTime = 0;  // 現在時刻格納用
  DWORD dwFrameCount = 0;   // 任意秒の間に更新描画がされた回数を格納

  // ウィンドウ表示
  ShowWindow(_hWnd,SW_SHOWDEFAULT);	// 非クライアント領域のみ表示
  UpdateWindow(_hWnd);			// クライアント領域が表示される

  // メッセージループ
  for(;;) {
    if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)	// メッセージが来たとき
    {
      //---- Windows プログラム ----
      if(msg.message == WM_QUIT) {
        break;
      }
      else {
        TranslateMessage(&msg);	// キー入力メッセージを文字メッセージに変換
        DispatchMessage(&msg);	// メッセージをWndProcにディスパッチ
      }
    }
    else	// メッセージが来ていないとき
    {
      //---- DirectX プログラム ----

      // FPS計測(500msに何回更新描画が行われたか)
      dwCurrentTime = timeGetTime();
      if((dwCurrentTime - dwFPSLastTime) >= 500) {
        // FPSカウント更新
        _nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

        // 次の計測のために初期化
        dwFPSLastTime = dwCurrentTime;
        dwFrameCount = 0;
      }

      // 任意のFPSで更新、描画を行う
      if((dwCurrentTime - dwExecLastTime) >= (1000 / 60.f))	// 60FPS
      {
        dwExecLastTime = dwCurrentTime;

        update();

        if(_bExitApplication) {
          if(MessageBox(NULL,"アプリケーションを終了しますか","終了確認",MB_YESNO | MB_ICONQUESTION) == IDYES) {
            DestroyWindow(_hWnd);	// WM_DESTROYメッセージを発行
          }

          _bExitApplication = false;
        }

        dwFrameCount++;
      }
    }
  }

  return 0;
}

//==============================================================================
// WinProc
//------------------------------------------------------------------------------
LRESULT CALLBACK Window::winProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
  switch(uMsg) {
  case WM_KEYDOWN:
    switch(wParam) {
    case VK_ESCAPE:
      if(MessageBox(hWnd,"アプリケーションを終了しますか","終了確認",MB_YESNO | MB_ICONQUESTION) == IDYES) {
        DestroyWindow(hWnd);
      }
      break;
    }
    break;

  case WM_CLOSE:
    if(MessageBox(hWnd,"アプリケーションを終了しますか","終了確認",MB_YESNO | MB_ICONQUESTION) == IDYES) {
      DestroyWindow(hWnd);
    }
    return 0;
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//==============================================================================
// title
//------------------------------------------------------------------------------
void Window::setTitle(const char* title) {
  SetWindowText(_hWnd,title);
}

//EOF