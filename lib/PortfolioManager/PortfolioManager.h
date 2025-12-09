#ifndef PORTFOLIOMANAGER_H
#define PORTFOLIOMANAGER_H
#include "IPortfolioManager.h"
#include "HttpLibWrap.h"
#include "CSVReportReader.h"
#include "PortfolioStats.h"

class PortfolioManager : IPortfolioManager{
    private:
        AccountSubType type;
        //create an instance of HttpLibWrap here so DIP observed but also makes it easy to then change out calls?
        HttpLibWrap& apiController;
        std::string accountHistoryFileLocation;
        CSVReportReader& CSVReader;
        PortfolioStats& stats;

    public:
        PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController, CSVReportReader& CSVReader, PortfolioStats& stats);
        
        ~PortfolioManager() = default;
        bool updateAccountSubType(const AccountSubType& newType) noexcept;
        bool getAccountInfo();
        bool getAccountHistory();
        bool readInCSV();
};

#endif  