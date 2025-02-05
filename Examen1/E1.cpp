#include <iostream>

struct TreeNode{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(): val(0), left(nullptr) , right(nullptr){};
    TreeNode(int x): val(x), left(nullptr), right(nullptr){};
    TreeNode(int x, TreeNode* left, TreeNode* right): val(x), left(left), right(right){};

};

// Me puse nervioso noooo :C

class Solution{
public:
    TreeNode* lcaDeepestLeaves(TreeNode* root){
        if (!root){
            return nullptr;
        }
        if (!root->left && !root->right){
            return root;
        }
        int cont = 1;
        TreeNode* left = lcaDeepestLeaves(root->left);
        TreeNode* right = lcaDeepestLeaves(root->right);
        if (!left->left){
            return left;
        }
        if (!right->right){
            return right;
        }
        return root;
    }
};




using namespace std;
int main() {

    return 0;
}
