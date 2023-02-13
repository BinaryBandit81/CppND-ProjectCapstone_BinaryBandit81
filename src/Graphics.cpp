#include <iostream>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "Graphics.h"

void Graphics::simulate()
{
    this->loadBackgroundImg();
    
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // update graphics
        this->drawStations();
    }
    
}

void Graphics::loadBackgroundImg()
{
    // create window
    _windowName = "Project Capstone";
    cv::namedWindow(_windowName, cv::WINDOW_NORMAL);

    // load image and create copy to be used for semi-transparent overlay
    cv::Mat background = cv::imread(_bgFilename);
    _images.push_back(background);         // first element is the original background
    _images.push_back(background.clone()); // second element will be the transparent overlay
    _images.push_back(background.clone()); // third element will be the result image for display
}

void Graphics::drawStations()
{
    // reset images
    _images.at(1) = _images.at(0).clone();
    _images.at(2) = _images.at(0).clone();

    // create overlay from all traffic objects
    for (auto it : _stations)
    {
        int posx, posy;
        it->getPosition(posx, posy);

        // set color according to traffic light and draw the intersection as a circle
        int red, green, blue;
        it->getCurrentColor(red, green, blue);
        cv::Scalar stationColor = cv::Scalar(blue, green, red);
        cv::circle(_images.at(1), cv::Point2d(posx, posy), 17, stationColor, -1);
    } 

    float opacity = 0.85;
    cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

    // The following code allows for resizing in case of graphics window taking up a large space 
    cv::Mat img;
    cv::resize(_images.at(2), img, cv::Size(2106,1262));//,0,0,1);
    cv::imshow(_windowName, img);
    
    cv::waitKey(33);
}
