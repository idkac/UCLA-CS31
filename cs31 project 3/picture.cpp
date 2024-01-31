
#include <iostream>
#include "grid.h"
#include <cassert>
#include <string>

using namespace std;
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
int commandGetter(string input);

int main() {

    const int HORIZ = 0; //horizontal line
    const int VERT = 1; //vertical line

    const int FG = 0; //replaces all spaces its in regardless of if theres a line or not
    const int BG = 1; //replaces spaces but not characters

//    setSize(12, 15);
//    assert(plotLine(3, 5, 2, HORIZ, '@', FG));
//    assert(plotLine(3,5,0,VERT,'@',FG));
//    for (int c = 5; c <= 7; c++)
//        assert(getChar(3, c) == '@');
//    assert(getChar(3, 8) == ' ');
//    clearGrid();
//    char pc = '%';
//    int m = FG;
//    int bad = 999;
//    // A successful command string should not change bad
//    assert(performCommands("V2", pc, m, bad) == 0  &&  getChar(3, 1) == '%'  &&  bad == 999);
//    assert(performCommands("V2H2Q2", pc, m, bad) == 1  &&  bad == 4);
//    assert(performCommands("H4V3V-1H-9", pc, m, bad) == 3  &&  bad == 7);
//    cout << "All tests succeeded." << endl;

//
//    for (;;)
//    {
//        cout << "Enter the number of grid rows and columns (max 30 each): ";
//        int nRows;
//        int nCols;
//        cin >> nRows >> nCols;
//        cin.ignore(10000, '\n');
//        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
//        {
//            setSize(nRows, nCols);
//            break;
//        }
//        cout << "The numbers must be between 1 and 30." << endl;
//    }
//    char currentChar = '*';
//    int currentMode = FG;
//    for (;;)
//    {
//        cout << "Enter a command string (empty line to quit): ";
//        string cmd;
//        getline(cin, cmd);
//        if (cmd == "")
//            break;
//        int bruh = 0;
//        int status = performCommands(cmd, currentChar, currentMode, bruh);
//        switch (status)
//        {
//            case 0:
//                draw();
//                break;
//            case 1:
//                cout << "Syntax error at position " << bruh << endl;
//                break;
//            case 2:
//                if (!isprint(currentChar))
//                    cout << "Current character is not printable" << endl;
//                if (currentMode != FG  &&  currentMode != BG)
//                    cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
//                break;
//            case 3:
//                cout << "Cannot perform command at position " << bruh << endl;
//                break;
//            default:
//                // It should be impossible to get here.
//                cout << "performCommands returned " << status << "!" << endl;
//        }
//    }

    setSize(5,10);
    if (plotLine(3, 5, 2, HORIZ, '@', 1))
        cout << "true" << endl;
    else
        cout << "false" << endl;

    draw();

    return 0;
}

// starts at 1,1
// returns 2 if !isprint plotChar, or if mode is not 1 or 2
// if the commandstring works then it returns 0
// if the command doesnt work, return 1 and set badpos to the point where there was a syntax error
// types of commands
//  1. horizontal, both h, up to 3 character, if the command doesnt work then doesnt plot
//  2. vertical, both v, same with h
//  3. foreground, upper lower f, followed by one character, sets to foreground mode
//  4. background, same as b
//  5. clear command, only one letter, clears everything, character to *, position to 1,1
//    const int HORIZ = 0; //horizontal line
//    const int VERT = 1; //vertical line
//
//    const int FG = 0; //replaces all spaces its in regardless of if theres a line or not
//    const int BG = 1; //replaces spaces but not characters

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    if (!isprint(plotChar) || (mode != 1 && mode != 0) ){  // checks if the command has right characters or if the mode is right
        return 2;
    }

    else { //runs the command

        bool syntaxError = false;  //tracks the error for syntax error and outofbounds error, since syntax error is given precedence over out of bounds
        bool outOfBounds = false;

        int outOfBoundsPos;

        int r = 1;
        int c = 1;
        int position = 0; //this will keep track of the position in the commands
//        badPos = 0;

        while (commandString.length() > 0) {

            bool hasNegative = false;

            string command = commandString.substr(0, 1);

            if (command != "H" && command != "h" && command != "v" && command != "V" && command != "f" &&  //checks if the first letter is a command
                command != "F" &&
                command != "b" && command != "B" && command != "c" && command != "C") {
                badPos = position;
                clearGrid(); //cleargrid so that if we plotted anything earlier it does not happen
                return 1; //if it is we check which command it is but if it isnt we just return the position because syntax error
            }


            else {
                if (command == "H" || command == "h") {  //code for horizontal movement

                    string check = commandString.substr(1, commandGetter(commandString.substr(1, commandString.length()-1)));  //gets the characters after
                    //the command until the next command comes in

                    for (int i = 1; i < check.length() + 1; i++) { //checks if the characters we got all numbers
                        if (check[0] == '-') { //ignore the first number if it is negative
                            hasNegative = true;
                        }
                        if (hasNegative == true) //if the first character is negative then we ignore it and do this loop
                        {
                            if (i == check.length()) //since we have our bound as check.length + 1, we stop the code once it hits check.length
                                // so no weird error is produced
                            {}
                            else if(!isdigit(check[i])) { //if there is a nondigit in the middle we output error
                                badPos = position + i + 1;
                                syntaxError = true;
                                clearGrid();
                                return 1;
                            }
                        }
                        else //this is the case where the number is positive, we start at 0 here
                        {
                            if (!isdigit(check[i-1]))
                            {
                                badPos = position + i;
                                syntaxError = true;
                                clearGrid();
                                return 1;
                            }
                        }
                    }

                    if (check.length() == 0) //if there is no numbers behind the command we output a syntax error
                    {
                        badPos = position + 1;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    int distance = stoi(check); //this convert the characters into integer so we can use it for distance
                    if (distance > 99) { // case where the distance is 3 or more characters for positive
                        badPos = position + 3; //outputs the last digit
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }
                    else if (distance < -99) { //case where the distance is 4 or more characters for negative
                        badPos = position + 4;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else {
                        if (!plotLine(r, c, distance, 0, plotChar, mode)) { //just checks if the command can run
                            if (outOfBounds == false && syntaxError == false) { //this checks the command goes out of boudns
                                //if it does then we set outofbounds to true and store the position, this way we can still output
                                //badpos if there is a syntax error, but if there isnt, we can output the stored outofboundpos
                                outOfBoundsPos = position;
                                outOfBounds = true;
                            }
                            commandString = commandString.erase(0, check.length() + 1); //this removes the code that had the out of bounds error
                            position += check.length() + 1;
                        }

                        else {
                            commandString = commandString.erase(0, check.length() + 1); //command to plot the line if there was no errors
                            plotLine(r, c, distance, 0, plotChar, mode);
                            position += check.length() + 1;
                            c += distance;
                        }
                    }
                }


                else if (command == "v" || command == "V") { //basically the same as the horizontal command
                    string check = commandString.substr(1, commandGetter(commandString.substr(1, commandString.length()-1)));  //gets the characters after
                    //the command until the next command comes in

                    for (int i = 1; i < check.length() + 1; i++) { //checks if the characters we got all numbers
                        if (check[0] == '-') { //ignore the first number if it is negative
                            hasNegative = true;
                        }
                        if (hasNegative == true) //if the first character is negative then we ignore it and do this loop
                        {
                            if (i == check.length()) //since we have our bound as check.length + 1, we stop the code once it hits check.length
                                // so no weird error is produced
                            {}
                            else if(!isdigit(check[i])) { //if there is a nondigit in the middle we output error
                                badPos = position + i + 1;
                                syntaxError = true;
                                clearGrid();
                                return 1;
                            }
                        }
                        else //this is the case where the number is positive, we start at 0 here
                        {
                            if (!isdigit(check[i-1]))
                            {
                                badPos = position + i;
                                syntaxError = true;
                                clearGrid();
                                return 1;
                            }
                        }
                    }

                    if (check.length() == 0)
                    {
                        badPos = position + 1;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    int distance = stoi(check);
                    if (distance > 99) {
                        badPos = position + 3;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }
                    if (distance < -99) {
                        badPos = position + 4;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else {
                        if (!plotLine(r, c, distance, 1, plotChar, mode)) {
                            if (outOfBounds == false && syntaxError == false) {
                                outOfBoundsPos = position;
                                outOfBounds = true;
                            }
                            commandString = commandString.erase(0, check.length() + 1);
                            position += check.length() + 1;
                        }
                        else {
                            commandString = commandString.erase(0, check.length() + 1);
                            plotLine(r, c, distance, 1, plotChar, mode);
                            position += check.length() + 1;
                            r += distance;
                        }
                    }
                }


                else if (command == "f" || command == "F") //command for foreground
                {
                    string check = commandString.substr(1, 1);
                    if (check.length() != 1) { //check to see if there is any character after the command
                        badPos = position + 1;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else if (!isprint(check[0])) { //just makes sure hte character is printable
                        badPos = position + 1;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else {
                        commandString = commandString.erase(0, check.length() + 1); //we rtemove the code and then change the mode as
                        //well as the character
                        position += 2;
                        mode = 0;
                        plotChar = check[0];
                    }
                }


                else if (command == "b" || command == "B") //command for background //basically hte same as teh one for foreground
                {
                    string check = commandString.substr(1, 1);
                    if (check.length() != 1) {
                        badPos = position + 1; //checks if there is only one character after the command
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else if (!isprint(check[0])) {
                        badPos = position + 1;
                        syntaxError = true;
                        clearGrid();
                        return 1;
                    }

                    else {
                        commandString = commandString.erase(0, check.length() + 1);
                        position +=  2;
                        mode = 1; //sets mode to background and then changes the plotChar to the char
                        plotChar = check[0];
                    }
                }


                else if (command == "c" || command == "C") //command for clear
                {
                    commandString = commandString.erase(0, 1);
                    position += 1;
                    plotChar = '*';
                    r = 1;
                    c = 1;
                    clearGrid();
                }
            }
        }


        if (syntaxError == false && outOfBounds == true) //checks that there was no syntax error and taht there was a out of bounds error
            //if there is we output out of bounds and the right error number
        {
            badPos = outOfBoundsPos;
            clearGrid();
            return 3;
        }
    }


    return 0;
}

int commandGetter(string input)  //functiuon to get 3 characters after the command
{
    int output;
    input = input.substr(0,3);
    for (int i = 0; i < 3; i++) {
        string temp = input.substr(i, 1); //this lets us take in the nessesary letters after the command and stops counting once we hit another command
        if (temp == "H" || temp == "h" || temp == "v" || temp == "V" || temp == "f" ||
            temp == "F" || temp == "b" || temp == "B" || temp == "c" || temp == "C"){
            output = i;
            break;
        }
        else if (i == input.length() - 1 || input.length() == 0) //this happens in teh case where the command reaches the end or if there are no letters after
            //the command
        {
            output = input.length();
            break;
        }
    }

    return output;
}

//(r,c)  is the starting point, distance is distance to endpoint //basically jsut (y,x)
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) //plotchar has to be part of the isprint section
{
    const int ROWLENGTH = getRows();
    const int COLLENGTH = getCols();

    if (!isprint(plotChar)) {
        return false;
    }

    if (fgbg != 0 && fgbg != 1)
        return false;

    if (fgbg == 1) { //case where only replaces space
        if (dir == 0) //case where the line is horizontal
        {
            if (distance < 0) //line goes left or upward // goes left in this case
            {
                if (c + distance < 0) // runs if the line goes past 0 and goes into the negatives
                    return false;
                else {
                    for (int i = c; i >= c + distance; i--) {
                        if(getChar(r, i) == ' ')//since this is for background, the setChar command will only run if the
                            //character there is a space
                            setChar(r, i, plotChar);
                    }
                    return true;
                }
            }
            if (distance > 0) //line goes right or downward // goes right in this case
            {
                if (c + distance > COLLENGTH) //checks if line will go past boundaries
                    return false;
                else {
                    for  (int i = c; i <= c + distance; i++) { //for loop to replace all the characters
                        if(getChar(r, i) == ' ')
                            setChar(r, i, plotChar);
                    }
                    return true;
                }
            }
            else {
                if (getChar(r, c) == ' ')
                    setChar(r, c, plotChar);
                return true;
            }
        }
        else //case where the line is vertical
        {
            if (distance < 0) //line goes left or upward // goes upward in this case
            {
                if (r + distance < 0) // runs if the line goes past 0 and goes into the negatives
                    return false;
                else {
                    for (int i = r; i >= r + distance; i--) {
                        if(getChar(i, c) == ' ')
                            setChar(i, c, plotChar);
                    }
                    return true;
                }
            }
            if (distance > 0) //line goes right or downward // goes downward in this case
            {
                if (r + distance > ROWLENGTH) //checks if the line will go past boundaries
                    return false;
                else {
                    for  (int i = r; i <= r + distance; i++) {
                        if(getChar(i, c) == ' ')
                            setChar(i, c, plotChar);
                    }
                    return true;
                }
            }
            else {
                if (getChar(r, c) == ' ')
                    setChar(r, c, plotChar);
                return true;
            }
        }
    }


    else { //the case where all characters will get replaced
        if (dir == 0) //case where the line is horizontal
        {
            if (distance < 0) //line goes left or upward // goes left in this case
            {
                if (c + distance < 0) // runs if the line goes past 0 and goes into the negatives
                    return false;
                else {
                    for (int i = c; i >= c + distance; i--) {
                            setChar(r, i, plotChar);
                    }
                    return true;
                }
            }
            if (distance > 0) //line goes right or downward // goes right in this case
            {
                if (c + distance > COLLENGTH) //checks if line will go past boundaries
                    return false;
                else {
                    for (int i = c; i <= c + distance; i++) { //for loop to replace all the characters
                            setChar(r, i, plotChar);
                    }
                    return true;
                }
            }
            else {
                setChar(r, c, plotChar);
                return true;
            }
        }
        else //case where the line is vertical
        {
            if (distance < 0) //line goes left or upward // goes upward in this case
            {
                if (r + distance < 0) // runs if the line goes past 0 and goes into the negatives
                    return false;
                else {
                    for (int i = r; i >= r + distance; i--) {
                            setChar(i, c, plotChar);
                    }
                    return true;
                }
            }
            if (distance > 0) //line goes right or downward // goes downward in this case
            {
                if (r + distance > ROWLENGTH) //checks if the line will go past boundaries
                    return false;
                else {
                    for (int i = r; i <= r + distance; i++) {
                            setChar(i, c, plotChar);
                    }
                    return true;
                }
            }
            else {
                setChar(r, c, plotChar);
                return true;
            }
        }
    }

}