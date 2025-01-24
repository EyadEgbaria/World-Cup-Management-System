#ifndef TREENODE_H
#define TREENODE_H


#include <iostream>
#include <memory>

using std::shared_ptr;

class tree;

template<class T>
class TreeNode {
private:
    shared_ptr<TreeNode<T>> left_node;
    shared_ptr<TreeNode<T>> right_node;
    shared_ptr<TreeNode<T>> father;
    shared_ptr<T> data;
    int height;
public:
    TreeNode (shared_ptr<T> data);
    TreeNode (const TreeNode<T>& tree_node) = default;
    ~TreeNode() = default;

    shared_ptr<TreeNode<T>> getLeft();
    shared_ptr<TreeNode<T>> getRight();
    shared_ptr<TreeNode<T>> getFather();
    shared_ptr<T> getData();
    int getHeight();
    void clearNode();

    void changeData(shared_ptr<T> new_data);
    void changeLeft(shared_ptr<TreeNode<T>> new_left);
    void changeRight(shared_ptr<TreeNode<T>> new_right);
    void changeFather(shared_ptr<TreeNode<T>> new_father);
    void changeHeight(int new_height);

    int max(int height_a, int height_b);

    void updateHeight();
    bool operator <(shared_ptr<T> to_compare);
    bool operator >(shared_ptr<T> to_compare);
};

template<class T>
TreeNode<T>::TreeNode(shared_ptr<T> data):
        data(data), height(0)
{
}

template<class T>
void TreeNode<T>::clearNode(){
    this->father = nullptr;
    this->left_node = nullptr;
    this->right_node = nullptr;
    this->data = nullptr;
}

template<class T>
shared_ptr<TreeNode<T>> TreeNode<T>::getLeft(){
    return this->left_node;
}

template<class T>
shared_ptr<TreeNode<T>> TreeNode<T>::getRight(){
    return this->right_node;
}

template<class T>
shared_ptr<TreeNode<T>> TreeNode<T>::getFather(){
    return this->father;
}

template<class T>
int TreeNode<T>::getHeight(){
    return this->height;
}

template<class T>
void TreeNode<T>::changeFather(shared_ptr<TreeNode<T>> new_father){
    this->father = new_father;
}

template<class T>
shared_ptr<T> TreeNode<T>::getData(){
    return this->data;
}

template<class T>
void TreeNode<T>::changeData(shared_ptr<T> new_data)
{
    this->data = new_data;
}

template<class T>
void TreeNode<T>::changeLeft(shared_ptr<TreeNode<T>> new_left){
    this->left_node = new_left;
}

template<class T>
void TreeNode<T>::changeRight(shared_ptr<TreeNode<T>> new_right){
    this->right_node = new_right;
}

template<class T>
void TreeNode<T>::changeHeight(int new_height){
    this->height = new_height;
}

template<class T>
int TreeNode<T>::max(int hight_a, int hight_b){
    return hight_a > hight_b ? hight_a : hight_b;
}


template<class T>
void TreeNode<T>::updateHeight(){
    int left_height = -1, right_height = -1;
    if(this->getLeft() != nullptr){
        left_height = this->getLeft()->getHeight();
    }
    if(this->getRight() != nullptr){
        right_height = this->getRight()->getHeight();
    }
    int new_height = 1 + max(left_height, right_height);
    this->changeHeight(new_height);
}


template<class T>
bool TreeNode<T>::operator <(shared_ptr<T> to_compare){
    return *(this->data) < (*to_compare);
}

template<class T>
bool TreeNode<T>::operator >(shared_ptr<T> to_compare){
    return *(this->data) > (*to_compare);
}

#endif /*TREENODE_H*/
