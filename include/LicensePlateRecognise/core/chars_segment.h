#ifndef LPR_CORE_CHARSSEGMENT_H_
#define LPR_CORE_CHARSSEGMENT_H_

#include "opencv2/opencv.hpp"
#include "LicensePlateRecognise/config.h"

using namespace cv;
using namespace std;

namespace LicensePlateRecognise {

class CCharsSegment {
 public:
  CCharsSegment();
  //! 使用ostu分割字符
  int charsSegment(Mat input, std::vector<Mat>& resultVec, Color color = BLUE);

  //! 字符分割的方法
  int charsSegmentUsingOSTU(Mat input, std::vector<Mat>& resultVec, std::vector<Mat>& grayChars, Color color = BLUE);
  int charsSegmentUsingMSER(Mat input, vector<Mat>& resultVec, vector<Mat>& grayChars, Color color = BLUE);

  int projectSegment(const Mat& input, Color color, vector<int>& out_indexs);
 
  //! 字符大小验证
  bool verifyCharSizes(Mat r);

  //! 中文字符二值化方法
  void judgeChinese(Mat in, Mat& out, Color plateType);
  void judgeChineseGray(Mat in, Mat& out, Color plateType);

  Mat preprocessChar(Mat in);

  //! 找到中文字符位置
  Rect GetChineseRect(const Rect rectSpe);

  //! 找到特殊的定位字符，用来定位中文字符
  int GetSpecificRect(const std::vector<Rect>& vecRect);

  //! 删除特殊字符前面的框，并选中后面的六个框
  int RebuildRect(const std::vector<Rect>& vecRect, std::vector<Rect>& outRect,
                  int specIndex);

  int SortRect(const std::vector<Rect>& vecRect, std::vector<Rect>& out);

  inline void setLiuDingSize(int param) { m_LiuDingSize = param; }
  inline void setColorThreshold(int param) { m_ColorThreshold = param; }

  inline void setBluePercent(float param) { m_BluePercent = param; }
  inline float getBluePercent() const { return m_BluePercent; }
  inline void setWhitePercent(float param) { m_WhitePercent = param; }
  inline float getWhitePercent() const { return m_WhitePercent; }

  static const int DEFAULT_DEBUG = 1;

  static const int CHAR_SIZE = 20;
  static const int HORIZONTAL = 1;
  static const int VERTICAL = 0;

  static const int DEFAULT_LIUDING_SIZE = 7;
  static const int DEFAULT_MAT_WIDTH = 136;
  static const int DEFAULT_COLORTHRESHOLD = 150;

  inline void setDebug(int param) { m_debug = param; }

  inline int getDebug() { return m_debug; }

 private:

  int m_LiuDingSize;

  int m_theMatWidth;

  int m_ColorThreshold;
  float m_BluePercent;
  float m_WhitePercent;

  int m_debug;
};

}

#endif  // LPR_CORE_CHARSSEGMENT_H_
