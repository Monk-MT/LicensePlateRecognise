#ifndef LPR_TRAIN_ANNCHTRAIN_H_
#define LPR_TRAIN_ANNCHTRAIN_H_

#include "LicensePlateRecognise/train/train.h"
#include "LicensePlateRecognise/util/kv.h"
#include "LicensePlateRecognise/core/feature.h"
#include <memory>

namespace LicensePlateRecognise {

	class AnnChTrain : public ITrain {
	public:
		explicit AnnChTrain(const char* chars_folder, const char* xml);

		virtual void train();

		virtual void test();

		std::pair<std::string, std::string> identifyGrayChinese(cv::Mat input);

		inline void setNumberForCount(int n) {
			m_number_for_count = n;
		}

	private:
		virtual cv::Ptr<cv::ml::TrainData> tdata();

		void trainVal(size_t number_for_count = 100);

		cv::Ptr<cv::ml::ANN_MLP> ann_;
		const char* ann_xml_;
		const char* chars_folder_;

		std::shared_ptr<Kv> kv_;
		int type;

		int m_number_for_count;

		annCallback extractFeature;
	};
}

#endif  // LPR_TRAIN_ANNCHTRAIN_H_


