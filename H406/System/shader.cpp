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

namespace {
  inline bool isfile(const char *filename) {
    FILE  *fp = nullptr;
    fopen_s(&fp, filename,"r");
    if(fp == nullptr)
      return false;
    fclose(fp);
    return true;
  }

  // return のポインターは必ず消去
 inline DWORD* fileread(const char* filename) {
    FILE *fp = nullptr;
    long size;
    fopen_s(&fp,filename,"r");
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    DWORD* dat = new DWORD[size];
    fread_s(dat,size * sizeof(DWORD),size,1,fp);
    fclose(fp);
    return dat;
  }

  const string kBaceShaderFolder("./data/shader/");
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
Shader::Shader(Renderer* renderer)
  :_nowPixShader(nullptr)
  ,_nowVtxShader(nullptr)
  ,_renderer(renderer) {

  /// シェーダフォルダを全て読み込む
  namespace sys = std::tr2::sys;
  sys::path p(kBaceShaderFolder);
  std::for_each(sys::directory_iterator(p),sys::directory_iterator(),
    [this](const sys::path& p) {
    if(sys::is_regular_file(p)) { // ファイルなら...
      const string& file = p.filename();
      if(file[0] == 'p' && file[1] == 's') {
        createPixShader((kBaceShaderFolder + file).c_str());
      }
      else if(file[0] == 'v' && file[1] == 's') {
        createVtxShader((kBaceShaderFolder + file).c_str());
      }
    }
  });
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
  auto pDevice = _renderer->getDevice();
  PixShader* pixelShader = new PixShader();

  if(!isfile(file)) {
    MessageBoxA(NULL,"そんなファイルネェ","error",MB_OK);
    App::instance().exit();
    return 0;
  }

  DWORD* shader = fileread(file);

  // constTable取得
  hr = D3DXGetShaderConstantTable(shader, &pixelShader->_constTable);
  if(FAILED(hr)) {
    MessageBoxA(NULL,"よくわからんけどエラー",("D3DXGetShaderConstantTable"),MB_OK);
    App::instance().exit();
    SafeDeleteArray(shader);
    return 0;
  }

  // ピクセルシェーダ生成
  hr = pDevice->CreatePixelShader(shader, &pixelShader->_shader);
  if(FAILED(hr)) {
    MessageBox(NULL,TEXT("PixelShaderFailed"),TEXT("CreatePixelShader"),MB_OK);
    App::instance().exit();
    SafeDeleteArray(shader);
    return 0;
  }

  pixelShader->filename = file;

  // インサート
  _pixShaderList.push_back(pixelShader);
  _pixShaderMap.insert(std::pair<std::string,unsigned int>(file,_pixShaderList.size() - 1));

  SafeDeleteArray(shader);

  return _pixShaderList.size() - 1;
}

//==============================================================================
// Shader
//------------------------------------------------------------------------------
unsigned int Shader::createVtxShader(const char * file) {
  HRESULT hr = MB_OK;
  auto pDevice = _renderer->getDevice();
  VtxShader* vtxShader = new VtxShader();

  if(!isfile(file)) {
    MessageBoxA(NULL,"そんなファイルネェ","error",MB_OK);
    App::instance().exit();
    return 0;
  }

  DWORD* shader = fileread(file);

  // constTable取得
  //hr = D3DXGetShaderConstantTable(shader,&vtxShader->_constTable);
  if(FAILED(hr)) {
    MessageBoxA(NULL,"よくわからんけどエラー",("D3DXGetShaderConstantTable"),MB_OK);
    App::instance().exit();
    SafeDeleteArray(shader);
    return 0;
  }

  // 頂点シェーダ生成
  hr = pDevice->CreateVertexShader(shader, &vtxShader->_shader);
  if(FAILED(hr)) {
    MessageBox(NULL,TEXT("VertexShaderFailed"),TEXT("CreateVertexShader"),MB_OK);
    App::instance().exit();
    SafeDeleteArray(shader);
    return 0;
  }

  vtxShader->filename = file;

  // インサート
  _vtxShaderList.push_back(vtxShader);
  _vtxShaderMap.insert(std::pair<std::string,unsigned int>(file,_vtxShaderList.size() - 1));

  SafeDeleteArray(shader);
  return _vtxShaderList.size() - 1;
}
//EOF