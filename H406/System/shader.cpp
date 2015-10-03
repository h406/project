//==============================================================================
//
// shader[shader.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "shader.h"
#include "renderer.h"
#include "iShader.h"

//==============================================================================
// Shader
//------------------------------------------------------------------------------
Shader::Shader(Renderer* renderer)
  :_nowPixShader(nullptr)
  ,_nowVtxShader(nullptr)
  ,_renderer(renderer) {


  //TODO shader生成
  /*
  // 生成
  createVtxShader("vs_model.fx");
  createVtxShader("vs_fog.fx");
  createVtxShader("vs_velocity.fx");
  createVtxShader("vs_far.fx");
  createVtxShader("vs_parpix.fx");

  // 
  createPixelShader("ps_original.fx");
  createPixelShader("ps_akarui.fx");
  createPixelShader("ps_motionBlur.fx");
  createPixelShader("ps_far.fx");
  createPixelShader("ps_parpix.fx");
  */
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
Shader::~Shader() {
  for(auto& obj : _pixShaderList) {
    SafeRelease(obj->_constTable);
    SafeRelease(obj->_shader);
    SafeDelete(obj);
  }
  _pixShaderList.clear();
  _pixShaderMap.clear();

  for(auto& obj : _vtxShaderList) {
    SafeRelease(obj->_constTable);
    SafeRelease(obj->_shader);
    SafeDelete(obj);
  }
  _vtxShaderList.clear();
  _vtxShaderMap.clear();
}


inline VtxShader* Shader::getVtxShader(unsigned int id) const {
  return _vtxShaderList[id];
}

inline VtxShader* Shader::getVtxShader(const char* fileName) const {
  auto it = _vtxShaderMap.find(fileName);
  if(it == _vtxShaderMap.end()) return nullptr;
  return getVtxShader(it->second);
}

inline PixShader* Shader::getPixShader(unsigned int id) const {
  return _pixShaderList[id];
}

inline PixShader* Shader::getPixShader(const char* fileName) const {
  auto it = _pixShaderMap.find(fileName);
  if(it == _pixShaderMap.end()) return nullptr;
  return getPixShader(it->second);
}


//==============================================================================
// Shader
//------------------------------------------------------------------------------
void Shader::setVtxShader(const char* fileName) {
  auto pDevice = App::instance().getRenderer()->getDevice();

  if(fileName == nullptr) {
    pDevice->SetVertexShader(nullptr);
    _nowVtxShader = nullptr;
    return;
  }

  if(_nowVtxShader != nullptr && _nowVtxShader->filename == fileName) {
    return;
  }
  auto it = _vtxShaderMap.find(fileName);

  if(it == _vtxShaderMap.end()) {
    return;
  }

  setVtxShader(it->second);
}

//==============================================================================
// shader
//------------------------------------------------------------------------------
void Shader::setVtxShader(unsigned int id) {
  auto pDevice = App::instance().getRenderer()->getDevice();
  _nowVtxShader = _vtxShaderList[id];
  pDevice->SetVertexShader(_nowVtxShader->_shader);
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
void Shader::setPixShader(const char* fileName) {
  auto pDevice = App::instance().getRenderer()->getDevice();
  if(fileName == nullptr) {
    pDevice->SetPixelShader(nullptr);
    _nowPixShader = nullptr;
    return;
  }
  if(_nowPixShader != nullptr && _nowPixShader->filename == fileName) {
    return;
  }

  auto it = _pixShaderMap.find(fileName);

  if(it == _pixShaderMap.end()) {
    return;
  }

  setPixShader(it->second);
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
void Shader::setPixShader(unsigned int id) {
  auto pDevice = App::instance().getRenderer()->getDevice();
  _nowPixShader = _pixShaderList[id];
  pDevice->SetPixelShader(_nowPixShader->_shader);
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
unsigned int Shader::createPixShader(const char * file) {
  HRESULT hr;
  LPD3DXBUFFER err;
  LPD3DXBUFFER code;
  auto pDevice = _renderer->getDevice();
  PixShader* pixelShader = new PixShader();

  // Pixelシェーダコンパイル
  hr = D3DXCompileShaderFromFile(
    file,
    nullptr,
    nullptr,
    "PS",
    "ps_2_0",
    0,
    &code,
    &err,
    &pixelShader->_constTable);
  if(FAILED(hr)) {
    MessageBoxA(NULL,(LPCSTR)err->GetBufferPointer(),("D3DXCompileShaderFromFile"),MB_OK);
    err->Release();
    return 0;
  }
  // ピクセルシェーダ生成
  hr = pDevice->CreatePixelShader(
    (DWORD*)code->GetBufferPointer(),
    &pixelShader->_shader);
  if(FAILED(hr)) {
    MessageBox(NULL,TEXT("PixelShaderFailed"),TEXT("CreatePixelShader"),MB_OK);
    return 0;
  }
  code->Release();

  pixelShader->filename = file;

  // インサート
  _pixShaderList.push_back(pixelShader);
  _pixShaderMap.insert(std::pair<std::string,unsigned int>(file,_pixShaderList.size() - 1));

  return _pixShaderList.size() - 1;
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
unsigned int Shader::createVtxShader(const char * file) {
  HRESULT hr;
  LPD3DXBUFFER err;
  LPD3DXBUFFER code;
  auto pDevice = _renderer->getDevice();

  VtxShader* vtxShader = new VtxShader();

  // Vtxシェーダコンパイル
  hr = D3DXCompileShaderFromFile(
    file,
    nullptr,
    nullptr,
    "VS",
    "vs_2_0",
    0,
    &code,
    &err,
    &vtxShader->_constTable);
  if(FAILED(hr)) {
    MessageBox(NULL,(LPCSTR)err->GetBufferPointer(),TEXT("D3DXCompileShaderFromFile"),MB_OK);
    err->Release();
    return 0;
  }
  // 頂点シェーダ生成
  hr = pDevice->CreateVertexShader(
    (DWORD*)code->GetBufferPointer(),
    &vtxShader->_shader);
  if(FAILED(hr)) {
    MessageBox(NULL,TEXT("VertexShaderFailed"),TEXT("CreateVertexShader"),MB_OK);
    return 0;
  }
  code->Release();

  vtxShader->filename = file;

  // インサート
  _vtxShaderList.push_back(vtxShader);
  _vtxShaderMap.insert(std::pair<std::string,unsigned int>(file,_vtxShaderList.size() - 1));

  return _vtxShaderList.size() - 1;
}
//EOF