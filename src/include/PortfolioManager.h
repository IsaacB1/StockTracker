#include <iostream>
#include <string>
#include <vector>
#ifndef PORTFOLIOMANAGER_H
#define PORTFOLIOMANAGER_H


class PortfolioManager {
    private:
        //string type depends on T212
        std::vector<std::string> stocks;

        //some sort of sort function and update/inital create 

    public:
        //need to pass some input from APi to fill stocks vector
        PortfolioManager();
        
        //need to pass aome input here as well
        bool PortfolioManager::update();

        ~PortfolioManager();
}; 

#endif