#include "timsort.h"
#include <vector>

int main(){
    // Note, that in text file all feedbacks are storing in format: rate|text|date;(| - delimiter)
    auto res = timSort("feedback.txt");
    return 0;
}