#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <sstream>
#include <streambuf>
#include <cstring>
#include <cassert>

using namespace std;

int render(int lineLength, istream& inf, ostream& outf);
vector<char> stringGetter(istream& inf, int remainingLength, bool& newLine, int lineLength, bool& returnOne, bool& hasPunct, bool& hasHyphen);
void printLineLength(ostream& outf, int lineLength);

void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}

int main() {
    std::ifstream inputFile("final input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1; // Return an error code
    }

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1; // Return an error code
    }

//    cout << "  This is a test  \n" << endl;
//    cout << "\n\n" << "This is\na test\n";

    render(40, inputFile, outputFile);

    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
    testRender(8, "  This is a test  \n", "This is\na test\n", 0);
    testRender(6, "Testing it\n", "Testin\ng it\n", 1);
    testRender(-5, "irrelevant", "irrelevant", 2);
    cerr << "Tests complete" << endl;

//REMEMBER TO CHANGE _ TO SPACE!!!!!!!!!

    return 0;
}

int render(int lineLength, istream& inf, ostream& outf)
{
    if (lineLength < 1)
        return 2;

    vector<char> output;
    int remainingLength = lineLength; //keeps track of how many more characters are available in the line
    char c;
    bool newLine; //checks if a new line needs to be added, set to true if the word exceeds the maximum character limit
    bool returnOne = false; //used to keep track of if we need to return one or not
    bool firstWord = true; //keeps track of the first word so that we can input a space after it
    bool hasPunct = false; //checks if the last char of a word has a punctuation, if it does we can output another space
    bool skipLine = false; //tells the rest of the function to not do anything because we are skipping a line due to @P@
    bool hasSkipped = false; //checks if the program has skipped a line or not
    bool startingWord = true; //fix problem of the program skipping the first line if the starting word is too long
    bool hasHyphen = false;

    while (inf.get(c)) //keep in mind this takes a char at the start so need to like code some stuff for that yea
    {
        inf.unget(); //undo the first .get because we are going to use while(inf.get) again in the stringGetter function
        //so this way we make sure we dont take in an extra char
        skipLine = false; //reset the boolean in case we need to run new string

        output = stringGetter(inf, remainingLength, newLine, lineLength, returnOne, hasPunct, hasHyphen);

        if (output.size() == 3)
        {
            if (output[0] == '@' && output[1] == 'P' && output[2] == '@' && hasSkipped == false) { //checks if we have skipped
                // a line before, if not then we skip a line (new paragraph)
                startingWord = false; //sets starting word to false to prevent bug
                skipLine = true; //set skip line to true so rest of the program does not run
                hasSkipped = true; //set hasskipped to true so we dont make another paragraph jump
                outf << '\n' << '\n'; //skip paragraph
                if (hasPunct == true) //haspunct set to false since we are going to start a new line
                    hasPunct = false;
                if (firstWord == false) // firstword set to true because the next word will be firstword
                    firstWord = true;
                remainingLength = lineLength; //reset remainingLength
            }
            else if (output[0] == '@' && output[1] == 'P' && output[2] == '@' && hasSkipped == true) {  //case where we have
                //already skipped using @P@, in which case we jsut consume this set of char and skip the line again
                skipLine = true; //set the skipline boolean to true again so we dont have any random outputs
                output.clear();
            }

        }

        if (!newLine && hasPunct && !skipLine && output.size() != 0) { //this is to check if the string before had a punctuation at the end
            // and if the next string after it (the one we currently have) can be fit into the same line. if it can fit,
            // we add an additional space, reduce remainingLength, and we set haspunct to false.
            outf << ' ';
            hasPunct = false;
            remainingLength--;
        }

        if (!newLine && output.size() < remainingLength && !skipLine && output.size() != 0) //check if the string can be fit on the line
        {
            startingWord = false;
            if (!firstWord && output.size() != 0) {//this is to check if the string can be fit into the current line and if it is not the first word
                //in the line. if it is not the first word, we put a space before it. this prevents the situation where we add a string to the end
                // of the line by accident
                if (!hasHyphen) {
                    outf << ' ';
                    remainingLength--;
                    hasHyphen = false;
                }
            }
            else if (output.size() != 0)
                firstWord = false; //if it is the first word, then we dont add any space before it since it is the first word
                // and we set firstword to false

            for (int i = 0; i < output.size(); i++)
            {
                outf << output[i];
            }

            if (output.size() > 0) {
                hasSkipped = false; //only resets the skip boolean if the program actually outputted another word after @P@
                if (output[output.size() - 1] == '.' || output[output.size() - 1] == '?'
                || output[output.size() - 1] == '!' || output[output.size() - 1] == ':') //this is to check if the last char
                    // of the string was a punctuation
                    hasPunct = true;
                else
                    hasPunct = false;
            }

            remainingLength -= output.size(); //update remainingLength
        }

        else if (!newLine && hasHyphen && !skipLine && output.size() <= remainingLength && output.size() != 0) //basically here just to solve a bug where words with
            //hyphen doesnt get outputed on the same line if the output.size = remainingLength
            //this is only here to take consideration of that case
        {
            startingWord = false;
            if (!firstWord && output.size() != 0) {//this is to check if the string can be fit into the current line and if it is not the first word
                //in the line. if it is not the first word, we put a space before it. this prevents the situation where we add a string to the end
                // of the line by accident
                if (!hasHyphen) {
                    outf << ' ';
                    remainingLength--;
                    hasHyphen = false;
                }
            }
            else if (output.size() != 0)
                firstWord = false; //if it is the first word, then we dont add any space before it since it is the first word
            // and we set firstword to false

            for (int i = 0; i < output.size(); i++)
            {
                outf << output[i];
            }

            if (output.size() > 0) {
                hasSkipped = false; //only resets the skip boolean if the program actually outputted another word after @P@
                if (output[output.size() - 1] == '.' || output[output.size() - 1] == '?'
                    || output[output.size() - 1] == '!' || output[output.size() - 1] == ':') //this is to check if the last char
                    // of the string was a punctuation
                    hasPunct = true;
                else
                    hasPunct = false;
            }

            remainingLength -= output.size(); //update remainingLength
        }

        else if (output.size() <= lineLength && !skipLine && output.size()!=0) //this is the case where we need to make a new line to put the string in
            // (we ran out of space), but the string is small enough to not require us to return 1
        {
            if (!startingWord && output.size()!= 0) { //just to make sure that it is not the first word of the entire input, so no random
                // \n at the start of the output text file
                remainingLength = lineLength;
                outf << '\n';
                startingWord = false;
            }
            for (int i = 0; i < output.size(); i++)
                outf << output[i];

            if (output.size() > 0) {
                hasSkipped = false;
                if (output[output.size() - 1] == '.' || output[output.size() - 1] == '?'
                    || output[output.size() - 1] == '!' || output[output.size() - 1] == ':')
                    hasPunct = true;
                else
                    hasPunct = false;
            }

            startingWord = false; //reset some variables
            hasSkipped = false;
            remainingLength -= output.size();
        }

        else if (output.size()!=0)//case where the string is larger than the line
        {
            if (!skipLine) {
                int count = 0; //since the string is going to be larger than the maximum line length, we create a counter
                //to keep track of how many extra characters there was
                if (!startingWord) {
                    returnOne = true;
                    if (!hasSkipped)
                        outf << '\n';
                    startingWord = false;
                }
                for (int i = 0; i < lineLength; i++)
                    outf << output[i];
                startingWord = false;
                outf << '\n'; //this is to put the rest of the chars onto another line
                for (int i = lineLength; i < output.size(); i++) {
                    outf << output[i];
                    count++;
                }

                if (output.size() > 0) {
                    hasSkipped = false;
                    if (output[output.size() - 1] == '.' || output[output.size() - 1] == '?'
                        || output[output.size() - 1] == '!' || output[output.size() - 1] == ':')
                        hasPunct = true;
                    else
                        hasPunct = false;
                    firstWord = false;
                }

                remainingLength = lineLength - count; //update the remainingLength using the counter variable
            }
        }
    }

    outf << '\n';

    if (returnOne)
        return 1;
    else
        return 0;
}

vector<char> stringGetter(istream& inf, int remainingLength, bool& newLine, int lineLength, bool& returnOne, bool& hasPunct, bool& hasHyphen)
// function to get the next string
{
    newLine = false;
    vector<char> result; //vector to store the string because this is easier to deal with than having to worry about
    // array going out of bounds
    char c;
    hasHyphen = false;

    while(inf.get(c))
    {
        if (!isspace(c) && c != '\n') //check to make sure that the char we take in is not a space
        {
            result.push_back(c); //add to vector
            if (c == '-') {//if the char is a hyphen that would be the end of a word portion so we jsut prematurely end the loop
                hasHyphen = true;
                break;
            }
        }
        else
            break;
    }

    if (hasPunct) { // we take in the hasPunct boolean from the previous string and use it to check if we can fit an extra
        // space and the word into the current line. if we can not we set newLine to true
        if ((int) result.size() >= (remainingLength - 1))
            newLine = true;
    }
    else //if the previous string does not have a punctuation we just check using remaining length
        if (result.size() > remainingLength)
            newLine = true;

    if (result.size() > lineLength) //this is to check if the string is larger than the maximum length
        returnOne = true;

    return result;

}

void printLineLength(ostream& outf, int lineLength) //just a command for me to check if the output was meeting the maximum
// char size requirement. (its kinda hard to tell if it matches requirement when the char length is >10)
{
    for (int i = 0; i < lineLength; i++)
    {
        if (i != lineLength)
            outf << ' ';
    }
    outf << '|' << lineLength << endl;
}