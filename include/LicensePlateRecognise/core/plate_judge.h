#ifndef LPR_CORE_PLATEJUDGE_H_
#define LPR_CORE_PLATEJUDGE_H_

#include "LicensePlateRecognise/core/plate.hpp"
#include "LicensePlateRecognise/core/feature.h"

namespace LicensePlateRecognise {

class PlateJudge {
 public:
  static PlateJudge* instance();
  void LoadModel(std::string path);

  int plateJudgeUsingNMS(const std::vector<CPlate>&, std::vector<CPlate>&, int maxPlates = 5);
  int plateSetScore(CPlate& plate);

  int plateJudge(const Mat& plateMat);
  int plateJudge(const std::vector<Mat> &inVec,
    std::vector<Mat> &resultVec);
  int plateJudge(const std::vector<CPlate> &inVec,
    std::vector<CPlate> &resultVec);

 private:
  PlateJudge();

  static PlateJudge* instance_;

  svmCallback extractFeature;

  cv::Ptr<ml::SVM> svm_;

};
}

#endif  // LPR_CORE_PLATEJUDGE_H_
