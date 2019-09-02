#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;

/* You may assume that each enemy has no more than twenty lines.
 * Error messages:
 * go to standard error.
 * include the program name,
 * no matter how it was compiled.
 * include the offending data,
 * if applicable
 * Produce an error message and stop the program
 * if: the key is not alphabetic
 * the key or value is missing
 * duplicate keys are encountered in an enemy
 * The output must end with a newline.
 * Newlines do not separate lines—newlines terminate lines.
 * Creativity is a wonderful thing, but your output format is not the place for it.
 * Your output should look exactly like the output shown above.
 * UPPERCASE/lowercase matters.
 * Spaces matter.
 * Blank lines matter.
 * Extra output matters. 
 */

// print the error message and exit the program with a non-0 return code
static void printError(const string& errorMessage) {
    // this would be MUCH cleaner with exceptions
    cout << errorMessage << endl;
    exit(-1);
}

// yup.  whitespace.  Is a global constant a global variable?
static const string whitespaceChars = " \t";

// a key/value
class EnemyAttribute {
public:
    void setKey(const string& newKey);
    void setValue(const string& newValue);
    const string &getKey();
    const string &getValue();

private:
    string key;
    string value;
};

void EnemyAttribute::setKey(const string& newKey) {
    key = newKey;
}

void EnemyAttribute::setValue(const string &newValue) {
    value = newValue;
}

const string &EnemyAttribute::getKey() {
    return key;
}

const string &EnemyAttribute::getValue() {
    return value;
}

// a collection of key/value pairs and logic to parse and validation logic
class Enemy {
public:
    void addAttribute(const string& attributeText);
    bool hasAttributes();
    void dumpToCout();
    void clearAllAttributes();

private:
    int findAttribute(const string& key);
    void printWhiteSpace(int count);

    static const string validKeyChars;

    int attributeCount = 0;
    EnemyAttribute attributes[20];
};

const string Enemy::validKeyChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void Enemy::addAttribute(const string &attributeText) {
    string line(attributeText);

    //parse the key
    std::size_t found = line.find_first_not_of(validKeyChars);
    if (found == std::string::npos) {
        printError(string("Line only contains key: ") + line);
    } else if (found == 0) {
        // first character is not alphabetic
        printError(string("Key contains illegal character: ") + line[0]);
    }
    string key = line.substr(0, found);
    line.erase(0, found);

    if (findAttribute(key) >=0) {
        printError(string("duplicate key: ") + key);
    }

    // parse white space separator
    found = line.find_first_not_of(whitespaceChars);
    if (found == std::string::npos) {
        printError("Line missing value: " + attributeText);
    } else if (found == 0) {
        // char following key is not a space
        printError(string("Key contains illegal character: ") + line[0]);
    } else {
        line.erase(0, found);
    }

    // parse value
    if (line.length() == 0) {
        // can this happen?
        printError("Line missing value: " + attributeText);
    }
    string value = line;

    attributes[attributeCount].setKey(key);
    attributes[attributeCount].setValue(value);
    attributeCount++;
}

bool Enemy::hasAttributes() {
    return attributeCount > 0;
}

void Enemy::dumpToCout() {
    // figure out how long the longest key is so we can line up the values column 
    int maxKeyLength = 0;
    for (int i = 0; i < attributeCount; i++) {
        int keyLength = attributes[i].getKey().length();
        maxKeyLength = (keyLength > maxKeyLength) ? keyLength : maxKeyLength;
    }
    // for each line, print the key, a variable number of spaces, and the value followed by a newline
    for (int i = 0; i < attributeCount; i++) {
        const string &key = attributes[i].getKey();
        cout << key;
        printWhiteSpace(maxKeyLength - key.length() + 1);
        cout << attributes[i].getValue() << '\n';
    }
}

void Enemy::clearAllAttributes() {
    attributeCount = 0;
    // could actually clear the key/value fields if desired
}

int Enemy::findAttribute(const string &key) {
    for (int i = 0; i < attributeCount; i++) {
        if (attributes[i].getKey() == key) {
            return i;
        }
    }
    return -1;
}

void Enemy::printWhiteSpace(int count) {
    for (int i = 0; i < count; i++) {
        cout << ' ';
    }
}

// main function cycles through std::in until input is all gone, parses and prints enemy lists
int main() {
    bool precedeEnemyWithBlankLine = false;
    Enemy enemy;

    string line;
    while (getline(std::cin, line)) {
        //strip trailing whitespace
        std::size_t found = line.find_last_not_of(whitespaceChars);
        if (found == string::npos) {
            // empty line or all whitespace.
            if (precedeEnemyWithBlankLine) {
                cout << "\n";
            } else {
                precedeEnemyWithBlankLine = true;
            }
            enemy.dumpToCout();
            enemy.clearAllAttributes();
        } else {
            // erase the trailing whitespace 
            line.erase(found + 1);
            // parse the remaining text 
            enemy.addAttribute(line);
        }
    }

    // we hit the end of file.  Dump any enemy we were working on.
    if (enemy.hasAttributes()) {
        if (precedeEnemyWithBlankLine) {
            cout << '\n';
        }
        enemy.dumpToCout();
    }

    cout.flush();
    return 0;
}
