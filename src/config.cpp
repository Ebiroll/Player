#include "config.h"
#include "stdio.h"
#include <string>

#include <fstream>
#include <sstream>
#include <streambuf>


#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/Query.h>

using namespace Poco::JSON;
using namespace Poco::Dynamic;


std::string readFileToString(std::string filename) {
    std::string ret;
    std::ifstream t(filename.c_str() /* "data/lib/jquery-2.2.3.min.js"*/);
    if (t.good())
      {
        std::stringstream buffer;
        buffer << t.rdbuf();
        ret = buffer.str();
      }
  return (ret);
}

conf config;

std::string getSelectedConfig()
{
    std::string ret="_config.json";    
    std::string current=readFileToString("data/current.txt");
    std::string type="CEA";
    
    if (current.find("CEA") != std::string::npos) {
      type="CEA";
    }  
    // DMT
    if (current.find("DMT") != std::string::npos) {
      type="DMT";
    }  
    int startpos=current.find("(");
    int endpos=current.find(")");
    if (startpos!= std::string::npos && endpos!= std::string::npos)
    {
        std::string mode=current.substr(startpos+1,endpos-startpos-1);
        ret = type + "_" + mode + "_config.json";
        std::cout << "Config file:" << ret << '\n';

    }
    // (4)
    // Todo parse   data/current.txt
    strcpy(config.name,ret.c_str());
    return ret;
}

void set_default() {
    config.r[logo].x=0;
    config.r[logo].y=0;
    config.r[logo].w=20;
    config.r[logo].h=20;

    config.r[no_smoke].x=20;
    config.r[no_smoke].y=20;
    config.r[no_smoke].w=20;
    config.r[no_smoke].h=20;


}


void load_current_config()
{
    // Set default config

    set_default();
    std::string filename=getSelectedConfig();
    std::string total= std::string("data/") + filename;


    try {

      std::string conf =readFileToString(total.c_str());
      std::cout << "---" << conf;
      std::cout << "---" << std::endl;
      Parser parser;
      Var result = parser.parse(conf);

      Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
      printf("Array \n");

      for (int i=0;i<arr->size(); i++) {
          Object::Ptr object = arr->getObject(i);
          std::string name=object->getValue<std::string>("name");
          std::string color=object->getValue<std::string>("color");
          config.r[i].x=object->getValue<int>("x");
          config.r[i].y=object->getValue<int>("y");
          config.r[i].w=object->getValue<int>("w");
          config.r[i].h=object->getValue<int>("h");
          strcpy(config.r[i].name,name.c_str());
          strcpy(config.r[i].color,color.c_str());

          printf("config w,h=%d,%d\n",object->getValue<int>("w"),object->getValue<int>("h"));
      }

      //Query myQuery(result);
      //Var topic=myQuery.find("code");
      //int value = topic.convert<int>();
      //app.logger().information(Poco::format("code %d.",value));
      //printf("Value=%d\n",value);
    } catch (...)
    {
        printf("Exception\n");

    }


}

