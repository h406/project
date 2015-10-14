//=============================================================================
//
// Sound[sound.cpp]
// Author : Hara Eisuke : 2015/10/07
// �X�V�@2015/10/07
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <xaudio2.h>

struct SoundData {
  // �\�[�X�{�C�X
  IXAudio2SourceVoice *_apSourceVoice;
  // �I�[�f�B�I�f�[�^
  BYTE *_apDataAudio;
  // �I�[�f�B�I�f�[�^�T�C�Y
  DWORD _aSizeAudio;
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Sound {
public:
  // �R���X�g���N�^
  Sound();
  // �f�X�g���N�^
  ~Sound();

  // �`�����N�̃`�F�b�N
  HRESULT checkChunk(HANDLE hFile,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition);
  // �`�����N�f�[�^�̓ǂݍ���
  HRESULT readChunkData(HANDLE hFile,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset);

  // ���[�h
  int load(const char* filePath);
  // Sound ID �擾
  int getId(const char* filepath);

  // �Z�O�����g�Đ�
  bool play(const char* filePath,bool loop);
  bool play(int id,bool loop);

  // �Z�O�����g��~
  void stop(const char* filePath);
  void stop(int id);
  void stop(void);

  IXAudio2* getDevice() const { return _pXAudio2; }

private:
  // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
  IXAudio2* _pXAudio2;
  // �}�X�^�[�{�C�X
  IXAudio2MasteringVoice *_pMasteringVoice;

  vector <SoundData> _soundList;
  map<string,int>   _soundMap;
};

#endif