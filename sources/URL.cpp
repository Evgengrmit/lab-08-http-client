// Copyright 2020 Your Name <your_email>

#include <URL.hpp>
#include <utility>

URL::URL(std::string url) : url_(std::move(url)) {}
std::string URL::parse_url() { return url_; }

std::string URL::parse_url_to_host() {
  if (url_.find(k_protocol_name) == 0) {
    url_ = url_.substr(k_size_name_of_protocol);
  }
  std::string result_host;
  for (char i : url_) {
    if (i == ':') {
      break;
    }
    result_host += i;
  }
  return result_host;
}
std::string URL::parse_url_to_port() {
  if (url_.find(k_protocol_name) == 0)
    url_ = url_.substr(k_size_name_of_protocol);
  std::string result_port;
  unsigned pos = 0;
  for (; pos < url_.size(); ++pos) {
    if (url_[pos] == ':') {
      break;
    }
  }
  for (unsigned i = pos + 1; i < url_.size(); ++i) {
    if (url_[i] == '/') {
      break;
    }
    result_port += url_[i];
  }

  return result_port;
}
std::string URL::parse_url_to_target() {
  if (url_.find(k_protocol_name) == 0)
    url_ = url_.substr(k_size_name_of_protocol);
  std::string result_target;
  unsigned pos = 0;
  for (; pos < url_.size(); ++pos) {
    if (url_[pos] == '/') break;
  }
  for (unsigned i = pos; i < url_.size(); ++i) {
    result_target += url_[i];
  }

  return result_target;
}
