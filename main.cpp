#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool issued;
    string issuedTo;

    Book(int id, string title, string author) : id(id), title(title), author(author), issued(false) {}
};

class Library {
private:
    vector<Book> books;
    stack<Book*> issuedBooks;

    Book* binarySearchById(int id) {
        int left = 0, right = books.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (books[mid].id == id) {
                return &books[mid];
            }
            if (books[mid].id < id) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return nullptr;
    }

    vector<Book*> linearSearchByTitle(string title) {
        vector<Book*> result;
        for (auto& book : books) {
            if (book.title == title) {
                result.push_back(&book);
            }
        }
        return result;
    }

    void quickSortById(int left, int right) {
        if (left >= right) return;
        int pivot = books[right].id;
        int partitionIndex = left;
        for (int i = left; i < right; ++i) {
            if (books[i].id <= pivot) {
                swap(books[i], books[partitionIndex]);
                partitionIndex++;
            }
        }
        swap(books[partitionIndex], books[right]);
        quickSortById(left, partitionIndex - 1);
        quickSortById(partitionIndex + 1, right);
    }

    void quickSortByTitle(int left, int right) {
        if (left >= right) return;
        string pivot = books[right].title;
        int partitionIndex = left;
        for (int i = left; i < right; ++i) {
            if (books[i].title <= pivot) {
                swap(books[i], books[partitionIndex]);
                partitionIndex++;
            }
        }
        swap(books[partitionIndex], books[right]);
        quickSortByTitle(left, partitionIndex - 1);
        quickSortByTitle(partitionIndex + 1, right);
    }

public:
    void addBook(int id, string title, string author) {
        if (binarySearchById(id)) {
            cout << "Book with this ID already exists.\n";
            return;
        }
        books.push_back(Book(id, title, author));
        cout << "Book added successfully.\n";
    }

    void searchBookById(int id) {
        Book* book = binarySearchById(id);
        if (book) {
            cout << "Book ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << ", Status: " << (book->issued ? "Issued" : "Available") << "\n";
        } else {
            cout << "Book not found.\n";
        }
    }

    void searchBooksByTitle(string title) {
        vector<Book*> books = linearSearchByTitle(title);
        if (!books.empty()) {
            for (auto book : books) {
                cout << "Book ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << ", Status: " << (book->issued ? "Issued" : "Available") << "\n";
            }
        } else {
            cout << "Book not found.\n";
        }
    }

    void issueBook(int id, string studentName) {
        Book* book = binarySearchById(id);
        if (book) {
            if (!book->issued) {
                book->issued = true;
                book->issuedTo = studentName;
                issuedBooks.push(book);
                cout << "Book issued successfully.\n";
            } else {
                cout << "Book is already issued.\n";
            }
        } else {
            cout << "Book not found.\n";
        }
    }

    void returnBook(int id) {
        if (!issuedBooks.empty() && issuedBooks.top()->id == id) {
            Book* book = issuedBooks.top();
            book->issued = false;
            book->issuedTo = "";
            issuedBooks.pop();
            cout << "Book returned successfully.\n";
        } else {
            cout << "Book is not issued or was not the last issued book.\n";
        }
    }

    void listAllBooks(bool sortByTitle = false) {
        if (sortByTitle) {
            quickSortByTitle(0, books.size() - 1);
        } else {
            quickSortById(0, books.size() - 1);
        }
        for (const auto& book : books) {
            cout << "Book ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << ", Status: " << (book.issued ? "Issued" : "Available") << "\n";
        }
    }

    void deleteBook(int id) {
        auto it = remove_if(books.begin(), books.end(), [id](const Book& book) {
            return book.id == id;
        });
        if (it != books.end()) {
            books.erase(it, books.end());
            cout << "Book deleted successfully.\n";
        } else {
            cout << "Book not found.\n";
        }
    }
};

int main() {
    Library library;
    int choice, id, numBooks;
    string title, author, studentName;

    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Add Multiple Books\n";
        cout << "3. Search Book by ID\n";
        cout << "4. Search Book by Title\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. List All Books\n";
        cout << "8. List All Books (Sorted by Title)\n";
        cout << "9. Delete Book\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Book ID: ";
            cin >> id;
            cout << "Enter Book Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Book Author: ";
            getline(cin, author);
            library.addBook(id, title, author);
            break;
        case 2:
            cout << "Enter number of books to add: ";
            cin >> numBooks;
            for (int i = 0; i < numBooks; ++i) {
                cout << "Enter Book ID: ";
                cin >> id;
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                library.addBook(id, title, author);
            }
            break;
        case 3:
            cout << "Enter Book ID: ";
            cin >> id;
            library.searchBookById(id);
            break;
        case 4:
            cout << "Enter Book Title: ";
            cin.ignore();
            getline(cin, title);
            library.searchBooksByTitle(title);
            break;
        case 5:
            cout << "Enter Book ID: ";
            cin >> id;
            cout << "Enter Student Name: ";
            cin.ignore();
            getline(cin, studentName);
            library.issueBook(id, studentName);
            break;
        case 6:
            cout << "Enter Book ID: ";
            cin >> id;
            library.returnBook(id);
            break;
        case 7:
            library.listAllBooks();
            break;
        case 8:
            library.listAllBooks(true);
            break;
        case 9:
            cout << "Enter Book ID: ";
            cin >> id;
            library.deleteBook(id);
            break;
        case 10:
            exit(0);
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
