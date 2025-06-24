#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

struct Crypto {
  std::string name;
  double price;
};

struct Portfolio {
  double balance;
  std::vector<std::pair<std::string, double>> holdings;
};

double generateRandomPrice(double minPrice, double maxPrice) {
  return minPrice +
         static_cast<double>(rand()) / RAND_MAX * (maxPrice - minPrice);
}

double findPrice(const std::string& str, const std::vector<Crypto>& cryptos) {
  for (const auto& crypto : cryptos) {
    if (crypto.name == str) return crypto.price;
  }
  return 0.0;
}

int main() {
  srand(static_cast<unsigned int>(time(0)));

  std::vector<Crypto> cryptos = {
      {"Bitcoin", generateRandomPrice(60000.0, 120000.0)},
      {"Ethereum", generateRandomPrice(1000.0, 4000.0)},
      {"Dogecoin", generateRandomPrice(0.05, 1.00)}};

  std::cout << "Willkommen im Krypto-Handelsimulator\n";

  for (const auto& crypto : cryptos) {
    std::cout << crypto.name << ": $" << crypto.price << "\n";
  }

  Portfolio userPortfolio = {100000, {}};

  char choice;
  int index = 1;
  int buyChoice = -1;
  int sellChoice = -1;
  double amountToBuy = 0.0;
  double amountToSell = 0.0;
  double cost = 0.0;
  double sellPrice = 0.0;
  Crypto* selectedCrypto = nullptr;
  std::pair<std::string, double>* selectedHolding = nullptr;
  bool found = false;

  do {
    std::cout << "\n--- Menue ---\n";
    std::cout << "1: Preise anzeigen\n";
    std::cout << "2: Kaufen\n";
    std::cout << "3: Verkaufen\n";
    std::cout << "4: Portfolio anzeigen\n";
    std::cout << "5: Beenden\n";
    std::cin >> choice;

    switch (choice) {
      case '1':
        index = 1;
        for (const auto& crypto : cryptos) {
          std::cout << index++ << ". " << crypto.name << ": $" << crypto.price
                    << "\n";
        }
        break;

      case '2':
        std::cout << "Waehle ein Krypto zum Kaufen:\n";
        index = 1;
        for (const auto& crypto : cryptos) {
          std::cout << index++ << ". " << crypto.name << ": $" << crypto.price
                    << "\n";
        }

        std::cin >> buyChoice;
        if (buyChoice < 1 || buyChoice > static_cast<int>(cryptos.size())) {
          std::cout << "Ungueltige Auswahl.\n";
          break;
        }

        selectedCrypto = &cryptos[buyChoice - 1];
        std::cout << "Wie viel moechten Sie kaufen?\n";
        std::cin >> amountToBuy;

        cost = amountToBuy * selectedCrypto->price;
        if (cost > userPortfolio.balance) {
          std::cout << "Nicht genug Guthaben! Dein Kontostand betraegt $"
                    << userPortfolio.balance << ".\n";
          break;
        }

        userPortfolio.balance -= cost;
        found = false;
        for (auto& holding : userPortfolio.holdings) {
          if (holding.first == selectedCrypto->name) {
            holding.second += amountToBuy;
            found = true;
            break;
          }
        }

        if (!found) {
          userPortfolio.holdings.push_back({selectedCrypto->name, amountToBuy});
        }

        std::cout << "Du hast " << amountToBuy << " von "
                  << selectedCrypto->name << " gekauft fuer $" << cost << ".\n";
        break;

      case '3':
        if (userPortfolio.holdings.empty()) {
          std::cout << "Du besitzt keine Kryptowaehrungen zum Verkaufen.\n";
          break;
        }

        std::cout << "Waehlen Sie ein Krypto zum Verkaufen:\n";
        index = 1;
        for (const auto& holding : userPortfolio.holdings) {
          std::cout << index++ << ". " << holding.first
                    << ": Menge: " << holding.second << "\n";
        }

        std::cin >> sellChoice;
        if (sellChoice < 1 ||
            sellChoice > static_cast<int>(userPortfolio.holdings.size())) {
          std::cout << "Ungueltige Auswahl.\n";
          break;
        }

        selectedHolding = &userPortfolio.holdings[sellChoice - 1];
        std::cout << "Wie viel moechten Sie verkaufen?\n";
        std::cin >> amountToSell;

        if (amountToSell < 1 || amountToSell > selectedHolding->second) {
          std::cout << "Ungueltige Menge.\n";
          break;
        }

        sellPrice = findPrice(selectedHolding->first, cryptos);
        userPortfolio.balance += amountToSell * sellPrice;
        selectedHolding->second -= amountToSell;

        if (selectedHolding->second == 0) {
          userPortfolio.holdings.erase(userPortfolio.holdings.begin() +
                                       sellChoice - 1);
        }

        std::cout << "Sie haben erfolgreich " << amountToSell << " von "
                  << selectedHolding->first << " verkauft fuer $"
                  << (amountToSell * sellPrice) << ".\n";
        break;

      case '4':
        std::cout << "\n--- Dein Portfolio ---\n";
        std::cout << "Kontostand: $" << userPortfolio.balance << "\n";
        if (userPortfolio.holdings.empty()) {
          std::cout << "Keine Kryptowaehrungen im Portfolio.\n";
        } else {
          for (const auto& holding : userPortfolio.holdings) {
            std::cout << holding.first << ": Menge: " << holding.second << "\n";
          }
        }
        break;

      case '5':
        std::cout << "Programm beendet. Danke fuers Spielen!\n";
        break;

      default:
        std::cout << "Ungueltige Auswahl. Bitte waehlen Sie erneut.\n";
        break;
    }
  } while (choice != '5');

  return 0;
}
