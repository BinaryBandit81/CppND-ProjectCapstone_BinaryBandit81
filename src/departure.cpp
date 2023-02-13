#include <chrono>

#include "departure.h"

int Departure::prevTime = 0;
int Departure::afterTime = 1;

Departure::Departure(std::string station, std::string id, std::shared_ptr<Line> line, std::chrono::time_point<std::chrono::system_clock> departureTime) {
    _station = station;
    _departureID = id;
    _departureTime = departureTime;
    _inStation = false;
    _active = true;
    _line = line;
}

void Departure::update() {
    auto Now = std::chrono::system_clock::now();
    //std::chrono::duration<double> diff = _departureTime - Now;
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(_departureTime - Now);

    //std::cout << "#" + _line->getName() + " t" + std::to_string(diff.count()) + " @" + _station  + " id: " + _departureID +"\n";
    if (diff.count() >= prevTime * 60.0) {
        if (_inStation == false) {
            _inStation = true;

            std::cout << "#" + _line->getName() + " t" + std::to_string(diff.count()) + " @" + _station  + " id: " + _departureID +"\n";
        } else {
            if (diff.count() > afterTime * 60.0) {
                _active = false;
                _inStation == false;
            }
        }
    } else {
        //std::cout << "No Arrival of: " << _line->getName() << " diff: " << diff.count() << " id: " << _departureID << std::endl;
        _inStation = false;
    }
}

bool Departure::validDepartureTime(std::chrono::time_point<std::chrono::system_clock> departureTime) {
    auto Now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(departureTime - Now);

    if ((diff.count() >= prevTime * 60.0) && (diff.count() <= afterTime * 60.0)) {
        return true;
    } else {
        //std::cout << "wrong departure time " << diff.count() << std::endl;
        return false;
    }
}