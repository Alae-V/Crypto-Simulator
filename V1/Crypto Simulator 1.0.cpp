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
double findPrice(std::string str, std::vector<Crypto> cryptos) {
  for (const auto& Crypto : cryptos) {
    if (Crypto.name == str) return Crypto.price;
  }
  return 0;
}
int main() {
  srand(static_cast<unsigned int>(time(0)));

  std::vector<Crypto> cryptos = {
      {"Bitcoin", generateRandomPrice(60000.0, 120000.0)},
      {"Ethereum", generateRandomPrice(1000.0, 4000.0)},
      {"Dogecoin", generateRandomPrice(0.05, 1.00)}};
  std::cout << "Willkommen im Krypto-Handelsimulator\n";

  for (const auto& Crypto : cryptos) {
    std::cout << Crypto.name << ":$" << Crypto.price << "\n";
  }
  Portfolio userPortfolio = {100000};
  char choice;
  do {
    std::cout << "\n--- Menü ---\n";
    std::cout << "1: Preise anzeigen\n";
    std::cout << "2: Kaufen\n";
    std::cout << "3: Verkaufen\n";
    std::cout << "4: Portfolio anzeigen\n";
    std::cout << "5: Beenden\n";
    std::cin >> choice;
    switch (choice) {
      case '1':
        int x = 1;
        for (const auto& crypto : cryptos) {
          std::cout << x++ << crypto.name << ":$" << crypto.price << "\n";
        }
        break;

      case '2':
        std ::cout << "Wähle ein Krypto zum Kaufen" << std ::endl;
        int choice;
        for (const auto& crypto : cryptos) {
          std::cout << crypto.name << ":$" << crypto.price << "\n";
        }
        std::cin >> (choice);
        if (choice < 1 || choice > cryptos.size()) {
          std::cout << "Ungültige Auswahl\n" << std::endl;
          return;
        }
        Crypto& selectedCrypto = cryptos[choice - 1];
        std::cout << "Wie viel möchten Sie kaufen ?";
        double Menge;
        std::cin >> Menge;
        double Kosten = Menge * selectedCrypto.price;
        if (Kosten > userPortfolio.balance) {
          std::cout << "Nicht genug Guthaben! Dein Kontostand beträgt $"
                    << userPortfolio.balance << ".\n";
          return;
        }
        userPortfolio.balance -= Kosten;
        bool found = false;
        for (auto& holding : userPortfolio.holdings) {
          if (holding.first == selectedCrypto.name) {
            holding.second += Menge;
            found = true;
          }
        }
        if (!found) {
          userPortfolio.holdings.push_back({selectedCrypto.name, Menge});
        }
        std ::cout << "Du hast " << Menge << " von " << selectedCrypto.name
                   << " gekauft für $" << Kosten << std ::endl;
        break;

      case '3':
        std::cout << "Wählen Sie ein Krypto zum Verkaufen\n";
        int x = 1;
        for (const auto& holding : userPortfolio.holdings) {
          std::cout << x++ << holding.first << holding.second << "\n";
        }
        int choice;
        std::cin >> choice;
        if (choice < 1 || choice > userPortfolio.holdings.size()) {
          std::cout << "Ungültige Auswahl\n" << std::endl;
          return;
        }
        auto& selectedCrypto1 = userPortfolio.holdings[choice - 1];
        double Menge;
        std::cin >> Menge;
        if (Menge < 1 || Menge > selectedCrypto1.second) {
          std::cout << "Ungültige Auswahl\n" << std::endl;
          return;
        }
        userPortfolio.balance +=
            Menge * findPrice(selectedCrypto1.first, cryptos);
        selectedCrypto1.second -= Menge;
        if (selectedCrypto1.second == 0) {
          userPortfolio.holdings.erase(userPortfolio.holdings.begin() + choice -
                                       1);
        }
        std::cout << "Sie haben erfolgreich" << Menge << "von"
                  << selectedCrypto1.first << "verkauft\n";

        break;

      case '4':
        std::cout << "Kontostand :$" << userPortfolio.balance << "\n";
        for (const auto& holding : userPortfolio.holdings) {
          std::cout << holding.first << ":" << holding.second << "\n";
        }
        break;

      case '5':
        std::cout << "Programm beendet.\n";
        break;
      default:
        std::cout << "Ungültige Auswahl.\n";
    }
  } while (choice != 5);
  return 0;
}
