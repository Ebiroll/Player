#ifndef _SERVER_H
#define _SERVER_H
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/Template.h"

using Poco::Net::ServerSocket;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;

typedef struct conf {
  int width;
  int height;
};

extern conf config;

class PageRequestHandler : public HTTPRequestHandler
        /// Return a HTML document with some JavaScript
{
public:
  void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
  void handleFileRequest(HTTPServerRequest& request, HTTPServerResponse& response);
  std::string filename;
};

class WebSocketRequestHandler : public HTTPRequestHandler
        /// Handle a WebSocket connection.
{
public:
  void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};

class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
  HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};

#endif
