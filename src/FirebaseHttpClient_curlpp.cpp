
#include "FirebaseHttpClient.h"

#include <stdio.h>
#include <sstream>

// The ordering of these includes matters greatly.
// #include <WiFiClientSecure.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>

// Detect whether stable version of HTTP library is installed instead of
// master branch and patch in missing status and methods.
#ifndef HTTP_CODE_TEMPORARY_REDIRECT
#define HTTP_CODE_TEMPORARY_REDIRECT 307
#define USE_ESP_ARDUINO_CORE_2_0_0
#endif

class FirebaseHttpClientCurlPP : public FirebaseHttpClient {
 public:
  FirebaseHttpClientCurlPP() {}

  void setReuseConnection(bool reuse) override {
    // do nothing
  }

  void begin(const std::string& url) override {
    curlpp::Cleanup cl;
    curlppRequest = new curlpp::Easy();
    os = new std::ostringstream();

    curlppRequest->setOpt<curlpp::options::Url>(url);
    curlppRequest->setOpt<curlpp::options::WriteStream>(os);
  }

  void begin(const std::string& host, const std::string& path) override {
    begin("https://"+host+"/"+path);
  }

  void end() override {
    // if (curlppRequest!=NULL) delete curlppRequest;
    // if (os!=NULL) delete os;
  }

  void addHeader(const std::string& name, const std::string& value) override {
    // curlppRequest->setOpt<curlpp::options::HttpHeader>(name+": "+ value);
  }

  void collectHeaders(const char* header_keys[], const int count) override {
    //http_.collectHeaders(header_keys, count);
  }

  std::string header(const std::string& name) override {
    //return http_.header(name.c_str()).c_str();
    return "";
  }

  int sendRequest(const std::string& method, const std::string& data) override {
    curlppRequest->setOpt<curlpp::options::CustomRequest>(method);
    curlppRequest->setOpt<curlpp::options::PostFields>(data);
    curlppRequest->setOpt<curlpp::options::PostFieldSize>(data.length());
    
    try
    {
      curlppRequest->perform();
    }  
    catch(curlpp::RuntimeError &e)
    {
      std::cout << e.what() << std::endl;
    }

    return 0;
  }

  std::string getString() override {
    return os->str();
  }

  std::ostringstream* getStreamPtr() override {
    return os;
  }

  std::string errorToString(int error_code) override {
    return "";
  }

 private:
  curlpp::Easy *curlppRequest;
  std::ostringstream *os;
};

FirebaseHttpClient* FirebaseHttpClient::create() {
  return new FirebaseHttpClientCurlPP();
}
 
