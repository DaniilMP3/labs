#include <iostream>
#include <fstream>
#include <sstream>




using namespace std;

string get_key(string &str){
    return str.substr(0, str.find(' '));
}



string mapping(const string &key){
    string line;
    ifstream dict("dict.txt");
    while(getline(dict, line)){
        string k = get_key(line);
        if(k == key){
            return line.substr(line.find(' ') + 1);
        }
    }
    return "0";
}


int main(){
    ifstream dict("dict.txt"), text("text.txt");

//    cout << mapping("apple");


    string line, res;
    while(getline(text, line)){
        istringstream stream(line); string key;
        int result = 0;
        while(getline(stream, key, ' ')){
            string str_value = mapping(key);
            result += stoi(str_value);
        }
        cout << result << '\n';


    }


    dict.close();

    return 0;
}