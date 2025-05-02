# Library Management System

A C++ console application for managing a library's book inventory and borrower records. This system allows librarians to maintain book and borrower information, process book borrowing and returns, and generate reports.

## Features

- **Book Management**
  - Display books (sorted alphabetically by title)
  - Search for books (using keywords, multiple keywords, or exact phrases)
  - Add new books to the inventory
  - Remove books from the inventory

- **Borrower Management**
  - Display borrowers (sorted alphabetically by name)
  - Search for borrowers by ID
  - Add new borrowers
  - Remove borrowers (if they have no borrowed books)

- **Borrowing Operations**
  - Borrow books (up to 5 books per borrower)
  - Return books
  - View most borrowed books

- **Data Import**
  - Import book data from CSV files
  - Import borrower data from CSV files

## Setup and Usage

1. **Compilation**
   - Compile the program using a C++ compiler:
     ```
     g++ main.cpp -o library_system
     ```

2. **Running the Program**
   - Execute the compiled program:
     ```
     ./library_system
     ```

3. **CSV File Import**
   - When prompted, you can import book and borrower data from CSV files
   - Sample files are included: [`BookList.csv`](BookList.csv) and [`BorrowerList.csv`](BorrowerList.csv)

## CSV File Formats

### Book List CSV Format
```
BookID,Title,Author,Publisher,Year
```

### Borrower List CSV Format
```
LastName,FirstName,ContactNumber
```

## System Requirements

- C++ compiler with C++11 support
- Standard C++ libraries

## Menu Navigation

The system provides intuitive menus for navigating through different features:

1. **Main Menu**
   - Manage books
   - Manage borrowers
   - Borrow book(s)
   - Return book(s)
   - Most borrowed books
   - Exit

2. **Book Management Menu**
   - Display books
   - Search book
   - Add book
   - Remove book
   - Back

3. **Borrower Management Menu**
   - Display borrowers
   - Search borrower
   - Add borrower
   - Remove borrower
   - Back

## Limitations

- Maximum 1000 books in the system
- Maximum 500 borrowers
- Each borrower can borrow up to 5 books

## Implementation Details

This system is implemented in C++ and uses:
- File I/O operations for CSV imports
- String manipulation for search operations
- Arrays for data storage
- Bubble sort for sorting operations
- Object-oriented design for book and borrower management