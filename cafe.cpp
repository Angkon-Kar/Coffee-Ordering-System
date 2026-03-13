// ============================================================
//  AK COFFEE HOUSE - v2.0 | Order Taking + Basic Pricing
//  Features: Menu display, item selection, subtotal calc, tax
// ============================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>

using namespace std;
typedef double Currency;

struct OrderItem {
    string name;
    Currency unitPrice;
    int qty;
};

const double TAX_RATE = 0.05;

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
    cout << left << setw(5) << "ID" << setw(30) << "COFFEE NAME" << "PRICE" << endl;
    cout << string(55, '-') << endl;
    cout << "1.   Cappuccino                     $6.00\n";
    cout << "2.   Cafe Latte                     $6.50\n";
    cout << "3.   Espresso                       $4.50\n";
    cout << "4.   Vanilla Latte                  $7.99\n";
    cout << "5.   Caffe Mocha                    $7.50\n";
    cout << "6.   Caramel Macchiato              $8.25\n";
    cout << "7.   Americano                      $5.00\n";
    cout << "8.   Flat White                     $6.75\n";
    cout << string(55, '-') << endl;
}

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

        cout << "Enter Quantity: ";
        cin >> item.qty;
        cart.push_back(item);

        cout << ">> " << item.name << " x" << item.qty << " added! Add more? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    // Calculate totals
    Currency subtotal = 0;
    cout << "\n" << string(55, '=') << endl;
    cout << "                  YOUR ORDER SUMMARY" << endl;
    cout << string(55, '=') << endl;

    for (const auto& entry : cart) {
        Currency lineTotal = entry.unitPrice * entry.qty;
        subtotal += lineTotal;
        cout << left << setw(30) << entry.name
             << "x" << setw(5) << entry.qty
             << "$" << fixed << setprecision(2) << lineTotal << endl;
    }

    Currency tax   = subtotal * TAX_RATE;
    Currency total = subtotal + tax;

    cout << string(55, '-') << endl;
    cout << left << setw(40) << "SUBTOTAL:"  << "$" << subtotal << endl;
    cout << left << setw(40) << "TAX (5%):"  << "$" << tax      << endl;
    cout << string(55, '-') << endl;
    cout << left << setw(40) << "TOTAL:"     << "$" << total    << endl;
    cout << string(55, '=') << endl;
    cout << "         THANK YOU! VISIT AK COFFEE HOUSE AGAIN!\n\n";

    return 0;
}