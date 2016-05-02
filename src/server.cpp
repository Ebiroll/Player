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
#include <iostream>
#include <ios>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/Query.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>

extern void setNewMode(const char *mode);

using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco::Net;

std::string testFile="test.json";


void PageRequestHandler::handleFileRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
 std::string message;
 response.setChunkedTransferEncoding(true);
 response.setContentType(type.c_str());
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
     printf("file read error");
 }

  std::ostream& ostr = response.send();
  ostr << message;

}

void ModeRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{

  std::string req=request.getURI();
  std::string name;
  std::string value;
  printf("Mode request\n");

  std::istream& instr = request.stream();
  instr >> value;

  std::cout << "[" << value << "]" << std::endl ;
  std::string silllyURI=std::string("http://localhost/api/mode?") + value;
  Poco::URI uri1(silllyURI);

  Poco::URI::QueryParameters params = uri1.getQueryParameters();

  std::string type;
  std::string mode;

  for (unsigned int i=0;i<params.size();i++) {
      std::pair < std::string, std::string > it=params[i];
      name=it.first;
      value=it.second;
      if (name=="type")
      {
          type=value;
      }
      if (name=="mode")
      {
          mode=value;
      }

      std::cout << name << "=" << value << std::endl ;
  }

  std::string all=type + std::string(" ") + value;
  setNewMode(all.c_str());

  std::cout << std::endl;

}

/*
  HTMLForm form( request );
  NameValueCollection::ConstIterator i = form.begin();

  while(i!=form.end()){

      name=i->first;
      value=i->second;
      std::cout << name << "=" << value << std::endl ;
      ++i;
  }
  */


void AjaxRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{

  std::string req=request.getURI();
  std::string name;
  std::string value;
  std::string allmess;
  printf("PUT CONFIG\n");

  std::istream& instr = request.stream();
  instr.unsetf(std::ios_base::skipws);
  while (instr >> value) {
      allmess = allmess + value;
  }

  std::cout << "[" << allmess << "]" << std::endl ;
  std::cout << std::endl;

  // Write to current config file in same dir...
  std::string total= std::string("data/") + std::string(config.name);
  {
      std::ofstream of(total.c_str());
      if (of.is_open())
      {
          of.unsetf(std::ios_base::skipws);
          of << allmess;
      }
      std::cout << "Saving to " << total << std::endl;
  }
  load_current_config();
}

void PageRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
      
      std::string req=request.getURI();
      // data/modes_DMT.json
      printf("------------- Serving web page: %s\n",req.c_str());

      if ( request.getURI()  == "/favicon.ico") {
          filename="data/favicon.ico";
          type="image/vnd.microsoft.icon";
          handleFileRequest(request, response);
          printf("FILE\n");
        return;
      }


      if ( request.getURI()  == "/lib/jquery-2.2.3.min.js") {
          filename="data/lib/jquery-2.2.3.min.js";
          type="text/javascript";
          handleFileRequest(request, response);
          printf("FILE\n");
	    return;
      }
      
       if ( request.getURI()  == "/modes_CEA.json") {
           filename="data/modes_CEA.json";
           type="text/json";
           handleFileRequest(request, response);
           printf("modes_CEA\n");
           return;
       }

       if ( request.getURI()  == "/modes_DMT.json") {
           filename="data/modes_DMT.json";
           type="text/json";
           handleFileRequest(request, response);
           printf("modes_DMT\n");
           return;
       }

       if ( request.getURI()  == "/config.json") {
           std::string total= std::string("data/") + std::string(config.name);
           std::cout << total;
           filename=total;
           type="text/json";
           handleFileRequest(request, response);
           printf("CURRENT-CONFIG\n");
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
        ostr << "<div id=\"output\">filename " + std::string(config.name);
        ostr << "<input type =\"button\" id = \"sb\" value=\"Save\"/> ";
        ostr << "<input type ='button'' id = 'lb'' value='Load'/> ";
        ostr << "<input type ='button'' id = 'def'' value='Default'/> ";
        // Restart button
        ostr << "  <select id='current'' name='rectselect' >";
        ostr << "  <option value='0'>video</option>";
        ostr << "  <option value='1'>logo</option>";
        ostr << "  <option value='2'>label</option>";
        ostr << "  <option value='3'>list</option>";
        ostr << "  <option value='4'>no_smoke</option>";
        ostr << "  <option value='5'>scroll</option>";
        ostr << "  <option value='6'>explode</option>";
        ostr << "  <option value='7'>clock</option>";
        ostr << "  </select><br /> </div>";
        ostr << "  <textarea id = 'currtext' rows='2' cols='128'></textarea>";
        //ostr << "<form action='/api/restart'>";
        //ostr << "        <input type='submit' value='Restart'>";
        //ostr << "</form>";

        ostr << "  <canvas id=\"canv\" color=\"lightgrey\" width=\"" << config.width << "\" height=\"" << config.height << "\"></canvas>";
//#endif
        ostr << "  <h2>Width " << config.width << "</h2><br />";
        ostr << "  <h2>Height " << config.height <<  "</h2><br />";
        ostr << "  <select id='cea'' name='ceaselect'' ><option selected='selected'>tv</option></select><input type ='button' id = 'ceaset' value='Set TV mode'/>";
        ostr << "  <select id='dmt'' name='dmtselect'' ><option selected='selected'>monitor</option></select><input type ='button' id = 'dmtset' value='Set Monitor mode'/>";
        ostr << "  <br><input type ='button' id = 'getconf' value='Get current config'/>";
        ostr << "  <br><input type ='button' id = 'saveconf' value='Save current config'/>";
        ostr << "  <textarea id = 'conf' rows='20' cols='100'>config.json<</textarea>";
        ostr << "<form action='/api/shutdown'>";
        ostr << "        <input type='password'' name='pass' value=''><br>";
        ostr << "        <input type='submit' value='Shutdown'>";
        ostr << "</form>";
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


std::string readFile(std::string filename) {
    std::string ret;
    std::ifstream t(filename.c_str());
    if (t.good())
      {
        std::stringstream buffer;
        buffer << t.rdbuf();
        ret = buffer.str();
      }
  return (ret);
}

// This is the websocket solution, we use PUT ajax request instead
void serveConfig(WebSocket &ws)
{

    std::string total= std::string("data/") + std::string(config.name);
    std::string conf =readFile(total.c_str());
    int flags;
    int n;
    ws.sendFrame(conf.c_str(),conf.size());

    do {
        if (ws.poll(100,Poco::Net::Socket::SELECT_READ || Poco::Net::Socket::SELECT_ERROR))
        {
            char buffer[1024];
            n = ws.receiveFrame(buffer, sizeof(buffer), flags);
            printf("got %s\n",buffer);
            std::string message=std::string(buffer,n);
            // Dont overwrite current config, This does not work anyways...
            std::ofstream of("data/test_config.json");
            if (of.is_open())
            {
                of << message;
            }
            // Save this to file
        }
    }
    while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
}


void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
      Application& app = Application::instance();
        try
        {
            // This should send a response.. Error in implementation :-P
            WebSocket ws(request, response);
            //app.logger().information("WebSocket connection established.");
             printf("WebSocket connection established.....................\n");

            serveConfig(ws);


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
                std::cout << "received " << subscribe << std::endl;


                Parser parser;
                Var result = parser.parse(subscribe);;

                Query myQuery(result);

                Var topic=myQuery.find("topic");
                std::string value = topic.convert<std::string>();
                app.logger().information(Poco::format("Topic %s.",value));

                serveConfig(ws);

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

        // ModeRequestHandler
        std::string uri=request.getURI();
        std::cout << "Request" << uri  << "\n";

        if (uri=="/api/shutdown") {
            // Shuting down
            printf("SHUT DOWN SYSTEM NOW!!");
            // ofExit();
        }
        if (uri=="/api/restart") {
            // Restart
            printf("Restarting!!");
        }


        if (uri=="/api/mode") {
            return new ModeRequestHandler;
        } else  if(uri=="/api/save") {
            return new AjaxRequestHandler;
        } else {


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
    }

