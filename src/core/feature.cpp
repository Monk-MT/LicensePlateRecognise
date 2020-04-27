#include "LicensePlateRecognise/core/feature.h"
#include "LicensePlateRecognise/core/core_func.h"
#include "thirdparty/LBP/lbp.hpp"

namespace LicensePlateRecognise {


Mat getHistogram(Mat in) {
  const int VERTICAL = 0;
  const int HORIZONTAL = 1;

  // ֱ��ͼ����
  Mat vhist = ProjectedHistogram(in, VERTICAL);
  Mat hhist = ProjectedHistogram(in, HORIZONTAL);

  int numCols = vhist.cols + hhist.cols;

  Mat out = Mat::zeros(1, numCols, CV_32F);

  int j = 0;
  for (int i = 0; i < vhist.cols; i++) {
    out.at<float>(j) = vhist.at<float>(i);
    j++;
  }
  for (int i = 0; i < hhist.cols; i++) {
    out.at<float>(j) = hhist.at<float>(i);
    j++;
  }

  return out;
}

void getHistogramFeatures(const Mat& image, Mat& features) {
  Mat grayImage;
  cvtColor(image, grayImage, CV_RGB2GRAY);

  Mat img_threshold;
  threshold(grayImage, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

  features = getHistogram(img_threshold);
}

// ������ɫֱ��ͼ
void getColorFeatures(const Mat& src, Mat& features) {
  Mat src_hsv;

  cvtColor(src, src_hsv, CV_BGR2HSV);
  int channels = src_hsv.channels();
  int nRows = src_hsv.rows;

  // ���Ƕ�ͨ����ͼ��
  int nCols = src_hsv.cols * channels;
  if (src_hsv.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
  }

  const int sz = 180;
  int h[sz] = { 0 };

  uchar* p;
  for (int i = 0; i < nRows; ++i) {
    p = src_hsv.ptr<uchar>(i);
    for (int j = 0; j < nCols; j += 3) {
      int H = int(p[j]);      // 0-180
      if (H > sz - 1) H = sz - 1;
      if (H < 0) H = 0;
      h[H]++;
    }
  }

  Mat mhist = Mat::zeros(1, sz, CV_32F);
  for (int j = 0; j < sz; j++) {
    mhist.at<float>(j) = (float)h[j];
  }

  // ��׼ֱ��ͼ
  double min, max;
  minMaxLoc(mhist, &min, &max);

  if (max > 0)
    mhist.convertTo(mhist, -1, 1.0f / max, 0);

  features = mhist;
}


void getHistomPlusColoFeatures(const Mat& image, Mat& features) {
  Mat feature1, feature2;
  getHistogramFeatures(image, feature1);
  getColorFeatures(image, feature2);
  hconcat(feature1.reshape(1, 1), feature2.reshape(1, 1), features);
}


//! LBP����
void getLBPFeatures(const Mat& image, Mat& features) {

  Mat grayImage;
  cvtColor(image, grayImage, CV_RGB2GRAY);

  Mat lbpimage;
  lbpimage = libfacerec::olbp(grayImage);
  Mat lbp_hist = libfacerec::spatial_histogram(lbpimage, 32, 4, 4);

  features = lbp_hist;
}

Mat charFeatures(Mat in, int sizeData) {
  const int VERTICAL = 0;
  const int HORIZONTAL = 1;

  Rect _rect = GetCenterRect(in);
  Mat tmpIn = CutTheRect(in, _rect);

  Mat lowData;
  resize(tmpIn, lowData, Size(sizeData, sizeData));

  // ֱ��ͼ����
  Mat vhist = ProjectedHistogram(lowData, VERTICAL);
  Mat hhist = ProjectedHistogram(lowData, HORIZONTAL);

  int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

  Mat out = Mat::zeros(1, numCols, CV_32F);

  int j = 0;
  for (int i = 0; i < vhist.cols; i++) {
    out.at<float>(j) = vhist.at<float>(i);
    j++;
  }
  for (int i = 0; i < hhist.cols; i++) {
    out.at<float>(j) = hhist.at<float>(i);
    j++;
  }
  for (int x = 0; x < lowData.cols; x++) {
    for (int y = 0; y < lowData.rows; y++) {
      out.at<float>(j) += (float)lowData.at <unsigned char>(x, y);
      j++;
    }
  }

  return out;
}

//����ѵ��
Mat charFeatures2(Mat in, int sizeData) {
  const int VERTICAL = 0;
  const int HORIZONTAL = 1;

  Rect _rect = GetCenterRect(in);
  Mat tmpIn = CutTheRect(in, _rect);

  Mat lowData;
  resize(tmpIn, lowData, Size(sizeData, sizeData));

  Mat vhist = ProjectedHistogram(lowData, VERTICAL);
  Mat hhist = ProjectedHistogram(lowData, HORIZONTAL);

  int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

  Mat out = Mat::zeros(1, numCols, CV_32F);

  int j = 0;
  for (int i = 0; i < vhist.cols; i++) {
    out.at<float>(j) = vhist.at<float>(i);
    j++;
  }
  for (int i = 0; i < hhist.cols; i++) {
    out.at<float>(j) = hhist.at<float>(i);
    j++;
  }
  for (int x = 0; x < lowData.cols; x++) {
    for (int y = 0; y < lowData.rows; y++) {
      out.at<float>(j) += (float)lowData.at <unsigned char>(x, y);
      j++;
    }
  }

  return out;
}

Mat charProjectFeatures(const Mat& in, int sizeData) {
  const int VERTICAL = 0;
  const int HORIZONTAL = 1;

  SHOW_IMAGE(in, 0);

  Mat lowData;
  resize(in, lowData, Size(sizeData, sizeData));

  SHOW_IMAGE(lowData, 0);

  Mat vhist = ProjectedHistogram(lowData, VERTICAL);
  Mat hhist = ProjectedHistogram(lowData, HORIZONTAL);

  int numCols = vhist.cols + hhist.cols;

  Mat out = Mat::zeros(1, numCols, CV_32F);

  int j = 0;
  for (int i = 0; i < vhist.cols; i++) {
    out.at<float>(j) = vhist.at<float>(i);
    j++;
  }
  for (int i = 0; i < hhist.cols; i++) {
    out.at<float>(j) = hhist.at<float>(i);
    j++;
  }

  return out;
}

void getGrayCharFeatures(const Mat& grayChar, Mat& features) {

  SHOW_IMAGE(grayChar, 0);
  SHOW_IMAGE(255 - grayChar, 0);

  bool useResize = false;
  bool useConvert = true;
  bool useMean = true;
  bool useLBP = false;

  //ͳһ��С
  Mat char_mat;
  if (useResize) {
    char_mat.create(kGrayCharHeight, kGrayCharWidth, CV_8UC1);
    resize(grayChar, char_mat, char_mat.size(), 0, 0, INTER_LINEAR);
  } else {
    char_mat = grayChar;
  }
  SHOW_IMAGE(char_mat, 0);

  // ת��Ϊ������
  Mat float_img;
  if (useConvert) {
    float scale = 1.f / 255;
    char_mat.convertTo(float_img, CV_32FC1, scale, 0);
  } else {
    float_img = char_mat;
  }
  SHOW_IMAGE(float_img, 0);

  //�Ӿ�ֵ���п�
  Mat mean_img;
  if (useMean) {
    float_img -= mean(float_img);
    mean_img = float_img;
  } else {
    mean_img = float_img;
  }
  SHOW_IMAGE(mean_img, 0);

  // ʹ��lbp���������
  Mat feautreImg;
  if (useLBP) {
    Mat lbpimage = libfacerec::olbp(char_mat);
    SHOW_IMAGE(lbpimage, 0);
    feautreImg = libfacerec::spatial_histogram(lbpimage, kCharLBPPatterns, kCharLBPGridX, kCharLBPGridY);
  } else {
    feautreImg = mean_img.reshape(1, 1);
  }

  features = feautreImg;
}


void getGrayPlusProject(const Mat& grayChar, Mat& features)
{

  SHOW_IMAGE(grayChar, 0);
  SHOW_IMAGE(255 - grayChar, 0);

  bool useResize = false;
  bool useConvert = true;
  bool useMean = true;
  bool useLBP = false;

  //ͳһ��С
  Mat char_mat;
  if (useResize) {
    char_mat.create(kGrayCharHeight, kGrayCharWidth, CV_8UC1);
    resize(grayChar, char_mat, char_mat.size(), 0, 0, INTER_LINEAR);
  }
  else {
    char_mat = grayChar;
  }
  SHOW_IMAGE(char_mat, 0);

  // ת��Ϊ������
  Mat float_img;
  if (useConvert) {
    float scale = 1.f / 255;
    char_mat.convertTo(float_img, CV_32FC1, scale, 0);
  }
  else {
    float_img = char_mat;
  }
  SHOW_IMAGE(float_img, 0);

  //�Ӿ�ֵ���п�
  Mat mean_img;
  if (useMean) {
    float_img -= mean(float_img);
    mean_img = float_img;
  }
  else {
    mean_img = float_img;
  }
  SHOW_IMAGE(mean_img, 0);

  // ʹ��lbp���������
  Mat feautreImg;
  if (useLBP) {
    Mat lbpimage = libfacerec::olbp(char_mat);
    SHOW_IMAGE(lbpimage, 0);
    feautreImg = libfacerec::spatial_histogram(lbpimage, kCharLBPPatterns, kCharLBPGridX, kCharLBPGridY);
  }
  else {
    feautreImg = mean_img.reshape(1, 1);
  }
  SHOW_IMAGE(grayChar, 0);
  Mat binaryChar;
  threshold(grayChar, binaryChar, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
  SHOW_IMAGE(binaryChar, 0);
  Mat projectFeature = charProjectFeatures(binaryChar, 32);

  hconcat(feautreImg.reshape(1, 1), projectFeature.reshape(1, 1), features);
}


void getGrayPlusLBP(const Mat& grayChar, Mat& features)
{

  SHOW_IMAGE(grayChar, 0);
  SHOW_IMAGE(255 - grayChar, 0);


  bool useResize = false;
  bool useConvert = true;
  bool useMean = true;
  bool useLBP = true;

  // ͳһ��С
  Mat char_mat;
  if (useResize) {
    char_mat.create(kGrayCharHeight, kGrayCharWidth, CV_8UC1);
    resize(grayChar, char_mat, char_mat.size(), 0, 0, INTER_LINEAR);
  }
  else {
    char_mat = grayChar;
  }
  SHOW_IMAGE(char_mat, 0);

  // ת��Ϊ������
  Mat float_img;
  if (useConvert) {
    float scale = 1.f / 255;
    char_mat.convertTo(float_img, CV_32FC1, scale, 0);
  }
  else {
    float_img = char_mat;
  }
  SHOW_IMAGE(float_img, 0);

  // �Ӿ�ֵ���п�
  Mat mean_img;
  if (useMean) {
    float_img -= mean(float_img);
    mean_img = float_img;
  }
  else {
    mean_img = float_img;
  }
  SHOW_IMAGE(mean_img, 0);

  // ʹ��lbp���������
  Mat originImage = mean_img.clone();
  Mat lbpimage = libfacerec::olbp(mean_img);
  SHOW_IMAGE(lbpimage, 0);
  lbpimage = libfacerec::spatial_histogram(lbpimage, kCharLBPPatterns, kCharLBPGridX, kCharLBPGridY);

  // 32x20 + 16x16
  hconcat(mean_img.reshape(1, 1), lbpimage.reshape(1, 1), features);
}

void getLBPplusHistFeatures(const Mat& image, Mat& features) {
  Mat grayImage;
  cvtColor(image, grayImage, CV_RGB2GRAY);

  Mat lbpimage;
  lbpimage = libfacerec::olbp(grayImage);
  Mat lbp_hist = libfacerec::spatial_histogram(lbpimage, 64, 8, 4);

  Mat greyImage;
  cvtColor(image, greyImage, CV_RGB2GRAY);

  Mat img_threshold;
  threshold(greyImage, img_threshold, 0, 255,
    CV_THRESH_OTSU + CV_THRESH_BINARY);
  Mat histomFeatures = getHistogram(img_threshold);

  hconcat(lbp_hist.reshape(1, 1), histomFeatures.reshape(1, 1), features);
}

}
