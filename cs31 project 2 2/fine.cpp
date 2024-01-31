#include <iostream>
#include <string>

using namespace std;
//Andrew Sun
//006159649


int main() {
    string name;
    double money;
    string location;
    double fine;
    bool noName;
    bool noLocation;

    cout.setf(ios::fixed);
    cout.precision(3);  //makes sure the final output for the fine will have 3 decimals

    cout << "Identification: ";
    getline (cin, name);
    if (name == "") //checks to see if the user forgot to input a name into the program
        noName = true;

    cout << "Overstated amount (in millions): ";
    cin >> money;

    cout << "Location: ";
    cin.ignore(); //removes the \n from the previous input
    getline(cin, location);
    if (location == "") //checks to see if the user forgot to input a location
        noLocation = true;

    cout << "---\n";

    if (noName) //if the user forgot to input a name, then we can output the accurate error message
        cout << "You must enter a property identification.\n";
    else if (noLocation) //same thing as noName
        cout << "You must enter a location.\n";
    else if (money <= 0) //checks to make sure the money inputted was not negative
        cout << "The overstated amount must be positive.\n";
    else { //if there is no mistake, this will be outputted
        if (location == "florida" || location == "new york") { //this checks to see if the location is different, in which case the fine will change
            if (money > 150)
                fine = 0.1111 * 60 + 0.18 * 90 + 0.21 * (money - 150);
            else if (money > 90)
                fine = 0.1111 * 60 + 0.18 * (money - 60);
            else
                fine = 0.1111 * money;
        } else {
            if (money > 150)
                fine = 0.1111 * 60 + 0.153 * 90 + 0.21 * (money - 150);
            else if (money > 90)
                fine = 0.1111 * 60 + 0.153 * (money - 60);
            else
                fine = 0.1111 * money;
        }

        cout << "The fine for " << name << " is $" << fine << " million.\n";
    }
}
