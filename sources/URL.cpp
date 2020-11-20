// Copyright 2020 Your Name <your_email>

#include <URL.hpp>
#include <utility>

URL::URL(std::string url) : url(std::move(url)) {}
std::string URL::parse_url() { return url; }

std::string URL::parse_url_to_host() {
  if (url.find(k_protocol_name) == 0) {
    url = url.substr(k_size_name_of_protocol);
  }
  std::string result_host;
  for (char i : url) {
    if (i == ':') {
      break;
    }
    result_host += i;
  }
  return result_host;
}
std::string URL::parse_url_to_port() {
  if (url.find(k_protocol_name) == 0) url = url.substr(k_size_name_of_protocol);
  std::string result_port;
  unsigned pos = 0;
  for (; pos < url.size(); ++pos) {
    if (url[pos] == ':') {
      break;
    }
  }
  for (unsigned i = pos + 1; i < url.size(); ++i) {
    if (url[i] == '/') {
      break;
    }
    result_port += url[i];
  }

  return result_port;
}
std::string URL::parse_url_to_target() {
  if (url.find(k_protocol_name) == 0) url = url.substr(k_size_name_of_protocol);
  std::string result_target;
  unsigned pos = 0;
  for (; pos < url.size(); ++pos) {
    if (url[pos] == '/') break;
  }
  for (unsigned i = pos; i < url.size(); ++i) {
    result_target += url[i];
  }

  return result_target;
}
