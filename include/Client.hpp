// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_CLIENT_HPP_
#define INCLUDE_CLIENT_HPP_
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

const char k_protocol_name[] = "http://";
const size_t k_size_name_of_protocol = 7;

//Парсинг url-получение адреса хоста
std::string parse_url_to_host(std::string url) {
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
//Парсинг url-получение порта
std::string parse_url_to_port(std::string url) {
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
//Парсинг url-получение цели
std::string parse_url_to_target(std::string url) {
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
int Client(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: http-client-sync <url> "
                 "<request>\n"
              << "Example:\n"
              << "    ./cmake-build-debug/tests  "
                 "http://localhost:8080/v1/api/suggest "
              << R"({\"input\":\"<user_input>\"})"
                 "\n";
    return EXIT_FAILURE;
  }
  try {
    // Устанавливаем хост, порт, цель и запрос
    // auto const host = argv[1];
    //    auto const port = argv[2];
    //    auto const target = argv[3];
    auto const url = argv[1];
    auto const host = parse_url_to_host(url);
    auto const port = parse_url_to_port(url);
    auto const target = parse_url_to_target(url);
    auto const request_body = argv[2];
    int version = 11;
    // io_context требуется для всех операций ввода-вывода
    net::io_context ioc;

    // Эти объекты выполняют ввод-вывод
    tcp::resolver resolver{ioc};
    beast::tcp_stream stream(ioc);
    // Просмотр доменного имени
    auto const results = resolver.resolve(host, port);
    // Соединение по IP-адресу, полученному из поиска
    stream.connect(results);
    // Настройка сообщения HTTP POST-запроса
    http::string_body::value_type body = request_body;

    http::request<http::string_body> req{http::verb::post, target, version};
    req.set(http::field::host, host);
    req.body() = body;
    req.prepare_payload();
    req.set(http::field::content_type, "text/plain");

    // Отправление HTTP-запроса на удаленный хост
    http::write(stream, req);

    // Этот буфер используется для чтения и должен быть сохранен
    boost::beast::flat_buffer buffer;

    // Объявление контейнера для хранения ответа
    http::response<http::string_body> res;

    // Получение HTTP-ответа
    http::read(stream, buffer, res);

    // Write the message to standard out
    std::cout << res.body() << std::endl;

    // Корректное закрытие сокета
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // иногда происходят not_connected (нет соединения)
    // так что не трудитесь сообщать об этом.
    //
    if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

    // Здесь связь будет закрыта корректно
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
#endif  // INCLUDE_CLIENT_HPP_
