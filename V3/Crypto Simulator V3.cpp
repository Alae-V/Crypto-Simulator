#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Crypto {
  std::string name;
  double price;
  std::vector<double> historicalPrices;
};

struct Portfolio {
  double balance;
  std::map<std::string, double> holdings;
};

double generateRandomPrice(double minPrice, double maxPrice) {
  return minPrice +
         static_cast<double>(rand()) / RAND_MAX * (maxPrice - minPrice);
}

void updatePrices(std::vector<Crypto>& cryptos) {
  for (auto& crypto : cryptos) {
    double newPrice =
        generateRandomPrice(crypto.price * 0.9, crypto.price * 1.1);
    crypto.historicalPrices.push_back(crypto.price);
    crypto.price = newPrice;
  }
}

double findPrice(const std::string& name, const std::vector<Crypto>& cryptos) {
  for (const auto& crypto : cryptos) {
    if (crypto.name == name) return crypto.price;
  }
  return 0.0;
}

void displayPrices(const std::vector<Crypto>& cryptos) {
  for (const auto& crypto : cryptos) {
    std::cout << crypto.name << ": $" << crypto.price << "\n";
  }
}

void displayHistoricalPrices(const std::vector<Crypto>& cryptos) {
  for (const auto& crypto : cryptos) {
    std::cout << "\nHistorische Preise fuer " << crypto.name << ": ";
    for (const auto& price : crypto.historicalPrices) {
      std::cout << price << " | ";
    }
    std::cout << "\n";
  }
}

int main() {
  srand(static_cast<unsigned int>(time(0)));

  std::vector<Crypto> cryptos = {
      {"Bitcoin", generateRandomPrice(60000.0, 120000.0), {}},
      {"Ethereum", generateRandomPrice(1000.0, 4000.0), {}},
      {"Tether", generateRandomPrice(1.0, 1.0), {}},
      {"Solana", generateRandomPrice(75.0, 300.0), {}},
      {"BNB", generateRandomPrice(300.0, 1000.0), {}},
      {"USD Coin", generateRandomPrice(0.9981, 0.999), {}},
      {"XRP", generateRandomPrice(1.0, 4.0), {}},
      {"Cardano", generateRandomPrice(0.6, 0.9), {}},
      {"Dogecoin", generateRandomPrice(0.05, 1.00), {}}};

  Portfolio userPortfolio = {100000, {}};
  char choice;

  do {
    std::cout << "\n--- Menue ---\n";
    std::cout << "1: Aktuelle Preise anzeigen\n";
    std::cout << "2: Kaufen\n";
    std::cout << "3: Verkaufen\n";
    std::cout << "4: Portfolio anzeigen\n";
    std::cout << "5: Historische Preise anzeigen\n";
    std::cout << "6: Beenden\n";
    std::cin >> choice;

    switch (choice) {
      case '1':
        updatePrices(cryptos);
        displayPrices(cryptos);
        break;

      case '2': {
        int buyChoice;
        double amountToBuy;
        std::cout << "Waehle eine Krypto zum Kaufen:\n";
        for (size_t i = 0; i < cryptos.size(); i++) {
          std::cout << i + 1 << ". " << cryptos[i].name << ": $"
                    << cryptos[i].price << "\n";
        }
        std::cin >> buyChoice;
        if (buyChoice < 1 || buyChoice > static_cast<int>(cryptos.size()))
          break;
        Crypto& selectedCrypto = cryptos[buyChoice - 1];
        std::cout << "Wie viel moechten Sie kaufen?\n";
        std::cin >> amountToBuy;
        double cost = amountToBuy * selectedCrypto.price;
        if (cost > userPortfolio.balance) {
          std::cout << "Nicht genug Guthaben!\n";
          break;
        }
        userPortfolio.balance -= cost;
        userPortfolio.holdings[selectedCrypto.name] += amountToBuy;
        std::cout << "Gekauft: " << amountToBuy << " " << selectedCrypto.name
                  << " fuer $" << cost << "\n";
        break;
      }

      case '3': {
        if (userPortfolio.holdings.empty()) {
          std::cout << "Keine Kryptowaehrungen im Portfolio.\n";
          break;
        }
        std::cout << "Welche Krypto moechten Sie verkaufen?\n";
        int sellChoice, index = 1;
        double amountToSell;
        for (const auto& holding : userPortfolio.holdings) {
          std::cout << index++ << ". " << holding.first
                    << " Menge: " << holding.second << "\n";
        }
        std::cin >> sellChoice;
        if (sellChoice < 1 ||
            sellChoice > static_cast<int>(userPortfolio.holdings.size()))
          break;
        auto it = userPortfolio.holdings.begin();
        std::advance(it, sellChoice - 1);
        std::cout << "Wie viel moechten Sie verkaufen?\n";
        std::cin >> amountToSell;
        if (amountToSell < 1 || amountToSell > it->second) {
          std::cout << "Ungueltige Menge.\n";
          break;
        }
        double sellPrice = findPrice(it->first, cryptos);
        userPortfolio.balance += amountToSell * sellPrice;
        it->second -= amountToSell;
        if (it->second == 0) userPortfolio.holdings.erase(it);
        std::cout << "Verkauft: " << amountToSell << " " << it->first
                  << " fuer $" << (amountToSell * sellPrice) << "\n";
        break;
      }

      case '4':
        std::cout << "\n--- Dein Portfolio ---\n";
        std::cout << "Kontostand: $" << userPortfolio.balance << "\n";
        for (const auto& holding : userPortfolio.holdings) {
          std::cout << holding.first << ": " << holding.second << "\n";
        }
        break;

      case '5':
        displayHistoricalPrices(cryptos);
        break;

      case '6':
        std::cout << "Programm beendet. Danke fuers Spielen!\n";
        break;
    }
  } while (choice != '6');
  return 0;
}
