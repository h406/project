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

  // �������[�N���o�t���O
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  //�E�B���h�E�N���X�쐬
  WNDCLASSEX wcex =
  {
    sizeof(WNDCLASSEX),           // �E�B���h�E�̗e�ʂ�`����
    CS_CLASSDC,                   // �N���X�̃X�^�C��
    winProc,                      // �J�X�^���E�B���h�E�v���V�[�W���ݒ�
    0,                            // �g��Ȃ�
    0,                            // �g��Ȃ�
    _hInst,                       // ���C���֐��̈����ł���C���X�^���X�n���h��
    NULL,                         // ���[�W�A�C�R�����w��
    LoadCursor(NULL,IDC_ARROW),   //�J�[�\���̎�ނ��w��
    (HBRUSH)(COLOR_WINDOW + 1),   //�w�i�F
    NULL,
    kClassName,                   // ���̃E�B���h�E�N���X�̖��O���w��
    NULL                          // �X���[���A�C�R�����w��
  };

  // �E�B���h�E���[�h����
  int res = MessageBox(NULL,"�t���X�N���[���ɂ��܂����H","�m�F",MB_YESNO);
  if(res == IDYES) {
    _isWindow = FALSE;
  }
  else {
    _isWindow = TRUE;
  }

  // �K���ȃE�B���h�E�̃T�C�Y�����߂�
  DWORD windowStyle;
  if(_isWindow) {
    // �E�B���h�E���[�h
    windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
  } else {
    // �t���X�N���[�����[�h
    windowStyle = WS_POPUPWINDOW;
  }

  ShowCursor(FALSE);

  // �E�B���h�E�T�C�Y�̌v�Z(��N���C�A���g�̈�l��)
  RECT rcAdjustWindow = {0,0,(LONG)width,(LONG)height};	// �N���C�A���g�̈�T�C�Y���w��
  AdjustWindowRect(&rcAdjustWindow,windowStyle,NULL);	// �E�B���h�E�T�C�Y����������

  // �E�B���h�E�N���X�̓o�^
  RegisterClassEx(&wcex);

  // �E�B���h�E�̐���
  _hWnd = CreateWindowEx(
    0,                                          // �g���E�B���h�E�N���X
    kClassName,                                 // �E�B���h�E�N���X�̖��O
    kDefaultTitle,                              // �E�B���h�E�g�ɕ\�������^�C�g��
    windowStyle,                                // �E�B���h�E�̃X�^�C���A�S�ʂɕ\��
    CW_USEDEFAULT,                              // �E�B���h�E�̍��p��X���W
    CW_USEDEFAULT,                              // �E�B���h�E�̍��p��Y���W
    rcAdjustWindow.right - rcAdjustWindow.left, // �E�B���h�E�̕�
    rcAdjustWindow.bottom - rcAdjustWindow.top, // �E�B���h�E�̍���
    NULL,                                       // �e�E�B���h�E�̃n���h��(�e���[�g�Ȃ̂�NULL)
    NULL,                                       // ���j���[�A�q�E�B���h�E��ID
    _hInst,                                     // ���C���֐��̈����̃C���X�^���X�n���h��
    NULL                                        // �ǉ�����(�Ȃ�)
    );

  timeBeginPeriod(1); // ����\1ms�ɐݒ�


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
  // ���b�Z�[�W�ϐ���p��
  MSG msg;
  
  //FPS �J�E���g�p
  DWORD nStartTime = timeGetTime();
  DWORD nLastUpdateTime = timeGetTime();
  DWORD nCountFrames = 0;
  DWORD nCurrentTime = 0;
  // bool bAdjustFps = false;
  bool isDrawSkip = false;
  _nCountFPS = 0;

  // �E�B���h�E�\��
  ShowWindow(_hWnd,SW_SHOWDEFAULT);	// ��N���C�A���g�̈�̂ݕ\��
  UpdateWindow(_hWnd);			// �N���C�A���g�̈悪�\�������

  for(;;) {
    if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)	// ���b�Z�[�W�������Ƃ�
    {
      //---- Windows �v���O���� ----
      if(msg.message == WM_QUIT) {
        _bExitApplication = true;
        break;
      }
      else {
        TranslateMessage(&msg);	// �L�[���̓��b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
        DispatchMessage(&msg);	// ���b�Z�[�W��WndProc�Ƀf�B�X�p�b�`
      }
    }
    else {
      nCurrentTime = timeGetTime();
      // FPS�v�Z
      double fps = double(nCountFrames) / (nCurrentTime - nStartTime) * 1000.0;

      // ��b�Ԃň�񃊃Z�b�g
      if(nCountFrames >= kFPS) {
        nCountFrames = 0;
        nStartTime = nCurrentTime;
      }

      // FPS
      _nCountFPS = static_cast<int>(fps + 0.5f);

      // �C�ӂ�FPS�ōX�V�A�`����s��
      if(fps <= kFPS)
      {
        nLastUpdateTime = nCurrentTime;

        // �X�V
        update();

        // �`�揈��
        if(!isDrawSkip)
        {
          draw();
        }

        // �I���t���O
        if(_bExitApplication) {
          // WM_DESTROY���b�Z�[�W�𔭍s
          DestroyWindow(_hWnd);
          return 0;
        }

        // ���Ԃ����肷��
        if((timeGetTime() - nLastUpdateTime) >= unsigned(1000.f / float(kFPS) + 0.5f)) {
          // 2��A���ł̓X�L�b�v���Ȃ�
          isDrawSkip = !isDrawSkip;
        }
        else {
          isDrawSkip = false;
        }
        
        // �P�t���[���J�E���g
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
      if(!App::instance().isWindow()|| MessageBox(hWnd,"�A�v���P�[�V�������I�����܂���","�I���m�F",MB_YESNO | MB_ICONQUESTION) == IDYES) {
        DestroyWindow(hWnd);
      }
      break;
    }
    break;

  case WM_CLOSE:
    if(!App::instance().isWindow() || MessageBox(hWnd,"�A�v���P�[�V�������I�����܂���","�I���m�F",MB_YESNO | MB_ICONQUESTION) == IDYES) {
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