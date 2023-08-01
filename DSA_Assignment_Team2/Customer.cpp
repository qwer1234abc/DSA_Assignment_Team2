#include "Customer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream> 

using namespace std;

Customer::Customer() {}

Customer::Customer(int id, const string& n, const string& l, const string& p, int lp) {
	customerID = id;
	name = n;
	loginID = l;
	password = p;
	loyaltyPoints = lp;
}

int Customer::getCustomerID() const {
	return customerID;
}

string Customer::getName() const {
	return name;
}

string Customer::getLoginID() const {
	return loginID;
}

string Customer::getPassword() const {
	return password;
}

int Customer::getLoyaltyPoints() const {
	return loyaltyPoints;
}

// Function to register a new customer
void Customer::registerCustomer(const string& filename) {
	string name, loginID, password;
	int loyaltyPoints = 0;

	cout << "Enter customer details:\n";
	cout << "Name: ";
	cin >> ws;
	getline(cin, name);
	cout << "Login ID: ";
	cin >> ws;
	getline(cin, loginID);
	cout << "Password: ";
	cin >> ws;
	getline(cin, password);

	if (!uniqueLoginID(filename, loginID)) {
		cout << "Error: Customer with this Login ID already exists.\n";
		return;
	}

	int customerID = 0; // Default ID if no accounts are present

	// Get the last customer ID from the file
	ifstream file(filename); // create input file stream object named file and opens the file
	if (file.is_open()) {
		string line;
		while (getline(file, line)) { // reads the file and getline reads each line into "line" variable
			istringstream iss(line); // extract comma-separated values from each line
			string customerIDFromFile;
			if (getline(iss, customerIDFromFile, ',')) { // read first value up to ',' and store in "customerIDFromFile" variable
				try {
					int tempID = stoi(customerIDFromFile); // convert extracted value from string to int
					if (tempID > customerID) {
						customerID = tempID; // Update customerID with the last ID
					}
				}
				catch (const invalid_argument& e) {
				}
			}
		}
		file.close();
	}
	else {
		cout << "Error: Unable to open the file.\n";
	}

	// Increment the customer ID for the new customer
	customerID++;

	// Create a new Customer object
	Customer newCustomer(customerID, name, loginID, password, loyaltyPoints);

	// Save the customer details to the CSV file
	ofstream fileToWrite(filename, ios::app); // creates output file stream object and opens file in APPEND mode
	if (fileToWrite.is_open()) {
		fileToWrite << newCustomer.getCustomerID() << "," << newCustomer.getName() << ","
			<< newCustomer.getLoginID() << "," << newCustomer.getPassword() << ","
			<< newCustomer.getLoyaltyPoints() << "\n";
		fileToWrite.close();
		cout << "Registration successful!\n";
	}
	else {
		cout << "Error: Unable to open the file.\n";
	}
}

// Function to check if the customer with the given email and password exists
bool Customer::customerExists(const string& filename, const string& loginID, const string& password) {
	ifstream file(filename); // create  input file stream object called "file" and opens the file
	if (file.is_open()) {
		// Skip the header line
		string header;
		getline(file, header);

		string line;
		while (getline(file, line)) { // read each line from the file
			istringstream iss(line); // extract comma-separated values from each line
			string customerIDFromFile, nameFromFile, loginIDFromFile, passwordFromFile, loyaltyPointsFromFile;

			// Read each field from the CSV line, separated by commas
			getline(iss, customerIDFromFile, ',');
			getline(iss, nameFromFile, ',');
			getline(iss, loginIDFromFile, ','); // Read the email from the third column
			getline(iss, passwordFromFile, ','); // Read the password from the fourth column
			getline(iss, loyaltyPointsFromFile, ',');

			string lowercaseLoginID = loginID;
			string lowercaseLoginIDFromFile = loginIDFromFile;

			for (char& c : lowercaseLoginID) { // convert all the characters in the input to lowercase
				c = tolower(c);
			}
			for (char& c : lowercaseLoginIDFromFile) { // convert all the characters in the file to lowercase
				c = tolower(c);
			}
			if (lowercaseLoginID == lowercaseLoginIDFromFile && password == passwordFromFile) {
				customerID = stoi(customerIDFromFile); // convert customerID to from string to integer
				name = nameFromFile;
				loyaltyPoints = stoi(loyaltyPointsFromFile); // convert loyalty points to integer
				file.close();
				return true; // return true if customer exists
			}
		}
		file.close();
	}
	else {
		cout << "Error: Unable to open the file.\n";
	}
	return false;
}

bool Customer::uniqueLoginID(const string& filename, const string& loginID)
{
	ifstream file(filename); // create  input file stream object called "file" and opens the file
	if (file.is_open()) {
		// Skip the header line
		string header;
		getline(file, header);

		string line;
		while (getline(file, line)) { // read each line from the file
			istringstream iss(line); // extract comma-separated values from each line
			string customerIDFromFile, nameFromFile, loginIDFromFile, passwordFromFile, loyaltyPointsFromFile;

			// Read each field from the CSV line, separated by commas
			getline(iss, customerIDFromFile, ',');
			getline(iss, nameFromFile, ',');
			getline(iss, loginIDFromFile, ','); // Read the email from the third column
			getline(iss, passwordFromFile, ','); // Read the password from the fourth column
			getline(iss, loyaltyPointsFromFile, ',');

			string lowercaseLoginID = loginID; // login id from input
			string lowercaseLoginIDFromFile = loginIDFromFile; // login id from file

			for (char& c : lowercaseLoginID) { // convert all the characters in the input to lowercase
				c = tolower(c);
			}
			for (char& c : lowercaseLoginIDFromFile) { // convert all the characters in the file to lowercase
				c = tolower(c);
			}

			if (lowercaseLoginID == lowercaseLoginIDFromFile) { // if id already exists
				file.close();
				return false;
			}
		}
		file.close();
	}
	else {
		cout << "Error: Unable to open the file.\n";
	}
	return true;
}

// Function to perform customer login
bool Customer::customerLogin(const string& filename) {
	string loginID, password;
	cout << "Enter your login credentials:\n";
	cout << "Login ID: ";
	cin >> ws;
	getline(cin, loginID);
	cout << "Password: ";
	cin >> ws;
	getline(cin, password);

	// Check if the customer exists with the given credentials
	if (customerExists(filename, loginID, password)) {
		cout << "Login successful!\n";
		return true;
	}
	else {
		cout << "Incorrect Login ID or password. Please try again.\n";
		return false;
	}
	return false;
}

void Customer::displayCustomerMenu() {
	cout << "Welcome " << name << "! " << "You have " << loyaltyPoints << " loyalty points." << endl;
	cout << "Here is the customer menu:" << endl;
	cout << "=====================================" << endl;
	cout << "           Customer Menu            " << endl;
	cout << "=====================================" << endl;
	cout << "1. Create a new order" << endl;
	cout << "2. View order history" << endl;
	cout << "3. Cancel an order" << endl;
	cout << "4. Log out" << endl;
	cout << "=====================================" << endl;
	cout << "Enter your choice: ";
}

HashTable<int, FoodItem> Customer::browseFoodItems(const string& foodItemsFile, const LinkedList<Restaurant>& restaurants, int restaurantID = -1)
{
	// Step 1: Read food items from the CSV file and store them in the hashtable
	HashTable<int, FoodItem> foodItemsTable;
	ifstream foodOptionsfile(foodItemsFile);
	if (!foodOptionsfile.is_open())
	{
		cout << "Error: Unable to open the file " << foodItemsFile << endl;
		return foodItemsTable; // Return an empty table as there are no food items read
	}

	// Skip the header line
	string header;
	getline(foodOptionsfile, header);

	string line;
	while (getline(foodOptionsfile, line))
	{
		istringstream iss(line);
		string foodItemIDStr, name, category, restaurantIDStr, priceStr;

		// Read each field from the CSV line, separated by commas
		getline(iss, foodItemIDStr, ',');
		getline(iss, name, ',');
		getline(iss, category, ',');
		getline(iss, restaurantIDStr, ',');
		getline(iss, priceStr, ',');

		// Convert the read data to appropriate data types
		int foodItemID = stoi(foodItemIDStr);
		double price = stod(priceStr);
		int restaurantIDFromFile = stoi(restaurantIDStr);

		// Check if the restaurant ID is provided and skip items from other restaurants
		if (restaurantID != -1 && restaurantID != restaurantIDFromFile)
		{
			continue;
		}

		// Create a FoodItem object
		FoodItem foodItem(foodItemID, name, category, price, restaurantIDFromFile);

		// Add the FoodItem object to the hashtable
		foodItemsTable.add(foodItem.getFoodItemID(), foodItem);
	}

	foodOptionsfile.close();

	if (restaurantID == -1)
	{
		// If the restaurant ID is not provided, print all food items
		foodItemsTable.print(restaurants);
	}
	else
	{
		// If the restaurant ID is provided, print only the food items from that restaurant
		foodItemsTable.print(restaurants, restaurantID);
	}

	return foodItemsTable; // Return the total number of food items read
}

LinkedList<OrderItem> Customer::addOrderItem(FoodItem fooditem, int quantity) {
	OrderItem order(fooditem, quantity);
	LinkedList<OrderItem> orderItemsList;
	orderItemsList.insert(order); // Insert the OrderItem into the LinkedList
	return orderItemsList;        // Return the modified LinkedList
}

int Customer::orderItemsMenu(const LinkedList<OrderItem>& orderItemsList, const LinkedList<Restaurant>& restaurants)
{
	// Calculate the padding to center the header
	int totalWidth = 45;
	string header = "Here are your ordered items:";
	int padding = (totalWidth - header.length()) / 2;

	// Print header
	cout << setw(padding + header.length()) << header << endl;

	cout << setw(10) << left << "Quantity" << setw(25) << left << "Food Item" << setw(15) << left << "Price" << endl;
	cout << setfill('=') << setw(totalWidth) << "=" << setfill(' ') << endl;

	for (int i = 0; i < orderItemsList.getLength(); i++)
	{
		OrderItem orderItem = orderItemsList.retrieve(i);
		FoodItem foodItem = orderItem.getFoodItem();
		double totalPrice = foodItem.getPrice() * orderItem.getQuantity();

		cout << setw(10) << left << orderItem.getQuantity()
			<< setw(25) << left << foodItem.getName()
			<< "$" << fixed << setprecision(2) << totalPrice << endl;
	}

	cout << setfill('=') << setw(totalWidth) << "=" << setfill(' ') << endl;

	// Get the restaurant name for the first order item
	OrderItem firstOrderItem = orderItemsList.retrieve(0);
	string restaurantName = firstOrderItem.getFoodItem().getRestaurantNameByID(firstOrderItem.getFoodItem().getRestaurantID(), restaurants);
	cout << "1. Add more items from " << restaurantName << endl;

	cout << "2. Remove an item" << endl;
	cout << "3. Confirm order" << endl;
	cout << "4. Cancel Order (Exit)" << endl;
	cout << setfill('=') << setw(totalWidth) << "=" << setfill(' ') << endl;
	cout << "Enter your choice: ";

	return firstOrderItem.getFoodItem().getRestaurantID();
}

void Customer::createOrder(const string& foodItemsFile, const string& ordersFile) {
	// TODO: Implement this. Read the food items file, let the customer choose an item, and add the order to the orders file.
}

void Customer::cancelOrder(const string& ordersFile) {
	// TODO: Implement this. Read the orders file, let the customer choose an order to cancel, and update the order status in the file.
}

void Customer::redeemPoints() {
	// TODO: Implement this. Deduct points from the customer's loyalty points and display a success message.
}