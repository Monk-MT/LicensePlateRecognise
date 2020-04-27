#ifndef LPR_CORE_CHARSRECOGNISE_H_
#define LPR_CORE_CHARSRECOGNISE_H_

#include "LicensePlateRecognise/core/chars_segment.h"
#include "LicensePlateRecognise/core/chars_identify.h"
#include "LicensePlateRecognise/core/core_func.h"
#include "LicensePlateRecognise/util/util.h"
#include "LicensePlateRecognise/core/plate.hpp"
#include "LicensePlateRecognise/config.h"

namespace LicensePlateRecognise {

class CCharsRecognise {
 public:
  CCharsRecognise();

  ~CCharsRecognise();

  int charsRecognise(cv::Mat plate, std::string& plateLicense);
  int charsRecognise(CPlate& plate, std::string& plateLicense);

  inline std::string getPlateColor(cv::Mat input) const {
    std::string color = "未知";
    Color result = getPlateType(input, true);
    if (BLUE == result) color = "蓝牌";
    if (YELLOW == result) color = "黄牌";
    if (WHITE == result) color = "白牌";
#ifdef OS_WINDOWS
    color = utils::utf8_to_gbk(color.c_str());
#endif
    return color;
  } 

  inline std::string getPlateColor(Color in) const {
    std::string color = "未知";
    if (BLUE == in) color = "蓝牌";
    if (YELLOW == in) color = "黄牌";
    if (WHITE == in) color = "白牌";
#ifdef OS_WINDOWS
    color = utils::utf8_to_gbk(color.c_str());
#endif
    return color;
  }

  inline void setLiuDingSize(int param) {
    m_charsSegment->setLiuDingSize(param);
  }
  inline void setColorThreshold(int param) {
    m_charsSegment->setColorThreshold(param);
  }
  inline void setBluePercent(float param) {
    m_charsSegment->setBluePercent(param);
  }
  inline float getBluePercent() const {
    return m_charsSegment->getBluePercent();
  }
  inline void setWhitePercent(float param) {
    m_charsSegment->setWhitePercent(param);
  }
  inline float getWhitePercent() const {
    return m_charsSegment->getWhitePercent();
  }

 private:
  //！字符分割

  CCharsSegment* m_charsSegment;
};

} 

#endif  // LPR_CORE_CHARSRECOGNISE_H_