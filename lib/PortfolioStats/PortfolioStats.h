#include <string>
#include <unordered_map>
#include "IPortfolioStats.h"
#include "CSVReportReader.h"
#ifndef PORTFOLIO_STATS_H
#define PORTFOLIO_STATS_H


struct Stock {
    std::string name;
    std::string ISIN;
    double noOfShares;
};

class PortfolioStats : public IPortfolioStats {

private:
    double total_invested{0.0};
    double curr_value{0.0};
    double total_made{0.0};
    double percent_change{0.0};
    double total_free{0.0};

    std::unordered_map<std::string, Stock> stocks;


public:
    PortfolioStats() = default;

    // Setters
    void set_total_invested(double total_invested) { this->total_invested = total_invested; }
    void set_curr_value(double curr_value) { this->curr_value = curr_value; }
    void set_total_made(double total_made) { this->total_made = total_made; }
    void set_percent_change(double percent_change) { this->percent_change = percent_change; }
    void set_total_free(double total_free) { this->total_free = total_free; }

    // Getters
    double get_total_invested() const { return total_invested; }
    double get_curr_value() const { return curr_value; }
    double get_total_made() const { return total_made; }
    double get_percent_change() const { return percent_change; }
    double get_total_free() const { return total_free; }

    void updateStats(const double& total_invested, const double& curr_value, const double& total_free) noexcept;

    void printStats() noexcept;
    void addStock(const Stock& newStock, const std::string& ticker);
    void minusStock(const Stock& oldStock, const std::string& ticker);
    //goes through all the actions creating and adding stocks and account information
    void syncStocks(const std::vector<Action>&);

    ~PortfolioStats() = default;
};

#endif