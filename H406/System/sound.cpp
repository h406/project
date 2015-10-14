//==============================================================================
//
// Sound[sound.cpp]
// Author : Yasuaki Yamashita : 2015/05/30
// �X�V�@2015/10/07
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "sound.h"

//==============================================================================
// �R���X�g���N�^
//------------------------------------------------------------------------------
Sound::Sound() {
  HRESULT hr;

  // �����o�ϐ�������
  _pXAudio2 = NULL;
  _pMasteringVoice = NULL;

  // COM���C�u�����̏�����
  CoInitializeEx(NULL,COINIT_MULTITHREADED);

  // XAudio2�I�u�W�F�N�g�̍쐬
  hr = XAudio2Create(&_pXAudio2,0);
  if(FAILED(hr)) {
    MessageBox(NULL,"XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I","�x���I",MB_ICONWARNING);
    // COM���C�u�����̏I������
    CoUninitialize();
    return ;
  }

  // �}�X�^�[�{�C�X�̐���
  hr = _pXAudio2->CreateMasteringVoice(&_pMasteringVoice);
  if(FAILED(hr)) {
    MessageBox(NULL,"�}�X�^�[�{�C�X�̐����Ɏ��s�I","�x���I",MB_ICONWARNING);
    if(_pXAudio2) {
      // XAudio2�I�u�W�F�N�g�̊J��
      _pXAudio2->Release();
      _pXAudio2 = NULL;
    }
    // COM���C�u�����̏I������
    CoUninitialize();
    return ;
  }

  // 0�Ԃ�NULL�f�[�^������
  SoundData nullData;
  memset(&nullData,NULL,sizeof(SoundData));
  _soundList.push_back(nullData);
}

//==============================================================================
// �f�X�g���N�^
//------------------------------------------------------------------------------
Sound::~Sound() {
  for(SoundData& soundData : _soundList) {
    if(soundData._apSourceVoice != NULL) {
      // �ꎞ��~
      soundData._apSourceVoice->Stop(0);

      // �\�[�X�{�C�X�̔j��
      soundData._apSourceVoice->DestroyVoice();
      soundData._apSourceVoice = NULL;

      // �I�[�f�B�I�f�[�^�̊J��
      free(soundData._apDataAudio);
      soundData._apDataAudio = NULL;
    }
  }

  _soundList.clear();
  _soundMap.clear();

  // �}�X�^�[�{�C�X�̔j��
  _pMasteringVoice->DestroyVoice();

  _pMasteringVoice = NULL;

  if(_pXAudio2) {
    // XAudio2�I�u�W�F�N�g�̊J��
    _pXAudio2->Release();
    _pXAudio2 = NULL;
  }

  // COM���C�u�����̏I������
  CoUninitialize();
}

//=============================================================================
// �`�����N�̃`�F�b�N
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

  if(SetFilePointer(hFile,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// �t�@�C���|�C���^��擪�Ɉړ�
    return HRESULT_FROM_WIN32(GetLastError());
  }

  while(hr == S_OK) {
    if(ReadFile(hFile,&dwChunkType,sizeof(DWORD),&dwRead,NULL) == 0) {// �`�����N�̓ǂݍ���
      hr = HRESULT_FROM_WIN32(GetLastError());
    }

    if(ReadFile(hFile,&dwChunkDataSize,sizeof(DWORD),&dwRead,NULL) == 0) {// �`�����N�f�[�^�̓ǂݍ���
      hr = HRESULT_FROM_WIN32(GetLastError());
    }

    switch(dwChunkType) {
    case 'FFIR':
      dwRIFFDataSize = dwChunkDataSize;
      dwChunkDataSize = 4;
      if(ReadFile(hFile,&dwFileType,sizeof(DWORD),&dwRead,NULL) == 0) {// �t�@�C���^�C�v�̓ǂݍ���
        hr = HRESULT_FROM_WIN32(GetLastError());
      }
      break;

    default:
      if(SetFilePointer(hFile,dwChunkDataSize,NULL,FILE_CURRENT) == INVALID_SET_FILE_POINTER) {// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//------------------------------------------------------------------------------
HRESULT Sound::readChunkData(HANDLE hFile,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset) {
  DWORD dwRead;

  if(SetFilePointer(hFile,dwBufferoffset,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// �t�@�C���|�C���^���w��ʒu�܂ňړ�
    return HRESULT_FROM_WIN32(GetLastError());
  }

  if(ReadFile(hFile,pBuffer,dwBuffersize,&dwRead,NULL) == 0) {// �f�[�^�̓ǂݍ���
    return HRESULT_FROM_WIN32(GetLastError());
  }

  return S_OK;
}

//==============================================================================
// ���[�h
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

  // �o�b�t�@�̃N���A
  memset(&wfx,0,sizeof(WAVEFORMATEXTENSIBLE));
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));

  // �T�E���h�f�[�^�t�@�C���̐���
  hFile = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
  if(hFile == INVALID_HANDLE_VALUE) {
    MessageBox(NULL,"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
    return 0;
  }
  if(SetFilePointer(hFile,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) {// �t�@�C���|�C���^��擪�Ɉړ�
    MessageBox(NULL,"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
    return 0;
  }

  // WAVE�t�@�C���̃`�F�b�N
  hr = checkChunk(hFile,'FFIR',&dwChunkSize,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
    return 0;
  }
  hr = readChunkData(hFile,&dwFiletype,sizeof(DWORD),dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
    return 0;
  }
  if(dwFiletype != 'EVAW') {
    MessageBox(NULL,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)","�x���I",MB_ICONWARNING);
    return 0;
  }

  // �t�H�[�}�b�g�`�F�b�N
  hr = checkChunk(hFile,' tmf',&dwChunkSize,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
    return 0;
  }
  hr = readChunkData(hFile,&wfx,dwChunkSize,dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
    return 0;
  }

  // �I�[�f�B�I�f�[�^�ǂݍ���
  hr = checkChunk(hFile,'atad',&soundData._aSizeAudio,&dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
    return 0;
  }
  soundData._apDataAudio = (BYTE*)malloc(soundData._aSizeAudio);
  hr = readChunkData(hFile,soundData._apDataAudio,soundData._aSizeAudio,dwChunkPosition);
  if(FAILED(hr)) {
    MessageBox(NULL,"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
    return 0;
  }

  // �\�[�X�{�C�X�̐���
  hr = _pXAudio2->CreateSourceVoice(&soundData._apSourceVoice,&(wfx.Format));
  if(FAILED(hr)) {
    MessageBox(NULL,"�\�[�X�{�C�X�̐����Ɏ��s�I","�x���I",MB_ICONWARNING);
    return 0;
  }

  // �o�b�t�@�̒l�ݒ�
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
  buffer.AudioBytes = soundData._aSizeAudio;
  buffer.pAudioData = soundData._apDataAudio;
  buffer.Flags = XAUDIO2_END_OF_STREAM;
  buffer.LoopCount = 0;

  // �I�[�f�B�I�o�b�t�@�̓o�^
  soundData._apSourceVoice->SubmitSourceBuffer(&buffer);

  // ���X�g�ǉ�
  _soundList.push_back(soundData);

  // �ǉ�
  const int id = _soundList.size() - 1;
  _soundMap.insert(pair<string,int>(filePath,id));

  return id;
}

//==============================================================================
// Sound ID �擾
//------------------------------------------------------------------------------
int Sound::getId(const char* filepath) {
  auto it = _soundMap.find(filepath);
  if(it == _soundMap.end()) {
    return load(filepath);
  }
  return it->second;
}

//==============================================================================
// �Z�O�����g�Đ�
//------------------------------------------------------------------------------
bool Sound::play(const char* filePath,bool loop) {
  return play(getId(filePath),loop);
}

//==============================================================================
// �Z�O�����g�Đ�
//------------------------------------------------------------------------------
bool Sound::play(int id,bool loop) {
  XAUDIO2_VOICE_STATE xa2state;
  XAUDIO2_BUFFER buffer;

  if(id == 0) return FALSE;

  // �o�b�t�@�̒l�ݒ�
  memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
  buffer.AudioBytes = _soundList[id]._aSizeAudio;
  buffer.pAudioData = _soundList[id]._apDataAudio;
  buffer.Flags = XAUDIO2_END_OF_STREAM;
  buffer.LoopCount = loop == TRUE ? XAUDIO2_LOOP_INFINITE : 0;

  // ��Ԏ擾
  _soundList[id]._apSourceVoice->GetState(&xa2state);
  if(xa2state.BuffersQueued != 0) {// �Đ���
    // �ꎞ��~
    _soundList[id]._apSourceVoice->Stop(0);

    // �I�[�f�B�I�o�b�t�@�̍폜
    _soundList[id]._apSourceVoice->FlushSourceBuffers();
  }

  // �I�[�f�B�I�o�b�t�@�̓o�^
  _soundList[id]._apSourceVoice->SubmitSourceBuffer(&buffer);

  // �Đ�
  _soundList[id]._apSourceVoice->Start(0);

  return TRUE;
}

//==============================================================================
// �Z�O�����g��~
//------------------------------------------------------------------------------
void Sound::stop(const char* filePath) {
  stop(getId(filePath));
}

//==============================================================================
// �Z�O�����g��~
//------------------------------------------------------------------------------
void Sound::stop(int id) {
  XAUDIO2_VOICE_STATE xa2state;

  if(id == 0) return;

  // ��Ԏ擾
  _soundList[id]._apSourceVoice->GetState(&xa2state);
  if(xa2state.BuffersQueued != 0) {// �Đ���
    // �ꎞ��~
    _soundList[id]._apSourceVoice->Stop(0);

    // �I�[�f�B�I�o�b�t�@�̍폜
    _soundList[id]._apSourceVoice->FlushSourceBuffers();
  }
}

//==============================================================================
// �Z�O�����g��~
//------------------------------------------------------------------------------
void Sound::stop(void) {
  // �ꎞ��~
  for(SoundData& soundData : _soundList) {
    if(soundData._apSourceVoice) {
      // �ꎞ��~
      soundData._apSourceVoice->Stop(0);
    }
  }
}

//EOF