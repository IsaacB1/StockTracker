#include <iostream>
#include <string>
#include <vector>
#ifndef IPORTFOLIOMANAGER_H
#define IPORTFOLIOMANAGER_H


class IPortfolioManager {
    private:
        //string type depends on T212
        std::vector<std::string> stocks;

        //some sort of sort function and update/inital create 

    public:
        //need to pass some input from APi to fill stocks vector
        IPortfolioManager();
        
        //need to pass aome input here as well
        bool IPortfolioManager::update();

        ~IPortfolioManager();
}; 

#endif