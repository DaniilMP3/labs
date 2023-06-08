#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
#include <fstream>


struct TreeNode{
    int val;
    TreeNode* left;
    TreeNode* right;
};


class TreeBuilder {
    TreeNode *root;
    std::string file_path;

    static TreeNode *generateBinaryTree(int n, int maxValue) {
        if (!n) {
            return nullptr;
        }
        auto *root = new TreeNode{rand() % maxValue};

        int leftN = rand() % n;
        root->left = generateBinaryTree(leftN, maxValue);
        root->right = generateBinaryTree(n - leftN - 1, maxValue);
        return root;
    }

    static int helper(TreeNode* root, std::map<int, int> &frequency, int &max_frequency){
        if(!root){
            return 0;
        }
        if(!root->left && !root->right){ // leaf case
            if(!frequency.count(root->val)){
                frequency[root->val] = 1;
            }
            else{
                frequency[root->val]++;
            }
            max_frequency = std::max(max_frequency, frequency[root->val]);
            return root->val;
        }

        int left_sum = helper(root->left, frequency, max_frequency);
        int right_sum = helper(root->right, frequency, max_frequency);

        int current_sum = left_sum + right_sum + root->val;
        if(!frequency[current_sum]){
            frequency[current_sum] = 1;
        }
        else{
            frequency[current_sum]++;
        }
        max_frequency = std::max(max_frequency, frequency[current_sum]);
        return current_sum;
    }

public:
    TreeBuilder(int n, int maxNodeValue) {
        srand(time(NULL));
        this->root = generateBinaryTree(n, maxNodeValue);
    }
    TreeBuilder(int n, int maxNodeValue, const std::string &file_path) {
        srand(time(NULL));
        this->root = generateBinaryTree(n, maxNodeValue);
        this->file_path = file_path;
    }
    TreeBuilder(TreeNode* root){
        this->root = root;
    }
    TreeBuilder(TreeNode* root, const std::string &file_path){
        this->root = root;
        this->file_path = file_path;
    }


    TreeNode *get_root() {
        return this->root;
    }
    void set_file_path(const std::string &new_file_path){
        this->file_path = new_file_path;
    }

    std::vector<int> findSubtreeMostCommonSum(){
        std::map<int, int> frequency;
        int max_frequency = 0;
        helper(this->root, frequency, max_frequency);
        std::vector<int> result;
        for(auto i: frequency){
            if(i.second == max_frequency){
                result.push_back(i.first);
            }
        }
        return result;
    }
    void serialize(){
        std::ofstream file(this->file_path, std::ios::binary | std::ios::trunc); // truncate existing content to - 0 bytes;
        file.close();

        std::ofstream write_file;
        if(!write_file){
            std::cout << "Cannot open the file\n";
        }
        write_file.open(this->file_path, std::ios::binary | std::ios::out);
        write_file.write((char*)&root, sizeof(TreeNode));
        write_file.close();
        if(!write_file.good()){
            std::cout << "Error while writing\n";
        }
    }
    TreeNode* deserialize(){
        std::ifstream read_file;
        if(!read_file){
            std::cout << "Cannot open the file\n";
        }
        read_file.open(this->file_path, std::ios::binary | std::ios::in);
        TreeNode* new_root = new TreeNode();
        read_file.read((char*)&new_root, sizeof(TreeNode));
        read_file.close();
        if(!read_file.good()){
            std::cout << "Error while reading\n";
        }
        return new_root;
    }
};