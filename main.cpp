#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

//function prototype
int readFile(string filename, char Data[][100], int row);
void extractFields(string line, char Data[][100]);
int mainMenu();
void manageBooksMenu();
void manageBorrowersMenu();
void borrowBooks();
void returnBooks();
void mostBorrow();

//Class for BookList and BorrowerList
class bookList {
public:
    void bookInfo(string id, string t, string a, string p, string yr) {
        bookID = id;
        title = t;
        author = a;
        publisher = p;
        year = yr;
        available = true;
        timesbkborrow = 0;
    }

    string bookID;
    string title;
    string author;
    string publisher;
    string year;
    bool available;
    int timesbkborrow;
};

class borrowerList {
public:
    void borrowerInfo(string id, string f, string l, string c) {
        borrowerID = id;
        firstName = f;
        lastName = l;
        contact = c;
        borrowedNum = 0;
    }

    string borrowerID;
    int borrowedNum;
    int borrowedBook[5];
    string firstName;
    string lastName;
    string contact;
};

bookList Book[1000];
borrowerList Borrower[500];
int bookNum = 0, borrowerNum = 0;

//function for reading .csv file and record in object array
int readFile(string filename, char Data[][100], int row) {
    fstream inFile;
    string line;
    int countRecords = 0;
    int countborrow = 0;
    inFile.open(filename);
    //Error message when the file is fail to open
    if (!inFile.is_open()) {
        cout << "Cannot open file \"" << filename << "\"\n";
        return 0;
    }
    //extrat every line and put every record into the book/borrower object
    while (getline(inFile, line, '\n')) {
        extractFields(line, Data);
        if (row == 5)
            Book[countRecords].bookInfo(Data[0], Data[1], Data[2], Data[3], Data[4]);
        else if (row == 3)
            Borrower[countRecords].borrowerInfo("HKCC" + to_string(countRecords + 1000), Data[1], Data[0], Data[2]);
        countRecords++;
    }

    cout << countRecords << " Record(s) imported.\n" << endl;
    inFile.close();
    return countRecords;
}

//extract the file to separate every fields
void extractFields(string line, char Data[][100]) {
    int start = 0;
    int row = 0;
    bool text = false;

    //read every single character
    for (int i = 0; line[i] != '\0'; i++) {
        //check if it is special character
        if (line[i] == ',' && !text) {
            Data[row++][start] = '\0';
            start = 0;
        }
        else if (line[i] == '"') {
            if (line[i + 1] == '"') {
                Data[row][start++] = line[i];
                i++;
            }
            else
                text = !text;
        }
        else
            Data[row][start++] = line[i];
    }
    Data[row][start] = '\0';
}

//R1: manage book menu
void manageBooksMenu() {
    char respond;
    do {
        cout << "*** Manage Books ***" << endl;
        cout << "[1] Display books" << endl;
        cout << "[2] Search book" << endl;
        cout << "[3] Add book" << endl;
        cout << "[4] Remove book" << endl;
        cout << "[5] Back" << endl;
        cout << "********************" << endl;
        cout << "Option (1 - 5): ";
        cin >> respond;
        cin.ignore(255, '\n');
        cout << "\n";
        switch (respond) {
        case '1': {
            bookList temp;

            //bubble sort to sort in alphabetical order
            for (int i = 0; i < bookNum - 1; i++) {
                for (int j = i + 1; j < bookNum; j++) {
                    int pos = -1;
                    do {
                        pos++;
                        if ((int)Book[i].title[pos] > (int)Book[j].title[pos]) {
                            temp = Book[i];
                            Book[i] = Book[j];
                            Book[j] = temp;
                        }
                    } while ((int)Book[i].title[pos] == (int)Book[j].title[pos] && pos < Book[i].title.length() - 1);
                }
            }

            cout << "Book list (alphabetical order of book title):" << endl;
            //display the book list
            cout << setw(15) << left << "ID" << setw(80) << "Book details" << "Availability" << endl;
            for (int i = 0; i < bookNum; i++) {
                cout << setw(15) << Book[i].bookID << setw(80) << Book[i].title << (Book[i].available ? "Yes" : "No") << endl;
                cout << setw(20) << " " << Book[i].author << endl;
                cout << setw(20) << " " << Book[i].publisher << " (" << Book[i].year << ")" << endl;
            }
            break;
        }
        case '2': {
            //searching function
            string keyword;
            bool match = false;
            int counter = 0;
            bool single_keyword = false;
            bool Multiple_keyword = false;
            bool double_quote = false;
            cout << "Enter keyword(s) to search for: ";
            getline(cin.ignore(), keyword);

            bool beginsWithQuote = (!keyword.empty() && keyword[0] == '"');
            bool endsWithQuote = (keyword[keyword.length() - 1] == '"');
            if (beginsWithQuote && endsWithQuote) {
                double_quote = true;
            }

            if (double_quote == false) {
                for (char c : keyword) {
                    if (isspace(c)) {
                        Multiple_keyword = true;
                        break;
                    }
                }
            }
            if (Multiple_keyword == false && double_quote == false) {
                single_keyword = true;
            }

            //for single keyword situation
            if (single_keyword) {
                for (int i = 0; i <= 800; i++) {
                    bool match = false;
                    if (Book[i].title.find(keyword) != string::npos ||
                        Book[i].author.find(keyword) != string::npos ||
                        Book[i].publisher.find(keyword) != string::npos ||
                        Book[i].bookID.find(keyword) != string::npos) {
                        match = true;
                    }

                    //display the searched result
                    if (match) {
                        cout << "\n" << setw(10) << left << "ID" << setw(80) << left << "Book details" << setw(15) << left << "Availability" << endl;
                        cout << setw(10) << left << Book[i].bookID << setw(80) << left << Book[i].title << setw(40) << left << (Book[i].available ? "Yes" : "No") << setw(11) << left << Book[i].author << setw(11) << '\n' << left << Book[i].publisher << "(" << Book[i].year << ")\n" << endl;
                        counter = counter + 1;
                    }
                    else if (i == 800 && counter == 0) {
                        cout << "Can't find the specific book that you are looking for,please try again later \n";
                    }
                }
            }

            //for multiple keyword situation
            if (Multiple_keyword) {
                char* words[10];
                int numWords = 0;
                char* currentWord = &keyword[0];

                for (char* p = &keyword[0]; *p != '\0'; p++) {
                    if (*p == ' ') {
                        *p = '\0';
                        words[numWords] = currentWord;
                        currentWord = p + 1;
                        numWords++;
                    }
                }
                words[numWords] = currentWord;
                numWords++;

                for (int i = 0; i < 800; i++) {
                    bool match = false;
                    for (int j = 0; j < numWords; j++) {
                        if (Book[i].title.find(words[j]) != string::npos || Book[i].author.find(words[j]) != string::npos || Book[i].publisher.find(words[j]) != string::npos || Book[i].bookID.find(words[j]) != string::npos) {
                            match = true;
                        }
                    }

                    //display searched result
                    if (match) {
                        cout << "\n" << setw(10) << left << "ID" << setw(80) << left << "Book details" << setw(15) << left << "Availability" << endl;
                        cout << setw(10) << left << Book[i].bookID << setw(80) << left << Book[i].title << setw(40) << left << (Book[i].available ? "Yes" : "No") << setw(11) << left << Book[i].author << setw(11) << '\n' << left << Book[i].publisher << "(" << Book[i].year << ")\n" << endl;
                        counter = counter + 1;
                    }
                    else if (i == 800 && counter == 0) {
                        cout << "Can't find the specific book that you are looking for,please try again later \n";
                    }
                }
            }

            //for double quote situation
            if (double_quote) {
                keyword = keyword.substr(1, keyword.length() - 2);
                for (int i = 0; i <= 800; i++) {
                    match = false;
                    if (keyword == Book[i].bookID || keyword == Book[i].title ||
                        keyword == Book[i].author || keyword == Book[i].publisher) {
                        match = true;
                    }

                    //display searched result
                    if (match) {
                        cout << "\n" << setw(10) << left << "ID" << setw(80) << left << "Book details" << setw(15) << left << "Availability" << endl;
                        cout << setw(10) << left << Book[i].bookID << setw(80) << left << Book[i].title << setw(40) << left << (Book[i].available ? "Yes" : "No") << setw(11) << left << Book[i].author << setw(11) << '\n' << left << Book[i].publisher << "(" << Book[i].year << ")\n" << endl;
                        counter = counter + 1;
                    }
                    else if (i == 800 && counter == 0) {
                        cout << "Can't find the specific book that you are looking for,please try again later \n";
                    }
                }
            }
            break;
        }
        case '3': {
            string id, title, author, publisher, year;
            bool uniqueID;

            //input book ID
            do {
                uniqueID = true;
                cout << "Enter book ID (max 10 characters): ";
                getline(cin.ignore(), id);

                //error message
                if (id.length() == 0 || id.length() > 10) {
                    cout << "Invalid ID length. Please enter a valid ID." << endl;
                    uniqueID = false;
                }
                else {
                    //search for the book ID
                    for (int i = 0; i < bookNum; i++) {
                        if (Book[i].bookID == id) {
                            cout << "ID already exists. Please enter a unique ID." << endl;
                            uniqueID = false;
                            break;
                        }
                    }
                }
            } while (!uniqueID);

            //enter book title
            cout << "Enter book title (max 100 characters): ";
            getline(cin, title);
            while (title.length() == 0 || title.length() > 100) {
                cout << "Title cannot be empty and must not exceed 100 characters."
                    << endl;
                cout << "Enter book title (max 100 characters): ";
                getline(cin, title);
            }

            //enter author
            cout << "Enter book author (max 50 characters, multiple names separated "
                "by ';'): ";
            getline(cin, author);
            while (author.length() == 0 || author.length() > 50) {
                cout << "Author name cannot be empty and must not exceed 50 characters."
                    << endl;
                cout << "Enter book author (max 50 characters, multiple names "
                    "separated by ';'): ";
                getline(cin, author);
            }

            //enter publisher
            cout << "Enter book publisher (max 50 characters): ";
            getline(cin, publisher);
            while (publisher.length() == 0 || publisher.length() > 50) {
                cout << "Publisher name cannot be empty and must not exceed 50 "
                    "characters."
                    << endl;
                cout << "Enter book publisher (max 50 characters): ";
                getline(cin, publisher);
            }

            //enter year of publication
            cout << "Enter year of publication: ";
            getline(cin, year);
            while (year.length() != 4 || stoi(year) < 0 || stoi(year) > 9999) {
                cout << "Year must be a positive integer between 0000 and 9999." << endl;
                cout << "Enter year of publication: ";
                getline(cin, year);
            }

            //create book object
            Book[bookNum++].bookInfo(id, title, author, publisher, year);
            cout << "Book added successfully." << endl;
            break;
        }
        case '4': {
            string id;
            char answer;

            //input book ID
            cout << "Enter book ID: ";
            cin >> id;

            //search for the book ID
            for (int i = 0; i < bookNum; i++) {
                if (Book[i].bookID == id) {
                    if (Book[i].available == true) {
                        cout << "Book details:" << endl;
                        cout << "ID: " << Book[i].bookID << endl;
                        cout << "Title: " << Book[i].title << endl;
                        cout << "Author: " << Book[i].author << endl;
                        cout << "Publisher: " << Book[i].publisher << endl;
                        cout << "Year of publication: " << Book[i].year << endl;
                        //confirmation
                        cout << "Is this the book you want to remove? (Y/N): ";
                        cin >> answer;
                        if (answer == 'Y') {
                            //remove the book
                            for (int j = i; j < bookNum - 1; j++)
                                Book[j] = Book[j + 1];
                            bookNum--;
                            cout << "Book removed successfully." << endl;
                            break;
                        }
                        else
                            cout << "No book is removed.";
                    }
                }
            }
            break;
        }
        case '5': break;
        default:
            cout << "Invalid option. Please choose again." << endl;
        }
        cin.ignore(255, '\n');
    } while (respond != '5');
}

//R2: manage borrower menu
void manageBorrowersMenu() {
    char respond;
    do {
        cout << "*** Manage Borrowers ***" << endl;
        cout << "[1] Display borrowers" << endl;
        cout << "[2] Search borrower" << endl;
        cout << "[3] Add borrower" << endl;
        cout << "[4] Remove borrower" << endl;
        cout << "[5] Back" << endl;
        cout << "************************" << endl;
        cout << "Option (1 - 5): ";
        cin >> respond;
        cin.ignore(255, '\n');
        cout << "\n";
        switch (respond) {
        case '1': {
            borrowerList temp;

            //bubble sort to sort the borrowers
            for (int i = 0; i < borrowerNum - 1; i++) {
                for (int j = i + 1; j < borrowerNum; j++) {
                    int pos = -1;
                    bool same = false;
                    do {
                        pos++;
                        if ((int)Borrower[i].lastName[pos] > (int)Borrower[j].lastName[pos]) {
                            temp = Borrower[i];
                            Borrower[i] = Borrower[j];
                            Borrower[j] = temp;
                        }
                        if (pos == Borrower[i].lastName.length() - 1)
                            same = true;
                    } while ((int)Borrower[i].lastName[pos] == (int)Borrower[j].lastName[pos] && same == false);
                    if (same) {
                        pos = -1;
                        do {
                            pos++;
                            if ((int)Borrower[i].firstName[pos] > (int)Borrower[j].firstName[pos]) {
                                temp = Borrower[i];
                                Borrower[i] = Borrower[j];
                                Borrower[j] = temp;
                            }
                        } while ((int)Borrower[i].firstName[pos] == (int)Borrower[j].firstName[pos] && pos < Borrower[i].firstName.length() - 1);
                    }
                }
            }

            cout << "Borrower list (alphabetical order of borrower's name):" << endl;
            //display the borrower in order
            cout << setw(10) << left << "ID" << setw(41) << "Borrower Name" << setw(10) << "Contact Number" << "Number of book borrowed" << endl;
            for (int i = 0; i < borrowerNum; i++)
                cout << setw(10) << Borrower[i].borrowerID << Borrower[i].lastName << " " << setw(40 - Borrower[i].lastName.length()) << Borrower[i].firstName << setw(10) << Borrower[i].contact << Borrower[i].borrowedNum << endl;

            break;
        }
        case '2': {
            string input;
            do {
                int found = false;
                //input borrower ID
                cout << "Please input borrower ID: ";
                cin.ignore(255, '\n');
                cin >> input;
                //check if the ID is correct
                for (int i = 0; i < borrowerNum; i++) {
                    if (Borrower[i].borrowerID.compare(input) == 0) {
                        found = true;
                        cout << "\n";
                        cout << setw(10) << left << "ID" << setw(41) << "Name" << setw(10) << "Contact Number" << endl;
                        cout << setw(10) << left << Borrower[i].borrowerID << Borrower[i].lastName << " " << setw(40 - Borrower[i].lastName.length()) << Borrower[i].firstName << setw(10) << Borrower[i].contact << endl;
                        cout << "\n";
                        cout << "List of borrowed books: " << endl;
                        cout << setw(15) << left << "ID" << setw(80) << "Book details" << "Availability" << endl;
                        if (Borrower[i].borrowedNum > 0) {
                            for (int j = 0; j < Borrower[i].borrowedNum; j++) {
                                cout << setw(15) << Book[Borrower[i].borrowedBook[j]].bookID << setw(80) << Book[Borrower[i].borrowedBook[j]].title << (Book[Borrower[i].borrowedBook[j]].available ? "Yes" : "No") << endl;
                                cout << setw(20) << " " << Book[Borrower[i].borrowedBook[j]].author << endl;
                                cout << setw(20) << " " << Book[Borrower[i].borrowedBook[j]].publisher << " (" << Book[Borrower[i].borrowedBook[j]].year << ")" << endl;
                            }
                        }
                    }
                }
                if (!found) {
                    cout << "Invalid ID or ID is not found, please input the ID again or type 'Q' to leave" << endl;
                    cin >> input;
                }
            } while (input.compare("Q") != 0);
            break;
        }
        case '3': {
            char firstName[30], lastName[10], contact[8];

            //input borrower information
            //borrower last name
            do {
                cout << "Enter borrower's last name (max 10 characters): ";
                cin.getline(lastName, '\n');
                if (strlen(lastName) == 0 || strlen(lastName) > 10)
                    cout << "Last name cannot be empty or exceed 10 characters." << endl;
            } while (strlen(lastName) == 0 || strlen(lastName) > 10);

            for (int i = 0; i < strlen(lastName); i++) {
                if (lastName[i] > 'a' && lastName[i] > 'z')
                    lastName[i] += 32;
            }

            //borrower first name
            do {
                cout << "Enter borrower's first name (max 30 characters): ";
                cin.getline(firstName, '\n');
                if (strlen(firstName) == 0 || strlen(firstName) > 30)
                    cout << "First name cannot be empty or exceed 30 characters." << endl;
            } while (strlen(firstName) == 0 || strlen(firstName) > 30);

            int pos = 0;
            for (int i = 0; i < strlen(firstName); i++) {
                if (firstName[i] == ' ')
                    pos = i + 1;
                if (firstName[pos] > 'a' && firstName[pos] > 'z')
                    firstName[i] += 32;
                if (i != pos && firstName[pos] > 'A' && firstName[pos] > 'Z')
                    firstName[i] -= 32;
            }

            //borrower contact number
            bool valid = true;
            do {
                valid = true;
                cout << "Enter borrower's contact number (max 8 digits): ";
                cin.getline(contact, '\n');

                if (strlen(contact) != 8 || (contact[0] != '2' && contact[0] != '3' && contact[0] != '5' && contact[0] != '6' && contact[0] != '9'))
                    valid = false;

                for (int i = 0; i < 8; i++) {
                    if (contact[i] < '0' || contact[i] > '9')
                        valid = false;

                }

                if (!valid)
                    cout << "Invalid contact number, please input again." << endl;
            } while (!valid);

            //create a object with given borrower information
            Borrower[borrowerNum].borrowerInfo("HKCC" + to_string(borrowerNum + 1000), firstName, lastName, contact);
            borrowerNum++;
            cout << "Borrower added successfully." << endl;
            break;
        }
        case '4': {
            string input;
            int found = false, valid = false;

            do {
                found = false;
                valid = false;
                //input borrower ID
                cout << "Please input borrower ID: ";
                cin >> input;
                //check if the borrower ID is correct
                for (int i = 0; i < borrowerNum; i++) {
                    int compare = Borrower[i].borrowerID.compare(input);
                    if (compare == 0) {
                        found = true;
                        if (Borrower[i].borrowedNum == 0) {
                            char respond;
                            //confirmation
                            cout << "Confirm to remove the borrower? [Y/N]: ";
                            cin >> respond;
                            if (respond == 'Y') {
                                valid = true;
                                //remove the borrower
                                for (int j = i; j < borrowerNum - 1; j++)
                                    Borrower[j] = Borrower[j + 1];
                                borrowerNum--;
                                cout << "Borrower removed successfully." << endl;
                            }
                            else
                                cout << "No borrower is removed." << endl;
                        }
                    }
                }
                //error message
                if (!found) {
                    if (!valid)
                        cout << "Invalid ID or ID is not found, please input the ID again or type 'Q' to leave" << endl;
                    else
                        cout << "The borrower has borrowed book(s), please check or type 'Q' to leave" << endl;
                    cin >> input;
                }
            } while (input.compare("Q") != 0);

            break;
        }
        case '5': break;
        default:
            cout << "Invalid option. Please choose again." << endl;
        }
        cin.ignore(255, '\n');
    } while (respond != '5');
}

//R3: borrow book
void borrowBooks() {
    string input;
    int found = false;
    //input borrower ID
    cout << "Please input borrower ID: ";
    cin >> input;
    //check if the borrower ID is correct
    for (int i = 0; i < borrowerNum; i++) {
        if (Borrower[i].borrowerID.compare(input) == 0) {
            found = true;
            if (Borrower[i].borrowedNum < 5) {
                //input the book ID
                cout << "Please input the book ID one by one, type 'end' to quit." << endl;
                int num = 1;
                char bookID[5][10];
                int index[5];

                do {
                    cout << "Book " << num << ": ";
                    cin >> bookID[num - 1];
                    if (strcmp(bookID[num - 1], "end") == 0)
                        break;
                    else {
                        int foundID = false, available = false;
                        //check if the Book ID is correct
                        for (int j = 0; j < bookNum; j++) {
                            if (Book[j].bookID.compare(bookID[num - 1]) == 0) {
                                foundID = true;
                                //check if the book ID is available
                                if (Book[j].available == true) {
                                    available = true;
                                    index[num - 1] = j;
                                }
                            }
                        }
                        //error message
                        if (!foundID)
                            cout << "Invalid book ID, please try again" << endl;
                        else if (!available)
                            cout << "The book is not available" << endl;
                        else
                            num++;
                    }
                } while (num <= 5);

                //not enough quota message
                if (num - 1 > 5 - Borrower[i].borrowedNum) {
                    cout << "You have not enough quota, no books will be borrowed." << endl;
                    cout << "Current quota: " << 5 - Borrower[i].borrowedNum << endl;
                }
                else {
                    //borrow book successfully
                    for (int j = 1; j < num; j++) {
                        Book[index[j - 1]].available = false;
                        Book[index[j - 1]].timesbkborrow++;
                        Borrower[i].borrowedBook[Borrower[i].borrowedNum] = index[j - 1];
                        Borrower[i].borrowedNum++;
                    }
                }
            }
        }
    }
}

//R4: return book
void returnBooks() {
    string input;
    int found = false;
    //input borrower ID
    cout << "Please input borrower ID: ";
    cin >> input;
    //check if the borrower ID is correct
    for (int i = 0; i < borrowerNum; i++) {
        if (Borrower[i].borrowerID.compare(input) == 0) {
            found = true;
            if (Borrower[i].borrowedNum > 0) {
                //input the book ID
                cout << "Please input the book ID one by one, type 'Q' to end." << endl;
                bool quit = false;
                int num = 1;
                char bookID[10];

                do {
                    cout << "Book " << num << ": ";
                    cin >> bookID;

                    if (strcmp(bookID, "Q") == 0)
                        quit = true;
                    else {
                        int foundID = false;
                        //check if the bookID is correct
                        for (int j = 0; j < Borrower[i].borrowedNum; j++) {
                            if (Book[Borrower[i].borrowedBook[j]].bookID.compare(bookID) == 0) {
                                foundID = true;
                                //remove borrowed book record from borrower
                                for (int k = j; k < Borrower[i].borrowedNum - 1; k++)
                                    Borrower[i].borrowedBook[k] = Borrower[i].borrowedBook[k + 1];
                                Book[Borrower[i].borrowedBook[j]].available = true;
                            }
                        }
                        Borrower[i].borrowedNum--;
                    }
                } while (!quit);
            }
            else {
                cout << "No borrowered book records." << endl;
            }
        }
    }
}

//R5: most borrower book ranking
void mostBorrow() {
    bookList temp;
    int mostBorrowedIndex = 0;
    //sorting to find out the top most borrowed book
    for (int i = 0; i < bookNum - 1; i++) {
        for (int j = i + 1; j < bookNum; j++) {
            int pos = -1;
            do {
                pos++;
                if ((int)Book[i].title[pos] > (int)Book[j].title[pos]) {
                    temp = Book[i];
                    Book[i] = Book[j];
                    Book[j] = temp;
                }
            } while ((int)Book[i].title[pos] == (int)Book[j].title[pos]);
        }
        if (Book[i].timesbkborrow > Book[mostBorrowedIndex].timesbkborrow) {
            mostBorrowedIndex = i;
        }
    }

    //display the most borrowed book
    cout << "Most borrowed book:" << endl;
    cout << setw(15) << left << "ID" << setw(80) << "Book details" << "Number of borrow" << endl;
    for (int i = 0; i < bookNum; i++) {
        if (Book[i].timesbkborrow == Book[mostBorrowedIndex].timesbkborrow) {
            cout << setw(15) << Book[i].bookID << setw(80) << Book[i].title << Borrower << endl;
            cout << setw(20) << " " << Book[i].author << endl;
            cout << setw(20) << " " << Book[i].publisher << " (" << Book[i].year << ")" << endl;
        }
    }
}

//R0: main menu
int mainMenu() {
    char respond;

    cout << "*** Library Management System ***" << endl;
    cout << "[1] Manage books" << endl;
    cout << "[2] Manage borrowers" << endl;
    cout << "[3] Borrow book(s)" << endl;
    cout << "[4] Return book(s)" << endl;
    cout << "[5] Most borrowed books" << endl;
    cout << "[6] Member List" << endl;
    cout << "[7] Exit" << endl;
    cout << "*********************************" << endl;
    cout << "Option (1 - 7): ";

    cin >> respond;
    switch (respond) {
    case '1': manageBooksMenu(); break;
    case '2': manageBorrowersMenu(); break;
    case '3': borrowBooks(); break;
    case '4': returnBooks(); break;
    case '5': mostBorrow(); break;
    case '6': {
        //R6: member list
        cout << "Group Member List (in ascending order): " << endl;
        cout << setw(20) << "Name" << setw(15) << "Student ID" << setw(5) << "Class" << endl;
        cout << setw(20) << "Liu Wing Him" << setw(15) << "22112985A" << setw(5) << "203B" << endl;
        cout << setw(20) << "Chan Chun Man" << setw(15) << "22132580A" << setw(5) << "203A" << endl;
        cout << setw(20) << "Lam Ying Tone" << setw(15) << "22095228A" << setw(5) << "203C" << endl;
        cout << setw(20) << "Lee Lok San" << setw(15) << "22131846A" << setw(5) << "203C" << endl;
        cout << setw(20) << "Lee Wing Hang" << setw(15) << "22168378A" << setw(5) << "203A" << endl;
        cout << setw(20) << "Yuen Cheuk Man" << setw(15) << "22126690A" << setw(5) << "203C" << endl;
        break;
    }
    case '7': {
        //R7: Exit
        do {
            cout << "Confirm to exit? [Y/N]: ";
            cin >> respond;
            //confirmation
            if (respond == 'Y')
                return 0;
            else if (respond == 'N')
                mainMenu();
            else
                cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
        } while (respond != 'Y' && respond != 'N');
        break;
    }
    default:
        cout << "Invalid option. Please choose again." << endl;
    }

    return 1;
}

//main function
int main() {
    string filename;
    string respond;
    char Data[5][100];
    do {
        //import book list
        cout << "Import book list from file? [Y/N]: ";
        getline(cin, respond);
        if (respond == "Y") {
            cout << "Path of book list file: ";
            getline(cin, filename);
            cout << "Importing book list . . . ";
            //read .csv file
            bookNum = readFile(filename, Data, 5);
        }
        else if (respond == "N")
            cout << "No book list is imported" << endl;
        //error message
        if (respond != "Y" && respond != "N")
            cout << "Invalid input, please try again." << endl;
    } while (respond != "Y" && respond != "N");

    do {
        //import borrower list
        cout << "Import borrower list from file? [Y/N]: ";
        getline(cin, respond);
        if (respond == "Y") {
            cout << "Path of borrower list file: ";
            getline(cin, filename);
            cout << "Importing borrower list . . . ";
            //read .csv file
            borrowerNum = readFile(filename, Data, 3);
        }
        else if (respond == "N")
            cout << "No borrower list is imported" << endl;
        //error message
        if (respond != "Y" && respond != "N")
            cout << "Invalid input, please try again." << endl;
    } while (respond != "Y" && respond != "N");

    //call main menu
    int status;
    do {
        status = mainMenu();
        cin.ignore(255, '\n');
    } while (status != 0);

    return 0;
}
