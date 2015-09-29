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

  // �������[�N���o�t���O
  //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

  // �E�B���h�E�T�C�Y�̌v�Z(��N���C�A���g�̈�l��)
  RECT rcAdjustWindow = {0,0,(LONG)width,(LONG)height};	// �N���C�A���g�̈�T�C�Y���w��
  DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);			// �E�B���h�E�̃X�^�C��
  AdjustWindowRect(&rcAdjustWindow,dwStyle,NULL);	// �E�B���h�E�T�C�Y����������

  // �E�B���h�E�N���X�̓o�^
  RegisterClassEx(&wcex);

  // �E�B���h�E�̐���
  _hWnd = CreateWindowEx(
    0,                                          // �g���E�B���h�E�N���X
    kClassName,                                 // �E�B���h�E�N���X�̖��O
    kDefaultTitle,                              // �E�B���h�E�g�ɕ\�������^�C�g��
    dwStyle,                                    // �E�B���h�E�̃X�^�C���A�S�ʂɕ\��
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

  // FPS�̂��ߕϐ�
  DWORD dwExecLastTime = 0; // �C�ӂ�FPS���䂷�邽�߂�
  DWORD dwFPSLastTime = 0;  // ���݂�FPS�v�����邽�߂�
  DWORD dwCurrentTime = 0;  // ���ݎ����i�[�p
  DWORD dwFrameCount = 0;   // �C�ӕb�̊ԂɍX�V�`�悪���ꂽ�񐔂��i�[

  // �E�B���h�E�\��
  ShowWindow(_hWnd,SW_SHOWDEFAULT);	// ��N���C�A���g�̈�̂ݕ\��
  UpdateWindow(_hWnd);			// �N���C�A���g�̈悪�\�������

  // ���b�Z�[�W���[�v
  for(;;) {
    if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)	// ���b�Z�[�W�������Ƃ�
    {
      //---- Windows �v���O���� ----
      if(msg.message == WM_QUIT) {
        break;
      }
      else {
        TranslateMessage(&msg);	// �L�[���̓��b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
        DispatchMessage(&msg);	// ���b�Z�[�W��WndProc�Ƀf�B�X�p�b�`
      }
    }
    else	// ���b�Z�[�W�����Ă��Ȃ��Ƃ�
    {
      //---- DirectX �v���O���� ----

      // FPS�v��(500ms�ɉ���X�V�`�悪�s��ꂽ��)
      dwCurrentTime = timeGetTime();
      if((dwCurrentTime - dwFPSLastTime) >= 500) {
        // FPS�J�E���g�X�V
        _nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

        // ���̌v���̂��߂ɏ�����
        dwFPSLastTime = dwCurrentTime;
        dwFrameCount = 0;
      }

      // �C�ӂ�FPS�ōX�V�A�`����s��
      if((dwCurrentTime - dwExecLastTime) >= (1000 / 60.f))	// 60FPS
      {
        dwExecLastTime = dwCurrentTime;

        update();

        if(_bExitApplication) {
          if(MessageBox(NULL,"�A�v���P�[�V�������I�����܂���","�I���m�F",MB_YESNO | MB_ICONQUESTION) == IDYES) {
            DestroyWindow(_hWnd);	// WM_DESTROY���b�Z�[�W�𔭍s
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
      if(MessageBox(hWnd,"�A�v���P�[�V�������I�����܂���","�I���m�F",MB_YESNO | MB_ICONQUESTION) == IDYES) {
        DestroyWindow(hWnd);
      }
      break;
    }
    break;

  case WM_CLOSE:
    if(MessageBox(hWnd,"�A�v���P�[�V�������I�����܂���","�I���m�F",MB_YESNO | MB_ICONQUESTION) == IDYES) {
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