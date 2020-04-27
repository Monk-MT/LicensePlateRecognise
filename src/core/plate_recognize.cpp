#include "LicensePlateRecognise/core/plate_recognize.h"
#include "LicensePlateRecognise/config.h"
#include "thirdparty/textDetect/erfilter.hpp"

namespace LicensePlateRecognise {

CPlateRecognize::CPlateRecognize() { 
  m_showResult = false;
}


// 主函数，车牌识别，包含两个部分
// 1. 车牌检测
// 2. 字符识别
int CPlateRecognize::plateRecognize(const Mat& src, std::vector<CPlate> &plateVecOut, int img_index) {
  // 调整为通一大小
  float scale = 1.f;
  Mat img = uniformResize(src, scale);

  // 1. 车牌检测
  std::vector<CPlate> plateVec;
  int resultPD = plateDetect(img, plateVec, img_index);
  if (resultPD == 0) {
    size_t num = plateVec.size();
    for (size_t j = 0; j < num; j++) {
      CPlate& item = plateVec.at(j);
      Mat plateMat = item.getPlateMat();
      SHOW_IMAGE(plateMat, 0);

      // 缩放
      item.setPlateScale(scale);
      RotatedRect rect = item.getPlatePos();
      item.setPlatePos(scaleBackRRect(rect, 1.f / scale));

      // 得到车牌颜色
      Color color = item.getPlateColor();
      if (color == UNKNOWN) {
        color = getPlateType(plateMat, true);
        item.setPlateColor(color);
      }
      std::string plateColor = getPlateColor(color);
      if (0) {
        std::cout << "plateColor:" << plateColor << std::endl;
      }

      // 2. 字符识别
      std::string plateIdentify = "";
      int resultCR = charsRecognise(item, plateIdentify);
      if (resultCR == 0) {
        std::string license = plateColor + ":" + plateIdentify;
        item.setPlateStr(license);
        plateVecOut.push_back(item);
        if (0) std::cout << "resultCR:" << resultCR << std::endl;
      }
      else {
        std::string license = plateColor;
        item.setPlateStr(license);
        plateVecOut.push_back(item);
        if (0) std::cout << "resultCR:" << resultCR << std::endl;
      }
    }
  }
  return resultPD;
}
}