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
#include "node.h"
#include "postEffect.h"

//==============================================================================
// renderer
//------------------------------------------------------------------------------
bool Renderer::init(const App* app) {
  
  // �f�o�C�X
  createDevice(app->getWindowSize(), app->getHWnd());

  // �e�N�X�`���}�l�[�W��
  _texture = new Texture(this);

  // �J����
  _camera = new Camera();

  // �V�F�[�_
  _shader = new Shader(this);

  // �t�F�[�h
  _fadeBG = nullptr;

  // �|�X�g�G�t�F�N�g
  _postEffect = new PostEffect();

  return true;
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

  // ���݂̃f�B�X�v���C���[�h���擾
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

  _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);			// �A���t�@�u�����h�̗L����
  _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// �\�[�X
  _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// �f�X�g
  _pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  _pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0x66);
  _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  _pD3DDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);


  for(int i = 0; i < 4; ++i) {
    //---- �T���v���[�X�e�[�g�̐ݒ� ----
    // �e�N�X�`��UV�l�̌J��Ԃ��ݒ�(�e�N�X�`���̌J��Ԃ��`��w��)
    _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
    _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
    // �e�N�X�`���̊g�k���̕�Ԑݒ�
    _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);	// ���`����(�k��)
    _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);	// ���`����(�g��)

    //---- �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� ----
    // �s�����e�N�X�`���Ƀ��l��K�p������
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,D3DTOP_MODULATE); // ���߂��u�����h
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	// �����P�F�e�N�X�`�����w�� 
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAARG2,D3DTA_CURRENT);	// �����Q�F���݂̃|���S���̕s�����x���w��
  }


  // ����Ȃ�
  pD3D->Release();

  // �v���W�F�N�V�����}�g���b�N�X
  D3DXMatrixIdentity(&_mtxProj);
  D3DXMatrixPerspectiveFovLH(&_mtxProj,		// �v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DX_PI / 4.0f,				// ����p
    (float)windowSize.cx / (float)windowSize.cy,	// �A�X�y�N�g��
    10.0f,						// rear�l
    5000.0f);					// far�l

  // �r���[�|�[�g�ݒ�
  D3DVIEWPORT9 vp;
  vp.X = 0;
  vp.Y = 0;
  vp.Width = (DWORD)windowSize.cx;
  vp.Height = (DWORD)windowSize.cy;
  vp.MaxZ = 1;
  vp.MinZ = 0;
  _pD3DDevice->SetViewport(&vp);

  // �}���`�����_�[
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A16B16G16R16F,D3DPOOL_DEFAULT,&_TexNormalDepth,0);
  _TexNormalDepth->GetSurfaceLevel(0,&_SurNormalDepth);
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A16B16G16R16F,D3DPOOL_DEFAULT,&_TexPos,0);
  _TexPos->GetSurfaceLevel(0,&_SurPos);
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&_TexColor,0);
  _TexColor->GetSurfaceLevel(0,&_SurColor);
}

//==============================================================================
// Renderer
//------------------------------------------------------------------------------
Renderer::~Renderer() {

  SafeDelete(_texture);
  SafeDelete(_camera);
  SafeDelete(_shader);
  SafeDelete(_postEffect);

  SafeRelease(_SurNormalDepth);
  SafeRelease(_TexNormalDepth);
  SafeRelease(_SurPos);
  SafeRelease(_TexPos);
  SafeRelease(_TexColor);
  SafeRelease(_SurColor);
  SafeRelease(_pD3DDevice);

  if(_fadeBG != nullptr) {
    _fadeBG->release();
  }
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Renderer::update() {
  _camera->update();

  if(_fadeBG != nullptr) {
    _fadeBG->updateChild();
    _fadeBG->updateMtxChild();
  }

  _postEffect->update();
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
bool Renderer::draw(node* baceNode) {

  // �����_�[�^�[�Q�b�g�ݒ�
  _pD3DDevice->SetRenderTarget(1,_SurNormalDepth);
  _pD3DDevice->SetRenderTarget(2,_SurPos);
  _pD3DDevice->SetRenderTarget(3,_SurColor);

  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    // �V�[���̃N���A
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(0,0,0),1.0f,0);
    // �V�[���̕`��I��
    _pD3DDevice->EndScene();
  }

  _pD3DDevice->SetRenderTarget(1,nullptr);
  _pD3DDevice->SetRenderTarget(2,nullptr);
  _pD3DDevice->SetRenderTarget(3,nullptr);

  // �V�[���̕`��J�n
  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    
    // �V�[���̃N���A
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(0,0,0),1.0f,0);
    
    // �x�[�X�m�[�h
    if(baceNode != nullptr) {
      {
        // �x�[�X�s�N�Z���V�F�[�_�[
        _shader->setPixShader("ps_bace.cso");

        // ���ʂ�3D
        baceNode->drawChild(this,NodeType::lightOff3D);
      }

      // �V�[���̕`��I��
      _pD3DDevice->EndScene();
    }

    _pD3DDevice->SetRenderTarget(1,_SurNormalDepth);
    _pD3DDevice->SetRenderTarget(2,_SurPos);
    _pD3DDevice->SetRenderTarget(3,_SurColor);

    // �V�[���̕`��J�n
    if(SUCCEEDED(_pD3DDevice->BeginScene())) {
      // �x�[�X�s�N�Z���V�F�[�_�[
      _shader->setPixShader("ps_bace3d.cso");

      // far�l
      _shader->getNowPixShader()->_constTable->SetFloat(_pD3DDevice,"gFar",2000.0f);

      // ���ʂ�3D
      baceNode->drawChild(this,NodeType::normal3D);

      // �V�[���̕`��I��
      _pD3DDevice->EndScene();
    }

    _pD3DDevice->SetRenderTarget(1,nullptr);
    _pD3DDevice->SetRenderTarget(2,nullptr);
    _pD3DDevice->SetRenderTarget(3,nullptr);

    // �V�[���̕`��J�n
    if(SUCCEEDED(_pD3DDevice->BeginScene())) {

      _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
      // �|�X�g�G�t�F�N�g
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_POINT);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
      }
      _postEffect->draw(this);

      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetTexture(i,nullptr);
      }

      _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

      // �x�[�X�s�N�Z���V�F�[�_�[
      _shader->setPixShader("ps_bace.cso");

      // �G�t�F�N�g
      baceNode->drawChild(this,NodeType::effect);

      //---- �T���v���[�X�e�[�g�̐ݒ� ----
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
      }

      // ���ʂ�2D
      baceNode->drawChild(this,NodeType::normal2D);
    }

    // �t�F�[�h�p
    if(_fadeBG != nullptr) {
      _fadeBG->draw(this);
    }

    // �V�[���̕`��I��
    _pD3DDevice->EndScene();
    // �V�[���̕\��
    _pD3DDevice->Present(NULL,NULL,NULL,NULL);
  }

  return true;
}

//EOF