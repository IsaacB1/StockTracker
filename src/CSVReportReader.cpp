#include "CSVReportReader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


CSVReportReader::CSVReportReader() : filePath(), actions(std::vector<Action>()){
}

//feels like this needs to be propelry returning errors rahter then booleans
bool CSVReportReader::readInFile(){

    if(filePath.empty()){
        return false;
    }
    std::ifstream CSVFile(this->filePath);

    if(!CSVFile){
        return false;
    }

    std::string line;

    // Skip header
    std::getline(CSVFile, line);

    // Read data lines
    while (std::getline(CSVFile, line)) {
        // Process each line
        //std::cout << line << std::endl;
        //this splits it and returns the values
        //BUT WE SHOULD FIGURE A MORE EFFICENT WAY OF DOINT THIS CURRENTLY O(N^2) BUT GONNA BE O(N^3) if another loop
        this->createAction(line);
        
    }
    return true;
}

void CSVReportReader::createAction(const std::string& line){

    std::stringstream ss(line);
    std::vector<std::string> values;
    //temporary 
    std::string field;

    //this stores each value in one row into values-vector
    while(std::getline(ss, field, ',')){
        //std::cout << field << std::endl;
        values.push_back(field);
    }

    std::string type = values.front();
    //checks action type
    if (type == "Deposit") {
        Deposit dep;
        //at the moment just assings while thing to date field in time will need to change
        Time depTime;
        try{
            depTime.date = values.at(1);
            dep.id = values.at(5);
            dep.total = std::stod(values.at(11));
            dep.currency = values.at(12);

            dep.time = depTime;
            dep.print();
            this->actions.push_back(dep);
        }catch(const std::out_of_range& e){
            std::cerr << "No value there " << e.what() << std::endl;
        }
        

    } else if (type == "Market buy") {
        MarketBuy markBuy;
        //at the moment just assings while thing to date field in time will need to change
        Time markBuyTime;
        try{
            markBuyTime.date = values.at(1);
            markBuy.ISIN = values.at(2);
            markBuy.ticker = values.at(3);
            markBuy.Name = values.at(4);
            markBuy.id = values.at(5);
            markBuy.NoShares = std::stod(values.at(6));
            markBuy.pricePerShare = std::stod(values.at(7));
            markBuy.currencyPricePerShare = values.at(8);
            markBuy.exchangeRate = std::stod(values.at(9));
            markBuy.total = std::stod(values.at(11));
            markBuy.currency = values.at(12);

            markBuy.time = markBuyTime;
            markBuy.print();
            this->actions.push_back(markBuy);
        }catch(const std::out_of_range& e){
            std::cerr << "No value there " << e.what() << std::endl;
        }
        
    } else {
        throw std::invalid_argument("Unknown action type: " + field);
    }
}



void CSVReportReader::setFilePath(const std::string& filePath) noexcept{
    this->filePath = filePath;
}
