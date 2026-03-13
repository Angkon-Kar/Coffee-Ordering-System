// ============================================================
//  AK COFFEE HOUSE - v3.0 | Size + Customization System
//  Features: v2 + Size upcharge, milk choice, temperature,
//            struct/union for coffee specs
// ============================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>

using namespace std;
typedef double Currency;

// --- Data Structures ---
struct CoffeeSpecs {
    int milkChoice;   // 1=Soy  2=Regular  3=Almond
    int milkTemp;     // 1=Hot  2=Cold
    int sizeChoice;
};

union ItemAddons {
    CoffeeSpecs coffee;
};

struct OrderItem {
    string     name;
    string     sizeName;
    Currency   unitPrice;
    int        qty;
    ItemAddons extras;
};

const double TAX_RATE = 0.05;

// --- Helpers ---
void displayGreeting() {
    time_t t = time(0);
    tm* now = localtime(&t);
    int hour = now->tm_hour;

    cout << "\n" << setfill('*') << setw(65) << "" << endl;
    if (hour < 12)
        cout << "             GOOD MORNING! WELCOME TO AK COFFEE HOUSE" << endl;
    else if (hour < 17)
        cout << "            GOOD AFTERNOON! WELCOME TO AK COFFEE HOUSE" << endl;
    else
        cout << "             GOOD EVENING! WELCOME TO AK COFFEE HOUSE" << endl;
    cout << setw(65) << "" << setfill(' ') << endl;
}

void displayMenu() {
    cout << left << setw(5) << "ID" << setw(30) << "COFFEE NAME" << "BASE (DEMI)" << endl;
    cout << string(65, '-') << endl;
    cout << "1.   Cappuccino                     $6.00\n";
    cout << "2.   Cafe Latte                     $6.50\n";
    cout << "3.   Espresso                       $4.50\n";
    cout << "4.   Vanilla Latte                  $7.99\n";
    cout << "5.   Caffe Mocha                    $7.50\n";
    cout << "6.   Caramel Macchiato              $8.25\n";
    cout << "7.   Americano                      $5.00\n";
    cout << "8.   Flat White                     $6.75\n";

    cout << "\n" << string(65, '-') << endl;
    cout << "                   SIZE & VOLUME GUIDE\n";
    cout << left << setw(15) << "SIZE" << setw(25) << "VOLUME" << "UPCHARGE" << endl;
    cout << string(65, '-') << endl;
    cout << "1. Demi        3 oz                      +$0.00\n";
    cout << "2. Short       8 oz                      +$1.00\n";
    cout << "3. Tall        12 oz                     +$1.50\n";
    cout << "4. Grande      16 oz                     +$2.25\n";
    cout << "5. Venti       20-24 oz                  +$3.50\n";
    cout << "6. Trenta      30-31 oz                  +$4.50\n";
    cout << string(65, '-') << endl;
}

string milkName(int m) {
    if (m == 1) return "Soy";
    if (m == 3) return "Almond";
    return "Regular";
}

string tempName(int t) { return (t == 2) ? "Iced" : "Hot"; }

int main() {
    vector<OrderItem> cart;
    char choice;
    int id;

    displayGreeting();
    displayMenu();

    do {
        OrderItem item;
        cout << "\nEnter Item ID (1-8): ";
        while (!(cin >> id) || id < 1 || id > 8) {
            cout << "Invalid. Enter 1-8: ";
            cin.clear(); cin.ignore(1000, '\n');
        }

        switch(id) {
            case 1: item.name = "Cappuccino";         item.unitPrice = 6.00; break;
            case 2: item.name = "Cafe Latte";         item.unitPrice = 6.50; break;
            case 3: item.name = "Espresso";           item.unitPrice = 4.50; break;
            case 4: item.name = "Vanilla Latte";      item.unitPrice = 7.99; break;
            case 5: item.name = "Caffe Mocha";        item.unitPrice = 7.50; break;
            case 6: item.name = "Caramel Macchiato";  item.unitPrice = 8.25; break;
            case 7: item.name = "Americano";          item.unitPrice = 5.00; break;
            case 8: item.name = "Flat White";         item.unitPrice = 6.75; break;
        }

        // Size
        cout << "Select Size (1-6): ";
        int sChoice; cin >> sChoice;
        item.extras.coffee.sizeChoice = sChoice;
        switch(sChoice) {
            case 1: item.sizeName = "Demi";                             break;
            case 2: item.sizeName = "Short";  item.unitPrice += 1.00;  break;
            case 3: item.sizeName = "Tall";   item.unitPrice += 1.50;  break;
            case 4: item.sizeName = "Grande"; item.unitPrice += 2.25;  break;
            case 5: item.sizeName = "Venti";  item.unitPrice += 3.50;  break;
            case 6: item.sizeName = "Trenta"; item.unitPrice += 4.50;  break;
            default: item.sizeName = "Tall";  item.unitPrice += 1.50;
        }

        // Customization
        cout << "Milk Type (1: Soy, 2: Regular, 3: Almond): ";
        cin >> item.extras.coffee.milkChoice;
        cout << "Temperature  (1: Hot, 2: Iced): ";
        cin >> item.extras.coffee.milkTemp;

        cout << "Quantity: ";
        cin >> item.qty;
        cart.push_back(item);

        cout << ">> " << tempName(item.extras.coffee.milkTemp) << " "
             << item.sizeName << " " << item.name
             << " [" << milkName(item.extras.coffee.milkChoice) << " milk] added!\n";
        cout << "Add more? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    // Summary
    Currency subtotal = 0;
    cout << "\n" << string(65, '=') << endl;
    cout << "                  YOUR ORDER SUMMARY" << endl;
    cout << string(65, '=') << endl;

    for (const auto& e : cart) {
        string label = tempName(e.extras.coffee.milkTemp) + " " + e.sizeName + " " + e.name
                       + " [" + milkName(e.extras.coffee.milkChoice) + "]";
        Currency line = e.unitPrice * e.qty;
        subtotal += line;
        cout << left << setw(45) << label
             << "x" << setw(4) << e.qty
             << "$" << fixed << setprecision(2) << line << endl;
    }

    Currency tax   = subtotal * TAX_RATE;
    Currency total = subtotal + tax;

    cout << string(65, '-') << endl;
    cout << left << setw(50) << "SUBTOTAL:"  << "$" << subtotal << endl;
    cout << left << setw(50) << "TAX (5%):"  << "$" << tax      << endl;
    cout << string(65, '-') << endl;
    cout << left << setw(50) << "TOTAL PAYABLE:" << "$" << total << endl;
    cout << string(65, '=') << endl;
    cout << "         THANK YOU! SEE YOU AT AK COFFEE HOUSE!\n\n";

    return 0;
}