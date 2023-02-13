#ifndef DEPARTURE_H
#define DEPARTURE_H
#include <iostream>
#include <ctime>
#include <chrono>
#include <memory>

#include "line.h"

class Departure
{
public:
    Departure(std::string station, std::string id, std::shared_ptr<Line> line, std::chrono::time_point<std::chrono::system_clock> departureTime);
    
    void getLine(std::shared_ptr<Line> line) { line = _line;}
    bool isInStation() {return _inStation;}
    std::shared_ptr<Line> getLine() {return _line;}
    void update();
    std::string getDepartureID() {return _departureID;}
    bool isActive() {return _active;}

private:
    std::string _station;
    std::shared_ptr<Line> _line;
    bool _inStation;
    bool _active;
    std::chrono::time_point<std::chrono::system_clock> _departureTime;
    std::string _departureID;
    int prevTime = 2;
    int afterTime = -4;
};

#endif