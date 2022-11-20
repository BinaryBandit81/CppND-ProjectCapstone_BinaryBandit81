#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <chrono>

#include <sstream>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <json/json.h>

#include "station.h"
#include "line.h"

using namespace curlpp::options;

//Station::Station(std::string id, std::string name, int x, int y, std::vector<std::shared_ptr<Line>> lines) {
    Station::Station(std::string id, std::string name, int x, int y, std::shared_ptr<Line> line) {
    _id = id;
    _name = name;
    _x = x;
    _y = y;
    //_lines = lines;
    _line = line;
}

Station::~Station() {
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}
std::shared_ptr<Line> Station::getLineByName(std::string name) {
    if (_line->getName() == name) {
        return _line;
    }
    return nullptr;
}

void Station::getCurrentColor(int &red, int &blue, int &green) {
    red = 0;
    blue = 0;
    green = 0;
    _mtx.lock();
    for (auto const&  dep : _departures) {
        if (dep->isInStation() && dep->isActive()) {
            /*if (dep->getLine() != nullptr) {
                dep->getLine()->getColor(red, blue, green);
            }*/
            _line->getColor(red, blue, green);
            //std::cout << "Vehicle is in Station" << std::endl;
        }
    }
    _mtx.unlock();
}

void Station::addDeparture(std::string depID, std::shared_ptr<Line> line, std::chrono::system_clock::time_point departureTime) {
    bool departureIsNew = true;
    _mtx.lock();
    for (auto const& dep : _departures){
        if (dep->getDepartureID() == depID) {
            departureIsNew = false;
            break;
        }
    }
    if (departureIsNew) {
        _departures.push_back(std::make_unique<Departure>(this->getName(), depID, line, departureTime));
    }
    _mtx.unlock();
}

void Station::simulate() {
    threads.emplace_back(std::thread(&Station::updateDepartures, this));
}

void Station::updateDepartures()
{
    // print id of the current thread
    std::stringstream threadID;
    threadID << std::this_thread::get_id();
    std::cout << "Running Thread " + threadID.str() + " for Station: " + _name +"\n";

    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::ostringstream response;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    //Json::Reader reader;
    Json::Value root;
    JSONCPP_STRING errs;

    std::string url = "https://www.mvg.de/api/fahrinfo/departure/"+ _id + "?footway=0";

    // continuously process the departure queue
    while (true)
    {
        
        // Setting the URL to retrive.
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        //std::cout << "ID: " << _id << " request: " << request << std::endl;

        std::string rawJson = std::string(response.str());
        const auto rawJsonLength = static_cast<int>(rawJson.length());
        
        if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &errs)) {
        //if (!parseFromStream(builder, rawJson, &root, &errs)) {
            std::cout << errs << std::endl;
            return;
        }
        //std::cout << "id: "<< _id << " departures: " << root["departures"][0]["label"] << std::endl;
        for (auto respDep : root["departures"]) {
            if (respDep["product"] == "UBAHN") {
                if (respDep["label"] == _line->getName()) {
                    //std::cout << "id: " << _id << " Label: " << respDep["label"]  << " time: " << respDep["departureTime"] << std::endl;
                    //std::cout << "lineResp: " << respDep["label"].asString() << " lineMem: " << getLineByName(respDep["label"].asString()) << std::endl;
                    std::chrono::system_clock::time_point departureTime(std::chrono::milliseconds(std::stoll(respDep["departureTime"].asString())));
                    addDeparture(respDep["departureId"].asString(), getLineByName(respDep["label"].asString()), departureTime);
                }
            }
        }

        // update all Departures
        for (auto const&  dep : _departures) {
            dep->update();
        }
        /*
        // delete all inactive departures
        for(unsigned int i = 0; i < _departures.size();){
            if (_departures[i]->isActive() == false) {
                //_mtx.lock();
                std::cout << "remove departure: " << _departures[i]->getDepartureID() << std::endl;
                _departures.erase(_departures.begin()+i);
                //_mtx.unlock();
            }
        }
        */
        
        _departures.erase(
            std::remove_if(_departures.begin(), _departures.end(), 
            [&](const std::unique_ptr<Departure> &delDep){
                return (delDep->isActive() == false);
            }), _departures.end()
        );
                

        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::cout << "Continue Thread " + threadID.str() + " for Station: " + _name +"\n";
    }
}