#ifndef LPR_CORE_COREFUNC_H_
#define LPR_CORE_COREFUNC_H_

#include "opencv2/opencv.hpp"
#include "LicensePlateRecognise/core/plate.hpp"
#include "LicensePlateRecognise/core/character.hpp"

using namespace cv;
using namespace std;

namespace LicensePlateRecognise {

//！ 查找与颜色匹配的二值化图像
//！ 输入RGB，要匹配颜色（蓝色或黄色）
//！ 输出灰色值，匹配为255，不匹配为0
Mat colorMatch(const Mat& src, Mat& match, const Color r,
               const bool adaptive_minsv);

bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv,
                     float& percent);

//！ 寻找边界
bool bFindLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight);
bool bFindLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight);
bool bFindLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight);

//！ 去除柳钉
bool clearLiuDing(Mat& img);
void clearLiuDingOnly(Mat& img);
void clearLiuDing(Mat mask, int& top, int& bottom);

Color getPlateType(const Mat& src, const bool adaptive_minsv);

Mat histeq(Mat in);
Rect GetCenterRect(Mat& in);
Mat CutTheRect(Mat& in, Rect& rect);
int ThresholdOtsu(Mat mat);

//！ 直方图
Mat ProjectedHistogram(Mat img, int t, int threshold = 20);

Mat showHistogram(const Mat& hist);

Mat preprocessChar(Mat in, int char_size);

Rect GetChineseRect(const Rect rectSpe);

//！ 大小判断
bool verifyCharSizes(Rect r);
bool verifyPlateSize(Rect mr);
bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug = false);

// 非极大值抑制
void NMStoCharacter(std::vector<CCharacter> &inVec, double overlap);

// 画出旋转的外围矩形
void rotatedRectangle(InputOutputArray img, RotatedRect rect,
  const Scalar& color, int thickness = 1,
  int lineType = LINE_8, int shift = 0);

//！ 空间otsu算法
void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type = BLUE);

// 大图缩小
Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio);

// 缩小旋转后的矩形
RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio);

//! 对先生成的候选字符筛选大小
void mserCharMatch(const Mat &src, std::vector<Mat> &match, std::vector<CPlate>& out_plateVec_blue, std::vector<CPlate>& out_plateVec_yellow,
  bool usePlateMser, std::vector<RotatedRect>& out_plateRRect_blue, std::vector<RotatedRect>& out_plateRRect_yellow, int index = 0, bool showDebug = false);

bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height, const float thresh, float& result);
float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height);

bool computeIOU(const Rect& rect1, const Rect& rect2, const float thresh, float& result);
float computeIOU(const Rect& rect1, const Rect& rect2);

// 从点到图的转换
Mat adaptive_image_from_points(const std::vector<Point>& points,
  const Rect& rect, const Size& size, const Scalar& backgroundColor = Scalar(0, 0, 0),
  const Scalar& forgroundColor = Scalar(255, 255, 255), bool gray = true);

// 画一个长宽相等的矩形
Rect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight = false);

// 计算安全矩形
bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src,
  Rect_<float>& safeBoundRect);
bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height,
  Rect_<float> &safeBoundRect);

// 调整图像为统一大小
Mat uniformResize(const Mat &result, float& scale);

// 调整车牌为统一大小
Mat uniformResizePlates(const Mat &result, float& scale);

// 扩大字符矩阵
Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height);
Rect rectFit(const Rect &src, const int mat_width, const int mat_height);

// 图片写入零时文件
void writeTempImage(const Mat& outImg, const string path, int index = 0);

// 移除边界的线
bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result, const float thresh = 1.f,
                    bool useExtendHeight = false);

// 清楚上下边界
void clearBorder(const Mat &img, Rect& cropRect);

//! 一维非极大值抑制算法
template<typename T>
void NMSfor1D(const vector<T>& arr, vector<int>& index) {

  int size = (int)arr.size();
  index.resize(size);
  for (int j = 0; j < size; j++)
    index.at(j) = 0;

  // nms
  int i = 1;
  while (i < size - 1) {
    if (arr.at(i) > arr.at(i + 1)) {
      if (arr.at(i) >= arr.at(i - 1))
        index.at(i) = 1;
    }
    else {
      while (i < size - 1 && arr.at(i) <= arr.at(i + 1))
        i = i + 1;
      if (i < size - 1)
        index.at(i) = 1;
    }
    i = i + 2;
  }
}

} 

#endif  // LPR_CORE_COREFUNC_H_
