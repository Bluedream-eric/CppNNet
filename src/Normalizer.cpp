#include "Normalizer.h"

CppNNet::Normalizer::Normalizer(std::vector<Eigen::VectorXf> &xlist, float ymin, float ymax) {
  unsigned long isize = xlist.size();
  long jsize = xlist[0].size();
  Eigen::VectorXf gain(jsize);
  Eigen::VectorXf xoffset(jsize);

  float diff = ymax - ymin;
  for (int j = 0; j < jsize; j++) {
    float xmin = 0, xmax = 0;
    for (int i = 0; i < isize; i++) {
      float cur = xlist[i][j];
      if (xmin > cur) xmin = cur;
      if (xmax < cur) xmax = cur;
    }
    xoffset[j] = xmin;
    gain[j] = diff / (xmax - xmin);
  }

  _settings.xoffset = xoffset;
  _settings.gain = gain;
  _settings.ymin = ymin;
}

CppNNet::Normalizer::Normalizer(Eigen::VectorXf &to_norm, Eigen::VectorXf xoffset, Eigen::VectorXf gain, float ymin) :
    Normalizer(std::move(xoffset), std::move(gain), ymin) {
  norm(to_norm);
}

CppNNet::Normalizer::Normalizer(Eigen::VectorXf &to_norm, NormSettings settings) {
  _settings = settings;
  for (long i = 0, size = to_norm.size(); i < size; i++)
    to_norm[i] = (to_norm[i] - settings.xoffset[i]) * settings.gain[i] + settings.ymin;
}

CppNNet::Normalizer::Normalizer(Eigen::VectorXf xoffset, Eigen::VectorXf gain, float ymin) {
  _settings.ymin = ymin;
  _settings.xoffset = std::move(xoffset);
  _settings.gain = std::move(gain);
}

CppNNet::Normalizer::Normalizer(NormSettings settings) { _settings = std::move(settings); }

void CppNNet::Normalizer::norm(Eigen::VectorXf &to_norm) {
  for (long i = 0, size = to_norm.size(); i < size; i++)
    to_norm[i] = (to_norm[i] - _settings.xoffset[i]) * _settings.gain[i] + _settings.ymin;
}

Eigen::VectorXf CppNNet::Normalizer::GetNorm(Eigen::VectorXf &to_norm) {
  Eigen::VectorXf n(to_norm.size());
  for (long i = 0, size = to_norm.size(); i < size; i++)
    n[i] = (to_norm[i] - _settings.xoffset[i]) * _settings.gain[i] + _settings.ymin;
  return n;
}

void CppNNet::Normalizer::reverse(Eigen::VectorXf &to_reverse) {
  for (long i = 0, size = to_reverse.size(); i < size; i++)
    to_reverse[i] = ((to_reverse[i] - _settings.ymin) / _settings.gain[i]) + _settings.xoffset[i];
}

void CppNNet::Normalizer::reverse(Eigen::VectorXf &to_reverse, NormSettings settings) {
  _settings = std::move(settings);
  reverse(to_reverse);
}

Eigen::VectorXf CppNNet::Normalizer::GetReverse(Eigen::VectorXf &to_reverse) {
  Eigen::VectorXf n = to_reverse;
  reverse(n);
  return n;
}

void CppNNet::Normalizer::batch_norm(std::vector<Eigen::VectorXf> &to_norm) {
  for (auto &item : to_norm)
    norm(item);
}

void CppNNet::Normalizer::batch_reverse(std::vector<Eigen::VectorXf> &to_reverse) {
  for (auto &item : to_reverse)
    reverse(item);
}

std::vector<Eigen::VectorXf> CppNNet::Normalizer::get_batch_norm(std::vector<Eigen::VectorXf> &to_norm) {
  std::vector<Eigen::VectorXf> n;
  for (auto &item : to_norm)
    n.push_back(GetNorm(item));
  return n;
}

std::vector<Eigen::VectorXf> CppNNet::Normalizer::get_batch_reverse(std::vector<Eigen::VectorXf> &to_reverse) {
  std::vector<Eigen::VectorXf> n;
  for (auto &item : to_reverse)
    n.push_back(GetReverse(item));
  return n;
}
