#include "src/server.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <fstream>
#include <sstream>
#include <streambuf>

// This Module is just used for testing of the websocket server on a machine with POCO but no open framework

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/Query.h>
#include "Poco/Util/ServerApplication.h"
#include "config.h"


using Poco::Util::ServerApplication;
using Poco::Util::Application;
using namespace Poco::JSON;
using namespace Poco::Dynamic;



void setNewMode(const char *mode) {
	
    printf("---tvservice %s---",mode);
}


ServerSocket *gSocketServer;
HTTPServer *gServer;

class WebSocketServer: public Poco::Util::ServerApplication
        /// The main application class.
{

};



void parseModesFile() {


    printf("Parse\n"    );

    try {
    //Application& app = Application::instance();
    /*
    [
     { "code":1, "width":640, "height":480, "rate":60, "aspect_ratio":"4:3", "scan":"p", "3d_modes":[] }
    ]
   */


      //std::string conf = std::string(" [ { \"code\":1 ,  \"width\":640, \"height\":480 } ,  { \"code\":3 ,  \"width\":800, \"height\":600 } ]");
      std::string conf =readFileToString("data/modes.json");
      Parser parser;
      Var result = parser.parse(conf);

      Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
      for (int i=0;i<arr->size(); i++) {
          Object::Ptr object = arr->getObject(i);
          printf("w,h=%d,%d\n",object->getValue<int>("width"),object->getValue<int>("height"));
      }

      Query myQuery(result);

      //Var topic=myQuery.find("code");
      //int value = topic.convert<int>();
      //printf("Value=%d\n",value);
    } catch (...)
    {
        printf("Exception\n");

    }

}

//========================================================================
int main(int argc,char *argv[] ){

    ServerApplication app;
    //system("/opt/vc/bin/tvservice -j -m CEA > data/modes.json");

    // To set mode,
    // tvservice -e "CEA 4"

    parseModesFile();

    config.width=1920;
    config.height=1280;

    load_current_config();

    gSocketServer=new  ServerSocket(10020);
    // set-up a HTTPServer instance
    gServer= new HTTPServer(new RequestHandlerFactory, *gSocketServer, new HTTPServerParams);
    
    if (gServer) gServer->start();

    if (gServer) printf("Server Start port 10020 ............\n");

    // This is not how you should do it.
    Poco::Thread::sleep(500000);
    //app.waitForTerminationRequest();
    return app.run(argc, argv);

}
