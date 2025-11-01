#include "CSVReportReader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


CSVReportReader::CSVReportReader() : filePath(), actions(std::vector<ActionType>()){
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
        std::cout << line << std::endl;
        //this splits it and returns the values
        //BUT WE SHOULD FIGURE A MORE EFFICENT WAY OF DOINT THIS CURRENTLY O(N^2) BUT GONNA BE O(N^3) if another loop
        this->splitField(line);
        
    }
    return true;
}

std::vector<std::string> CSVReportReader::splitField(const std::string& line){

    std::stringstream ss(line);
    std::vector<std::string> values;
    //temporary 
    std::string field;
    while(std::getline(ss, field, ',')){
        std::cout << field << std::endl;
        values.push_back(field);
    }
    return values;
}
void CSVReportReader::setFilePath(const std::string& filePath) noexcept{
    this->filePath = filePath;
}
