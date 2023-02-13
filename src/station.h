#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "departure.h"
#include "line.h"

class Station {
public:
    //Station(std::string id, std::string name, int x, int y, std::vector<std::shared_ptr<Line>> lines);
    Station(std::string id, std::string name, int x, int y, std::shared_ptr<Line> line);
    ~Station();

    std::string getName(){return _name;};
    void getPosition(int &x, int &y) { x = _x; y = _y;};
    void getCurrentColor(int &red, int &blue, int &green);

    void simulate();
    void updateDepartures();
    std::shared_ptr<Line> getLineByName(std::string name);
    void addDeparture(std::string depID, std::shared_ptr<Line> line, std::chrono::system_clock::time_point departureTime);
private:
    std::vector<std::thread> threads;
    std::string _id, _name;
    //std::vector<std::shared_ptr<Departure>> _departures;
    std::vector<std::unique_ptr<Departure>> _departures;
    //std::vector<std::shared_ptr<Line>> _lines;
    std::shared_ptr<Line> _line;
    int _x, _y;
    std::mutex _mtx;
};
#endif