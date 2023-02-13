#include <iostream>
#include <vector>
#include <fstream>

#include "Graphics.h"
#include "line.h"
#include "station.h"

std::vector<std::string> splitString(std::string line, char delimiter){
    std::vector<std::string> returnVector;
    std::string tempString;
    for (auto c : line){
        if (c == delimiter){
            returnVector.push_back(tempString);
            tempString = "";
        } else {
            tempString += c;
        }
    }
    returnVector.push_back(tempString);
    return returnVector;
}

void LoadLinesFile(std::string filename, std::vector<std::shared_ptr<Line>> &lines)
{
    // load file
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            if (lineStr.size() > 0)
            {
                std::vector<std::string> splitLine = splitString(lineStr, ',');
                int colorR = std::stoi(splitLine.at(1).substr(3,2),0,16);
                int colorB = std::stoi(splitLine.at(1).substr(5,2),0,16);
                int colorG = std::stoi(splitLine.at(1).substr(7,2),0,16);
                lines.push_back(std::make_shared<Line>(splitLine.at(0), colorR, colorB, colorG));
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }
}

void LoadStationsFile(std::string filename, std::vector<std::shared_ptr<Station>> &stations, std::vector<std::shared_ptr<Line>> &lines)
{
    int offset = 15;
    // load file
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {  
           std::vector<std::string> splitLine = splitString(lineStr, ';');
           //std::cout << "id: " << splitLine.at(0) << " name: " << splitLine.at(1) << " x: " << splitLine.at(2) << " y: " << splitLine.at(3) << std::endl;
           
           std::shared_ptr<Line> singleLine;
           std::string lineName = splitLine.at(2);
           for (auto &line: lines){
            if (line->getName() == lineName) {
                singleLine = line;
                break;
            }
           }
           stations.push_back(std::make_shared<Station>(splitLine.at(0), splitLine.at(1), std::stoi(splitLine.at(3),0,10)+offset, std::stoi(splitLine.at(4),0,10)+offset, singleLine));
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }
}

int main()
{
    std::cout << "Starting CapStone Project" << std::endl;
    // Loading File for Lines
    std::vector<std::shared_ptr<Line>> lines;
    LoadLinesFile("../data/lines.csv", lines);
    for (std::shared_ptr<Line> el : lines){
        std::cout << el->getName() << std::endl;
    }

    // Loading File for Stations
    std::vector<std::shared_ptr<Station>> stations;
    LoadStationsFile("../data/stations.csv", stations, lines);
    for (std::shared_ptr<Station> el : stations){
        //std::cout << el->getName() << std::endl;
        el->simulate();
    }

    // Starting Graphics Engine
    std::string backgroundImg = "../data/mvg_subwaystations.png";
    Graphics *graphics = new Graphics();
    graphics->setBgFilename(backgroundImg);
    graphics->setStations(stations);
    graphics->simulate();
}