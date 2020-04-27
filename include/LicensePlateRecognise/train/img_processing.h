#ifndef LPR_IMG_PROCESSING_H_
#define LPR_IMG_PROCESSING_H_

#include "opencv2/opencv.hpp"
#include "LicensePlateRecognise/config.h"

using namespace cv;
using namespace std;


namespace LicensePlateRecognise {

  // �ƶ�ͼƬ
  Mat translateImg(Mat img, int offsetx, int offsety, int bk = 0);
  // ��תͼƬ
  Mat rotateImg(Mat source, float angle, int bk = 0);

  // ����ͼƬ
  Mat cropImg(Mat src, int x, int y, int shift, int bk = 0);

  Mat generateSyntheticImage(const Mat& image, int use_swap = 1);

} 

#endif  // LPR_IMG_PROCESSING_H_
