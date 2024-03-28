// Sieve of Eratosthenes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

//test functions
void printVector(std::vector<int>);
void testColors();

std::vector<int> getPrimes(int);
std::vector<std::string> colorList(std::vector<int>);

void validateArgs(int, char* [], int&, int&, bool&, bool&);
void printInstructions();

void tableDimentions(int, int&, int&, bool);
void drawKey(std::vector<int>, std::vector<std::string>, int col = 6, int row = 4);
void drawSieve(int, int, int, std::vector<int>, std::vector<std::string>, bool);
void drawLine(int, int);

int isPrime(int, std::vector<int>);

// Constant string for reseting console formatting and getting inverse for black and white
const std::string resetCode = "\033[0m";
const std::string inverse = "\033[7m";

int main(int argc, char* argv[])
{
    //testColors();
    int columns, rows, items;
    bool userColumns = false;
    bool highlight = false;
    validateArgs(argc, argv, items, columns, userColumns, highlight);// get args
    std::vector<int> primes = getPrimes(items); // get list of primes
    std::vector<std::string> colors = colorList(primes); // get colors associated with primes
    tableDimentions(items, columns, rows, userColumns);
    
    std::cout << "\033[4mPrime Factor Key" + resetCode << std::endl << "Total Primes: " << primes.size();
    drawKey(primes, colors);
    std::cout << '\n';

    std::cout << "\033[4mSieve of Eratosthenes" + resetCode << std::endl << "Items: " << items << " Columns: " << columns << " Rows: " << rows;
    drawSieve(items, columns, rows, primes, colors, highlight);

    std::cout << "Finished!";
}

void drawLine(int cells, int width) {
    using std::cout;
    using std::setfill;
    using std::setw;
    using std::endl;

    cout << endl << "|";
    for (int i = 0; i < cells - 1; i++) {
        cout << setfill('-') << setw(width + 1) << "+";
    }
    cout << setw(width + 1) << "|" << setfill(' ') << endl;
}

void drawKey(std::vector<int> primes, std::vector<std::string> colors, int col, int row) {
    using std::cout;
    using std::setfill;
    using std::setw;
    using std::endl;

    int max = primes.size() - 1;
    int w = primes[max] > 9? 2 : 1;
    int cur = 0;

    tableDimentions(primes.size(), col, row, true);

    // adds top row of table
    drawLine(col, w);

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) { // iterates through columns in row
            if (cur > max) {
                cout << "|" << setw(w) << " ";
            }
            else {
                cout << "|" << colors[cur] << setw(w) << primes[cur] << resetCode;
            }
            if (c + 1 == col)
                cout << "|";
            ++cur;
        }
        drawLine(col, w); // below each row including bottem
    }
}

void drawSieve(int max, int col, int row, std::vector<int> primes, std::vector<std::string> colors, bool highlight) {
    using std::cout;
    using std::setfill;
    using std::setw;
    using std::endl;
    // add a key for what colors represent which primes
    
    int w = max > 99999 ? 6 : max > 9999 ? 5 : max > 999 ? 4 : max > 99 ? 3 : max > 9 ? 2 : 1; // sets cell width based on size of max item in table
    int cur = 1;
    int index;

    drawLine(col, w); // adds top row of table
    
    for (int r = 0; r < row; r++) { // iterates through rows
        for (int c = 0; c < col; c++) { // iterates through columns in row
            index = isPrime(cur, primes);
            if (cur == 1 || cur > max) {
                cout << "|" << setw(w) << " ";
            }
            else if (index != -1) {
                cout << "|" << colors[index] << setw(w) << cur << resetCode;
            }
            else if (highlight){
                cout << "|" << inverse << setw(w) << cur << resetCode;
            }
            else {
                cout << "|" << setw(w) << cur;
            }


            if (c + 1 == col)
                cout << "|";
            ++cur;
        }
        drawLine(col, w); // below each row including bottem
    }
}

int isPrime(int num, std::vector<int> primes) {
    for (int i = primes.size() - 1; i >= 0; --i) {
        if (num != primes[i] && num % primes[i] == 0) {
            return i; // returnes index
        }
    }
    return -1; // signifies it isnt prime
}

std::vector<std::string> colorList(std::vector<int> primes) {
    std::vector<std::string> v;
    for (int i = 0; i < primes.size(); i++) {
        std::string temp = "";
        if (i >= 0 && i <= 5) {// normal colored text
            temp = "\033[" + std::to_string(i + 31) + "m";
        }
        else if (i >= 6 && i <= 11) { // bold text
            temp = "\033[1;" + std::to_string(i + 31 - 6) + "m";
        }
        else if (i >= 12 && i <= 17) { // underlined
            temp = "\033[4;" + std::to_string(i + 31 - 12) + "m";
        }
        else if (i >= 18 && i <= 23) { // bold and underlined
            temp = "\033[1;4;" + std::to_string(i + 31 - 18) + "m";
        }
        v.push_back(temp);
    }
    return v;
}

void testColors() {
    std::vector<std::string> colors = colorList(getPrimes(89 * 89));
    std::vector<int> primes = getPrimes(89 * 89);
    for (int i = 0; i < colors.size(); i++) {
        std::cout << colors[i] << std::setw(2) << primes[i] << resetCode << " ";
        if ((i + 1) % 6 == 0)
            std::cout << std::endl;
    }
}

void validateArgs(int argc, char* argv[], int& items, int& columns, bool& userColumns, bool& highlight) {
    if (argc < 2 || argc > 4) {
        printInstructions();
        exit(1);
    }

    if (argc >= 2) {
        items = std::stoi(argv[1]);
        if (items < 4 || items > 9408) {
            printInstructions();
            exit(1);
        }
    }
    if (argc >= 3) {
        columns = std::stoi(argv[2]);
        if (columns < 1) {
            printInstructions();
            exit(1);
        }
        userColumns = true;
    }
    if (argc == 4) {
        std::string temp = argv[3];
        if (temp == "Y" || temp == "y") {
            highlight = true;
        }
        else if (temp == "N" || temp == "n") {
            highlight = false;
        }
        else {
            printInstructions();
            exit(1);
        }
    }
}
void printInstructions() {
    using std::ifstream;
    using std::cout;

    ifstream stream("Instructions.txt");
    if (stream.fail()) {
        cout << "Instruction file failed to open Dx";
        return;
    }
    char next;
    while (stream.get(next) && !stream.eof()) {
        cout << next;
    }
}

void tableDimentions(int items, int& columns, int& rows, bool givenColumns) {
    
    if (givenColumns) { // if the user gave how many columns they want
        int temp = items / columns;
        if (items == temp * columns)
            rows = temp;
        else
            rows = temp + 1;
        
        if (columns > items) {
            columns = items;
        }
        return;
    }
    double square = sqrt(items);
    if (square < 20) { // 50 is max number of columns
        if (square == (int)square) { // if items has a perfect square  make square table
            columns = square;
            rows = square;
            return;
        }
        else {// this will make a table where there is one more row than columns
            columns = square;
            rows = square + 1;
            return;
        }
    }
    else {
        columns = 20; // table with 50 columns (max width)
        rows = (items / 20.0) + 1; // and enough rows to fit all items
    }
}

std::vector<int> getPrimes(int num) {
    //find largest integer below sqrt(num)
    int max = sqrt(num); // truncates to highest int below sqrt num

    //find primes and add them to vector
    std::vector<int> primes;
    for (int i = 2; i <= max; i++) { // loops through ever number 2-max
        for (int k = 2; k <= i; k++) { // loops through ever number through i to find factors (excluding 1 and i)
            if (k == i) {
                primes.push_back(i); // , if no factors exist when k < i, i gets added to primes
            }
            if (i % k == 0) { // if k is a factor of i loop exits
                break;
            }
        }
    }

    //printVector(primes);

    return primes;
}

void printVector(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
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
