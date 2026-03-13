// ============================================================
//  AK COFFEE HOUSE - v4.0 | Receipt + Discount System
//  Features: v3 + Student discount, bulk reward, packaging fee,
//            unique order ID, formatted receipt with date
// ============================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
typedef double Currency;

struct CoffeeSpecs {
    int milkChoice;
    int milkTemp;
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

// --- Constants ---
const double   TAX_RATE       = 0.05;
const Currency PACKAGING_FEE  = 0.50;
const Currency FREE_CUP_REWARD = 5.00;

// --- Display ---
void displayGreeting() {
    time_t t = time(0);
    tm* now = localtime(&t);
    int hour = now->tm_hour;
    cout << "\n" << setfill('*') << setw(65) << "" << endl;
    if (hour < 12)      cout << "             GOOD MORNING! WELCOME TO AK COFFEE HOUSE" << endl;
    else if (hour < 17) cout << "            GOOD AFTERNOON! WELCOME TO AK COFFEE HOUSE" << endl;
    else                cout << "             GOOD EVENING! WELCOME TO AK COFFEE HOUSE" << endl;
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
    cout << "1. Demi     3 oz           +$0.00\n";
    cout << "2. Short    8 oz           +$1.00\n";
    cout << "3. Tall     12 oz          +$1.50\n";
    cout << "4. Grande   16 oz          +$2.25\n";
    cout << "5. Venti    20-24 oz       +$3.50\n";
    cout << "6. Trenta   30-31 oz       +$4.50\n";
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
    int id, serviceChoice, studentChoice, totalQty = 0;
    char choice;

    displayGreeting();
    displayMenu();

    do {
        OrderItem item;
        cout << "\nEnter Item ID (1-8): ";
        while (!(cin >> id) || id < 1 || id > 8) {
            cout << "Invalid. Enter 1-8: "; cin.clear(); cin.ignore(1000, '\n');
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

        int sChoice;
        cout << "Select Size (1-6): "; cin >> sChoice;
        item.extras.coffee.sizeChoice = sChoice;
        switch(sChoice) {
            case 1: item.sizeName = "Demi";                            break;
            case 2: item.sizeName = "Short";  item.unitPrice += 1.00; break;
            case 3: item.sizeName = "Tall";   item.unitPrice += 1.50; break;
            case 4: item.sizeName = "Grande"; item.unitPrice += 2.25; break;
            case 5: item.sizeName = "Venti";  item.unitPrice += 3.50; break;
            case 6: item.sizeName = "Trenta"; item.unitPrice += 4.50; break;
            default: item.sizeName = "Tall";  item.unitPrice += 1.50;
        }

        cout << "Milk (1: Soy, 2: Regular, 3: Almond): "; cin >> item.extras.coffee.milkChoice;
        cout << "Temp (1: Hot, 2: Iced): ";               cin >> item.extras.coffee.milkTemp;
        cout << "Quantity: "; cin >> item.qty;
        totalQty += item.qty;
        cart.push_back(item);

        cout << ">> Added! Add more? (y/n): "; cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "\nAre you a student? (1: Yes, 2: No): "; cin >> studentChoice;
    cout << "Service Type (1: Dine-in, 2: Takeaway): "; cin >> serviceChoice;

    // --- Calculations ---
    Currency subtotal = 0;
    for (const auto& e : cart) subtotal += (e.unitPrice * e.qty);

    Currency studentDiscount = (studentChoice == 1) ? (subtotal * 0.10) : 0.00;
    Currency bulkReward      = (totalQty > 10) ? FREE_CUP_REWARD : 0.00;
    Currency packing         = (serviceChoice == 2) ? PACKAGING_FEE : 0.00;
    Currency taxableAmount   = max(0.0, (subtotal - studentDiscount - bulkReward) + packing);
    Currency tax             = taxableAmount * TAX_RATE;
    Currency total           = taxableAmount + tax;

    // --- Receipt ---
    srand(time(0));
    int uniqueID = rand() % 9000 + 1000;
    time_t t_now = time(0);
    tm* r_now = localtime(&t_now);

    cout << "\n" << setfill('#') << setw(65) << "" << endl;
    cout << " ORDER #" << uniqueID
         << "  |  DATE: " << (r_now->tm_mon + 1) << "/"
         << r_now->tm_mday << "/" << (r_now->tm_year + 1900) << endl;
    cout << "                AK COFFEE HOUSE - OFFICIAL RECEIPT" << endl;
    cout << setw(65) << "" << setfill(' ') << endl;

    for (const auto& e : cart) {
        string label = tempName(e.extras.coffee.milkTemp) + " "
                     + e.sizeName + " " + e.name
                     + " [" + milkName(e.extras.coffee.milkChoice) + "]";
        cout << left << setw(45) << label
             << "x" << setw(4) << e.qty
             << "$" << fixed << setprecision(2) << (e.unitPrice * e.qty) << endl;
    }

    cout << string(65, '-') << endl;
    cout << left << setw(50) << "SUBTOTAL:" << "$" << subtotal << endl;
    if (studentDiscount > 0)
        cout << left << setw(50) << "STUDENT DISCOUNT (10%):" << "-$" << studentDiscount << endl;
    if (bulkReward > 0)
        cout << left << setw(50) << "BULK REWARD (FREE SMALL CUP):" << "-$" << bulkReward << endl;
    if (serviceChoice == 2)
        cout << left << setw(50) << "PACKAGING FEE:" << "$" << packing << endl;
    cout << left << setw(50) << "TAX (5%):" << "$" << tax << endl;
    cout << string(65, '-') << endl;
    cout << left << setw(50) << "TOTAL PAYABLE:" << "$" << total << endl;
    cout << string(65, '=') << endl;
    cout << "            THANK YOU! YOUR BREW IS READY.\n\n";

    return 0;
}