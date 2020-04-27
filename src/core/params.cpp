#include "LicensePlateRecognise/core/params.h"

namespace LicensePlateRecognise {
  CParams* CParams::instance_ = nullptr;

  CParams* CParams::instance() {
    if (!instance_) {
      instance_ = new CParams;
    }
    return instance_;
  }
}
