#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <ctime>

using namespace std;

// Data structures
struct Book {
    int id;
    string title;
    string author;
    bool isAvailable;
    string category; // New field for category
};

struct Member {
    int id;
    string name;
    string membershipStatus;
    stack<int> borrowedBooks;
};

struct Transaction {
    int bookId;
    int memberId;
    string transactionType; // Borrow, Return, Extend
    string date;
    string dueDate; // New field for due date
};

struct User {
    string username;
    string password;
    string role; // Role: Admin, Petugas, Supervisor
};

// Global Data
vector<Book> books;
vector<Member> members;
vector<Transaction> transactions;
vector<User> users;

// Utility functions
time_t parseDate(const string& dateStr) {
    struct tm tm = {};
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
        throw std::runtime_error("Invalid date format: " + dateStr);
    }
    tm.tm_year -= 1900; // Adjust year (tm_year is years since 1900)
    tm.tm_mon -= 1;     // Adjust month (tm_mon is 0-based)
    return mktime(&tm);
}

string formatDate(time_t date) {
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", localtime(&date));
    return string(buffer);
}

// Role-based Access Control Functions
bool login(User& loggedInUser) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            loggedInUser = user;
            return true;
        }
    }
    return false;
}

// Function declarations
void registerNewBook();
void viewBookCatalog();
void searchBook();
void updateBookStatus();
void deleteBook();  // New function to delete a book

void registerNewMember();
void viewMember();
void searchMember();
void updateMemberProfile();
void deleteMember();  // New function to delete a member
void manageMemberProfile();
void viewBorrowHistory();

void processBorrowing();
void processReturning();
void extendBorrowing();
void calculateLateFee();

void borrowBook();
void viewTransactions();
void deleteTransaction();

// Book Management Menu
void bookManagementMenu() {
    int choice;
    do {
        cout << "\nBook Management\n";
        cout << "1. Register New Book\n";
        cout << "2. View Book Catalog\n";
        cout << "3. Search Book\n";
        cout << "4. Update Book Information\n";
        cout << "5. Delete Book\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerNewBook();
            break;
        case 2:
            viewBookCatalog();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            updateBookStatus();
            break;
        case 5:
            deleteBook();
            break;
        case 6:
            return; // Back to main menu
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Feature Implementations
void registerNewBook() {
    Book newBook;
    cout << "Enter book ID: ";
    cin >> newBook.id;
    cin.ignore();
    cout << "Enter book title: ";
    getline(cin, newBook.title);
    cout << "Enter author: ";
    getline(cin, newBook.author);
    cout << "Enter category: ";
    getline(cin, newBook.category);
    newBook.isAvailable = true;
    books.push_back(newBook);
    cout << "Book registered successfully.\n";
}

void viewBookCatalog() {
    cout << "\nBook Catalog:\n";
    
    // Print table header with consistent column widths
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";
    cout << "| Book ID    | Title                                         | Author                                        | Category            | Availability         |\n";
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";
    
    bool found = false;
    
    // Print each book's details with appropriate padding and alignment
    for (size_t i = 0; i < books.size(); ++i) {
        cout << "| " << setw(10) << left << books[i].id
             << " | " << setw(45) << left << books[i].title
             << " | " << setw(45) << left << books[i].author
             << " | " << setw(19) << left << books[i].category
             << " | " << setw(20) << left << (books[i].isAvailable ? "Yes" : "No") << " |\n";
             
        found = true;
    }
    
    if (!found) {
        cout << "| Data books not found.\n";
    }

    // Print table footer with consistent borders
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";
}

void searchBook() {
    string searchQuery;
    cout << "Enter book title or author to search: ";
    cin.ignore();  // To clear the input buffer before getting string input
    getline(cin, searchQuery);

    bool found = false;
    
    cout << "\nSearch Results:\n";
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";
    cout << "| Book ID    | Title                                         | Author                                        | Category            | Availability         |\n";
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";

    for (size_t i = 0; i < books.size(); ++i) {
        // Check if the search query matches the book's title or author (case-insensitive search)
        if (books[i].title.find(searchQuery) != string::npos || books[i].author.find(searchQuery) != string::npos) {
            found = true;
            cout << "| " << setw(10) << left << books[i].id
                 << " | " << setw(45) << left << books[i].title
                 << " | " << setw(45) << left << books[i].author
                 << " | " << setw(19) << left << books[i].category
                 << " | " << setw(20) << left << (books[i].isAvailable ? "Yes" : "No") << " |\n";
        }
    }

    if (!found) {
        cout << "| No books found matching your search criteria.\n";
    }

    // Print table footer with consistent borders
    cout << "+------------+-----------------------------------------------+-----------------------------------------------+---------------------+----------------------+\n";
}

void updateBookStatus() {
    int bookId;
    cout << "Enter book ID to update: ";
    cin >> bookId;
    cin.ignore();

    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].id == bookId) {
            cout << "Enter new title (leave blank to keep current): ";
            string newTitle;
            getline(cin, newTitle);
            if (!newTitle.empty()) books[i].title = newTitle;

            cout << "Enter new author (leave blank to keep current): ";
            string newAuthor;
            getline(cin, newAuthor);
            if (!newAuthor.empty()) books[i].author = newAuthor;

            cout << "Enter new category (leave blank to keep current): ";
            string newCategory;
            getline(cin, newCategory);
            if (!newCategory.empty()) books[i].category = newCategory;

            cout << "Is the book available? (1 for Yes, 0 for No): ";
            cin >> books[i].isAvailable;

            cout << "Book updated successfully.\n";
            return;
        }
    }
    cout << "Book with ID " << bookId << " not found.\n";
}

void deleteBook() {
    int bookId;
    cout << "Enter book ID to delete: ";
    cin >> bookId;

    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].id == bookId) {
            books.erase(books.begin() + i);
            cout << "Book deleted successfully.\n";
            return;
        }
    }
    cout << "Book with ID " << bookId << " not found.\n";
}

// Member Management Menu
void memberManagementMenu() {
    int choice;
    do {
        cout << "\nMember Management\n";
        cout << "1. Register New Member\n";
        cout << "2. View Member\n";
        cout << "3. Search Member\n";
        cout << "4. Update Member Profile\n";
        cout << "5. Delete Member\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerNewMember();
            break;
        case 2:
            viewMember();
            break;
        case 3:
            searchMember();
            break;
        case 4:
            updateMemberProfile();
            break;
        case 5:
            deleteMember();
            break;
        case 6:
            return; // Back to main menu
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Feature Implementations (Member CRUD)

void registerNewMember() {
    Member newMember;
    cout << "Enter member ID: ";
    cin >> newMember.id;
    cin.ignore();
    cout << "Enter member name: ";
    getline(cin, newMember.name);
    newMember.membershipStatus = "Active";
    members.push_back(newMember);
    cout << "Member registered successfully.\n";
}

void viewMember() {
    cout << "\nMember Catalog:\n";
    cout << "+------------+-----------------------------------------------+---------------------+\n";
    cout << "| Member ID  | Name                                          | Status              |\n";
    cout << "+------------+-----------------------------------------------+---------------------+\n";
    
    bool found = false;
    
    for (size_t i = 0; i < members.size(); ++i) {
        cout << "| " << setw(10) << left << members[i].id
             << " | " << setw(45) << left << members[i].name
             << " | " << setw(19) << left << members[i].membershipStatus << " |\n";
             
        found = true;
    }
    
    
    if (!found) {
        cout << "| Data members not found.\n";
    }

    cout << "+------------+-----------------------------------------------+---------------------+\n";
}

void searchMember() {
    string searchQuery;
    cout << "Enter member name to search: ";
    cin.ignore();  
    getline(cin, searchQuery);

    bool found = false;
    
    cout << "\nSearch Results:\n";
    cout << "+------------+-----------------------------------------------+---------------------+\n";
    cout << "| Member ID  | Name                                          | Status              |\n";
    cout << "+------------+-----------------------------------------------+---------------------+\n";

    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i].name.find(searchQuery) != string::npos) {
            found = true;
            cout << "| " << setw(10) << left << members[i].id
                 << " | " << setw(45) << left << members[i].name
                 << " | " << setw(19) << left << members[i].membershipStatus << " |\n";
        }
    }

    if (!found) {
        cout << "| No members found matching your search criteria.\n";
    }

    cout << "+------------+-----------------------------------------------+---------------------+\n";
}

void updateMemberProfile() {
    int memberId;
    cout << "Enter member ID to update: ";
    cin >> memberId;
    cin.ignore();

    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i].id == memberId) {
            cout << "Enter new name (leave blank to keep current): ";
            string newName;
            getline(cin, newName);
            if (!newName.empty()) members[i].name = newName;

            cout << "Enter new membership status (leave blank to keep current): ";
            string newStatus;
            getline(cin, newStatus);
            if (!newStatus.empty()) members[i].membershipStatus = newStatus;

            cout << "Member updated successfully.\n";
            return;
        }
    }
    cout << "Member with ID " << memberId << " not found.\n";
}

void deleteMember() {
    int memberId;
    cout << "Enter member ID to delete: ";
    cin >> memberId;

    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i].id == memberId) {
            members.erase(members.begin() + i);
            cout << "Member deleted successfully.\n";
            return;
        }
    }
    cout << "Member with ID " << memberId << " not found.\n";
}

// Transaction Management Menu
void transactionManagementMenu() {
    int choice;
    do {
        cout << "\nTransaction Management\n";
        cout << "1. Borrow Book\n";  // Tambahkan opsi meminjam buku
        cout << "2. Return Book\n";
        cout << "3. Extend Borrowing\n";
        cout << "4. Delete Transaction\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            borrowBook();  // Panggil fungsi peminjaman buku
            break;
        case 2:
            processReturning();
            break;
        case 3:
            extendBorrowing();
            break;
        case 4:
            deleteTransaction();
            break;
        case 5:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// CRUD for Transactions
void borrowBook() {
    int bookId, memberId;
    cout << "Enter Book ID to borrow: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    // Cari buku berdasarkan ID
    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].id == bookId) {
            // Cek apakah buku tersedia
            if (!books[i].isAvailable) {
                cout << "Book is currently not available.\n";
                return;
            }

            // Cari anggota berdasarkan ID
            for (size_t j = 0; j < members.size(); ++j) {
                if (members[j].id == memberId) {
                    // Tambahkan transaksi baru
                    Transaction newTransaction;
                    newTransaction.bookId = bookId;
                    newTransaction.memberId = memberId;
                    newTransaction.transactionType = "Borrow";

                    // Set tanggal transaksi
                    time_t now = time(nullptr);
                    newTransaction.date = formatDate(now);

                    // Set tanggal jatuh tempo (7 hari dari sekarang)
                    time_t dueDate = now + 7 * 24 * 60 * 60;
                    newTransaction.dueDate = formatDate(dueDate);

                    // Simpan transaksi
                    transactions.push_back(newTransaction);

                    // Tandai buku sebagai tidak tersedia
                    books[i].isAvailable = false;

                    // Tambahkan buku ke daftar peminjaman anggota
                    members[j].borrowedBooks.push(bookId);

                    cout << "Book borrowed successfully. Due date: " << newTransaction.dueDate << "\n";
                    return;
                }
            }
            cout << "Member not found.\n";
            return;
        }
    }
    cout << "Book not found.\n";
}

void processReturning() {
    int bookId, memberId;
    cout << "Enter Book ID to return: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    for (auto& transaction : transactions) {
        if (transaction.bookId == bookId && transaction.memberId == memberId && transaction.transactionType == "Borrow") {
            transaction.transactionType = "Return";
            transaction.date = formatDate(time(nullptr));

            for (auto& book : books) {
                if (book.id == bookId) {
                    book.isAvailable = true;
                    break;
                }
            }

            for (auto& member : members) {
                if (member.id == memberId) {
                    if (!member.borrowedBooks.empty() && member.borrowedBooks.top() == bookId) {
                        member.borrowedBooks.pop();
                    }
                    break;
                }
            }

            cout << "Book returned successfully.\n";
            return;
        }
    }

    cout << "No active borrowing transaction found for the given Book ID and Member ID.\n";
}

void deleteTransaction() {
    int bookId, memberId;
    cout << "Enter Book ID: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].bookId == bookId && transactions[i].memberId == memberId) {
            transactions.erase(transactions.begin() + i);
            cout << "Transaction deleted successfully.\n";
            return;
        }
    }
    cout << "Transaction not found.\n";
}

// Extend Borrowing
void extendBorrowing() {
    int bookId, memberId;
    cout << "Enter Book ID: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    for (auto& t : transactions) {
        if (t.bookId == bookId && t.memberId == memberId && t.transactionType == "Borrow") {
            time_t dueDate = parseDate(t.dueDate);
            dueDate += 7 * 24 * 60 * 60; // Extend by 7 days
            t.dueDate = formatDate(dueDate);
            t.transactionType = "Extend";
            cout << "Borrowing period extended successfully. New due date: " << t.dueDate << "\n";
            return;
        }
    }
    cout << "Transaction not found or not eligible for extension.\n";
}

// Reports Menu
void reportMenu() {
    int choice;
    do {
        cout << "\nReports\n";
        cout << "1. Calculate Late Fee\n";
        cout << "2. View Transactions\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            calculateLateFee();
            break;
        case 2:
        	viewTransactions();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void viewTransactions() {
    cout << "\nTransaction History:\n";
    cout << "+------------+------------+----------------+------------+------------+\n";
    cout << "| Book ID    | Member ID  | Transaction    | Date       | Due Date   |\n";
    cout << "+------------+------------+----------------+------------+------------+\n";

    for (const auto& t : transactions) {
        cout << "| " << setw(10) << left << t.bookId
             << " | " << setw(10) << left << t.memberId
             << " | " << setw(14) << left << t.transactionType
             << " | " << setw(10) << left << t.date
             << " | " << setw(10) << left << t.dueDate << " |\n";
    }

    cout << "+------------+------------+----------------+------------+------------+\n";
}

// Calculate Late Fee
void calculateLateFee() {
    int bookId, memberId;
    cout << "Enter Book ID: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    for (const auto& t : transactions) {
        if (t.bookId == bookId && t.memberId == memberId && t.transactionType == "Borrow") {
            time_t dueDate = parseDate(t.dueDate);
            time_t currentDate = time(nullptr);

            if (currentDate > dueDate) {
                int daysLate = (currentDate - dueDate) / (24 * 60 * 60);
                int lateFee = daysLate * 5000; // Late fee: 5000 per day
                cout << "Book is late by " << daysLate << " days. Late fee: Rp " << lateFee << "\n";
            } else {
                cout << "No late fee. Book is returned on time.\n";
            }
            return;
        }
    }
    cout << "Transaction not found or not eligible for late fee calculation.\n";
}

void showMainMenu(User& loggedInUser) {
    int choice;
    do {
        cout << "\nLibrary Management System\n";

        if (loggedInUser.role == "Admin") {
            cout << "1. Book Management\n";
            cout << "2. Member Management\n";
            cout << "3. Transactions\n";
            cout << "4. Reports\n";
            cout << "5. Exit\n";
        }
        else if (loggedInUser.role == "Petugas") {
            cout << "1. Transactions\n";
            cout << "2. Exit\n";
        }
        else if (loggedInUser.role == "Supervisor") {
            cout << "1. Book Management\n";
            cout << "2. Member Management\n";
            cout << "3. Transactions\n";
            cout << "4. Reports\n";
            cout << "5. Exit\n";
        }

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (loggedInUser.role == "Admin" || loggedInUser.role == "Supervisor") {
                bookManagementMenu();
            }
            else if (loggedInUser.role == "Petugas") {
                transactionManagementMenu();
            }
            break;
        case 2:
            if (loggedInUser.role == "Admin" || loggedInUser.role == "Supervisor") {
                memberManagementMenu();
            }
            else if (loggedInUser.role == "Petugas") {
                transactionManagementMenu();
            }
            break;
        case 3:
            if (loggedInUser.role == "Admin" || loggedInUser.role == "Supervisor") {
                transactionManagementMenu();
            }
            break;
        case 4:
            if (loggedInUser.role == "Admin" || loggedInUser.role == "Supervisor") {
                reportMenu();
            }
            break;
        case 5:
            if (loggedInUser.role == "Admin" || loggedInUser.role == "Supervisor") {
                cout << "Exiting the system.\n";
                return;
            }
            else if (loggedInUser.role == "Petugas") {
                cout << "Exiting the system.\n";
                return;
            }
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// Main function
int main() {
    // Adding some users to simulate a login system
    users.push_back({"admin", "admin123", "Admin"});
    users.push_back({"petugas", "petugas123", "Petugas"});
    users.push_back({"supervisor", "supervisor123", "Supervisor"});

    User loggedInUser;

    if (login(loggedInUser)) {
        cout << "Login successful! Welcome, " << loggedInUser.username << "\n";
        showMainMenu(loggedInUser);
    }
    else {
        cout << "Login failed! Please check your credentials.\n";
    }

    return 0;
}
