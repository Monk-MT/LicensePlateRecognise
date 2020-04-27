#ifndef LPR_CORE_PLATERECOGNIZE_H_
#define LPR_CORE_PLATERECOGNIZE_H_

#include "LicensePlateRecognise/core/plate_detect.h"
#include "LicensePlateRecognise/core/chars_recognise.h"


namespace LicensePlateRecognise {

  class CPlateRecognize : public CPlateDetect, public CCharsRecognise {
  public:
    CPlateRecognize();

    int plateRecognize(const Mat& src, std::vector<CPlate> &plateVec, int img_index = 0);
    int plateRecognize(const Mat& src, std::vector<std::string> &licenseVec);

    inline void setLifemode(bool param) { CPlateDetect::setPDLifemode(param); }
    inline void setDetectType(int param) { CPlateDetect::setDetectType(param); }

    inline void setResultShow(bool param) { m_showResult = param; }
    inline bool getResultShow() const { return m_showResult; }
    inline void setDebug(bool param) { setResultShow(param); }

  private:
    bool m_showResult;
    DISABLE_ASSIGN_AND_COPY(CPlateRecognize);
  };

} 

#endif  // LPR_CORE_PLATERECOGNIZE_H_
