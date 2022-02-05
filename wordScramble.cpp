#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cctype>     // tolower()
#include <vector>     // For vectors
#include <cassert>

using namespace std;

void PrintIntro() { //prints the beginning statement
    cout << "Program 5: Unscrambler" << endl;
    cout << "CS 141, Fall 2021, UIC" << endl << endl;
    cout << "You will be presented with a board that is filled with scrambled words. Rotate the rows and columns to get the words displayed in the right order." << endl << endl;
    cout << "13893 words have been read in from the dictionary." << endl << endl; //displays numLines
}

class Game {
public:
    int gameSize; //saves the length of the board
    int area; //saves the area of the board
    int numScramble; //userInput to the amount of times scrambled
    void setGameSize(); //function to set the game size
    void defaultBoard(int area); //function to push back empty values into the board array
    void arrayInitialization(int area); //initialize arrays to have an index equal to that of the game's area
    void boardAssign(vector <string> file, int area); //a function uses a seeded random to grab values from the file vector, as well as making sure that the words don't exceed the limit
    void setScrambleSize(); //function to allow the user to input the numScramble
    void scrambleBoard(int gameSize, int area, int numScramble); //function that scrambles the board
    void displayBoard(int gameSize, int area); //function to build the boards pieces, lines, and index
    void Menu(); //list of options on how to manipulate the board
    void rotateRow(int RowColNum, int numRotate); //rotates row x amount of times based on user input
    void rotateCol(int RowColNum, int numRotate); //rotates col x amount of times based on user input
    void subMenu(); //list of board manipulation options
    void finishedBoard(); //displays the board with the complete array
    void reset(int area); //resets the arrays to null values
    void correctVersion(int area); //sorts complete array to initial board array
    void Winners(int area); //builds string values and matches them to find if player has won
private:
    //initialize arrays with 100000
    char board[100000];
    char complete[100000];
    char restart[100000];
};

void Game::rotateRow(int RowColNum, int numRotate) { //function to rotate row
    for (int i = 0; i < abs(numRotate); i++) { //num times that the user wants row to rotate
        if (numRotate > 0) { //rotate left
            char tempRow = board[(RowColNum + 1) * gameSize - 1]; //last value in board
            for (int j = 0; j < gameSize - 1; j++) { //index for every board piece in a row
                board[(RowColNum + 1) * gameSize - 1 - j] = board[(RowColNum + 1) * gameSize - 2 - j]; //assign new rotated array values
            }
            board[gameSize * RowColNum] = tempRow; //first value in board
        } else if (numRotate < 0) { //rotate right
            char tempRow = board[gameSize * RowColNum]; //first value in board
            for (int j = 0; j < gameSize - 1; j++) { //index for every board piece in a row
                board[RowColNum * gameSize + j] = board[RowColNum * gameSize + j + 1]; //assign new rotated array values
            }
            board[gameSize * (RowColNum + 1) - 1] = tempRow; //last value in board
        }
    }
}

void Game::rotateCol(int RowColNum, int numRotate) { //function to rotate col
    for (int i = 0; i < abs(numRotate); i++) { //num times that the user wants col to rotate
        if (numRotate > 0) { //rotate up
            char tempCol = board[area - gameSize + RowColNum]; //takes the bottom value
            for (int j = 0; j < gameSize - 1; j++) { //for every index value
                board[area - (gameSize * (j + 1)) + RowColNum] = board[(area) - (gameSize * (j + 2)) + RowColNum]; //assigns bottom to top
            }
            board[RowColNum] = tempCol; //assigns the top value to bottom value
        } else if (numRotate < 0) { //rotate down
            char tempCol = board[RowColNum]; //takes the top value
            for (int k = 0; k < gameSize - 1; k++) { //for every index value
                board[RowColNum + (gameSize * k)] = board[RowColNum + (gameSize * (k + 1))]; //assigns top to bottom
            }
            board[area- gameSize + RowColNum] = tempCol; //assigns the bottom value to the top
        }
    }
}

void fileExtract(vector <string> &file, string fileName) { //takes file and puts it in a vector
    ifstream inStream; //creating name for instance
    inStream.open(fileName);
    assert(inStream.fail() == false);
    file.clear(); //clearing previous values out of vector for a clean list
    string word; //temporary value that each dictionary word will be assigned to
    while (inStream >> word) { //pushes dictionary value into word
        file.push_back(word); //adds line to vector
    }
    inStream.close(); //close file
}

void Game::setGameSize() { //sets the size of board based on user input
    cout << endl << "Choose your board size (must be a number greater than or equal to 4): ";
    cin >> gameSize; //userInput
    if (gameSize < 4) { //checks if gameSize is less than 4
        cout << endl << "The board size must be at least 4. Retry.";
        setGameSize(); //another attempt
    }
    area = gameSize * gameSize; //area of square board Area = A*A
}

void Game::setScrambleSize() { //sets the number of times that random rows and columns of the board will rotate before user display
    cout << endl << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1): ";
    cin >> numScramble; //userInput
    if (numScramble < 1) { //checks if numScramble is less than 1
        cout << endl << "The number of times you select must be at least 1. Retry.";
        setScrambleSize(); //another attempt
    }
}

void Game::defaultBoard(int area) { //assigns board arrays to char ' '
    for (int i = 0; i < area; i++) { //for every index value
        board[i] = ' ';
    }
}

void Game::correctVersion(int area){ //assigns complete array to board array before scramble or game functions
    for (int i = 0; i < area; i++) { //for every index value
        complete[i] = board[i];
    }
}

void Game::arrayInitialization(int area) { //initialize arrays from the max values an array can carry to the area size
    board[0] = board[area];
    complete[0] = complete[area];
    restart[0] = complete[area];
}

void Game::boardAssign(vector <string> file, int numSpaces) { //Assign values to each board index
    int count = 0; //iterating values during value assignments to prevent overlap
    while (numSpaces > 2) { //while there is greater than two spaces left on the board
        int index = rand() % file.size(); //multiplication of hardcoded file size
        if (file.at(index).size() > numSpaces) { //if the size of the file's index value is greater than the number of spaces remaining
            continue; //restart to the top of the while loop
        } else { //when above isn't the case
            string sampleWord = file.at(index); //assign the files index value to a string
            for (int i = 0; i < sampleWord.size(); i++) { //for each character in the string
                board[count] = sampleWord.at(i); //push_back character's value to the board
                count++; //iteration of count var
            }
            numSpaces -= sampleWord.size(); //subtract total number of spaces from the size of file
            if (numSpaces != 0) { //if there is no remaining spaces
                numSpaces--; //decrease numSpace by 1
                board[count] = ' '; //assign char ' ' to board array value
                count++; //iterate count
            }
        }
    }
    if (numSpaces > 0) { //for the remaining numSpace values
        for (int j = count; j < numSpaces; j++) { //iterate through each value
            board[j] = ' '; //assign board index value to char ' '
        }
    }
}

void Game::scrambleBoard(int gameSize, int area, int numScramble) { //scrambles rand row and col
    for (int i = 0; i < numScramble; i++) { //for each scramble
        int randRow = rand() % gameSize;
        int randCol = rand() % gameSize;
        char tempRow = board[(randRow + 1) * gameSize - 1]; //finds the bottom index in row
        for (int k = 0; k < gameSize; k++) { //for each index value in the Row
            board[(randRow + 1) * gameSize - 1 - k] = board[(randRow + 1) * gameSize - 2 - k]; //all values down
        }
        board[gameSize * randRow] = tempRow; //finds top index in row
        char tempCol = board[((area - gameSize) + randCol)];
        for (int k = 0; k < gameSize - 1; k++) { //for each value in Col
            board[area - (gameSize * (k+1)) + randCol] = board[area - (gameSize * (k+2)) + randCol]; //all values right
        }
        board[randCol] = tempCol; //changes values for leftmost index
    }
    for (int i = 0; i < area; i++) {
        restart[i] = board[i]; //assigns the restart to this board
    }
}

void Game::displayBoard(int gameSize, int area) { //display board functions
    cout << endl << "These are the words that you should try to spell out using the board, in order:" << endl << "   ";
    for (int i = 0; i < area; i++) {
        if (area - i < 4 && complete[i] == ' ') {
            break;
        }
        if (complete[i] == ' ') {
            cout << ", ";
        } else {
            cout << complete[i];
        }
    }
    cout << "." << endl << endl << "Current board:" << endl << "     ";
    for (int i = 0; i < gameSize; i++) {
        cout << "  " << i << " ";
    }
    cout << endl << "      ";
    for (int i = 0; i < gameSize * 4 - 1; i++) {
        cout << "-";
    }
    for (int i = 0; i < gameSize; i++) {
        cout << endl << "   " << i << " | ";
        for (int j = 0; j < gameSize; j++) {
            cout << board[j + i * gameSize] << " | ";
        }
        cout << endl << "      ";
        for (int j = 0; j < (gameSize * 4) - 1; j++) {
            cout << "-";
        }
    }
    cout << endl << "Current board words: ";
    for (int i = 0; i < area; i++) {
        cout << board[i];
    }
    cout << endl;
}

void Game::Menu() { //main manipulation menu
    cout << endl << "Enter one of the following: " << endl;
    cout << "   R to rotate a row or column," << endl;
    cout << "   C to view what the completed board should look like," << endl;
    cout << "   B to reset the board back to the beginning," << endl;
    cout << "   G to generate a new board with new random words," << endl;
    cout << "   S to have the computer automatically solve a board that you enter (5 points extra credit)," << endl;
    cout << "   Q to quit." << endl;
    cout << "Your choice: ";
}

void Game::subMenu() { //Rotate menu
    cout << "   Enter the row/column you would like to rotate, and the number of positions to rotate by." << endl;
    cout << "   This should be in the format of <R or C> <row/column number> <number of positions to rotate>," << endl;
    cout << "   where valid row and column numbers are between 0 and " << gameSize - 1 << ", " << endl;
    cout << "   E.g. R 0 1 would rotate the top row (row 0) of the board to the right once, " << endl;
    cout << "        c 1 -2 would rotate the second column (col 1) of the board upwards twice." << endl;
    cout << "   Your choice:";
    char RowOrCol; //userInput
    int RowColNum; //userInput
    int numRotate; //userInput
    cin >> RowOrCol >> RowColNum >> numRotate; //statement to take all above functions
    RowOrCol = tolower(RowOrCol); //changes character to lowercase
    if (RowOrCol == 'r') {
        if (RowColNum < 0 || RowColNum > gameSize - 1) { //makes sure that the rowNumber is within bounds
            cout << endl << "   Number must be between 0 and " << gameSize - 1 <<". Try again.";
            subMenu(); //another try
        } else { 
            rotateRow(RowColNum, numRotate); //if userInput satisfies
        }
    } else if (RowOrCol == 'c') {
        if (RowColNum < 0 || RowColNum > gameSize - 1) { //makes sure that the colNumber is within bounds
            cout << endl << "   Number must be between 0 and " << gameSize - 1 <<". Try again.";
            subMenu(); //another try
        } else {
            rotateCol(RowColNum, numRotate); //if userInput satisfies
        }
    } else if (RowOrCol != 'c' || RowOrCol != 'r') { //if the userInput is invalid
        cout << endl << "   First input must be 'R' or 'C'. Try again." << endl;
        subMenu(); //another try
    }
}

void Game::Winners( int area) {
    string sample1, sample2; //two random strings
    for (int i = 0; i < area; i++) { //for the values in the board
        sample1 = sample1 + board[i]; //creates the current board index values into a string
        sample2 = sample2 + complete[i]; //creates the complete board index values into a string
    }
    if (sample1 == sample2) { //if both above strings are the same

        cout << endl << "      ";
        for (int i = 0; i < gameSize; i++) {
            cout << "  " << i << " ";
        }
        cout << endl << "      ";
        for (int i = 0; i < gameSize * 4 - 1; i++) {
            cout << "-";
        }
        for (int i = 0; i < gameSize; i++) {
            cout << endl << "   " << i << " | ";
            for (int j = 0; j < gameSize; j++) {
                cout << complete[j + i * gameSize] << " | ";
            }
            cout << endl << "      ";
            for (int j = 0; j < (gameSize * 4) - 1; j++) {
                cout << "-";
            }
        }
        cout << endl << "Current board words: ";
        for (int i = 0; i < area; i++) {
            cout << complete[i]; //prints characters
        }
        cout << endl << "Congratulations, you won! Thank you for playing!" << endl << "Exiting program..." << endl;
        exit(0); //exit function
    }
}

void Game::finishedBoard() { //displays the final board
    cout << endl << "The completed board should look like:" << endl << "     ";
    for (int i = 0; i < gameSize; i++) {
        cout << "  " << i << " ";
    }
    cout << endl << "      ";
    for (int i = 0; i < gameSize * 4 - 1; i++) {
        cout << "-";
    }
    for (int i = 0; i < gameSize; i++) {
        cout << endl << "   " << i << " | ";
        for (int j = 0; j < gameSize; j++) {
            cout << complete[j + i * gameSize] << " | ";
        }
        cout << endl << "      ";
        for (int j = 0; j < (gameSize * 4) - 1; j++) {
            cout << "-";
        }
    }
    cout << endl << "Current board words: ";
    for (int i = 0; i < area; i++) {
        cout << complete[i]; //prints characters
    }
}

void Game::reset(int area) { //changes each board to the post scrambled board
    for (int i = 0; i < area; i++) { //accounts for the total number of spaces in the board
        board[i] = restart[i]; //changes the board array to the reset array
    }
}

void completeGame() { //all game functions
    Game User;
    vector <string> file; //file vector storing words in the dictionary
    string fileName = "dictionary-3to5-letters.txt";
    fileExtract(file, fileName);
    User.setGameSize();
    User.defaultBoard(User.gameSize);
    User.arrayInitialization(User.area);
    User.boardAssign(file, User.area);
    User.setScrambleSize();
    User.correctVersion(User.area);
    User.scrambleBoard(User.gameSize, User.area, User.numScramble);
    User.displayBoard(User.gameSize, User.area);
    char userInput;
    while (true) {
        User.Menu();//runs main menu message
        cin >> userInput; //UserInput
        userInput = tolower(userInput); //lowercase userInput
        if (userInput == 'r') {
            User.subMenu(); //subMenu for function
        } else if (userInput == 'c') { //shows the completed board
            User.finishedBoard();
        } else if (userInput == 'b') { //resets the board
            User.reset(User.area);
        } else if (userInput == 'g') { //resets the letters
            User.defaultBoard(User.area);
            User.boardAssign(file, User.area);
            User.correctVersion(User.area);
            User.scrambleBoard(User.gameSize, User.area, User.numScramble);
        } else if (userInput == 'q') {
            cout << "Thank you for playing!" << endl << "Exiting program..." << endl;
            exit(0);
        } else {
            cout << endl << "You did not enter one of the valid options. Try again.";
            continue;
        }
        User.Winners(User.area);
        User.displayBoard(User.gameSize, User.area);
    }
}

void starter(){ //starter function
    cout << "Enter Q to quit or B to begin:";
    char userInput;
    cin >> userInput;
    userInput = tolower(userInput); //changes the char to the lowercase
    if(userInput == 'b') {
        completeGame(); //function containing the complete game
        cout << "Congratulations, you won! Thank you for playing!" << endl << "Exiting program..." << endl;
        exit(0);
    }
    else if(userInput == 'q'){
        cout << "Thank you for playing!" << endl << "Exiting program..." << endl;
        exit(0);
    }
    else{
        starter();
    }
}

int main() {
    srand(1); //seeded rand
    PrintIntro(); //Prints the Introduction message
    starter(); // contains the first menu option
    return 0;
}
