// ╔══════════════════════════════════════════════════════════════╗
//  AK COFFEE HOUSE - v5.0 | THE COMPLETE EXPERIENCE
//  ─────────────────────────────────────────────────────────────
//  NEW IN v5.0:
//  ✔ Loyalty Points System  (earn 1 pt per $1 spent)
//  ✔ Redeem Points          ($5 off every 50 pts)
//  ✔ Full Input Validation  (no crashes on bad input)
//  ✔ Order Modification     (remove item before checkout)
//  ✔ Animated Progress Bar  (barista crafting your order)
//  ✔ Multi-line Order Summary before receipt
//  ✔ Structured receipt with all discounts itemised
//  ✔ Random unique Order ID + timestamp
//  ✔ Clean, modular functions (easy to maintain/extend)
// ╚══════════════════════════════════════════════════════════════╝

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;
typedef double Currency;

// ─── Constants ───────────────────────────────────────────────
const double   TAX_RATE        = 0.05;
const Currency PACKAGING_FEE   = 0.50;
const Currency FREE_CUP_REWARD = 5.00;
const int      LOYALTY_REDEEM_THRESHOLD = 50;   // pts needed to redeem
const Currency LOYALTY_REDEEM_VALUE     = 5.00; // $ value per redemption

// ─── Data Structures ─────────────────────────────────────────
struct CoffeeSpecs {
    int milkChoice;  // 1=Soy  2=Regular  3=Almond
    int milkTemp;    // 1=Hot  2=Iced
    int sizeChoice;
};

union ItemAddons { CoffeeSpecs coffee; };

struct OrderItem {
    string     name;
    string     sizeName;
    string     milkLabel;
    string     tempLabel;
    Currency   unitPrice;
    int        qty;
    ItemAddons extras;
};

struct MenuEntry {
    string   name;
    Currency basePrice;
};

// ─── Menu Data ───────────────────────────────────────────────
const MenuEntry MENU[8] = {
    {"Cappuccino",        6.00},
    {"Cafe Latte",        6.50},
    {"Espresso",          4.50},
    {"Vanilla Latte",     7.99},
    {"Caffe Mocha",       7.50},
    {"Caramel Macchiato", 8.25},
    {"Americano",         5.00},
    {"Flat White",        6.75}
};

const string SIZE_NAMES[6]    = {"Demi","Short","Tall","Grande","Venti","Trenta"};
const Currency SIZE_UP[6]     = {0.00,  1.00,   1.50,  2.25,    3.50,   4.50};
const string MILK_NAMES[3]    = {"Soy","Regular","Almond"};

// ─── Utility Helpers ─────────────────────────────────────────
int readInt(const string& prompt, int lo, int hi) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= lo && val <= hi) return val;
        cout << "  [!] Please enter a number between " << lo << " and " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void separator(char c = '-', int w = 65) { cout << string(w, c) << endl; }

// ─── Display Functions ───────────────────────────────────────
void displayGreeting() {
    time_t t   = time(0);
    tm*  now   = localtime(&t);
    int  hour  = now->tm_hour;

    cout << "\n" << setfill('*') << setw(65) << "" << "\n";
    if (hour < 12)
        cout << "             GOOD MORNING! WELCOME TO AK COFFEE HOUSE\n";
    else if (hour < 17)
        cout << "            GOOD AFTERNOON! WELCOME TO AK COFFEE HOUSE\n";
    else
        cout << "             GOOD EVENING! WELCOME TO AK COFFEE HOUSE\n";
    cout << setw(65) << "" << setfill(' ') << "\n";
}

void displayMenu() {
    cout << "\n" << left << setw(5) << "ID" << setw(32) << "COFFEE NAME" << "BASE PRICE\n";
    separator();
    for (int i = 0; i < 8; ++i)
        cout << (i+1) << ".   " << left << setw(32) << MENU[i].name
             << "$" << fixed << setprecision(2) << MENU[i].basePrice << "\n";

    cout << "\n"; separator();
    cout << "                   SIZE & VOLUME GUIDE\n";
    cout << left << setw(15) << "SIZE" << setw(22) << "VOLUME" << "UPCHARGE\n";
    separator();
    string vols[6] = {"3 oz","8 oz","12 oz","16 oz","20-24 oz","30-31 oz"};
    for (int i = 0; i < 6; ++i)
        cout << (i+1) << ". " << left << setw(13) << SIZE_NAMES[i]
             << setw(22) << vols[i]
             << "+$" << fixed << setprecision(2) << SIZE_UP[i] << "\n";
    separator();
}

void displayCart(const vector<OrderItem>& cart) {
    if (cart.empty()) { cout << "  (Cart is empty)\n"; return; }
    cout << "\n"; separator('=');
    cout << "  #   ITEM                                        QTY   PRICE\n";
    separator();
    for (int i = 0; i < (int)cart.size(); ++i) {
        string label = cart[i].tempLabel + " " + cart[i].sizeName + " "
                     + cart[i].name + " [" + cart[i].milkLabel + "]";
        cout << "  " << (i+1) << ".  " << left << setw(44) << label
             << "x" << setw(5) << cart[i].qty
             << "$" << fixed << setprecision(2)
             << (cart[i].unitPrice * cart[i].qty) << "\n";
    }
    separator('=');
}

void showProgressBar() {
    cout << "\n  [Barista is crafting your order...]\n  ";
    string bar(20, '-');
    for (int i = 0; i < 20; ++i) {
        bar[i] = '#';
        cout << "\r  [" << bar << "] " << (i + 1) * 5 << "%" << flush;
        this_thread::sleep_for(chrono::milliseconds(60));
    }
    cout << "\n\n";
}

// ─── Loyalty Points ──────────────────────────────────────────
void showLoyaltyInfo(int pts) {
    cout << "\n  ★ LOYALTY POINTS: " << pts << " pts";
    if (pts >= LOYALTY_REDEEM_THRESHOLD)
        cout << "  →  You can redeem " << (pts / LOYALTY_REDEEM_THRESHOLD)
             << " reward(s) worth $"
             << fixed << setprecision(2)
             << (pts / LOYALTY_REDEEM_THRESHOLD) * LOYALTY_REDEEM_VALUE << "!";
    cout << "\n";
}

// ─── Main ────────────────────────────────────────────────────
int main() {
    vector<OrderItem> cart;
    int totalQty    = 0;
    int loyaltyPts  = 0;  // simulate persistent points (starts at 0 this session)
    char choice;

    displayGreeting();
    displayMenu();

    // ── Ordering Loop ───────────────────────────────────────
    do {
        OrderItem item;
        int id = readInt("\nEnter Item ID (1-8): ", 1, 8);
        item.name      = MENU[id-1].name;
        item.unitPrice = MENU[id-1].basePrice;

        int sChoice = readInt("Select Size   (1-6): ", 1, 6);
        item.extras.coffee.sizeChoice = sChoice;
        item.sizeName   = SIZE_NAMES[sChoice-1];
        item.unitPrice += SIZE_UP[sChoice-1];

        int mChoice = readInt("Milk Type     (1:Soy  2:Regular  3:Almond): ", 1, 3);
        item.extras.coffee.milkChoice = mChoice;
        item.milkLabel  = MILK_NAMES[mChoice-1];

        int tChoice = readInt("Temperature   (1:Hot  2:Iced): ", 1, 2);
        item.extras.coffee.milkTemp = tChoice;
        item.tempLabel  = (tChoice == 2) ? "Iced" : "Hot";

        item.qty = readInt("Quantity      (1-20): ", 1, 20);
        totalQty += item.qty;
        cart.push_back(item);

        cout << "  ✔ " << item.tempLabel << " " << item.sizeName << " "
             << item.name << " [" << item.milkLabel << " milk] x"
             << item.qty << " added to cart!\n";

        displayCart(cart);

        // ── Cart Management ─────────────────────────────
        cout << "  Options: [y] Add more  [r] Remove item  [n] Checkout\n";
        cout << "  Choice: "; cin >> choice;

        if (choice == 'r' || choice == 'R') {
            if (!cart.empty()) {
                int removeIdx = readInt("  Enter item # to remove: ", 1, (int)cart.size());
                totalQty -= cart[removeIdx-1].qty;
                cout << "  ✖ Removed: " << cart[removeIdx-1].name << "\n";
                cart.erase(cart.begin() + removeIdx - 1);
            } else {
                cout << "  Cart is already empty.\n";
            }
            choice = 'y'; // keep loop going after removal
        }

    } while (choice == 'y' || choice == 'Y');

    if (cart.empty()) {
        cout << "\n  No items in cart. Goodbye!\n";
        return 0;
    }

    // ── Customer Info ────────────────────────────────────────
    cout << "\n"; separator('=');
    int studentChoice = readInt("Are you a student? (1:Yes  2:No): ", 1, 2);
    int serviceChoice = readInt("Service type      (1:Dine-in  2:Takeaway): ", 1, 2);

    // ── Loyalty Redemption ───────────────────────────────────
    // (Simulate: user already has some points from past visits)
    loyaltyPts = 75; // example: returning customer has 75 pts
    showLoyaltyInfo(loyaltyPts);

    int redeemChoice = 0;
    int redeemable   = loyaltyPts / LOYALTY_REDEEM_THRESHOLD;
    Currency loyaltyDiscount = 0.00;
    if (redeemable > 0) {
        cout << "  Redeem loyalty points? (1:Yes  2:No): ";
        redeemChoice = readInt("  Redeem loyalty points? (1:Yes  2:No): ", 1, 2);
        if (redeemChoice == 1) {
            int howMany = readInt(
                "  How many rewards to redeem? (1-" + to_string(redeemable) + "): ",
                1, redeemable);
            loyaltyDiscount = howMany * LOYALTY_REDEEM_VALUE;
            loyaltyPts     -= howMany * LOYALTY_REDEEM_THRESHOLD;
        }
    }

    showProgressBar();

    // ── Calculations ─────────────────────────────────────────
    Currency subtotal        = 0;
    for (const auto& e : cart) subtotal += (e.unitPrice * e.qty);

    Currency studentDiscount = (studentChoice == 1) ? (subtotal * 0.10) : 0.00;
    Currency bulkReward      = (totalQty > 10) ? FREE_CUP_REWARD : 0.00;
    Currency packing         = (serviceChoice == 2) ? PACKAGING_FEE : 0.00;

    Currency afterDiscounts  = subtotal - studentDiscount - bulkReward - loyaltyDiscount + packing;
    if (afterDiscounts < 0) afterDiscounts = 0;

    Currency tax             = afterDiscounts * TAX_RATE;
    Currency total           = afterDiscounts + tax;

    // Earn new loyalty points
    int earnedPts  = (int)total;
    loyaltyPts    += earnedPts;

    // ── Receipt ──────────────────────────────────────────────
    srand(time(0));
    int   uniqueID = rand() % 9000 + 1000;
    time_t t_now   = time(0);
    tm* r_now      = localtime(&t_now);

    cout << setfill('#') << setw(65) << "" << "\n" << setfill(' ');
    cout << "  ORDER #" << uniqueID
         << "  |  DATE: "
         << setw(2) << setfill('0') << (r_now->tm_mon + 1) << "/"
         << setw(2) << r_now->tm_mday << "/"
         << (r_now->tm_year + 1900) << setfill(' ') << "\n";
    cout << "             AK COFFEE HOUSE - OFFICIAL RECEIPT\n";
    cout << setfill('#') << setw(65) << "" << "\n" << setfill(' ');

    for (const auto& e : cart) {
        string label = e.tempLabel + " " + e.sizeName + " " + e.name
                     + " [" + e.milkLabel + "]";
        cout << "  " << left << setw(45) << label
             << "x" << setw(4) << e.qty
             << "$" << fixed << setprecision(2)
             << (e.unitPrice * e.qty) << "\n";
    }

    separator();
    cout << left << setw(50) << "  SUBTOTAL:"       << "$" << fixed << setprecision(2) << subtotal       << "\n";
    if (studentDiscount > 0)
        cout << left << setw(50) << "  STUDENT DISCOUNT (10%):"    << "-$" << studentDiscount  << "\n";
    if (bulkReward > 0)
        cout << left << setw(50) << "  BULK REWARD (>10 items):"   << "-$" << bulkReward       << "\n";
    if (loyaltyDiscount > 0)
        cout << left << setw(50) << "  LOYALTY REWARD REDEEMED:"   << "-$" << loyaltyDiscount  << "\n";
    if (serviceChoice == 2)
        cout << left << setw(50) << "  PACKAGING FEE (TAKEAWAY):"  << "+$" << packing          << "\n";
    cout << left << setw(50) << "  TAX (5%):"       << "+$" << tax   << "\n";
    separator();
    cout << left << setw(50) << "  TOTAL PAYABLE:"  << " $" << total  << "\n";
    separator('=');

    cout << "\n  ★ LOYALTY POINTS EARNED THIS ORDER: +" << earnedPts << " pts\n";
    cout << "  ★ YOUR NEW BALANCE: " << loyaltyPts << " pts";
    if (loyaltyPts >= LOYALTY_REDEEM_THRESHOLD)
        cout << "  (Ready to redeem on next visit!)";
    cout << "\n\n";

    separator('=');
    cout << "         ☕  THANK YOU!  YOUR BREW IS READY.  ☕\n";
    cout << "         Visit us again at AK COFFEE HOUSE!\n";
    separator('=');
    cout << "\n";

    return 0;
}