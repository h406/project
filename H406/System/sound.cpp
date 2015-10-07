//==============================================================================
//
// Sound[sound.cpp]
// Author : Yasuaki Yamashita : 2015/05/30
// 更新　2015/10/07
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "sound.h"

//==============================================================================
// コンストラクタ
//------------------------------------------------------------------------------
Sound::Sound() {
  HRESULT hr;

  // メンバ変数初期化
  _pXAudio2 = NULL;
  _pMasteringVoice = NULL;

  // COMライブラリの初期化
  CoInitializeEx(NULL,COINIT_MULTITHREADED);

  // XAudio2オブジェクトの作成
  hr = XAudio2Create(&_pXAudio2,0);
  if(FAILED(hr)) {
    MessageBox(NULL,"XAudio2オブジェクトの作成に失敗！","警告！",MB_ICONWARNING);
    // COMライブラリの終了処理
    CoUninitialize();
    return ;
  }

  // マスターボイスの生成
  hr = _pXAudio2->CreateMasteringVoice(&_pMasteringVoice);
  if(FAILED(hr)) {
    MessageBox(NULL,"マスターボイスの生成に失敗！","警告！",MB_ICONWARNING);
    if(_pXAudio2) {
      // XAudio2オブジェクトの開放
      _pXAudio2->Release();
      _pXAudio2 = NULL;
    }
    // COMライブラリの終了処理
    CoUninitialize();
    return ;
  }

  // 0番にNULLデータを入れる
  SoundData nullData;
  memset(&nullData,NULL,sizeof(SoundData));
  _soundList.push_back(nullData);
}

//==============================================================================
// デストラクタ
//------------------------------------------------------------------------------
Sound::~Sound() {
  for(SoundData& soundData : _soundList) {
    if(soundData._apSourceVoice != NULL) {
      // 一時停止
      soundData._apSourceVoice->Stop(0);

      // ソースボイスの破棄
      soundData._apSourceVoice->DestroyVoice();
      soundData._apSourceVoice = NULL;

      // オーディオデータの開放
      free(soundData._apDataAudio);
      soundData._apDataAudio = NULL;
    }
  }

  _soundList.clear();
  _soundMap.clear();

  // マスターボイスの破棄
  _pMasteringVoice->DestroyVoice();

  _pMasteringVoice = NULL;

  if(_pXAudio2) {
    // XAudio2オブジェクトの開放
    _pXAudio2->Release();
    _pXAudio2 = NULL;
  }

  // COMライブラリの終了処理
  CoUninitialize();
}

//=============================================================================
// チャンクのチェック
//------------------------------------------------------------------------------
HRESULT Sound::checkChunk(HANDLE hFile,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition) {
  HRESULT hr = S_OK;
  DWORD dwRead;
  DWORD dwChunkType;
  DWORD dwChunkDataSize;
  DWORD dwRIFFDataSize = 0;
  DWORD dwFileType;
  DWORD dwBytesRead = 0;
  DWORD dwOffset = 0;

  if(SetFilePointer(hFile,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// ファイルポインタを先頭に移動
    return HRESULT_FROM_WIN32(GetLastError());
  }

  while(hr == S_OK) {
    if(ReadFile(hFile,&dwChunkType,sizeof(DWORD),&dwRead,NULL) == 0) {// チャンクの読み込み
      hr = HRESULT_FROM_WIN32(GetLastError());
    }

    if(ReadFile(hFile,&dwChunkDataSize,sizeof(DWORD),&dwRead,NULL) == 0) {// チャンクデータの読み込み
      hr = HRESULT_FROM_WIN32(GetLastError());
    }

    switch(dwChunkType) {
    case 'FFIR':
      dwRIFFDataSize = dwChunkDataSize;
      dwChunkDataSize = 4;
      if(ReadFile(hFile,&dwFileType,sizeof(DWORD),&dwRead,NULL) == 0) {// ファイルタイプの読み込み
        hr = HRESULT_FROM_WIN32(GetLastError());
      }
      break;

    default:
      if(SetFilePointer(hFile,dwChunkDataSize,NULL,FILE_CURRENT) == INVALID_SET_FILE_POINTER) {// ファイルポインタをチャンクデータ分移動
        return HRESULT_FROM_WIN32(GetLastError());
      }
    }

    dwOffset += sizeof(DWORD) * 2;
    if(dwChunkType == format) {
      *pChunkSize = dwChunkDataSize;
      *pChunkDataPosition = dwOffset;

      return S_OK;
    }

    dwOffset += dwChunkDataSize;
    if(dwBytesRead >= dwRIFFDataSize) {
      return S_FALSE;
    }
  }

  return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//------------------------------------------------------------------------------
HRESULT Sound::readChunkData(HANDLE hFile,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset) {
  DWORD dwRead;

  if(SetFilePointer(hFile,dwBufferoffset,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// ファイルポインタを指定位置まで移動
    return HRESULT_FROM_WIN32(GetLastError());
  }

  if(ReadFile(hFile,pBuffer,dwBuffersize,&dwRead,NULL) == 0) {// データの読み込み
    return HRESULT_FROM_WIN32(GetLastError());
  }

  return S_OK;
}

//==============================================================================
// ロード
//------------------------------------------------------------------------------
int Sound::load(const char* filePath) {
  SoundData soundData;
  HANDLE hFile;
  DWORD dwChunkSize = 0;
  DWORD dwChunkPosition = 0;
  DWORD dwFiletype;
  WAVEFORMATEXTENSIBLE wfx;
  XAUDIO2_BUFFER buffer;
  HRESULT hr;

  const auto it = _soundMap.find(filePath);
  if(it != _soundMap.end()) {
    return it->second;
  }

  // バッファのクリア
  memset(&wfx,0,sizeof(WAVEFORMATEXTENSIBLE));
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));

  // サウンドデータファイルの生成
  hFile = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
  if(hFile == INVALID_HANDLE_VALUE) {
    MessageBox(NULL,"サウンドデータファイルの生成に失敗！(1)","警告！",MB_ICONWARNING);
    return 0;
  }
  if(SetFilePointer(hFile,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// ファイルポインタを先頭に移動
    MessageBox(NULL,"サウンドデータファイルの生成に失敗！(2)","警告！",MB_ICONWARNING);
    return 0;
  }

  // WAVEファイルのチェック
  hr = checkChunk(hFile,'FFIR',&dwChunkSize,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"WAVEファイルのチェックに失敗！(1)","警告！",MB_ICONWARNING);
    return 0;
  }
  hr = readChunkData(hFile,&dwFiletype,sizeof(DWORD),dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"WAVEファイルのチェックに失敗！(2)","警告！",MB_ICONWARNING);
    return 0;
  }
  if(dwFiletype != 'EVAW') {
    MessageBox(NULL,"WAVEファイルのチェックに失敗！(3)","警告！",MB_ICONWARNING);
    return 0;
  }

  // フォーマットチェック
  hr = checkChunk(hFile,' tmf',&dwChunkSize,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"フォーマットチェックに失敗！(1)","警告！",MB_ICONWARNING);
    return 0;
  }
  hr = readChunkData(hFile,&wfx,dwChunkSize,dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"フォーマットチェックに失敗！(2)","警告！",MB_ICONWARNING);
    return 0;
  }

  // オーディオデータ読み込み
  hr = checkChunk(hFile,'atad',&soundData._aSizeAudio,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"オーディオデータ読み込みに失敗！(1)","警告！",MB_ICONWARNING);
    return 0;
  }
  soundData._apDataAudio = (BYTE*)malloc(soundData._aSizeAudio);
  hr = readChunkData(hFile,soundData._apDataAudio,soundData._aSizeAudio,dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"オーディオデータ読み込みに失敗！(2)","警告！",MB_ICONWARNING);
    return 0;
  }

  // ソースボイスの生成
  hr = _pXAudio2->CreateSourceVoice(&soundData._apSourceVoice,&(wfx.Format));
  if(FAILED(hr)) {
    MessageBox(NULL,"ソースボイスの生成に失敗！","警告！",MB_ICONWARNING);
    return 0;
  }

  // バッファの値設定
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
  buffer.AudioBytes = soundData._aSizeAudio;
  buffer.pAudioData = soundData._apDataAudio;
  buffer.Flags = XAUDIO2_END_OF_STREAM;
  buffer.LoopCount = 0;

  // オーディオバッファの登録
  soundData._apSourceVoice->SubmitSourceBuffer(&buffer);

  // リスト追加
  _soundList.push_back(soundData);

  // 追加
  const int id = _soundList.size() - 1;
  _soundMap.insert(pair<string,int>(filePath,id));

  return id;
}

//==============================================================================
// Sound ID 取得
//------------------------------------------------------------------------------
int Sound::getId(const char* filepath) {
  auto it = _soundMap.find(filepath);
  if(it == _soundMap.end()) {
    return load(filepath);
  }
  return it->second;
}

//==============================================================================
// セグメント再生
//------------------------------------------------------------------------------
bool Sound::play(const char* filePath,bool loop) {
  return play(getId(filePath),loop);
}

//==============================================================================
// セグメント再生
//------------------------------------------------------------------------------
bool Sound::play(int id,bool loop) {
  XAUDIO2_VOICE_STATE xa2state;
  XAUDIO2_BUFFER buffer;

  if(id == 0) return FALSE;

  // バッファの値設定
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
  buffer.AudioBytes = _soundList[id]._aSizeAudio;
  buffer.pAudioData = _soundList[id]._apDataAudio;
  buffer.Flags = XAUDIO2_END_OF_STREAM;
  buffer.LoopCount = loop == TRUE ? XAUDIO2_LOOP_INFINITE : 0;

  // 状態取得
  _soundList[id]._apSourceVoice->GetState(&xa2state);
  if(xa2state.BuffersQueued != 0) {// 再生中
    // 一時停止
    _soundList[id]._apSourceVoice->Stop(0);

    // オーディオバッファの削除
    _soundList[id]._apSourceVoice->FlushSourceBuffers();
  }

  // オーディオバッファの登録
  _soundList[id]._apSourceVoice->SubmitSourceBuffer(&buffer);

  // 再生
  _soundList[id]._apSourceVoice->Start(0);

  return TRUE;
}

//==============================================================================
// セグメント停止
//------------------------------------------------------------------------------
void Sound::stop(const char* filePath) {
  stop(getId(filePath));
}

//==============================================================================
// セグメント停止
//------------------------------------------------------------------------------
void Sound::stop(int id) {
  XAUDIO2_VOICE_STATE xa2state;

  if(id == 0) return;

  // 状態取得
  _soundList[id]._apSourceVoice->GetState(&xa2state);
  if(xa2state.BuffersQueued != 0) {// 再生中
    // 一時停止
    _soundList[id]._apSourceVoice->Stop(0);

    // オーディオバッファの削除
    _soundList[id]._apSourceVoice->FlushSourceBuffers();
  }
}

//==============================================================================
// セグメント停止
//------------------------------------------------------------------------------
void Sound::stop(void) {
  // 一時停止
  for(SoundData& soundData : _soundList) {
    if(soundData._apSourceVoice) {
      // 一時停止
      soundData._apSourceVoice->Stop(0);
    }
  }
}

//EOF