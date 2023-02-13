#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "station.h"

class Graphics
{
public:
    // constructor / desctructor

    // getters / setters
    void setBgFilename(std::string filename) { _bgFilename = filename; }
    void setStations(std::vector<std::shared_ptr<Station>> &stations) { _stations = stations; };

    // typical behaviour methods
    void simulate();

private:
    // typical behaviour methods
    void loadBackgroundImg();
    void drawStations();

    // member variables
    std::vector<std::shared_ptr<Station>> _stations;
    std::string _bgFilename;
    std::string _windowName;
    std::vector<cv::Mat> _images;
};

#endif