//==============================================================================
//
// Renderer[renderer.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "app.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "effect.h"

//==============================================================================
// renderer
//------------------------------------------------------------------------------
Renderer::Renderer(const App* app) {
  
  // �f�o�C�X
  createDevice(app->getWindowSize(), app->getHWnd());

  // �e�N�X�`���}�l�[�W��
  _texture = new Texture(this);

  // �J����
  _camera = new Camera();

  // �V�F�[�_
  _shader = new Shader(this);

  // effect
  _effect = new Effect(this);
}

//==============================================================================
// �f�o�C�X����
//------------------------------------------------------------------------------
void Renderer::createDevice(const SIZE& windowSize, HWND hWnd) {

  D3DDISPLAYMODE d3ddm;
  D3DPRESENT_PARAMETERS d3dpp;	// �f�B�X�v���C�ւ̏o�͕��@�����߂���

  // Direct3D�I�u�W�F�N�g�̐���
  LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);

  if(pD3D == NULL) {
    MessageBox(NULL,"Direct3D�I�u�W�F�N�g�̐����Ɏ��s���܂����B","�G���[�ʒm",MB_OK);
    return;
  }

  // ���݂̃f�B3�X�v���C���[�h���擾
  if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm))) {
    return;
  }

  ZeroMemory(&d3dpp,sizeof(d3dpp));	// 0��bit��������
  d3dpp.BackBufferWidth = windowSize.cx;	// �o�b�t�@�[�̕�
  d3dpp.BackBufferHeight = windowSize.cy;	// �o�b�t�@�[�̍���
  d3dpp.BackBufferFormat = d3ddm.Format;	// �J���[���[�h�̎w��
  d3dpp.BackBufferCount = 1;				// �o�b�t�@�[�̐�
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �o�b�t�@�؂�ւ����f�B�X�v���C�h���C�o�ɈϔC
  d3dpp.Windowed = TRUE;				// �E�B���h�E/�t���X�N���[�����w��
  d3dpp.EnableAutoDepthStencil = TRUE;	// �[�x�X�e���V���o�b�t�@���g�p
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// 16bit�Ɏw��
  d3dpp.FullScreen_RefreshRateInHz = 0;  // ���̃E�B���h�E������̂ŁA���[�g���w�肵�Ȃ�
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // �`��𑦎��Ɏw��(���t���b�V�����[�g�Ƃ̕s��v���l��)
  d3dpp.hDeviceWindow = hWnd;

  // �f�o�C�X�I�u�W�F�N�g�̐���
  // [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
  if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    nullptr,
    D3DCREATE_HARDWARE_VERTEXPROCESSING,
    &d3dpp,&_pD3DDevice))) {
    // �擾���s
    MessageBox(nullptr,TEXT("�f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���\r\n��������Â��\��������܂�"),TEXT("�x��"),MB_OK);
    SafeRelease(pD3D);
    return;
  }

  //---- �����_�[�X�e�[�g�̐ݒ� ----
  _pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	// �J�����O�̐ݒ�

  _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);			// �A���t�@�u�����h�̗L����
  _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// �\�[�X
  _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// �f�X�g
  _pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  _pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

  //---- �T���v���[�X�e�[�g�̐ݒ� ----
  // �e�N�X�`��UV�l�̌J��Ԃ��ݒ�(�e�N�X�`���̌J��Ԃ��`��w��)
  _pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  _pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
  // �e�N�X�`���̊g�k���̕�Ԑݒ�
  _pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);	// ���`����(�k��)
  _pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);	// ���`����(�g��)

  //---- �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� ----
  // �s�����e�N�X�`���Ƀ��l��K�p������
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); // ���߂��u�����h
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	// �����P�F�e�N�X�`�����w�� 
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);	// �����Q�F���݂̃|���S���̕s�����x���w��

  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  // ����Ȃ�
  pD3D->Release();

  // �v���W�F�N�V�����}�g���b�N�X
  D3DXMatrixIdentity(&_mtxProj);
  D3DXMatrixPerspectiveFovLH(&_mtxProj,		// �v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DX_PI / 4.0f,				// ����p
    (float)windowSize.cx / (float)windowSize.cy,	// �A�X�y�N�g��
    0.0f,						// rear�l
    1000.0f);					// far�l

  // �r���[�|�[�g�ݒ�
  D3DVIEWPORT9 vp;
  vp.X = 0;
  vp.Y = 0;
  vp.Width = (DWORD)windowSize.cx;
  vp.Height = (DWORD)windowSize.cy;
  vp.MaxZ = 1;
  vp.MinZ = 0;
  _pD3DDevice->SetViewport(&vp);
}

//==============================================================================
// Renderer
//------------------------------------------------------------------------------
Renderer::~Renderer() {
  SafeRelease(_pD3DDevice);
  SafeDelete(_texture);
  SafeDelete(_camera);
  SafeDelete(_shader);
  SafeDelete(_effect);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Renderer::update() {
  _camera->update();

  _effect->update();
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
bool Renderer::draw(node* baceNode) {
    // �V�[���̕`��J�n
  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    // �V�[���̃N���A
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(100,100,100),1.0f,0);

    // �x�[�X�s�N�Z���V�F�[�_�[
    _shader->setPixShader("ps_bace.cso");
    
    // �x�[�X�m�[�h
    if(baceNode != nullptr) {
      baceNode->drawChild(this);
    }

    // �G�t�F�N�g�\��
    _effect->draw();

    // �V�[���̕`��I��
    _pD3DDevice->EndScene();
    // �V�[���̕\��
    _pD3DDevice->Present(NULL,NULL,NULL,NULL);
  }

  return true;
}

//EOF