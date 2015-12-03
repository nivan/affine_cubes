#include "crow.h"
#include "json.h"
#include <sstream>
#include <iostream>
#include "../affine_cube/bvtree.h"


class ExampleLogHandler : public crow::ILogHandler {
    public:
        void log(std::string message, crow::LogLevel level) override {
//            cerr << "ExampleLogHandler -> " << message;
        }
};

struct ExampleMiddleware 
{
    std::string message;

    ExampleMiddleware() 
    {
        message = "foo";
    }

    void setMessage(std::string newMsg)
    {
        message = newMsg;
    }

    struct context
    {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        CROW_LOG_DEBUG << " - MESSAGE: " << message;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        // no-op
    }
};

int numDimensions = 4;
BVTree* myTree = NULL;

void createCube(){

    //
    std::vector<Vector> points;

    //random data
    int numPoints = 1000;
    for(int i = 0 ; i < numPoints ; ++i){
      Vector point(numDimensions);
      for(int d = 0 ; d < numDimensions ; ++d){
	point[d] = (rand() * 1.0) / RAND_MAX;
      }
      points.push_back(point);
    }

    myTree = new BVTree(points);
}

std::string bin2JSONObject(Histogram2DBin& bin){
  std::stringstream ss;
  ss << "{\"xmin\":" << bin.minX << ",\"xmax\":" << bin.maxX << ",\"ymin\":" << bin.minY << ",\"ymax\":" << bin.maxY << ",\"count\":" << bin.count << "}";
  return ss.str();
}

int main(){
  createCube();

  crow::App<ExampleMiddleware> app;

  app.get_middleware<ExampleMiddleware>().setMessage("hello");

  //
  CROW_ROUTE(app,"/num_points/")
    ([](){
      std::ostringstream os;
      os << myTree->getNumPoints();
      return crow::response(os.str());
    });

  //
  CROW_ROUTE(app, "/build_histogram_2d")
    .methods("POST"_method)
    ([](const crow::request& req){
      std::cout << "----> Build Histogram" << std::endl;
      auto x = crow::json::load(req.body);
      if (!x)
	return crow::response(400);

      std::cout << "----------> JSON OK" << std::endl;

      //
      auto vectorX = x["x_axis"];
      int xDim = vectorX.size();
      auto vectorY = x["y_axis"];
      int yDim = vectorY.size();
      auto treeDepth = x["depth"];

      std::cout << "x type " << (vectorX.t() == crow::json::type::List) << std::endl;
      std::cout << "y type " << (vectorY.t() == crow::json::type::List) << std::endl;
      std::cout << "d type " << (treeDepth.t() == crow::json::type::Number) << std::endl;
      std::cout << "xDim " << xDim << " yDim " << yDim << " numDimensions " << numDimensions << std::endl;

      if(vectorX.t() != crow::json::type::List || vectorY.t() != crow::json::type::List || 
	 treeDepth.t() != crow::json::type::Number || xDim != numDimensions || yDim != numDimensions)
	return crow::response(400);
      
      std::cout << "HERE+++++++++++" << std::endl;

      //get parameters
      int depth = treeDepth.d();
      Vector xAxis(numDimensions); 
      Vector yAxis(numDimensions);
      for(int i = 0 ; i < numDimensions ; ++i){
	xAxis[i] = vectorX[i].d();
	yAxis[i] = vectorY[i].d();
      }
      //compute result
      std::vector<Histogram2DBin> result;
      myTree->getUnbinnedHistogram(xAxis,yAxis,depth,result);
      int numbins = result.size();
      
      std::ostringstream os;
      
      os << "[";
      if(numbins > 0){
	Histogram2DBin bin = result.at(0);
	std::string binStr = bin2JSONObject(bin);
	os << binStr;
	for(int i = 1 ; i < numbins ; ++i){
	  Histogram2DBin bin = result.at(i);
	  std::string binStr = bin2JSONObject(bin);
	  os << "," << binStr;
	}
      }
      os << "]";

      //
      crow::response myResponse = {os.str()};
      myResponse.add_header("Access-Control-Allow-Origin", "http://localhost:8000");
      myResponse.add_header("Access-Control-Allow-Methods", "GET, PUT, POST, DELETE, OPTIONS");
      myResponse.add_header("Access-Control-Max-Age", "1000");
      myResponse.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
  
      return myResponse;
    });

  // ignore all log
  crow::logger::setLogLevel(crow::LogLevel::DEBUG);
  //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

  app.port(18080)
    .multithreaded()
    .run();
}


#if 0
void scratch(){
  CROW_ROUTE(app, "/")
    .name("hello")
    ([]{
      return "Hello World!";
    });

  CROW_ROUTE(app, "/about")
    ([](){
      return "About Crow example.";
    });

  // a request to /path should be forwarded to /path/
  CROW_ROUTE(app, "/path/")
    ([](){
      return "Trailing slash test case..";
    });


  // simple json response
  // To see it in action enter {ip}:18080/json
  CROW_ROUTE(app, "/json")
    ([]{
      crow::json::wvalue x;
      x["message"] = "Hello, World!";
      return x;
    });

  // To see it in action enter {ip}:18080/hello/{integer_between -2^32 and 100} and you should receive
  // {integer_between -2^31 and 100} bottles of beer!
  CROW_ROUTE(app,"/hello/<int>")
    ([](int count){
      if (count > 100)
	return crow::response(400);
      std::ostringstream os;
      os << count << " bottles of beer! " << myTree->getNumPoints();;
      return crow::response(os.str());
    });

  // To see it in action submit {ip}:18080/add/1/2 and you should receive 3 (exciting, isn't it)
  CROW_ROUTE(app,"/add/<int>/<int>")
    ([](const crow::request& req, crow::response& res, int a, int b){
      std::ostringstream os;
      os << a+b;
      res.write(os.str());
      res.end();
    });

  // Compile error with message "Handler type is mismatched with URL paramters"
  //CROW_ROUTE(app,"/another/<int>")
  //([](int a, int b){
  //return crow::response(500);
  //});

  // more json example

  // To see it in action, I recommend to use the Postman Chrome extension:
  //      * Set the address to {ip}:18080/add_json
  //      * Set the method to post
  //      * Select 'raw' and then JSON
  //      * Add {"a": 1, "b": 1}
  //      * Send and you should receive 2
  CROW_ROUTE(app, "/add_json")
    .methods("POST"_method)
    ([](const crow::request& req){
      auto x = crow::json::load(req.body);
      if (!x)
	return crow::response(400);
      int sum = x["a"].i()+x["b"].i();
      std::ostringstream os;
      os << sum;
      return crow::response{os.str()};
    });

  // Example of a request taking URL parameters
  // If you want to activate all the functions just query
  // {ip}:18080/params?foo='blabla'&pew=32&count[]=a&count[]=b
  CROW_ROUTE(app, "/params")
    ([](const crow::request& req){
      std::ostringstream os;

      // To get a simple string from the url params
      // To see it in action /params?foo='blabla'
      os << "Params: " << req.url_params << "\n\n"; 
      os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";

      // To get a double from the request
      // To see in action submit something like '/params?pew=42'
      if(req.url_params.get("pew") != nullptr) {
	double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
	os << "The value of 'pew' is " <<  countD << '\n';
      }

      // To get a list from the request
      // You have to submit something like '/params?count[]=a&count[]=b' to have a list with two values (a and b)
      auto count = req.url_params.get_list("count");
      os << "The key 'count' contains " << count.size() << " value(s).\n";
      for(const auto& countVal : count) {
	os << " - " << countVal << '\n';
      }
      return crow::response{os.str()};
    });    
}
#endif
