#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <map>
using namespace std;

map<string, int> itemSalesCount;
int customerCounter = 1;
float DAILY_TOTAL = 0.0;

class FoodItem {
public:
    int id;
    string name;
    float price;
    FoodItem(int id, string name, float price) : id(id), name(name), price(price) {}
    void display() {
        cout << id << ". " << name << " - Rs. " << price << endl;
    }
};

class BillItem : public FoodItem {
public:
    int quantity;
    BillItem(FoodItem item, int quantity) : FoodItem(item), quantity(quantity) {}
    float getTotal() {
        return price * quantity;
    }
};

class Order {
public:
    string customerID;
    string timestamp;
    vector<BillItem> items;
    float total;

    Order(string cid, string time, vector<BillItem> items, float total)
        : customerID(cid), timestamp(time), items(items), total(total) {}

    void displayOrder() {
        cout << "\n--- Order for " << customerID << " ---\n";
        cout << "Timestamp: " << timestamp;
        for (auto &item : items) {
            cout << item.name << " x" << item.quantity << " = Rs. " << item.getTotal() << endl;
        }
        cout << fixed << setprecision(2);
        cout << "Total: Rs. " << total << endl;
    }
};

vector<FoodItem> menu;
vector<Order> allOrders;

void showMenu() {
    cout << "\n--- Menu ---\n";
    for (auto &item : menu) {
        item.display();
    }
}

void addMenuItem() {
    int id = menu.size() + 1;
    string name;
    float price;
    cout << "Enter item name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter item price: ";
    cin >> price;
    menu.emplace_back(id, name, price);
    cout << "Item added!\n";
}

void deleteMenuItem() {
    showMenu();
    int id;
    cout << "Enter item ID to delete: ";
    cin >> id;
    if (id > 0 && id <= menu.size()) {
        menu.erase(menu.begin() + id - 1);
        cout << "Item deleted.\n";
    } else {
        cout << "Invalid ID.\n";
    }
}

string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

void takeOrder() {
    vector<BillItem> bill;
    int id, qty;
    float subtotal = 0;
    string choice;
    string custID = "CUST" + to_string(customerCounter);
    string time = getCurrentTime();

    cout << "\nTaking order for Customer ID: " << custID << endl;
    cout << "Timestamp: " << time;

    do {
        showMenu();
        cout << "Enter item ID to order: ";
        cin >> id;
        cout << "Enter quantity: ";
        cin >> qty;
        if (id > 0 && id <= menu.size()) {
            bill.emplace_back(menu[id - 1], qty);
            subtotal += menu[id - 1].price * qty;
            itemSalesCount[menu[id - 1].name] += qty;
        } else {
            cout << "Invalid ID.\n";
        }
        cout << "Add more? (y/n): ";
        cin >> choice;
    } while (choice == "y");

    float gst = subtotal * 0.05;
    float vat = subtotal * 0.1;
    float total = subtotal + gst + vat;

    cout << "\n--- BILL ---\n";
    for (auto &item : bill) {
        cout << item.name << " x" << item.quantity << " = Rs. " << item.getTotal() << endl;
    }
    cout << fixed << setprecision(2);
    cout << "Subtotal: Rs. " << subtotal << endl;
    cout << "GST (5%): Rs. " << gst << endl;
    cout << "VAT (10%): Rs. " << vat << endl;
    cout << "Total: Rs. " << total << endl;

    allOrders.emplace_back(custID, time, bill, total);
    DAILY_TOTAL += total;
    customerCounter++;
}

void showAllOrdersWithSummary() {
    cout << "\n--- Individual Orders ---\n";
    for (auto &order : allOrders) {
        order.displayOrder();
    }

    cout << "\n--- Items Sold Summary ---\n";
    for (auto &entry : itemSalesCount) {
        cout << entry.first << ": " << entry.second << endl;
    }

    cout << "\nTotal Sales: Rs. " << fixed << setprecision(2) << DAILY_TOTAL << endl;
}

int main() {
    int choice;
    do {
        cout << "\n--- Fast Food Management ---\n";
        cout << "1. Add Menu Item\n2. View Menu\n3. Delete Menu Item\n4. Take Order\n5. Show Individual Orders & Sales Summary\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addMenuItem(); break;
            case 2: showMenu(); break;
            case 3: deleteMenuItem(); break;
            case 4: takeOrder(); break;
            case 5: showAllOrdersWithSummary(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid!\n";
        }
    } while (choice != 6);

    return 0;
}
