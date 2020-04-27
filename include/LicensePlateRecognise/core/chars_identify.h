#ifndef LPR_CORE_CHARSIDENTIFY_H_
#define LPR_CORE_CHARSIDENTIFY_H_

#include <memory>
#include "opencv2/opencv.hpp"

#include "LicensePlateRecognise/util/kv.h"
#include "LicensePlateRecognise/core/character.hpp"
#include "LicensePlateRecognise/core/feature.h"

namespace LicensePlateRecognise {

class CharsIdentify {
public:
  static CharsIdentify* instance();

  int classify(cv::Mat f, float& maxVal, bool isChinses = false, bool isAlphabet = false);
  void classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs,
                std::vector<float>& out_maxVals, std::vector<bool> isChineseVec);
  void classify(std::vector<CCharacter>& charVec);

  void classifyChinese(std::vector<CCharacter>& charVec);
  void classifyChineseGray(std::vector<CCharacter>& charVec);

  std::pair<std::string, std::string> identify(cv::Mat input, bool isChinese = false, bool isAlphabet = false);
  int identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
               std::vector<bool> isChineseVec);

  std::pair<std::string, std::string> identifyChinese(cv::Mat input, float& result, bool& isChinese);
  std::pair<std::string, std::string> identifyChineseGray(cv::Mat input, float& result, bool& isChinese);

  bool isCharacter(cv::Mat input, std::string& label, float& maxVal, bool isChinese = false);

  void LoadModel(std::string path);
  void LoadChineseModel(std::string path);
  void LoadGrayChANN(std::string path);
  void LoadChineseMapping(std::string path);

private:
  CharsIdentify();
  annCallback extractFeature;
  static CharsIdentify* instance_;

  // �������ַ�������
  cv::Ptr<cv::ml::ANN_MLP> ann_;

  // ���Ķ������ַ�������
  cv::Ptr<cv::ml::ANN_MLP> annChinese_;

  // ���Ļ�ɫ�ַ�������
  cv::Ptr<cv::ml::ANN_MLP> annGray_;

  // ����ӳ��
  std::shared_ptr<Kv> kv_;
};
}

#endif  //  LPR_CORE_CHARSIDENTIFY_H_
