// ============================================================
//  AK COFFEE HOUSE - v1.0 | Basic Menu Display
//  Features: Time-aware greeting, formatted menu display
// ============================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

using namespace std;

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
    cout << left << setw(5) << "ID" << setw(30) << "COFFEE NAME" << "BASE PRICE" << endl;
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
    cout << "\n  ** All prices are base (Demi size) prices **\n";
}

int main() {
    displayGreeting();
    displayMenu();

    cout << "\n  Thank you for visiting AK Coffee House!\n";
    cout << "  We'll be back with ordering soon. Stay tuned!\n\n";

    return 0;
}