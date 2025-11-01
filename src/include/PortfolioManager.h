#ifndef PORTFOLIOMANAGER_H
#define PORTFOLIOMANAGER_H
#include "IPortfolioManager.h"
#include "HttpLibWrap.h"
#include "CSVReportReader.h"


class PortfolioManager : IPortfolioManager{
    private:
        std::vector<Stock> stocks;
        AccountValue accVal;
        AccountSubType type;
        //create an instance of HttpLibWrap here so DIP observed but also makes it easy to then change out calls?
        HttpLibWrap& apiController;
        std::string accountHistoryFileLocation;
        CSVReportReader& CSVReader;

    public:
        PortfolioManager(const AccountSubType& type, HttpLibWrap& apiController, CSVReportReader& CSVReader);
        ~PortfolioManager() = default;
        bool update(Stock newStock);
        //wont be implemented but could be used to connet to my server to store data
        void updateAccountSubType(const AccountSubType& newType) noexcept;
        bool getAccountInfo();
        bool getAccountHistory();
        bool readInCSV(const std::string& filePath);
};

#endif  