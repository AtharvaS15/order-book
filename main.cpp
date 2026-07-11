#include <iostream>
#include <map>
#include <vector>
#include <numeric>
#include <ranges>

bool getOrder()
{
    std::string order_type = "";

    while (true)
    {
        std::cout << "What Would You Like to Do? \nBuy (B) \nSell (S) \n";
        std::cin >> order_type;

        if (order_type == "B")
        {
            return true;
        }
        else if (order_type == "S")
        {
            return false;
        }
        else
        {
            std::cout << "Not a Valid Order Type \n";
        }
    }
    return true; // buy order by default, shouldn't ever trigger
}

bool getOrderType()
{
    std::string order_type = "";

    while (true)
    {
        std::cout << "What Type of Order Would You Like? \nMarket (M) \nLimit (L) \n";
        std::cin >> order_type;

        if (order_type == "M")
        {
            return true;
        }
        else if (order_type == "L")
        {
            return false;
        }
        else
        {
            std::cout << "Not a Valid Order Type \n";
        }
    }
    return true; // market order by default, shouldn't ever trigger
}

int getQuantity()
{
    int quantity;
    std::cout << "What Quantity Would You Like to Order? \n";
    std::cin >> quantity;
    return quantity;
}

int getPrice()
{
    int price;
    std::cout << "What Price Would You Like to Order?\n";
    std::cin >> price;
    return price;
}

void cleanOrderBook(std::map<float, std::pair<std::vector<int>, std::vector<int>>> &orderBook)
{
    for (auto it = orderBook.begin(); it != orderBook.end();)
    {
        auto &buyQuantities = it->second.first;
        auto &sellQuantities = it->second.second;

        if (buyQuantities.empty() && sellQuantities.empty())
        {
            it = orderBook.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void handleBuyOrder(std::map<float, std::pair<std::vector<int>, std::vector<int>>> &orderBook,
                    int &quantity, int &target_price, bool &marketOrder)
{
    for (auto &[price, quantityPair] : orderBook)
    {
        if (!marketOrder && target_price < price)
        {
            break;
        }

        std::vector<int> &sellQuantities = quantityPair.second;

        // while there are elements in both the buy and sell pair
        while (quantity > 0 && !sellQuantities.empty())
        {
            // always go to the zero index since that's the oldest order
            int currentSellOrder = sellQuantities[0];

            if (quantity > currentSellOrder)
            { // decrease buy order by amount sold, and get rid of the sell order
                quantity -= currentSellOrder;
                sellQuantities.erase(sellQuantities.begin());

                std::cout << currentSellOrder << " orders have been bought\n";
            }
            else if (quantity < currentSellOrder)
            { // decrease sell order by amount bought and get rid of the buy order
                currentSellOrder -= quantity;
                std::cout << quantity << " orders have been bought\n";
                quantity = 0;
                sellQuantities[0] = currentSellOrder;
            }
            else
            { // get rid of both
                std::cout << currentSellOrder << " orders have been bought\n";
                quantity = 0;
                sellQuantities.erase(sellQuantities.begin());
            }
        }
    }

    cleanOrderBook(orderBook);

    if (quantity > 0 && !marketOrder)
    {
        orderBook[target_price].first.push_back(quantity);
        quantity = 0;
    }

    if (quantity != 0)
        std::cout << quantity << " Units Of Your Order Could Not Be Completed Bought\n";
}

void handleSellOrder(std::map<float, std::pair<std::vector<int>, std::vector<int>>> &orderBook,
                     int &quantity, int &target_price, bool &marketOrder)
{
    for (auto &[price, quantityPair] : std::ranges::reverse_view(orderBook))
    {
        if (!marketOrder && target_price > price)
        {
            break;
        }

        std::vector<int> &buyQuantities = quantityPair.first;

        // while there are elements in both the buy and sell pair
        while (quantity > 0 && !buyQuantities.empty())
        {
            // always go to the zero index since that's the oldest order
            int currentBuyOrder = buyQuantities[0];

            if (quantity > currentBuyOrder)
            { // decrease buy order by amount sold, and get rid of the sell order
                quantity -= currentBuyOrder;
                buyQuantities.erase(buyQuantities.begin());

                std::cout << currentBuyOrder << " orders have been sold\n";
            }
            else if (quantity < currentBuyOrder)
            { // decrease sell order by amount bought and get rid of the buy order
                currentBuyOrder -= quantity;
                std::cout << quantity << " orders have been sold\n";
                quantity = 0;
                buyQuantities[0] = currentBuyOrder;
            }
            else
            { // get rid of both
                std::cout << quantity << " orders have been sold\n";
                quantity = 0;
                buyQuantities.erase(buyQuantities.begin());
            }
        }
    }

    cleanOrderBook(orderBook);

    // add the leftover quantity to the vector at that price
    if (quantity > 0 && !marketOrder)
    {
        orderBook[target_price].second.push_back(quantity);
        quantity = 0;
    }

    if (quantity != 0)
        std::cout << quantity << " Units Of Your Order Could Not Be Sold \n";
}

int main()
{

    // keep track of all prices (sorted) and quantities demanded at each price for buying and selling
    std::map<float, std::pair<std::vector<int>, std::vector<int>>> orderBook;
    bool started = false;

    while (true)
    {
        if (started)
        {
            std::string participation;
            std::cout << "Would You Like to Keep Going? Yes (Y) or No (N): \n";
            std::cin >> participation;
            if (participation != "Y")
                break;
        }

        started = true;

        int price;
        int quantity;

        bool buyOrder = getOrder();
        bool marketOrder = getOrderType();

        quantity = getQuantity();
        if (!marketOrder)
            price = getPrice();

        std::cout << (buyOrder ? "Buy" : "Sell") << " " << quantity << (marketOrder ? "" : " at " + std::to_string(price)) << "\n";

        if (buyOrder)
        {
            handleBuyOrder(orderBook, quantity, price, marketOrder);
        }
        else
        { // if it's a limit order, just add the quantity to the buy or sell vector at the target price
            handleSellOrder(orderBook, quantity, price, marketOrder);
        }
    }
}