//==============================================================================
//
// qrdecode[qrdecode.cpp]
// Author : Yasuaki Yamashita : 2015/10/13
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************

#define _CRT_SECURE_NO_WARNINGS
#include "QRDetector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "decodeqr.h"

#pragma comment(lib, "WS2_32.LIB")


#ifdef _DEBUG
#pragma comment(lib, "libdecodeqr_d.lib")
#else
#pragma comment(lib, "libdecodeqr.lib")
#endif


#include "qrdecode.h"

//==============================================================================
// �R���X�g���N�^
//------------------------------------------------------------------------------
QRreader::QRreader() {
  // �J��������̃r�f�I�L���v�`����������
  _VideoCapture = NULL;
  
  try {
    // Web�J�������擾��
    for(int i = 0;_VideoCapture == NULL;i++) {
      _VideoCapture = cvCreateCameraCapture(i);
      if(5 < i) break;
    }
  }
  catch(...) {

  }

  // Decoder��p��(decodeqr.h)
  _decoder = qr_decoder_open();
}

//==============================================================================
// �f�X�g���N�^
//------------------------------------------------------------------------------
QRreader::~QRreader() {

  qr_decoder_close(_decoder);

  // ���
  //cvReleaseImage(&qrimg);
  if(_VideoCapture != NULL) {
    cvReleaseCapture(&_VideoCapture);
  }
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void QRreader::update() {
  // �J��������1�t���[�����擾
  if(_VideoCapture != NULL) {
    IplImage *img = cvQueryFrame(_VideoCapture);

    if(img != 0) {
      try {
        // QR�R�[�h�̃f�R�[�h����
        qr_decoder_set_image_buffer(_decoder,img);

        if(!qr_decoder_is_busy(_decoder)) {
          short sz = 25;
          short stat = qr_decoder_decode(_decoder,sz);

          for(sz = 25,stat = 0; (sz >= 3) && ((stat & QR_IMAGEREADER_DECODED) == 0); sz -= 2)
            stat = qr_decoder_decode(_decoder,sz);
        }

        // QR�R�[�h����e�L�X�g���擾
        int text_size = 0;
        unsigned char* text = new unsigned char[text_size];

        QrCodeHeader header;
        if(qr_decoder_get_header(_decoder,&header)) {
          if(text_size < header.byte_size + 1) {
            if(text) delete[] text;
            text_size = header.byte_size + 1;
            text = new unsigned char[text_size];
          }
          qr_decoder_get_body(_decoder,text,text_size);

          _string = (char*)text;

        }
        delete[] text;

      }
      catch(...) {

        //error
      }
    }
  }
}

//EOF