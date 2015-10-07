//=============================================================================
//
// Sound[sound.cpp]
// Author : Hara Eisuke : 2015/10/07
// 更新　2015/10/07
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <xaudio2.h>

struct SoundData {
  // ソースボイス
  IXAudio2SourceVoice *_apSourceVoice;
  // オーディオデータ
  BYTE *_apDataAudio;
  // オーディオデータサイズ
  DWORD _aSizeAudio;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Sound {
public:
  // コンストラクタ
  Sound();
  // デストラクタ
  ~Sound();

  // チャンクのチェック
  HRESULT checkChunk(HANDLE hFile,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition);
  // チャンクデータの読み込み
  HRESULT readChunkData(HANDLE hFile,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset);

  // ロード
  int load(const char* filePath);
  // Sound ID 取得
  int getId(const char* filepath);

  // セグメント再生
  bool play(const char* filePath,bool loop);
  bool play(int id,bool loop);

  // セグメント停止
  void stop(const char* filePath);
  void stop(int id);
  void stop(void);

  IXAudio2* getDevice() const { return _pXAudio2; }

private:
  // XAudio2オブジェクトへのインターフェイス
  IXAudio2* _pXAudio2;
  // マスターボイス
  IXAudio2MasteringVoice *_pMasteringVoice;

  vector <SoundData> _soundList;
  map<string,int>   _soundMap;
};

#endif