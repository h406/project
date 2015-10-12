//
//  QRDetector.m
//  OpenCVテンプレート
//
//  Created by 北村 一真 on 11/01/17.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "QRDetector.h"

//-------------------------------------------------
//  name: findFinderPattern
//  desc: 
//-------------------------------------------------
CvSeq *findFinderPattern(IplImage *src) {
  IplImage *tmp = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
  CvMemStorage* _stor = cvCreateMemStorage(0);
  CvMemStorage* _stor_tmp = cvCreateMemStorage(0);

  CvSeq *seq_finder_pattern = cvCreateSeq(CV_SEQ_ELTYPE_GENERIC,
    sizeof(CvSeq),
    sizeof(CvBox2D),
    _stor);
  cvClearSeq(seq_finder_pattern);

  // for contour list
  cvClearMemStorage(_stor_tmp);
  CvSeq *cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT,
    sizeof(CvSeq),sizeof(CvPoint),
    _stor_tmp);

  //
  // Find all contours.
  //
  // cvFindContours() spoil source image.
  //
  cvCopy(src,tmp);
  cvFindContours(tmp,
    _stor_tmp,&cont,sizeof(CvContour),
    CV_RETR_LIST,CV_CHAIN_APPROX_NONE,cvPoint(0,0));

  // for marker candidates list
  CvSeq *candidates = cvCreateSeq(CV_SEQ_ELTYPE_GENERIC,
    sizeof(CvSeq),sizeof(ImageReaderCandidate),
    _stor_tmp);
  //
  // check each block
  //
  CvSeq *cont_head = cont;
  for(; cont; cont = cont->h_next) {
    CvRect feret = cvBoundingRect(cont);
    double area = fabs(cvContourArea(cont));
    double area_ratio = area / (double)(feret.width*feret.height);
    double feret_ratio = ((feret.width<feret.height) ?
      (double)feret.width / (double)feret.height :
      (double)feret.height / (double)feret.width);

    //
    // search square（正方形を探索）
    //
    if(area >= MIN_AREA && area_ratio >= MIN_AREA_RATIO && feret_ratio >= MIN_FERET_RATIO) {
      ImageReaderCandidate c;
      c.feret.x = feret.x;
      c.feret.y = feret.y;
      c.feret.width = feret.width;
      c.feret.height = feret.height;
      c.contour = cont;
      cvSeqPush(candidates,&c);
    }
    else {
      cvClearSeq(cont);
    }
  }
  cvRelease((void **)&cont_head);

  //
  // check each sqare has inner squire
  //
  int i;
  for(i = 0; i < candidates->total; i++) {
    ImageReaderCandidate *cand1 = (ImageReaderCandidate *)cvGetSeqElem(candidates,i);

    int inner_contour = 0;
    int j;
    for(j = 0; j < candidates->total; j++) {
      if(i == j) continue;

      ImageReaderCandidate *cand2 = (ImageReaderCandidate *)cvGetSeqElem(candidates,j);
      CvRect max_rect = cvMaxRect(&(cand1->feret),&(cand2->feret));
      if(cand1->feret.x == max_rect.x&&
        cand1->feret.y == max_rect.y&&
        cand1->feret.width == max_rect.width&&
        cand1->feret.height == max_rect.height)
        inner_contour++;
    }

    //
    // There were 2 squires (white and black) inside a squire,
    // the most outer squire assumed as position marker.
    //
    if(inner_contour == 2) {
      CvBox2D box = cvMinAreaRect2(cand1->contour);
      cvSeqPush(seq_finder_pattern,&box);
    }
  }

  //
  // clear buffers
  //
  for(i = 0;i<candidates->total;i++) {
    ImageReaderCandidate *cand1 = (ImageReaderCandidate *)cvGetSeqElem(candidates,i);
    cvClearSeq(cand1->contour);
  }
  cvRelease((void **)&candidates);
  cvClearMemStorage(_stor_tmp);
  cvReleaseImage(&tmp);

  return(seq_finder_pattern);
}


//-------------------------------------------------
//  name : findCodeAreaContour
// 
//-------------------------------------------------
CvPoint *findCodeAreaContour(IplImage *src,CvSeq *seq_finder_pattern,double th) {
  CvBox2D _finderpattern_boxes[3];
  memset(_finderpattern_boxes,0,sizeof(CvBox2D) * 3);

  CvMemStorage* _stor_tmp = cvCreateMemStorage(0);
  cvClearMemStorage(_stor_tmp);

  //
  // create position maker mask
  //
  CvBox2D box;
  CvPoint2D32f pt_32f[4];
  CvSeq *markers_vertex = cvCreateSeq(CV_SEQ_ELTYPE_POINT,
    sizeof(CvSeq),
    sizeof(CvPoint),
    _stor_tmp);

  int c = seq_finder_pattern->total,i;
  for(i = 0; i < c; i++) {

    box = *(CvBox2D *)cvGetSeqElem(seq_finder_pattern,i);
    _finderpattern_boxes[i] = box;

    //
    // get each position maker's vertex 
    //
    cvBoxPoints(box,pt_32f);
    for(int j = 0; j < 4; j++) {
      CvPoint p = cvPointFrom32f(pt_32f[j]);
      cvSeqPush(markers_vertex,&p);
    }
  }

  //
  // create Minimal-area bounding rectangle which condist
  // every position makers
  // 点列を包含する最小矩形をboxに格納
  //
  box = cvMinAreaRect2(markers_vertex);
  cvRelease((void **)&markers_vertex);
  CvPoint2D32f box_point[4];
  cvBoxPoints(box,box_point);

  //
  // create code area mask
  // pointsに四隅の座標がはいる
  //
  cvBoxPoints(box,pt_32f);
  CvPoint *points = new CvPoint[4];
  for(i = 0; i < 4; i++) {
    //squarePoints[i]=cvPointFrom32f(pt_32f[i]);
    points[i] = cvPointFrom32f(pt_32f[i]);
  }

  cvClearMemStorage(_stor_tmp);

  return points;
}

//-------------------------------------------------
//  name : cropImage
// 
//-------------------------------------------------
void cropImage(IplImage *src,IplImage *dst,CvPoint *p) {
  CvMat    src_point;
  double a[8];

  int j = 0;
  for(int i = 0; i < 4; ++i) {
    a[j++] = p[i].x;
    a[j++] = p[i].y;
  }
  src_point = cvMat(4,2,CV_64FC1,a);

  CvMat    dst_point;
  double    b[] = {
    0,0,
    0,dst->height - 1,
    dst->width - 1,dst->height - 1,
    dst->width - 1,0
  };
  dst_point = cvMat(4,2,CV_64FC1,b);

  CvMat    *h = cvCreateMat(3,3,CV_64FC1);
  cvFindHomography(&src_point,&dst_point,h);
  cvWarpPerspective(src,dst,h);
}


IplImage *detectQRcode(IplImage *src,int th) {
  IplImage *bin = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
  IplImage *crop = cvCreateImage(cvSize(256,256),IPL_DEPTH_8U,3);
  IplImage *tmp = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

  //
  // binarize
  //
  cvCvtColor(src,tmp,CV_BGR2GRAY);
  cvSmooth(tmp,bin,CV_MEDIAN,3);
  cvCopy(bin,tmp);
  cvThreshold(tmp,bin,th,255,CV_THRESH_BINARY_INV);
  cvAdaptiveThreshold(tmp,bin,128,CV_ADAPTIVE_THRESH_MEAN_C,
    CV_THRESH_BINARY_INV,DEFAULT_ADAPTIVE_TH_SIZE,DEFAULT_ADAPTIVE_TH_DELTA);
  //
  // find finder patterns from binarized image
  //　seq_finder_patternに3つの目玉の中心座標が入る
  // 
  CvSeq *seq_finder_pattern = findFinderPattern(bin);

  if(seq_finder_pattern->total != 3) {
    fprintf(stderr,"%d finder pattern can be detected:\n",seq_finder_pattern->total);
    exit(-1);
  }

  //
  // find code area from binarized image using finder patterns
  // 4隅の座標が戻り値として得られる
  //
  CvPoint *vertex
    = findCodeAreaContour(bin,seq_finder_pattern,FIND_CODE_AREA_POLY_APPROX_TH);

  //
  // cropImage
  // 
  //
  cropImage(src,crop,vertex);

  return crop;
}
