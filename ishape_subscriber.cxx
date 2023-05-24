//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>
//
#include <iostream>

//
// zenoh.hxx automatically selects zenoh-c or zenoh-pico C++ wrapper
// depending on ZENOHCXX_ZENOHPICO or ZENOHCXX_ZENOHC setting
// and places it to the zenoh namespace
//
#include <iostream>
#include <unistd.h>
#include <memory>
#include <fstream>
// including JsonCpp header file
#include <jsoncpp/json/json.h>
#include<thread>

#include "zenoh.hxx"
using namespace zenoh;

     void data_handler(const Sample *sample) 
    {

		if (sample) {
			std::cout << ">> [Subscriber] Received " << " ('"<< sample->get_payload().as_string_view()
					<< "')\n";
		}
    }
    
    
const char* const colorString_[] =
{
    "BLUE",
    "RED",
    "GREEN",
    "ORANGE",
    "YELLOW",
    "MAGENTA",
    "CYAN",
    "GRAY",
    "BLACK"
};
    
    
class ZenohShapeDynamics
{
	public:
	Subscriber subscriber_;
	std::shared_ptr<Session> session_;
	typedef ::std::shared_ptr<ZenohShapeDynamics> ref_type;
	

	ZenohShapeDynamics(std::shared_ptr<Session> session, const std::string& keyExp): session_(std::move(session)),subscriber_(nullptr)
	{
		KeyExprView keyexpr(keyExp);
		std::cout<<"Subscriber Created for KeyExpr: "<<keyExp<<std::endl;
		auto subs= std::get<Subscriber>(session_->declare_subscriber(keyexpr,[&] (const Sample *sample)// Capturing object by reference
    		{ // Capturing object by reference
   
   			
			if (sample) {

			    std::cout<<"Subscriber Triggered. "<<std::endl;
			    //std::cout << ">> [Subscriber] Received " << " ('"<< sample->get_payload().as_string_view()<<" )'"<<std::endl;

			    std::string_view str_1 = sample->get_payload().as_string_view();
			    std::string text = static_cast<std::string>(str_1);
			    // json reader
			    Json::Reader reader;
			    // this will contain complete JSON data
			    Json::Value completeJsonData;
			    // reader reads the data and stores it in completeJsonData
			    reader.parse(text, completeJsonData);

			    std::cout<<"color: "<<completeJsonData["color"]<<" x:"<<completeJsonData["x"]<<" y:"<<completeJsonData["y"]<<" shapesize:"<<completeJsonData["shapesize"]<<std::endl;
		    	}
		}
		));
		subscriber_ = std::move(subs);
	}
};

class ShapeManagerClass {
   public:
    std::shared_ptr<Session> session;
    std::vector<ZenohShapeDynamics::ref_type> subList;

    ShapeManagerClass(ShapeManagerClass&&) = delete;
    ShapeManagerClass(const ShapeManagerClass&) = delete;
    ShapeManagerClass& operator=(const ShapeManagerClass&) = delete;
    ShapeManagerClass& operator=(ShapeManagerClass&&) = delete;
    
    enum { CIRCLE = 0, SQUARE = 1, TRIANGLE = 2 };

    ShapeManagerClass(const char * conf = nullptr) {

	    if(!conf)
	    {
		std::cout<<"in default configuration. "<<std::endl;
		Config config;
		Session s = std::get<Session>(zenoh::open(std::move(config)));
		session = std::make_shared<Session>(std::move(s));
	    }
	    else{

		std::cout<<"from json5 file configuration. file Name: "<<conf<<std::endl;
		Config config = std::get<Config>(config_from_file(conf));
		 auto s = std::get<Session>(zenoh::open(std::move(config)));
		 session = std::make_shared<Session>(std::move(s));
	    }
    
    }
    
    void createShapedSubscriber(int sIdx)
    {
    	    switch (sIdx)
    	    {

		case CIRCLE:
		{
		    for (int i = 0; i < 8; ++i)
		    { 
		            std::string colorStr(colorString_[i]);
		            const std::string& keyExp = "demo/ishape/shapeType/circle/"+colorStr;

		            ZenohShapeDynamics::ref_type
		            dynamics(new ZenohShapeDynamics(session, keyExp));
		            subList.push_back(dynamics);
		            
		    }
		    break;
		}

		case SQUARE:
		{
		    for (int i = 0; i < 8; ++i)
		    {
		            std::string colorStr(colorString_[i]);
		            const std::string& keyExp = "demo/ishape/shapeType/square/"+colorStr;

		            ZenohShapeDynamics::ref_type
		            dynamics(new ZenohShapeDynamics(session, keyExp));
		            subList.push_back(dynamics);
		    }

		    break;
		}

		case TRIANGLE:
		{
		    for (int i = 0; i < 8; ++i)
		    {
		            std::string colorStr(colorString_[i]);
		            const std::string& keyExp = "demo/ishape/shapeType/triangle/"+colorStr;

		            ZenohShapeDynamics::ref_type
		            dynamics(new ZenohShapeDynamics(session, keyExp));
		            subList.push_back(dynamics);
		            
		    }
		break;
		}

		default:
		    break;
     }
}
};

int main(int argc, char** argv) {

	const char * conf = argv[1];
        
       ShapeManagerClass shapeManager(conf);
            
    int choice = -1;
    while(1){
    	std::cout<< "Select Option: "<<std::endl;
    	std::cout<< "Press 0 : for Circle "<<std::endl;
    	std::cout<< "Press 1 : for Square "<<std::endl;
    	std::cout<< "Press 2 : for Triangle "<<std::endl;
    	std::cin>> choice;
    	shapeManager.createShapedSubscriber(choice);
    	}
    	
    return 0;
}
