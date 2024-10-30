// ConsoleArbitrage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ConsoleArbitrage.h";
#include "ArbitrageLogic.h"
#include <iostream>
#include <string>
#include <vector>

// Declare Global variables
std::vector<std::string> currencies = { "USD", "EUR", "JPY" };   // example
std::string currency = "USD"; 
std::string default_currency = "USD";
double initial_capital = 0.00;
double capital = 0.00;
bool quit = false;

// Declare Functions
void select_currency();
void list_currencies();
std::string get_currency();
void set_initial_capital(double initial_capital);
double get_initial_capital();
void show_initial_capital();
void set_capital(double new_capital);
double get_capital();
void display_status();
bool quit_question();
void ending_status();

int main(){
    // Parse data
    select_currency();
    show_initial_capital();
    display_status();
    while (quit == false) {
        quit = quit_question();
    }
    ending_status();
}

void select_currency() {
    std::string choice = " ";
    int currency_choice;
    std::string confirm_choice = " ";

    std::cout << "Default currency: " + default_currency + "\n";
    while (choice != "N" && choice != "n") {
        std::cout << "Select Another currency (Y/N)? ";
        std::cin >> choice;
        if (choice == "Y" || choice == "y") {
            list_currencies();
            std::cout << "Enter the number corresponding to your currency choice: ";
            std::cin >> currency_choice;
            // Confirm currency_choice within bounds
            if (currency_choice > 0 && currency_choice < currencies.size() + 1) {
                currency = currencies[currency_choice - 1];
                std::cout << "Selected Currency: " << currency + "\n";

                // Obtain user confirmation
                std::cout << "Confirm (Y/N): ";
                std::cin >> confirm_choice;
                if (confirm_choice == "Y" || confirm_choice == "y") {
                    display_status();
                    break;
                } else if (confirm_choice != "N" && confirm_choice != "n") {
                    std::cout << "Invalid confirmation. Please enter Y or N.\n";
                } else if (confirm_choice == "N" || confirm_choice == "n") {
                    currency = default_currency;
                    display_status();
                }
            } else {
                std::cout << "Invalid currency.\n\nCurrent currency : " << currency + "\n";
            }
        } else if (choice != "N" && choice != "n") {
            std::cout << "Invalid choice. Please enter Y or N.\n";
        }
    }
}

// List all currencies
void list_currencies() {
    std::cout << "Currency Options:\n";
    for (int i = 0; i < currencies.size(); i++) {
        std::cout << "\t" << (i + 1) << ". " << currencies[i] << "\n";
    }
}

std::string get_currency() {
    return currency;
}

void show_initial_capital() {
    double new_initial_capital = 0.00;
    std::cout << "Initial Amount (" << currency << "): ";
    std::cin >> new_initial_capital;   // change to get amount from foreign exchange api
    set_initial_capital(new_initial_capital);
    set_capital(get_initial_capital());
    std::cout << "You have " << initial_capital << " " << currency << "\n\n";
}

void set_initial_capital(double new_initial_capital) {
    initial_capital = new_initial_capital;
}

double get_initial_capital() {
    return initial_capital;
}

void set_capital(double new_capital) {
    capital = new_capital;
}

double get_capital() {
    return capital;
}

void display_status() {
    std::cout << "Status: " << get_capital() << " " << get_currency() << "\n";
}

static double calculate_change() {
    double change = get_capital() - get_initial_capital();
    return change;
}

static void display_change() {
    std::cout << "\tChange: " << calculate_change();
}

bool quit_question() {
    std::string choice = " ";
    std::string confirm_choice = " ";
    std::cout << "Exit Program (Y/N): ";
    std::cin >> choice;

    if (choice == "Y" || choice == "y") {
        // Obtain user confirmation
        std::cout << "Confirm (Y/N): ";
        std::cin >> confirm_choice;
        if (confirm_choice == "Y" || confirm_choice == "y") {
            return true;
        } else if (confirm_choice != "N" && confirm_choice != "n") {
            std::cout << "Invalid confirmation. Please enter Y or N.\n";
        }
    }
    return false;
}

void ending_status() {
    std::cout << "\nFinal ";
    display_status();
    display_change();
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
