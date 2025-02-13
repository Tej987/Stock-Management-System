#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Product {
private:
    int productID;
    string name;
    double price;
    int quantity;

public:
    Product(int id, string n, double p, int q) {
        productID = id;
        name = n;
        price = p;
        quantity = q;
    }

    int getProductID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void updateQuantity(int q) {
        quantity += q;
    }

    void displayProduct() const {
        cout << "ID: " << productID << " | Name: " << name << " | Price: ?" << fixed << setprecision(2) << price 
             << " | Quantity: " << quantity << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << productID << "," << name << "," << price << "," << quantity << endl;
    }

    static Product loadFromFile(ifstream &infile) {
        int id, quantity;
        string name;
        double price;
        infile >> id;
        infile.ignore();
        getline(infile, name, ',');
        infile >> price >> quantity;
        return Product(id, name, price, quantity);
    }
};

// Function to check if product ID is unique
bool isProductIDUnique(vector<Product> &products, int id) {
    for (const auto &p : products) {
        if (p.getProductID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a product by ID
Product* findProduct(vector<Product> &products, int id) {
    for (auto &p : products) {
        if (p.getProductID() == id) {
            return &p;
        }
    }
    return nullptr;
}

// Save products to file
void saveProductsToFile(vector<Product> &products) {
    ofstream outfile("stock.txt", ios::trunc);
    for (const auto &p : products) {
        p.saveToFile(outfile);
    }
    outfile.close();
}

// Load products from file
void loadProductsFromFile(vector<Product> &products) {
    ifstream infile("stock.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, quantity;
        string name;
        double price;

        infile >> id;
        infile.ignore();
        if (getline(infile, name, ',') && infile >> price >> quantity) {
            products.push_back(Product(id, name, price, quantity));
        }
    }
    infile.close();
}

// Function to delete a product
void deleteProduct(vector<Product> &products, int id) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getProductID() == id) {
            products.erase(it);
            cout << "Product deleted successfully!\n";
            saveProductsToFile(products);
            return;
        }
    }
    cout << "Product not found!\n";
}

int main() {
    vector<Product> products;
    loadProductsFromFile(products);

    int choice;
    while (true) {
        cout << "\n===== Stock Management System =====\n";
        cout << "1. Add Product\n";
        cout << "2. View Stock\n";
        cout << "3. Update Stock\n";
        cout << "4. Delete Product\n";
        cout << "5. Search Product\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, quantity;
            string name;
            double price;
            cout << "Enter Product ID: ";
            cin >> id;
            cin.ignore();

            if (!isProductIDUnique(products, id)) {
                cout << "Error! Product ID already exists.\n";
                continue;
            }

            cout << "Enter Product Name: ";
            getline(cin, name);
            cout << "Enter Price: ?";
            cin >> price;
            cout << "Enter Quantity: ";
            cin >> quantity;

            products.push_back(Product(id, name, price, quantity));
            saveProductsToFile(products);
            cout << "Product added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Available Stock =====\n";
            for (const auto &p : products) {
                p.displayProduct();
            }

        } else if (choice == 3) {
            int id, quantity;
            cout << "Enter Product ID to update stock: ";
            cin >> id;
            Product* product = findProduct(products, id);

            if (product) {
                cout << "Enter Quantity to Add (Negative to Reduce): ";
                cin >> quantity;
                product->updateQuantity(quantity);
                saveProductsToFile(products);
                cout << "Stock updated successfully!\n";
            } else {
                cout << "Product not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Product ID to delete: ";
            cin >> id;
            deleteProduct(products, id);

        } else if (choice == 5) {
            int id;
            cout << "Enter Product ID to search: ";
            cin >> id;
            Product* product = findProduct(products, id);

            if (product) {
                product->displayProduct();
            } else {
                cout << "Product not found!\n";
            }

        } else if (choice == 6) {
            cout << "Exiting Stock Management System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
