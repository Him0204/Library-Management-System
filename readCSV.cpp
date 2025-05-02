#include <iostream>
#include <fstream>	// for file input/output
#include <string>
using namespace std;

int extractFields(string line, char fields[][101]) {
	/*
	*	Suggested algorithm:
	*	1. Use loop to check every char of string
	*	2. Handle when it is " or ,
	*	3. Copy the content char into 2D array fields 
	*	4. Count the number of fields
	*/
	int numFields = 0;

	// **********
	// Implement your code to extract fields from the line
	// **********
	cout << line << endl;	// *** show the line content, for you to know what it reads
							// *** you should remove this line in your code
	
	return numFields;
}

void readCSV(string filename) {
	fstream inFile;				// for handling file
	string line;				// for storing 1 line in a file
	char fields[10][101] = {};	// for storing extracted fields (assume max. 10 fields per line, each field has max. 100 char)
	int numFields;				// for storing number of fields per line
	int countRecords = 0;		// for counting the number of records in csv file

	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "Cannot open file \"" << filename << "\"\n";
		return;
	}

	while (getline(inFile, line, '\n')) {				// read line by line until end of file
		numFields = extractFields(line, fields);		// call function to extract fields from the line

		for (int i = 0; i < numFields; i++)				// *** display the fields of this line
			cout << i << ": " << fields[i] << endl;		// *** you should modify this code for fields processing
		cout << "\n\n";									// *** e.g. assign fields to data members of an object
		
		countRecords++;
	}

	cout << countRecords << " Record(s) imported.\n";
	inFile.close();
}

int main() {
	string filename;

	cout << "Input path to CSV file: ";
	getline(cin, filename);				// Path with space is allowed
	readCSV(filename);

	return 0;
}
