#include <iostream>
#include <stdexcept>
#include <vector>


using std::string;
using std::cout;


class TextEditor {
private:
    struct Node{
        char data = ' ';
        Node* prev = nullptr;
        Node* next = nullptr;
    };

    Node *head = new Node; //Beginning of the 'file', const
    Node *tail = head; //End of the file

    Node *cursor = head; //Cursor = head; by default
    int len = 0;


public:
    string name;

    static string reverse(const string &str){
        string res;
        for(int i = str.length() - 1; i >= 0; i--){
            res += str[i];
        }
        return res;

    }
    void addText(const string &text) { //By default - move cursor with text
        for (char c: text) {
            Node *newNode = new Node;
            newNode->data = c;
            newNode->next = nullptr;


            if (cursor->next) { //If addText somewhere in the middle - check if there's next node for cursor
                cursor->next->prev = newNode;
                newNode->next = cursor->next;
            }
            if (cursor == head) {
                head->next = newNode;
                newNode->prev = head;
            }
            else {
                newNode->prev = cursor;
                cursor->next = newNode;
            }

            cursor = newNode;
            if (cursor->next == nullptr){ //If noting is after cursor - change tail
                tail = cursor;
            }
            len++;
        }
    }

    string cursorLeft(int n) {
        if(n < 0){
            return "";
        }

        string res;

        while(n > 0 && cursor != head){
            cursor = cursor->prev;
            n--;
        }
        //Continue iteration but without moving cursor:
        int x = 0;
        Node* current = cursor;
        while(x < 10){
            if(current == head){
                break;
            }
            res += current->data;
            current = current->prev;
            x++;
        }
        return reverse(res); //return reversed because moving from right to left
    }


    string cursorRight(int n){
        if(n < 0){
            return "";
        }

        string res;

        while(n > 0 && cursor != tail){
            cursor = cursor->next;
            n--;
        }
        //Continue iteration but without moving cursor:
        int x = 0;
        Node* current = cursor;
        while(x < 10){
            if(current == tail){
                break;
            }
            res += current->data;
            current = current->next;
            x++;
        }
        return res; //return reversed because moving from right to left
    }

    int deleteText(int n){ //Delete text right from cursor(including cursor)
        Node* current = cursor;
        Node* next;
        if(current == head){
            current = current->next;
        }

        Node* tail_copy;
        Node* cursor_copy;
        if(current->prev){
            tail_copy = current->prev;
            cursor_copy = current->prev;
        }

        int x = 0;
        while(n > 0 && current != nullptr){
            if(current == tail){
                tail = tail_copy;
                cursor = tail;
            }
            if(current->prev){
                if(current->next){
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                else {
                    current->prev->next = nullptr;
                }
            }
            next = current->next;
            delete current;
            current = next;
            n--; len--; x++;
        }
        cursor = cursor_copy;
        return x;
    }

    void del(){
        Node* current = head->next;
        Node* next;
        while(current){
            next = current->next;
            delete current;
            current = next;
            len--;
        }
        delete head;
    }

    void print() {
        if (len <= 0){
            return;
        }
        Node *currentNode = head->next;
        while (currentNode != nullptr) {
            cout << currentNode->data;
            currentNode = currentNode->next;
        }
    }
    string getText(){
        if (len <= 0){
            return "";
        }
        string res;
        Node *currentNode = head->next;
        while (currentNode != nullptr) {
            res += currentNode->data;
            currentNode = currentNode->next;
        }
        return res;
    }

    Node* get_cursor(){
        return cursor;
    }

};


std::vector<TextEditor> text_files = {};

string openTextEditor(const string &filename){
    for(TextEditor file: text_files){
        if(file.name == filename){
            return file.getText();
        }
    }
    TextEditor new_file;
    new_file.name = filename;
    text_files.push_back(new_file);
    return "";
}


int main(){
    //Init class object
    TextEditor text_editor;
    text_editor.name = "123";
    text_files.push_back(text_editor);

    //E.g: manipulations with "file"
    text_editor.addText("12345t6");

    text_editor.cursorLeft(10);

    text_editor.deleteText(1) << '\n';


    text_editor.addText("0");

    text_editor.cursorRight(10);
    text_editor.deleteText(2);
    text_editor.cursorLeft(7);

    text_editor.addText("1");
    text_editor.cursorRight(2);
    text_editor.addText("7");

    text_editor.deleteText(2);

    text_editor.cursorLeft(2);
    text_editor.addText("55");
    text_editor.deleteText(2);
    text_editor.cursorRight(4);
    text_editor.addText("55");
    text_editor.deleteText(1);
    text_editor.print();
    //Close the file
    text_editor.del();

    return 0;
}