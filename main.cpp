
/*      My Digital Library Information System       */

/*                  Features
    
    - Perform CURD Operations
    - Store Books Based on Categories
    - Search Book Based on Category, Author Name and Book Title
    - Smooth Architecture
    - Save Records with Unique ID

*/ 

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib> // for random unique_id
#include <ctime> // for random unique_id
#include <cctype>  // for tolower

// Utility class handle file operations and holds helper functions
class Utility; // forward declartion to use it in BookModel
class BACKEND; // forward declaration 

// Book Models
class BookModel {
    // Book schema
    int unique_id; // book unique id
    char book_name[70]; // book name
    char book_author[50]; // book author
    int year_of_publish; // Book published year
    char category[12]; // book category
    std::string categories[4] = {"General", "Literature", "Science", "Religion"}; // default categories

    // friend class Utility; // Access to BookModel Schema
    friend class BACKEND; // Access to BookModel Schema

public:
    // constructor
    BookModel(int uid=0, const char* bn="", const char* author="", int yop=0, const char* category=""){
        this->unique_id = uid;
        strcpy(book_name, bn);
        strcpy(book_author, author);
        this->year_of_publish = yop;
        strcpy(this->category, category);
    };

    // Input 
    void input(){
        // Best; O(1)
        // Worst: O(n) We don't know how much time validation loop will iterate.

        // On every input call, generate a new unique id for each book
        std::srand(time(0)); 
        int random_number = rand();

        unique_id = random_number;

        do{ // validate Book name
            try
            {
                std::cout << "Enter Book Name: ";
                std::cin.getline(book_name, 70);

                if(strlen(book_name) < 4 )
                    throw("Error: <Shor Name> Please enter atleast 4-char name");
                if(strlen(book_name) > 70)
                    throw("Error: <Long Name> Please enter 70-char name only");
                
                break;
            }
            catch(const char* error)
            {
                std::cerr << error << '\n';
            }
            
        } while (true);

        do{ // validate Author name
            try
            {
                std::cout << "Enter Book Author: ";
                std::cin.getline(book_author, 50);
                if(strlen(book_author) < 3)
                    throw("Error: <Short Name> Please enter atleast 3-char name");
                if(strlen(book_author) > 50)
                    throw("Error: <Long Name> Please enter 50-char name only");
                break;
            }
            catch(const char* error)
            {
                std::cerr << error << '\n';
            }
        } while (true);

        do{ // validate Year of publish
            try
            {
                std::cout << "Enter Year of Publish: ";
                std::cin >> year_of_publish;

                // terminate program incase of bad type error
                if(std::cin.fail()){ // bad type error
                    std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception message before program terminates
                    exit(0); // exist program
                }

                if(year_of_publish > 2024)
                    throw("Error: <Invalid publish year> Publish year can't be > 2024");
                if(year_of_publish < 1000)
                    throw("Error: <Invalid publish year> Please enter 4-digit number");
                break;
            }
            catch(const char* error)
            {
                std::cerr << error << '\n';
            }
        } while (true);

        std::cin.ignore();

        do{ // validate Category
            try
            {
                std::cout << "Select a category [General, Literature, Science, Religion]: ";
                std::cin.getline(category, 12);
                if(!find_category(category) || strlen(category) > 12)
                    throw("Error: <Invalid category> Please select a valid category");
                break;
            }
            catch(const char* error)
            {
                std::cerr << error << '\n';
            }
            
        } while (true);
    }
    
    // Display
    void display() const{
        // Best: O(1)
        // Worst: O(1)

        std::cout << "Book ID: " << unique_id << std::endl
                  << "Book Name: " << book_name << std::endl
                  << "Book Author: " << book_author << std::endl
                  << "Year of Publish: " << year_of_publish << std::endl
                  << "Category: " << category << std::endl;
    }

    // check if entered category match with the defined categories
    bool find_category(const char* str) const{
        // Best: O(1)
        // worst: O(1)
        for (int i = 0; i < 4; ++i){
            if(str == categories[i])
                return true;
        }
        return false;
    }
    
};

// Utility class: holds helper functions
class Utility {

public:

    // Return Total length of Records/Objects in a binary file
    int records_len(const char* filename){
        // setup ifstream
        std::ifstream f(filename, std::ios::binary | std::ios::in);
        
        if(!f){ // if not file throw exception
            f.close();
            throw("Error: <file> Unable to open file");
        } else { // if file is available

            // Get the file size
            f.seekg(0, std::ios::end);
            std::streampos file_size = f.tellg();
            f.seekg(0, std::ios::beg);

            f.close(); // close file

            // calculate the no. of objects || Students records
            int size = file_size / sizeof(BookModel);

            // return Student objects size || total records
            return size;
        }
    }

    // Write to a file
    void writeToFile(const char* filename, const BookModel* books, int size){
        // Best: O(n)
        // Worst: O(n)

        // setup ofstream
        std::ofstream f(filename, std::ios::binary | std::ios::app | std::ios::out);

        if(!f){ // if file is not available
            f.close();
            throw("Error: <file> Unable to open file");
        } else { // if available
            for (int i = 0; i<size; ++i){
                f.write((const char *)&books[i], sizeof(BookModel));
            }    
        }

        f.close(); // close file
    }

    // Read from file
    void readFromFile(const char* filename, BookModel* books, int size){
        // Best: O(1)
        // Worst: O(1)

        // setup ifstream
        std::ifstream f(filename, std::ios::binary | std::ios::in);

        if(!f){ // if file is not available
            f.close();
            throw("Error: <file> Unable to open file");
        } else { // if available
            f.read((char *)books, size * sizeof(BookModel)); // read file
            f.close(); // close file
        }
    }

    // Convert String To LowerCase
    void toLowerCase(char* str) {
        for (int i = 0; str[i] != '\0'; ++i) {
            str[i] = std::tolower(str[i]); 
        }
    }

    /* FRONTEND MENUS */

    // welcome_page_menu
    void home_page_menu(const char* page_title) const{
        std::cout << page_title << std::endl; // Menu title

        // Menu Options
        std::cout << "1. Search a Book" << std::endl
                  << "2. View All Books" << std::endl
                  << "3. Contribute a Book" << std::endl
                  << "4. Update a Book" << std::endl
                  << "5. Delete a Book" << std::endl
                  << "6. Import & Export CSV File Data" << std::endl
                  << "7. Help" << std::endl
                  << "8. exit" << std::endl;
    }

    // search_page_menu
    void search_page_menu(const char* page_title) const{
        std::cout << page_title << std::endl; // Menu title

        // Menu Options
        std::cout << "1. Search a Book by Name" << std::endl
                  << "2. Search a Book by Category" << std::endl
                  << "3. Search a Book by Author Name" << std::endl
                  << "4. Back to Main Menu" << std::endl
                  << "5. Help" << std::endl
                  << "6. exit" << std::endl;

    }

    // help_page_menu
    void help_page_menu(const char* page_title) const{
        std::cout << page_title << std::endl; // Menu title

        // Menu Options
        std::cout << "1. About My Digital Library System" << std::endl
                  << "2. How can I use it?" << std::endl
                  << "3. List Contributors" << std::endl
                  << "4. View Source Code" << std::endl
                  << "5. View LICENSE" << std::endl
                  << "6. Back to Main Menu" << std::endl
                  << "7. exit" << std::endl;
    }

};

// Perform CURD operations & Application Logic
class BACKEND {
    Utility utils; // helper functions
    BookModel *books; // Application data model i.e. Book Schema

public:

    /* Methods: GET | POST | PUT | DELETE */

    // GET RECORDS
    void GET(const char* filename){
        // Best: O(n)
        // Worst: O(n)
        try
        {
            int size = utils.records_len(filename);
            books = new BookModel[size];
            utils.readFromFile(filename, books, size);
            std::cout << "\n\t All Library Books \n" << std::endl;
            for (int i = 0; i < size; ++i){
                books[i].display();
                std::cout << std::endl;
            }
            delete[] books;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            delete[] books;
        }
    }

    // POST RECORD
    void POST(const char* filename, int size){
        // Best: O(n)
        // Worst: O(n^2)
        try
        {
            books = new BookModel[size]; // allocate memory 

            for (int i = 0; i < size; ++i){
                std::cout << std::endl; // for better readibility in console
                books[i].input(); // input records
            }

            utils.writeToFile(filename, books, size); // Write records to file

            delete[] books; // free memory
        }
        catch(const char* error)
        {
            std::cerr << error << std::endl;
            delete[] books; // free memory incase of error
        }
    }

    // PUT RECORD
    void PUT(const char* filename, int index){
        try
        {
            std::cout << "Starting PUT function" << std::endl;
            int size = utils.records_len(filename); // get objects size
            std::cout << "Size of records: " << size << std::endl;

            while (index < 0 || index >= size){
                std::cout << "Invalid index: " << index << std::endl;
                throw("Error: <Invalid index> Please enter valid index");
                std::cout << "Enter a index: ";
                std::cin >> index;
            }

            books = new BookModel[size]; // assign memory
            std::cout << "Memory allocated for books array." << std::endl;

            utils.readFromFile(filename, books, size); // read records
            std::cout << "Records read from file." << std::endl;

            std::cout << std::endl;
            std::cout << "Taking new input for book at index: " << index << std::endl;
            books[index].input(); // take new input

            std::cout << "Input taken successfully." << std::endl;

            // setup ofstream to update file
            std::ofstream f(filename, std::ios::binary | std::ios::out);

            std::cout << "File opened for writing" << std::endl;

            if(!f){
                f.close();
                throw("Error: <file> Unable to open file"); // incase of file is not availble
            } else{
                std::cout << "Writing updated record to file";
                f.write((const char *)&books[index], sizeof(BookModel)); // update the file
                std::cout << "Successfully written to file." << std::endl;
                std::cout << "\n file closed";
                std::cout << "File closed." << std::endl;
            }

            delete[] books;
            std::cout << "Memory deallocated for books array." << std::endl;
        }
        catch(const char* error)
        {
            delete[] books; // free memory incase of error
            std::cerr << error << std::endl;
        }
    }

    // DELETE RECORD
    void DELETE(const char* filename, int index){

    }

    /* Array Filter Methods */

    // Search by category
    void SearchByCategory(const char* filename, char *category){

            int size = utils.records_len(filename); // get length of available objects in file

            books = new BookModel[size]; // allocate memory

            utils.readFromFile(filename, books, size); // read data

            bool currentState = true; // flag

            for (int i = 0; i < size; ++i){ // iterate over array

                // ignore cases
                utils.toLowerCase(category); 
                utils.toLowerCase(books[i].category);

                if(strcmp(category, books[i].category) == 0){ // filter based category
                    currentState = false; // set currentState to false any record found
                    books[i].display(); // only display matched records
                    std::cout << std::endl;
                }
            }

            if(currentState){ // incase if there is no record available
                std::cout << "\nSorry! We couldn't find any record for query '" << category << "' "
                        << std::endl << "Please Try Search Again..." << std::endl;
            }

            delete[] books; // free memory
    }

    // Search by bookname
    void SearchByBookName(const char* filename, char *book_title){

        try
        {
            int size = utils.records_len(filename); // get records size

            books = new BookModel[size]; // allocate memory

            utils.readFromFile(filename, books, size); // read records

            bool currentState = true; // flag

            for (int i = 0; i < size; ++i){

                // ignore cases
                utils.toLowerCase(book_title);
                utils.toLowerCase(books[i].book_name);

                if(strcmp(book_title, books[i].book_name) == 0){
                    currentState = false; // set currentState to false any record found
                    books[i].display(); // display records
                    std::cout << std::endl;
                }
            }

            if(currentState){ // incase if there is no record available
                std::cout << "\nSorry! We couldn't find any record for query '" << book_title << "' "
                        << std::endl << "Please Try Search Again..." << std::endl;
            }

            delete[] books; // free memory
        }
        catch(const char* error)
        {
            delete[] books; // free memory incase of error
            std::cerr<<error<<std::endl;
        }
        
    }

    // Search by authorname
    void SearchByAuthorName(const char* filename, const char *author_name){

            int size = utils.records_len(filename); // get total records

            books = new BookModel[size]; // allocate memory

            utils.readFromFile(filename, books, size); // read records

            bool currentState = true; // flag 

            for (int i = 0; i < size; ++i){
                if(strcmp(author_name, books[i].book_author) == 0){ // comapre author name
                    currentState = false; // set currentState to false any record found
                    books[i].display(); // display records
                    std::cout << std::endl;
                }
            }

            if(currentState){ // incase if there is no record available
                std::cout << "\nSorry! We couldn't find any record for query '" << author_name << "'"
                        << std::endl << "Please Try Search Again..." << std::endl;
            }

            delete[] books; // free memory
        
    }

    // Check if a book id is already taken? 
    bool FindByUID(const char* filename, int uid){
        // Best: O(1)
        // Worst: O(n)

        try
        {
            // get total objects size from file
            int size = utils.records_len(filename);

            books = new BookModel[size];

            utils.readFromFile(filename, books, size);
            for (int i = 0; i < size; ++i){
                if(books[i].unique_id == uid){
                    return true;
                }
            }
            delete[] books; // clean memory
            return false;
        }
        catch(const char* error)
        {
            std::cerr << error << std::endl;
            delete[] books; // clean memory incase of exception
        }

    }

};

// Render UI in console
class FRONTEND {
    BACKEND backend; // Application Logic
    Utility utils; // Reusable components/menus

public:
    void home_page(const char* filename, const char* menu_title){
        utils.home_page_menu(menu_title); // Home page menu from Utility class
        do{
            try
            {
                // input option and render data menu wise
                int option;
                std::cout << "\nEnter a option [1, 2, 3, 4, 5, 6, 7]: ";
                std::cin >> option;

                // terminate program incase of bad type error
                if(std::cin.fail()){ // bad type error
                    std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception message before program terminates
                    exit(0); // exist program
                }

                if(option <= 0 || option > 8){ // validate option
                    throw("Error: <Invalid option> Please enter valid option");
                } else { // perform actions 
                    switch (option)
                    {
                    case 1: // Search page
                        search_page(filename);
                        break;

                    case 2: // fetch all books records from file
                        backend.GET(filename);
                        break;

                    case 3: // Insert a new record
                        int n;
                        std::cout << "\nEnter no. of Books you want to contribute: ";
                        std::cin >> n;

                        // terminate program incase of bad type error
                        if(std::cin.fail()){ // bad type error
                            std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception message before program terminates
                            exit(0); // exist program
                        }

                        std::cin.ignore(); 
                        backend.POST(filename, n);
                        break;

                    case 4: // Update existing record
                        int index;
                        std::cout << "\nEnter Book record index to update it: ";
                        std::cin >> index;

                        // terminate program incase of bad type error
                        if(std::cin.fail()){ // bad type error
                            std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception message before program terminates
                            exit(0); // exist program
                        }

                        std::cin.ignore();
                        backend.PUT(filename, index);
                        break;

                    case 5: // Delete a record
                        std::cout << "Delete Page" << std::endl;
                        break;

                    case 6: // handle CSV import export
                        std::cout << "Import Export page" << std::endl;
                        break;

                    case 7: // Show help i.e. how to use Application
                        help_page(filename);
                        break;

                    case 8: // Terminate program
                        std::cout << "\nThank You for using My Digital Library System" << std::endl;
                        exit(0);

                    default: // Throw exception incase of invalid option
                        throw("Error: <Invalid option> Please enter valid option");
                        break;
                    }
                }

                break; // get out of loop
            
            }
            catch(const char* error)
            {
                std::cerr << error << std::endl;
            }
            
        } while (true);
    }

    void search_page(const char* filename){
        do
        {
            try {
                    utils.search_page_menu("\n\tMy Digital Library Search System\n"); // display Search Menu
                    int option;
                    std::cout << "\nEnter a option [1, 2, 3, 4, 5]: ";
                    std::cin >> option;

                    // terminate program incase of bad type error
                    if(std::cin.fail()){
                        std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception messabefore program terminates
                        exit(0); // exist program
                    }

                    std::cin.ignore(); 

                    if(option <= 0 || option > 6){
                        throw("Error: <Invalid option> Please enter valid option");
                    } else { // if everything goes well then run program

                        switch (option)
                        {
                            case 1: // case 1: Search by Book Name

                                char BookNameQuery[70];
                                std::cout << "\nEnter Book Name: ";
                                std::cin.getline(BookNameQuery, 70);
                                
                                backend.SearchByBookName(filename, BookNameQuery);

                                break;

                            case 2: // Case 2: Search by Category
                                char CategoryQuery[12];
                                std::cout << "\nEnter Category: ";
                                std::cin.getline(CategoryQuery, 12);

                                backend.SearchByCategory(filename, CategoryQuery);

                            break;
                            
                            case 3: // Case 3: Search by Author Name
                                char AuthorNameQuery[50];
                                std::cout << "\nEnter Author Name: ";
                                std::cin.getline(AuthorNameQuery, 50);
                                
                                backend.SearchByAuthorName(filename, AuthorNameQuery);

                            break;

                            case 4: // Case 4: Go back to main menu
                                home_page(filename, "\n\tWelcome Back to My Digital Library System\n");
                                break;

                            case 5: // help page
                                help_page(filename);
                                break;

                            case 6: // exit program
                                std::cout << "\nThank You for using My Digital Library System" << std::endl;
                                exit(0);

                            default: // Throw exception incase of invalid option
                                throw("Error: <Invalid option> Please enter valid option");
                                break;
                        }
                    }

                } 
                catch(const char* error) {
                    std::cerr << error << std::endl;
                }
                            
        } while (true);
                        
    }

    void help_page(const char* filename){

        while(true) {

            try
            {
                utils.help_page_menu("\n\tMy Digital Library Help Center\n"); // Page title

                // input option and render data menu wise
                int option;
                std::cout << "\nEnter a option [1, 2, 3, 4, 5, 6, 7]: ";
                std::cin >> option;

                // terminate program incase of bad type error
                if(std::cin.fail()){ // bad type error
                    std::cout << "Error: <Invalid type> Program crashed due to invalid type"; // print exception message before program terminates
                    exit(0); // exist program
                }

                if(option <= 0 || option > 8){ // validate option
                    throw("Error: <Invalid option> Please enter valid option");
                } else { // perform actions 

                    switch (option)
                    {
                        case 1: // About My Digital Library
                            std::cout << "\nMy Digital Library is a console based application built using C++. It includes various features such as: " <<std::endl
                            << "\n- Perform CURD Operations\n- Store Books Based on Categories\n- Search Books Based on Category, Author Name and Book Title\n- Smooth Architecture\n- Save Records with Unique ID's and more" << std::endl;
                            break;

                        case 2: // How I can use it?
                            std::cout << "How to use it?" << std::endl;
                            break;

                        case 3: // List Contributors
                            std::cout << "\nContributor: (1)" << std::endl
                                    << "Saqib Bedar [GitHub: https://github.com/saqibbedar/]" << std::endl;
                            break;
                        
                        case 4: // View Source Code
                            std::cout << "\nSource Code: https://github.com/saqibbedar/digital-library" << std::endl;
                            break;

                        case 5: // View LICENSE
                            std::cout << "\nMIT LICENSE" << std::endl
                                    << "Copyright (c) 2024 Saqib Bedar" << std::endl;
                            break;

                        case 6: // Back to Main Menu
                            home_page(filename, "\n\tMy Digital Library Search System\n");
                            break;

                        case 7: // Terminate Program
                            std::cout << "\nThank You for using My Digital Library System" << std::endl;
                            exit(0);

                        default: // Throw exception incase of invalid option
                            throw("Error: <Invalid option> Please enter valid option");
                            break;
                    }
                }
            }

            catch(const char* error)
            {
                std::cerr << error << std::endl;
            }

        }
        
    }

};

int main(){
    const char filename[14] = "BookModel.bin";

    FRONTEND f;

    bool isFirstIteration = true; // to manage welcome_page title
    do
    { // Best O(n) // worst: O(n^4)
        try
        {
            if(isFirstIteration){
                f.home_page(filename, "\n\tWelcome to My Digital Library System\n");
                isFirstIteration = false;
            } else {
                f.home_page(filename, "\n\tMy Digital Library System Menu\n");
            }
        }
        catch(const char* error)
        {
            std::cerr << error << std::endl;
        }
    } while (true);
    
    return 0;
}
