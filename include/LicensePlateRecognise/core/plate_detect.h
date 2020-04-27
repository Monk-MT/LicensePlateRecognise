#ifndef LPR_CORE_PLATEDETECT_H_
#define LPR_CORE_PLATEDETECT_H_

#include "LicensePlateRecognise/core/plate_locate.h"
#include "LicensePlateRecognise/core/plate_judge.h"

namespace LicensePlateRecognise {

class CPlateDetect {
 public:
  CPlateDetect();

  ~CPlateDetect();

  int plateDetect(Mat src, std::vector<CPlate> &resultVec, int type,
                  bool showDetectArea, int img_index = 0);


  int plateDetect(Mat src, std::vector<CPlate> &resultVec, int img_index = 0);


  inline void setPDLifemode(bool param) { m_plateLocate->setLifemode(param); }

  inline bool getPDDebug() { return m_plateLocate->getDebug(); }

  inline void setDetectType(int param) { m_type = param; }

  inline void setGaussianBlurSize(int param) {
    m_plateLocate->setGaussianBlurSize(param);
  }

  inline int getGaussianBlurSize() const {
    return m_plateLocate->getGaussianBlurSize();
  }

  inline void setMorphSizeWidth(int param) {
    m_plateLocate->setMorphSizeWidth(param);
  }

  inline int getMorphSizeWidth() const {
    return m_plateLocate->getMorphSizeWidth();
  }

  inline void setMorphSizeHeight(int param) {
    m_plateLocate->setMorphSizeHeight(param);
  }

  inline int getMorphSizeHeight() const {
    return m_plateLocate->getMorphSizeHeight();
  }

  inline void setVerifyError(float param) {
    m_plateLocate->setVerifyError(param);
  }

  inline float getVerifyError() const {
    return m_plateLocate->getVerifyError();
  }

  inline void setVerifyAspect(float param) {
    m_plateLocate->setVerifyAspect(param);
  }

  inline float getVerifyAspect() const {
    return m_plateLocate->getVerifyAspect();
  }

  inline void setVerifyMin(int param) { m_plateLocate->setVerifyMin(param); }

  inline void setVerifyMax(int param) { m_plateLocate->setVerifyMax(param); }

  inline void setJudgeAngle(int param) { m_plateLocate->setJudgeAngle(param); }

  inline void setMaxPlates(int param) { m_maxPlates = param; }

  inline int getMaxPlates() const { return m_maxPlates; }


 private:

  int m_maxPlates;

  CPlateLocate* m_plateLocate;

  int m_type;

  static std::string m_pathSvm;

};

}

#endif  // LPR_CORE_PLATEDETECT_H_
