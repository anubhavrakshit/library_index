#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <unordered_map>

using namespace std;

// Build an Index of [pattern_hash, pattern, vector [ Book_Name, Freq_of_pattern]]
// JobBuildIndex(Books) {
//  Step 1: Select a Book
//  Step 2: GetWord(Book)
// 	Step 3: Create Hash for the word and lookup if it exists:
//		Step 3a: Does not exist, Insert Word
//		Step 3b: Word exists, find the book and update the Freq_of_pattern
//
// }

class Book {
private:
    string bookname;
    string filepath;
    fstream filestream;
public:
    Book (string name, string filepath) : bookname(name), filestream(filepath) {}
    string GetBookName() {
        return bookname;
    }
    string GetWords() {
        // iterator to return strings from a book
        string word;
        filestream >> word;

        // remove punctuations
        size_t len = word.size();
        while (len && ispunct(word[len -1])) {
            word.erase(len - 1, 1);
            len = word.size();
        }
        return word;
    }
    bool StreamEof() {
        return filestream.eof();
    }
};

bool sortbycount(const pair<string,int> &a,
                 const pair<string,int> &b) {
    return (a.second > b.second);
}

class Index {
    struct index_entry {
        unordered_map <string, int> entry; // BookName and Count
    };
    unordered_map <string, index_entry> index;
    int total_keys = 0;
    long total_words = 0;

public:
    // Index is essentially a Map with
    // Key as Word from Book
    // Value as a Map of Book Name and Frequency of the Word
    //  WORD -> [Book1: Freq_Count] [Book2: Freq_Count] .. [BookN: Freq_Count]
    // We choose a map to store Book, Freq_Count as that makes Inserts fast.
    void InsertWord(string word, string book_name) {
        total_words++;
        if (index.find(word) == index.end()) {
            // New word and New Book to add to index
            index[word].entry.insert(make_pair(book_name, 1));
            total_keys++;
        } else {
            // Word is already in index
            // Need to check if book is present or not
            index_entry& t_entry = index[word];
            // New Book entry
            if (t_entry.entry.find(book_name) == t_entry.entry.end()) {
                index[word].entry.insert(make_pair(book_name, 1));
            } else {
                // Increment count
                t_entry.entry[book_name]++;
            }
        }
    }

    void Print() {
        // Print Index
       // unordered_map<string, index_entry>::iterator itr;
        for (auto itr = index.begin(); itr != index.end(); itr++) {
            std::cout << "***************" << std::endl;
            std::cout << "Word: " << itr->first << std::endl;
            // unordered_map<string, int>::iterator itr_books;
            struct index_entry t_entry = itr->second;
            for (auto itr_books = t_entry.entry.begin(); itr_books != t_entry.entry.end(); itr_books++) {
                std::cout << "Book: " << itr_books->first << " " << "Count: " << itr_books->second << std::endl;
            }
            std::cout << "***************" << std::endl;
        }
        cout << "Total Keys: " << total_keys << endl;
        cout << "Total Words scanned: " << total_words << endl;
    }

    vector <pair<string, int>> Lookup(string key) {
        cout << "Lookup key: " << key << endl;
        vector <pair<string, int>> result;
        auto index_itr = index.find(key);
        if (index_itr != index.end()) {
            index_entry e = index_itr->second;
            auto book_itr = e.entry.begin();
            while (book_itr != e.entry.end()) {
                //cout << book_itr->first << " : " << book_itr->second << endl;
                result.push_back(make_pair(book_itr->first, book_itr->second));
                book_itr++;
            }

            // sort results
            sort(result.begin(), result.end(), sortbycount);
                    }
        return result;
    }
};


// TODO: 1. Create Index
// 		 2. Search through Index and list top ten books
int main()
{
    int num_books;
    int num_keys = 0;
    vector<Book *> books;

    // Parse and store info about books
    // Format is
    // Int_num_books
    // Str_Book_Name_1 Str_Book_Path_1
    // .
    // .
    // Str_Book_Name_N Str_Book_Path_N
    // Int_num_keys_to_lookup
    // Int_Key1
    // Int_Key2
    // .
    // Int_KeyN


    //7
    //Alice /Users/arakshit/Downloads/Alice.txt
    //Mobydick /Users/arakshit/Downloads/moby.txt
    //Beowulf /Users/arakshit/Downloads/Beowulf.txt
    //Dracula /Users/arakshit/Downloads/dracula.txt
    //TomSawyer /Users/arakshit/Downloads/tom.txt
    //Republic /Users/arakshit/Downloads/republic.txt
    //WarPeace /Users/arakshit/Downloads/war.txt
    //15
    //creation
    //worry
    //the
    //vampire
    //she
    //he
    //him
    //lost
    //apple
    //orange
    //life
    //supreme
    //war
    //peace
    //happy
    //

    cout << "Number of books to read: ";
    cin >> num_books;
    cout << "NameOfBook PathOfBook" << endl;
    while (num_books--) {
        string book_name, book_path;
        cin >> book_name  >> book_path;
        Book* book_t = new Book(book_name, book_path);
        books.push_back(book_t);
    }
    cout << "Number of keys to lookup: ";
    cin >> num_keys;
    vector <string> keys;
    while (num_keys--) {
        string key;
        cin >> key;
        keys.push_back(key);
    }

    Index index;

    // Scan words and store in Index
    for (auto b: books) {
        string book_name = b->GetBookName();
        while (!b->StreamEof()) {
            string word = b->GetWords();
            index.InsertWord(word, book_name);
        }
    }

    vector <pair<string, int>> result;
    for (auto key: keys) {
       result = index.Lookup(key);
       // print results
       cout << "Top Results for key: " << key << endl;
       for (auto x: result) {
           cout << x.first << " " << x.second << endl;
       }
    }


    //index.Print();
    return 0;
}
