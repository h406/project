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
#include "app.h"

//==============================================================================
// constant
//------------------------------------------------------------------------------
namespace {
  const char* kClassName = "ShaderTest";
  const char* kDefaultTitle = "ShaderTest";

  const int kFPS = 60;
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
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

  // ウィンドウモード判定
  int res = MessageBox(NULL,"フルスクリーンにしますか？","確認",MB_YESNO);
  if(res == IDYES) {
    _isWindow = FALSE;
  }
  else {
    _isWindow = TRUE;
  }

  // 適正なウィンドウのサイズを求める
  DWORD windowStyle;
  if(_isWindow) {
    // ウィンドウモード
    windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
  } else {
    // フルスクリーンモード
    windowStyle = WS_POPUPWINDOW;
  }

  ShowCursor(FALSE);

  // ウィンドウサイズの計算(非クライアント領域考慮)
  RECT rcAdjustWindow = {0,0,(LONG)width,(LONG)height};	// クライアント領域サイズを指定
  AdjustWindowRect(&rcAdjustWindow,windowStyle,NULL);	// ウィンドウサイズを自動調整

  // ウィンドウクラスの登録
  RegisterClassEx(&wcex);

  // ウィンドウの生成
  _hWnd = CreateWindowEx(
    0,                                          // 拡張ウィンドウクラス
    kClassName,                                 // ウィンドウクラスの名前
    kDefaultTitle,                              // ウィンドウ枠に表示されるタイトル
    windowStyle,                                // ウィンドウのスタイル、全面に表示
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
  
  //FPS カウント用
  DWORD nStartTime = timeGetTime();
  DWORD nLastUpdateTime = timeGetTime();
  DWORD nCountFrames = 0;
  DWORD nCurrentTime = 0;
  // bool bAdjustFps = false;
  bool isDrawSkip = false;
  _nCountFPS = 0;

  // ウィンドウ表示
  ShowWindow(_hWnd,SW_SHOWDEFAULT);	// 非クライアント領域のみ表示
  UpdateWindow(_hWnd);			// クライアント領域が表示される

  for(;;) {
    if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)	// メッセージが来たとき
    {
      //---- Windows プログラム ----
      if(msg.message == WM_QUIT) {
        _bExitApplication = true;
        break;
      }
      else {
        TranslateMessage(&msg);	// キー入力メッセージを文字メッセージに変換
        DispatchMessage(&msg);	// メッセージをWndProcにディスパッチ
      }
    }
    else {
      nCurrentTime = timeGetTime();
      // FPS計算
      double fps = double(nCountFrames) / (nCurrentTime - nStartTime) * 1000.0;

      // 一秒間で一回リセット
      if(nCountFrames >= kFPS) {
        nCountFrames = 0;
        nStartTime = nCurrentTime;
      }

      // FPS
      _nCountFPS = static_cast<int>(fps + 0.5f);

      // 任意のFPSで更新、描画を行う
      if(fps <= kFPS)
      {
        nLastUpdateTime = nCurrentTime;

        // 更新
        update();

        // 描画処理
        if(!isDrawSkip)
        {
          draw();
        }

        // 終了フラグ
        if(_bExitApplication) {
          // WM_DESTROYメッセージを発行
          DestroyWindow(_hWnd);
          return 0;
        }

        // 時間かかりすぎ
        if((timeGetTime() - nLastUpdateTime) >= unsigned(1000.f / float(kFPS) + 0.5f)) {
          // 2回連続ではスキップしない
          isDrawSkip = !isDrawSkip;
        }
        else {
          isDrawSkip = false;
        }
        
        // １フレームカウント
        ++nCountFrames;
      }
      else {
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
      if(!App::instance().isWindow()|| MessageBox(hWnd,"アプリケーションを終了しますか","終了確認",MB_YESNO | MB_ICONQUESTION) == IDYES) {
        DestroyWindow(hWnd);
      }
      break;
    }
    break;

  case WM_CLOSE:
    if(!App::instance().isWindow() || MessageBox(hWnd,"アプリケーションを終了しますか","終了確認",MB_YESNO | MB_ICONQUESTION) == IDYES) {
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