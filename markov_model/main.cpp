#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>


#define MAX_PREFIXES 10000
#define MAX_TOKENS 10000


using std::string;
using std::ifstream;
using std::stringstream;


void pre_process_data(string &text) {
    // Function to pre-process data(delete whitespaces in words and convert everything to lowercase)
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char c) { return tolower(c); });
    text.erase(
            remove_if(text.begin(), text.end(),
                      [](unsigned char c) { return !(isalpha(c) || isspace(c)); }),
            text.end());
}

void tokenize(const string &text, string tokens[], int &count) {
    stringstream ss(text);
    string token;
    count = 0;
    while (ss >> token) {
        if(count > MAX_TOKENS){ // If out of bounds - break
            break;
        }
        tokens[count++] = token;
    }
}

void buildMarkovModel(string tokens[], int size, int order, string prefixes[],
                      string suffixes[], int &modelSize) {
    modelSize = 0;
    for (int i = 0; i < size - order; i++) {
        string prefix;
        for (int j = 0; j < order; j++) {
            prefix += tokens[i + j] + " ";
        }
        string suffix = tokens[i + order];
        bool found = false;

        for (int k = 0; k < modelSize; k++) {
            if (prefix == prefixes[k]) {
                suffixes[k] += " " + suffix;
                found = true;
                break;
            }
        }
        if (!found) {
            prefixes[modelSize] = prefix;
            suffixes[modelSize] = suffix;
            modelSize++;
        }
    }
}

string generateRecipe(string prefixes[], string suffixes[], int modelSize,
                    int order, int length) {
    srand(time(NULL)); // seed random
    int index = rand() % modelSize;
    string currentPrefix = prefixes[index];
    string currentSuffixes;
    string text = currentPrefix;
    while(length){
        int index = rand() % modelSize;
        currentSuffixes = suffixes[index];
        int whiteSpaceCounter = 1;
        for(char c: currentSuffixes){
            if(c == ' '){
                whiteSpaceCounter++;
            }
        }

        string currentSuffix = currentSuffixes;
        if(whiteSpaceCounter >= 2){
            int random_value = rand() % whiteSpaceCounter;
            stringstream suffixStream(currentSuffixes);
            int flag = 0;
            while(suffixStream.eof()){
                getline(suffixStream, currentSuffix, ' ');
                if(flag == random_value){
                    break;
                }
                flag++;
            }
        }
        text += currentSuffix;
        text += ' ';

        currentPrefix = currentPrefix.substr(currentPrefix.find(' ') + 1);
        currentPrefix += currentSuffix;
        currentPrefix += " ";

        length--;
    }

    return text;
}


int main(){
    string relativePathToTrainingData = "recipes.txt";

    ifstream file(relativePathToTrainingData);
    stringstream buffer;
    buffer << file.rdbuf();

    string training_data = buffer.str();
    pre_process_data(training_data);
    file.close();


    string tokens[MAX_TOKENS];
    int count;
    tokenize(training_data, tokens, count);

    string prefixes[MAX_PREFIXES];
    string suffixes[MAX_PREFIXES];
    int modelSize = 0;
    int order = 4;

    buildMarkovModel(tokens, count, order, prefixes, suffixes, modelSize);

    //Test file data:
    // Gavaian Pizza
    // Fettuccine Alfredo
    // Cheeseburger
    // Hamburger with beef

    int length = 4;
    string generatedRecipe = generateRecipe(prefixes, suffixes, modelSize, order, length);
    std::cout << generatedRecipe;


    return 0;
}

