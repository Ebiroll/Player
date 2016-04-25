//
// server.cpp
//
// Simple websocket server
//

#include "server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/Query.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timestamp.h>


using namespace Poco::JSON;
using namespace Poco::Dynamic;

std::string testFile="test.json";


void PageRequestHandler::handleFileRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
 std::string message;
 response.setChunkedTransferEncoding(true);
 response.setContentType("text/javascript");
try {
  std::ifstream t(filename.c_str() /* "data/lib/jquery-2.2.3.min.js"*/);
  if (t.good())
    {
      std::stringstream buffer;
      buffer << t.rdbuf();
      message = buffer.str();
    }
 }
 catch(...)
   {
     printf("jquery read error");
   }

  std::ostream& ostr = response.send();
  ostr << message;

}


void PageRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
      
      std::string req=request.getURI();
      printf("------------- Serving web page: %s\n",req.c_str());
      if ( request.getURI()  == "lib/jquery-2.2.3.min.js") {
          filename="data/lib/jquery-2.2.3.min.js";
          handleFileRequest(request, response);
          printf("FILE\n");
	    return;
      }
      
      response.setChunkedTransferEncoding(true);
      response.setContentType("text/html");
        std::ostream& ostr = response.send();
        ostr << "<html>";
        ostr << "<head>";
        //ostr << "<META HTTP-EQUIV=\"refresh\" CONTENT=\"5;url=''\"> ";
        ostr << "<title>Config server</title>";
	// lib/jquery-2.2.3.min.js
	ostr << "<script type=\"text/javascript\" src=\"lib/jquery-2.2.3.min.js\"></script>";
        ostr << "<script type=\"text/javascript\">";
        //ostr << " window.location = \"index.html\"";

        std::string script;
        try {
	    //app.logger().information("Reading %s",testFile);
            std::ifstream t("data/config.js");
            if (t.good())
            {
                std::stringstream buffer;
                buffer << t.rdbuf();
                script = buffer.str();
            }
        }
        catch(...)
        {
            printf("config.js read error");
        }
	
        ostr << script;
        ostr << "</script>";
        ostr << "</head>";
        ostr << "<body>";
//#ifdef TEST_CHANGE
        ostr << "  <h1>Configuration of display</h1><br />";
        ostr << "  <canvas id=\"canv\" width=\"" << config.width << "\" height=\"" << config.height << "\"></canvas>";
//#endif
        ostr << "  <h2>Width " << config.width << "</h2><br />";
        ostr << "  <h2>Height " << config.height <<  "</h2><br />";
	//        ostr << "<table>";
        //for (int i=0;i<debugArray.size();i++)
        //{
        //    ostr << "<tr>";
        //    ostr << "  <td>" << debugKeys[i] <<  "</td><td>""[" << debugArray[i] << "]" "</td>";
        //    ostr << "</tr>";
        //}
        //ostr << "</tr>";
        //ostr << "</table>";
         ostr << "</body>";
        ostr << "</html>";
    }


void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
      Application& app = Application::instance();
        try
        {
            WebSocket ws(request, response);
            //app.logger().information("WebSocket connection established.");
	    printf("WebSocket connection established.....................\n");
            char buffer[1024];
            int flags;
            int n;
            do
            {
                n = ws.receiveFrame(buffer, sizeof(buffer), flags);
                //app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
                /* check for data */
                //app.logger().information(Poco::format("Data %s.",std::string(buffer,n)));

                std::string subscribe=std::string(buffer,n);


                Parser parser;
                Var result = parser.parse(subscribe);;

                Query myQuery(result);

                Var topic=myQuery.find("topic");
                std::string value = topic.convert<std::string>();
                app.logger().information(Poco::format("Topic %s.",value));


                if (value=="/atis/EVRA/ATIS_ARRDEP/marked")
                {
		  //serverMarkedTopic(ws);
                }
                else if (value=="/sys/subscriptions/heartbeat")
                {
		  //serverSmartToolsHeartbeat(ws,value);
                } else
                {
		  //serverATraccHeartbeat(ws,value);
                }

            }
            while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
            app.logger().information("WebSocket connection closed.");
        }
        catch (WebSocketException& exc)
        {
            app.logger().log(exc);
            switch (exc.code())
            {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
                // fallthrough
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
            }
        }
        catch (...)
        {
             app.logger().information("Probably timeout.");
        }
    }


HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request)
    {
      	printf("Request Factory ...................\n");
        //Application& app = Application::instance();
        //app.logger().information("Request from "
        //                         + request.clientAddress().toString()
        //                         + ": "
        //                         + request.getMethod()
        //                         + " "
        //                         + request.getURI()
        //                         + " "
        //                         + request.getVersion());

        for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
        {
	  //app.logger().information(it->first + ": " + it->second);
        }

        std::string uri=request.getURI();

        if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
	  {
	    printf("WebSocket Request\n");
            return new WebSocketRequestHandler;
	  }
	else
	  {
     	     printf("Page Request\n");
             return new PageRequestHandler;
	  }
    }

