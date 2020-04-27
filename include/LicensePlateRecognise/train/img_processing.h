#ifndef LPR_IMG_PROCESSING_H_
#define LPR_IMG_PROCESSING_H_

#include "opencv2/opencv.hpp"
#include "LicensePlateRecognise/config.h"

using namespace cv;
using namespace std;


namespace LicensePlateRecognise {

  // ÒÆ¶¯Í¼Æ¬
  Mat translateImg(Mat img, int offsetx, int offsety, int bk = 0);
  // Ðý×ªÍ¼Æ¬
  Mat rotateImg(Mat source, float angle, int bk = 0);

  // ¼ô²ÃÍ¼Æ¬
  Mat cropImg(Mat src, int x, int y, int shift, int bk = 0);

  Mat generateSyntheticImage(const Mat& image, int use_swap = 1);

} 

#endif  // LPR_IMG_PROCESSING_H_
