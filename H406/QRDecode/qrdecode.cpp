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
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "QRDetector.h"
#include "qrdecode.h"

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow) {

  // �J��������̃r�f�I�L���v�`����������
  CvCapture *videoCapture = NULL;

  // Web�J�������擾��
  for(int i = 0;videoCapture == NULL;i++) {
    videoCapture = cvCreateCameraCapture(i);
    if(5 < i) return -1;
  }

  // �E�C���h�E���쐬
  char windowName[] = "camera";
  cvNamedWindow(windowName,CV_WINDOW_AUTOSIZE);

  // Decoder��p��(decodeqr.h)
  QrDecoderHandle decoder = qr_decoder_open();

  IplImage *qrimg;
  int c;

  while(1) {
    // Esc�L�[�������ꂽ��I��
    c = cvWaitKey(2);
    if(c == '\x1b') break;

    // �J��������1�t���[�����擾
    IplImage *img = cvQueryFrame(videoCapture);

    if(img != 0) {

      try {
        // QR�R�[�h�̃f�R�[�h����
        qr_decoder_set_image_buffer(decoder,img);

        if(!qr_decoder_is_busy(decoder)) {
          short sz = 25;
          short stat = qr_decoder_decode(decoder,sz);

          for(sz = 25,stat = 0; (sz >= 3) && ((stat & QR_IMAGEREADER_DECODED) == 0); sz -= 2)
            stat = qr_decoder_decode(decoder,sz);
        }

        // QR�R�[�h����e�L�X�g���擾
        int text_size = 0;
        unsigned char* text = new unsigned char[text_size];

        QrCodeHeader header;
        if(qr_decoder_get_header(decoder,&header)) {
          if(text_size < header.byte_size + 1) {
            if(text) delete[] text;
            text_size = header.byte_size + 1;
            text = new unsigned char[text_size];
          }
          qr_decoder_get_body(decoder,text,text_size);

          // �摜�Ɏ擾�e�L�X�g��`��
          CvFont dfont;
          float hscale = 1.0f;
          float vscale = 1.0f;
          cvInitFont(&dfont,CV_FONT_HERSHEY_COMPLEX,hscale,vscale);
          cvPutText(img,(char*)text,cvPoint(65,60),&dfont,CV_RGB(0,255,0));
        }
        delete[] text;

      }
      catch(...) {

        // �摜��Error��`��
        CvFont dfont;
        float hscale = 1.0f;
        float vscale = 1.0f;
        cvInitFont(&dfont,CV_FONT_HERSHEY_COMPLEX,hscale,vscale);
        cvPutText(img,"Error",cvPoint(65,60),&dfont,CV_RGB(255,0,0));
      }

      // �E�B���h�E�ɉ摜��\��
      cvShowImage(windowName,img);
    }
  }
  qr_decoder_close(decoder);

  // ���
  cvReleaseImage(&qrimg);
  cvReleaseCapture(&videoCapture);

  // �E�B���h�E��j��
  cvDestroyWindow(windowName);

  return 0;
}

//EOF