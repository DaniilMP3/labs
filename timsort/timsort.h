#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>


#define RUN_SIZE 32


struct datetime{
    int year = 0;
    int month = 0;
    int day = 0;
};

struct feedback{
    datetime date;
    std::string text;
    int rate;
};


int custom_find(const std::string &str, char character, int n){
    // Find n-th "character" from sequence
    int count = 0;
    for(int i = 0; i < str.length(); i++){
        if(str[i] == character){
            if(count == n){
                return i;
            }
            count++;
        }
    }
    return -1;
}

datetime stringToDatetime(const std::string &str_datetime){
    // String format: YY-MM-DD. Assume that all strings on input are valid
    datetime newDatetime;
    std::vector<int> res(3); // Vector to hold year, month and day

    std::stringstream ss; ss.str(str_datetime);
    std::string item;
    int i = 0;
    while(std::getline(ss, item, '-')){
        res[i] = std::stoi(item); i++;
    }

    newDatetime.year = res[0];
    newDatetime.month = res[1];
    newDatetime.day = res[2];
    return newDatetime;

}


feedback stringToFeedback(const std::string &str_feedback){
    // String format: rate,text,date
    int delim1_index = custom_find(str_feedback, '|', 0);
    int delim2_index = custom_find(str_feedback, '|', 1);

    std::string date_string = str_feedback.substr(delim2_index + 1);
    datetime newDatetime = stringToDatetime(date_string);

    std::string rateString = str_feedback.substr(0, delim1_index);
    int rate = std::stoi(rateString);

    std::string text = str_feedback.substr(delim1_index + 1, delim2_index - delim1_index - 1);
    feedback newFeedback = feedback{newDatetime, text, rate};
    return newFeedback;
}

std::vector<feedback> formFeedbacksVector(const std::string &file_path){
    std::vector<feedback> feedbackVector;
    std::ifstream file(file_path);
    std::string tmp;
    while(std::getline(file, tmp)){
        feedback newFeedback = stringToFeedback(tmp);
        feedbackVector.push_back(newFeedback);
    }
    return feedbackVector;
}


bool firstDateBigger(datetime &date1, datetime &date2){
    // Boolean function which checks if date1 bigger than date2
    if(date1.year != date2.year){
        return date1.year > date2.year;
    }
    else if(date1.month != date2.month){
        return date1.month > date2.month;
    }
    else{
        return date1.day > date2.day;
    }
}


void insertionSort(std::vector<feedback> &arr, int beg, int end){
    for(int i = beg + 1; i <= end; i++){
        feedback tmp = arr[i];
        int j = i - 1;

        while(j >= 0 && firstDateBigger(arr[j].date, tmp.date)){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = tmp;
    }
}


void merge(std::vector<feedback> &arr, int beg, int mid, int end){
    int len1 = mid - beg + 1;
    int len2 = end - mid;
    std::vector<feedback> left_arr(len1), right_arr(len2);
    for(int i = 0; i < len1; i++){
        left_arr[i] = arr[beg + i];
    }
    for(int j = 0; j < len2; j++){
        right_arr[j] = arr[mid + 1 + j];
    }
    int i, j, k;
    i = 0; j = 0; k = beg;
    while(i < len1 && j < len2){
        if(firstDateBigger(right_arr[j].date, left_arr[i].date)) {
            arr[k] = left_arr[i];
            i++;
        }
        else{
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // Also add cases if len1 != len2
    while(i < len1){
        arr[k] = left_arr[i];
        i++; k++;
    }
    while(j < len2){
        arr[k] = right_arr[j];
        j++; k++;
    }
}



void timSortFunc(std::vector<feedback> &arr){
    int len_arr = arr.size();
    for(int i = 0; i < len_arr; i+=RUN_SIZE){
        insertionSort(arr, i, std::min(i + RUN_SIZE - 1, len_arr - 1)); // If size of vector < RUN_SIZE - number of runs will be 1(just that vector)
    }
    for(int size = RUN_SIZE; size < len_arr; size = 2 * size){
        for(int beg = 0; beg < len_arr; beg += size * 2){
            // Mid - ending point of left subarray
            int mid = beg + size - 1;
            int end = std::min(beg + 2 * size- 1, len_arr - 1);

            if(mid < end){
                merge(arr, beg, mid, end);
            }
        }
    }
}


void timSort(std::vector<feedback> &arr){
    timSortFunc(arr);
}

std::vector<feedback> timSort(const std::string &file_path){
    auto feedbacks = formFeedbacksVector(file_path);
    timSortFunc(feedbacks);
    return feedbacks;
}