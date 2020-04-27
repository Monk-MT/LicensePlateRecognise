#ifndef LPR_CORE_FEATURE_H_
#define LPR_CORE_FEATURE_H_

#include "opencv2/opencv.hpp"

using namespace cv;

namespace LicensePlateRecognise {

//! 获得车牌的特征数
cv::Mat getHistogram(cv::Mat in);

//! LicensePlateRecognise的getFeatures回调函数
//! 用于从车牌的image生成svm的训练特征features
typedef void (*svmCallback)(const cv::Mat& image, cv::Mat& features);

//! LicensePlateRecognise的getFeatures回调函数
//! 从图像转换为直方图，用于ann
typedef void (*annCallback)(const cv::Mat& image, cv::Mat& features);

//! 灰色特征
void getGrayPlusProject(const cv::Mat& grayChar, cv::Mat& features);

//!  LicensePlateRecognise的getFeatures回调函数
//! 获取垂直和水平的直方图图值
void getHistogramFeatures(const cv::Mat& image, cv::Mat& features);

//! 获取LBP特征
void getLBPFeatures(const cv::Mat& image, cv::Mat& features);

//! 获取颜色特征
void getColorFeatures(const cv::Mat& src, cv::Mat& features);

//! 获取颜色特征和直方图
void getHistomPlusColoFeatures(const cv::Mat& image, cv::Mat& features);

//! 获取字符特征
cv::Mat charFeatures(cv::Mat in, int sizeData);
cv::Mat charFeatures2(cv::Mat in, int sizeData);

//! 获取LBP特征和直方图特征
void getLBPplusHistFeatures(const cv::Mat& image, cv::Mat& features);

//! 获取灰色字符特征
void getGrayCharFeatures(const cv::Mat& grayChar, cv::Mat& features);

void getGrayPlusLBP(const Mat& grayChar, Mat& features);
} 

#endif  // LPR_CORE_FEATURE_H_
