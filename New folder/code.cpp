#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

class Node
{
public:
    string word;
    string meaning;
    Node *left;
    Node *right;
    Node(string wrd, string mng)
    {
        word = wrd;
        meaning = mng;
        left = nullptr;
        right = nullptr;
    }
};

string toLowerCase(string s) {
    for (char &ch : s)
        ch = tolower(ch);
    return s;
}

void showErrorMsg(const string &er){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColor = consoleInfo.wAttributes;


    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);


    cout << "\n";
    cout << "==================================================\n";
    cout << "| " << er << "\n";
    cout << "==================================================\n";
    cout << "\n";


    SetConsoleTextAttribute(hConsole, originalColor);
}

void showInfoMsg(const string &er){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColor = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "\n";
    cout << "==================================================\n";
    cout << ":" << er << "\n";
    cout << "==================================================\n";
    cout << "\n";

    SetConsoleTextAttribute(hConsole, originalColor);
}

void showDictionaryTable(const vector<pair<string, string>>& data) {
    if((int)data.size() == 0){
        showInfoMsg("No Words Available In Dictionary. Please Add New Word");
        return;
    }
    cout << " All Words: \n";
    const int wordWidth = 20;
    const int meaningWidth = 40;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    auto printBorder = [&]() {
        cout << "+" << string(wordWidth + 2, '-') << "+" << string(meaningWidth + 2, '-') << "+\n";
    };

    printBorder();

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColor = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);


    cout << "| " << left << setw(wordWidth) << "Word"
         << " | " << left << setw(meaningWidth) << "Meaning" << " |\n";


    SetConsoleTextAttribute(hConsole, originalColor);


    printBorder();


    for (const auto& entry : data) {
        cout << "| " << left << setw(wordWidth) << entry.first
             << " | " << left << setw(meaningWidth) << entry.second << " |\n";
        printBorder();
    }
}



void showWordMeaningTable(pair<string, string> entry, pair<string, string> val) {
    const int wordWidth = 22;
    const int meaningWidth = 42;

    cout << "\n";

    string line = "+" + string(wordWidth + 2, '-') + "+" + string(meaningWidth + 2, '-') + "+";

    cout << line << endl;
    cout << "| " << left << setw(wordWidth) << val.first
         << " | " << left << setw(meaningWidth) << val.second << " |" << endl;
    cout << line << endl;

    string word = entry.first;
    string meaning = entry.second;

    cout << "| " << left << setw(wordWidth) << word
         << " | " << left << setw(meaningWidth) << meaning << " |" << endl;
    cout << line << endl;
}



pair<Node*, Node*> searchOnBST(Node *root, string &keyWord)
{
    Node *currPtr = root;
    Node *prevPtr = nullptr;

    while (currPtr){
        prevPtr = currPtr;
        if (currPtr->word == keyWord){
            return {currPtr, prevPtr};
        }
        else if (keyWord < currPtr->word){
            currPtr = currPtr->left;
        }
        else{
            currPtr = currPtr->right;
        }
    }
    return {nullptr, prevPtr};
}

pair<Node*, Node*> searchOnBSTtoFindParNChild(Node *root, string &keyWord){
    Node *parentPtr = nullptr;
    Node *currPtr = root;

    while (currPtr && currPtr->word != keyWord)
    {
        parentPtr = currPtr;
        if (keyWord < currPtr->word)
        {
            currPtr = currPtr->left;
        }
        else
        {
            currPtr = currPtr->right;
        }
    }

    return {currPtr, parentPtr};
}


bool insertToBST(Node *&root, string &keyWord, string &meaning)
{
    if (root == nullptr){
        root = new Node(keyWord, meaning);
        return true;
    }

    pair<Node*, Node*> resultNode = searchOnBST(root, keyWord);
    Node *currNode = resultNode.first;
    Node *prevNode = resultNode.second;

    if(currNode){
        showInfoMsg("Word already exists on Dictornary");
        return true;
    }


    if (keyWord < prevNode->word){
        prevNode->left = new Node(keyWord, meaning);
    }
    else{
        prevNode->right = new Node(keyWord, meaning);
    }
    return true;
}

pair<Node*, Node*> getInorderSuccessor(Node *currPtr){
    Node* succPar = currPtr;
    currPtr = currPtr->right;

    while(!currPtr && !(currPtr->left)){
        succPar = currPtr;
        currPtr = currPtr->left;
    }
    return {currPtr, succPar};
}

bool deleteWordFromBST(Node *&root, string &keyWord)
{

    pair<Node*, Node*> resultNode = searchOnBSTtoFindParNChild(root, keyWord);
    Node *currPtr = resultNode.first;
    Node *parPtr = resultNode.second;

    if (!currPtr)
    {
        showErrorMsg("Word not found!");
        return false;
    }

    if (!currPtr->left || !currPtr->right)
    {
        Node *child = nullptr;
        if(currPtr->left){
            child = currPtr->left;
        }else{
          child = currPtr->right;
        }

        if (!parPtr)
        {
            root = child;
        }
        else if (parPtr->left == currPtr)
        {
            parPtr->left = child;
        }
        else
        {
            parPtr->right = child;
        }
        delete currPtr;
    }
    else
    {
        pair<Node*, Node*> resultNode = getInorderSuccessor(currPtr);
        Node* succPtr = resultNode.first;
        Node* succParent = resultNode.second;

        currPtr->word = succPtr->word;
        currPtr->meaning = succPtr->meaning;

        if (succParent->left == succPtr)
        {
            succParent->left = succPtr->right;
        }
        else
        {
            succParent->right = succPtr->right;
        }
        delete succPtr;
    }
}

void deleteWord(Node *&root) {
    string keyWord;
    cout << " Enter the word you want to Delete: ";
    getline(cin >> ws, keyWord);
    bool ck = deleteWordFromBST(root, keyWord);
    if(ck){
        showInfoMsg("Word Deleted Successfully");
    }
}

void updateWord(Node *root)
{
    string word, new_definition;
    cout << " Enter the word you want to update: ";

    getline(cin >> ws, word);

    word = toLowerCase(word);

    pair<Node*, Node*> result = searchOnBST(root, word);
    Node *curr = result.first;
    if (curr)
    {
        cout << " Enter the new definition: ";
        getline(cin >> ws, new_definition);

        string oldMeaning = curr->meaning;

        curr->meaning = new_definition;

        showInfoMsg("Word definition updated successfully");
        showWordMeaningTable({oldMeaning, curr->meaning}, {"Old Meaning", "Updated Meaning"});
    }
    else
    {
        showErrorMsg("Word not found in the dictionary");
    }
}

void addWord(Node *&root)
{
    string word, meaning;
    cout << " Enter a word to add: ";
    getline(cin >> ws, word);

    pair<Node*, Node*> result = searchOnBST(root, word);
    Node *curr = result.first;
    if (curr){
      showInfoMsg("This Word Already exist in Dictionary. You can update the meaning");
    }else{
    cout << " Enter the meaning of this word: ";
    getline(cin >> ws, meaning);

    word = toLowerCase(word);
    insertToBST(root, word, meaning);
     showInfoMsg("This word added successfully");
    }
}

void searchWord(Node *root)
{
    string word;
    cout << " Enter a word to search: ";
    //cin.ignore();
    getline(cin >> ws, word);
    word = toLowerCase(word);

     pair<Node*, Node*> result = searchOnBST(root, word);
    Node *curr = result.first;
    if (curr)
    {
        showWordMeaningTable({curr->word, curr->meaning}, {"Word", "Meaning"});
    }
    else
    {
        showErrorMsg("Word not found in the dictionary");
    }
}


bool readDataFromTxtFile(Node *&root)
{
    fstream inputFile("dictionary.txt", ios::in | ios::out);

    if (!inputFile){
         inputFile.open("dictionary.txt", ios::out);
        inputFile.close();
       //showErrorMsg("File opening error!");
       return false;
    }
    string line;

    while (getline(inputFile, line))
    {
        if (line == "!!!")
            break;

        size_t pos = line.find('#');
        if (pos != string::npos)
        {
            string word = line.substr(0, pos);
            string meaning = line.substr(pos + 1);
            word = toLowerCase(word);
            bool ck = insertToBST(root, word, meaning);
            if(!ck){
                showErrorMsg("Error on Inserting words to BST!");
            }
        }
    }

    inputFile.close();
    return true;
}

void viewAllWords(Node *root, vector<pair<string, string>> &data)
{
    if (root)
    {
        viewAllWords(root->left, data);
        data.push_back({root->word, root->meaning});
        viewAllWords(root->right, data);
    }
}

void inorder(Node *root, ofstream &outfile)
{
    if (root)
    {
        inorder(root->left, outfile);
        outfile << root->word << "#" << root->meaning << endl;
        inorder(root->right, outfile);
    }
}

void writeInFile(Node *root)
{
    ofstream outfile("dictionary.txt");
    inorder(root, outfile);
    outfile.close();
}


/*
 UI WORKS HERE
*/

void drawBanner() {
    cout << "\n";
    cout << " " << char(201);
    for (int i = 0; i < 49; ++i) cout << char(205);
    cout << char(187) << "\n";

    cout << " " << char(186);
    for (int i = 0; i < 12; ++i) cout << " ";
    cout << "DICTIONARY USING BST";
    for (int i = 0; i < 17; ++i) cout << " ";
    cout << char(186) << "\n";

    cout << " " << char(200);
    for (int i = 0; i < 49; ++i) cout << char(205);
    cout << char(188) << "\n";
}
void showMenu() {
    //(1 = blue)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 1);

    cout << "\n\n";
    cout << " " << char(218);
    for (int i = 0; i < 40; ++i) cout << char(196);
    cout << char(191) << "\n";

    cout << " " << char(179) << "  1. Add New Word                       " << char(179) << "\n";
    cout << " " << char(179) << "  2. Search Word Meaning                " << char(179) << "\n";
    cout << " " << char(179) << "  3. Update Word Meaning                " << char(179) << "\n";
    cout << " " << char(179) << "  4. Delete Word from Dictionary        " << char(179) << "\n";
    cout << " " << char(179) << "  5. View All Words                     " << char(179) << "\n";
    cout << " " << char(179) << "  6. Clear The Window                   " << char(179) << "\n";
    cout << " " << char(179) << "  0. Exit                               " << char(179) << "\n";

    cout << " " << char(192);
    for (int i = 0; i < 40; ++i) cout << char(196);
    cout << char(217) << "\n";

    //(7 = white)
    SetConsoleTextAttribute(hConsole, 7);
}

/*
 UI WORK END
*/

void processChoice(int choice, Node *&root) {
    cout << "\n\n";

    if (choice == 1) {
        addWord(root);
        writeInFile(root);
    }
    else if (choice == 2) {
        searchWord(root);
    }
    else if (choice == 3) {
        updateWord(root);
        writeInFile(root);
    }
    else if (choice == 4) {
        deleteWord(root);
        writeInFile(root);
    }
    else if (choice == 5) {
        vector<pair<string, string>> data;
        viewAllWords(root, data);
        showDictionaryTable(data);
    }
    else {
        showErrorMsg("Invalid choice!");
    }
}


int main(){

   Node *root = nullptr;
   readDataFromTxtFile(root);

   int choice;
    drawBanner();

    while (true) {

        showMenu();

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 1);
        cout << "\n Enter your choice : ";
        SetConsoleTextAttribute(hConsole, 7);

        cin >> choice;

        if (choice == 0) break;
        else if(choice == 6){
            system("cls");
            drawBanner();
            continue;
        }
        else processChoice(choice, root);
    }

  return 0;

}
