//
//  QRDetector.h
//  OpenCVテンプレート
//
//  Created by 北村 一真 on 11/01/17.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>

#define DEFAULT_ADAPTIVE_TH_SIZE 25
#define DEFAULT_ADAPTIVE_TH_DELTA 10
#define MIN_AREA 49
#define MIN_AREA_RATIO .65
#define MIN_FERET_RATIO .7
#define FIND_CODE_AREA_POLY_APPROX_TH 50

IplImage* detectQRcode(IplImage *src, int th);

typedef struct{
	CvRect feret;
	CvSeq *contour;
} ImageReaderCandidate;

