#ifndef LPR_CORE_COREFUNC_H_
#define LPR_CORE_COREFUNC_H_

#include "opencv2/opencv.hpp"
#include "LicensePlateRecognise/core/plate.hpp"
#include "LicensePlateRecognise/core/character.hpp"

using namespace cv;
using namespace std;

namespace LicensePlateRecognise {

//�� ��������ɫƥ��Ķ�ֵ��ͼ��
//�� ����RGB��Ҫƥ����ɫ����ɫ���ɫ��
//�� �����ɫֵ��ƥ��Ϊ255����ƥ��Ϊ0
Mat colorMatch(const Mat& src, Mat& match, const Color r,
               const bool adaptive_minsv);

bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv,
                     float& percent);

//�� Ѱ�ұ߽�
bool bFindLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight);
bool bFindLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight);
bool bFindLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight);

//�� ȥ������
bool clearLiuDing(Mat& img);
void clearLiuDingOnly(Mat& img);
void clearLiuDing(Mat mask, int& top, int& bottom);

Color getPlateType(const Mat& src, const bool adaptive_minsv);

Mat histeq(Mat in);
Rect GetCenterRect(Mat& in);
Mat CutTheRect(Mat& in, Rect& rect);
int ThresholdOtsu(Mat mat);

//�� ֱ��ͼ
Mat ProjectedHistogram(Mat img, int t, int threshold = 20);

Mat showHistogram(const Mat& hist);

Mat preprocessChar(Mat in, int char_size);

Rect GetChineseRect(const Rect rectSpe);

//�� ��С�ж�
bool verifyCharSizes(Rect r);
bool verifyPlateSize(Rect mr);
bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug = false);

// �Ǽ���ֵ����
void NMStoCharacter(std::vector<CCharacter> &inVec, double overlap);

// ������ת����Χ����
void rotatedRectangle(InputOutputArray img, RotatedRect rect,
  const Scalar& color, int thickness = 1,
  int lineType = LINE_8, int shift = 0);

//�� �ռ�otsu�㷨
void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type = BLUE);

// ��ͼ��С
Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio);

// ��С��ת��ľ���
RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio);

//! �������ɵĺ�ѡ�ַ�ɸѡ��С
void mserCharMatch(const Mat &src, std::vector<Mat> &match, std::vector<CPlate>& out_plateVec_blue, std::vector<CPlate>& out_plateVec_yellow,
  bool usePlateMser, std::vector<RotatedRect>& out_plateRRect_blue, std::vector<RotatedRect>& out_plateRRect_yellow, int index = 0, bool showDebug = false);

bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height, const float thresh, float& result);
float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height);

bool computeIOU(const Rect& rect1, const Rect& rect2, const float thresh, float& result);
float computeIOU(const Rect& rect1, const Rect& rect2);

// �ӵ㵽ͼ��ת��
Mat adaptive_image_from_points(const std::vector<Point>& points,
  const Rect& rect, const Size& size, const Scalar& backgroundColor = Scalar(0, 0, 0),
  const Scalar& forgroundColor = Scalar(255, 255, 255), bool gray = true);

// ��һ��������ȵľ���
Rect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight = false);

// ���㰲ȫ����
bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src,
  Rect_<float>& safeBoundRect);
bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height,
  Rect_<float> &safeBoundRect);

// ����ͼ��Ϊͳһ��С
Mat uniformResize(const Mat &result, float& scale);

// ��������Ϊͳһ��С
Mat uniformResizePlates(const Mat &result, float& scale);

// �����ַ�����
Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height);
Rect rectFit(const Rect &src, const int mat_width, const int mat_height);

// ͼƬд����ʱ�ļ�
void writeTempImage(const Mat& outImg, const string path, int index = 0);

// �Ƴ��߽����
bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result, const float thresh = 1.f,
                    bool useExtendHeight = false);

// ������±߽�
void clearBorder(const Mat &img, Rect& cropRect);

//! һά�Ǽ���ֵ�����㷨
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
