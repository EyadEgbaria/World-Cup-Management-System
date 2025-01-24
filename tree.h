#ifndef TREE_H
#define TREE_H

#include "treeNode.h"
#include <iostream>
#include <memory>
#include <cmath>

using std::shared_ptr;
using std::endl;
using std::cout;
using std::iostream;

template<class T>
class Tree {
private:
    shared_ptr<TreeNode<T>> primary_root;
    int size;
public:
    Tree ();
    Tree (const Tree<T>& Tree) = default;
    ~Tree() = default;
    void destroyTree(shared_ptr<TreeNode<T>> root);

    void rrRotate (shared_ptr<TreeNode<T>> node);
    void llRotate (shared_ptr<TreeNode<T>> node);
    void lrRotate (shared_ptr<TreeNode<T>> node);
    void rlRotate (shared_ptr<TreeNode<T>> node);

    int checkBalance (shared_ptr<TreeNode<T>> node);
    bool createBalance (shared_ptr<TreeNode<T>> node);

    void insert(shared_ptr<T> to_add);
    void remove(shared_ptr<T> to_remove);

    void inOrderToArray (shared_ptr<TreeNode<T>> root, shared_ptr<TreeNode<T>>* array, int* index);
    void inOrderDataToArray(shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index);
    void LimitedinOrderDataToArray (shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index, int* limit);
    void minMaxInOrderDataToArray (shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index, shared_ptr<T> min, shared_ptr<T> max);

    int getSize();
    shared_ptr<TreeNode<T>> getRoot();
    void changeRoot(shared_ptr<TreeNode<T>> new_root);
    void changeSize(int new_size);

    shared_ptr<Tree<T>> merge(shared_ptr<Tree<T>> root_a, shared_ptr<Tree<T>> root_b);
    shared_ptr<TreeNode<T>> find (shared_ptr<T> to_find);
    shared_ptr<TreeNode<T>> findMinimalNode (shared_ptr<TreeNode<T>> root);
    shared_ptr<TreeNode<T>> findMaximalNode (shared_ptr<TreeNode<T>> root);

    shared_ptr<TreeNode<T>>* TreeToArray();
};
template <class T>
shared_ptr<TreeNode<T>>* mergeArrays(shared_ptr<TreeNode<T>>* array_a, shared_ptr<TreeNode<T>>* array_b, int size_a, int size_b);
template <class T>
Tree<T> mergeArrayToTree(shared_ptr<TreeNode<T>>* merge_array, int start, int end);
template <class T>
shared_ptr<TreeNode<T>> mergeArrayToTreeAux(shared_ptr<TreeNode<T>>* merge_array,shared_ptr<TreeNode<T>> father, int start, int end);
template <class T>
shared_ptr<Tree<T>> mergeTrees(shared_ptr<Tree<T>> tree_a, shared_ptr<Tree<T>> tree_b);


template <class T>
Tree<T>::Tree() :
        size(0)
{
}

template <class T>
void Tree<T>::destroyTree(shared_ptr<TreeNode<T>> root){
    if(root == nullptr) {
        return;
    }
    destroyTree(root->getLeft());
    destroyTree(root->getRight());
    root->clearNode();
    size = 0;
}

template <class T>
shared_ptr<TreeNode<T>> Tree<T>::getRoot(){
    return this->primary_root;
}

template <class T>
int Tree<T>::getSize(){
    return this->size;
}

template <class T>
void Tree<T>::changeSize(int new_size){
    this->size = new_size;
}

template <class T>
void Tree<T>::changeRoot(shared_ptr<TreeNode<T>> new_root){
    this->primary_root = new_root;
}

template<class T>
shared_ptr<TreeNode<T>> Tree<T>::find (shared_ptr<T> to_find){
    shared_ptr<TreeNode<T>> temp = this->primary_root;
    while(temp != nullptr){
        if(*(temp->getData())== *to_find){
            return temp;
        }
        else if(*temp->getData() > *to_find){
            temp = temp->getLeft();
        }
        else{
            temp = temp->getRight();
        }
    }
    return nullptr;
}

template<class T>
void Tree<T>::insert(shared_ptr<T> to_add)
{
    shared_ptr<TreeNode<T>> new_node(new TreeNode<T>(to_add));
    shared_ptr<TreeNode<T>> temp = this->getRoot();
    shared_ptr<TreeNode<T>> temp_update = nullptr;
    //find the parent of new node and update his father
    while(temp != nullptr){
        temp_update = temp;
        if (*temp->getData() == *to_add){
            return;//added to check if has an influenceif (*temp->getData() < *to_add)
        }
        if (*temp->getData() > *to_add){
            temp = temp->getLeft();
        }
        else if(*temp->getData() < *to_add){
            temp = temp->getRight();
        }
    }
    //update tree size
    size++;
    //update new node to be the son of his father
    new_node->changeFather(temp_update);        //new node is the root
    if (temp_update == nullptr){
        this->primary_root = new_node;
    }
    else if(*temp_update->getData() > *to_add){    //new node is left son of his parent
        temp_update->changeLeft(new_node);
    }
    else{
        temp_update->changeRight(new_node);       //new node is right son of his parent
    }
    //touring from the new leaf to the root in order to balance the tree
    while(temp_update != nullptr){
        temp_update->updateHeight();
        if(createBalance(temp_update)){
            return;
        }
        temp_update = temp_update->getFather();
    }
}

template<class T>
void Tree<T>::rrRotate (shared_ptr<TreeNode<T>> node)
{
    shared_ptr<TreeNode<T>> temp = node->getRight();
    node->changeRight(temp->getLeft());
    if(temp->getLeft() != nullptr){
        temp->getLeft()->changeFather(node);
    }
    temp->changeFather(node->getFather());
    if(node->getFather() == nullptr){               //node is the root of the tree
        this->primary_root = temp;
    }
    else if (node == node->getFather()->getRight()){ //node is right sun
        node->getFather()->changeRight(temp);
    }
    else{                                           //node is left sun
        node->getFather()->changeLeft(temp);
    }
    temp->changeLeft(node);
    node->changeFather(temp);
    node->updateHeight();
    temp->updateHeight();
}

template<class T>
void Tree<T>::llRotate (shared_ptr<TreeNode<T>> node)
{
    shared_ptr<TreeNode<T>> temp = node->getLeft();
    node->changeLeft(temp->getRight());
    if(temp->getRight() != nullptr){
        temp->getRight()->changeFather(node);
    }
    temp->changeFather(node->getFather());
    if(node->getFather() == nullptr){                //node is the root of the tree
        this->primary_root = temp;
    }
    else if (node == node->getFather()->getRight()){ //node is right sun
        node->getFather()->changeRight(temp);
    }
    else{                                            //node is left sun
        node->getFather()->changeLeft(temp);
    }
    temp->changeRight(node);
    node->changeFather(temp);

    node->updateHeight();
    temp->updateHeight();
}

template<class T>
void Tree<T>::lrRotate (shared_ptr<TreeNode<T>> node)
{
    shared_ptr<TreeNode<T>> temp = node->getLeft();
    rrRotate(temp);
    llRotate(node);
}

template<class T>
void Tree<T>::rlRotate (shared_ptr<TreeNode<T>> node)
{
    shared_ptr<TreeNode<T>> temp = node->getRight();
    llRotate(temp);
    rrRotate(node);
}

template<class T>
shared_ptr<TreeNode<T>> Tree<T>::findMinimalNode (shared_ptr<TreeNode<T>> root)
{
    if(root == nullptr){
        return nullptr;
    }
    while (root->getLeft() != nullptr){
        root = root->getLeft();
    }
    return root;
}

template<class T>
shared_ptr<TreeNode<T>> Tree<T>::findMaximalNode (shared_ptr<TreeNode<T>> root)
{
    if(root == nullptr){
        return nullptr;
    }
    while (root->getRight() != nullptr){
        root = root->getRight();
    }
    return root;
}
/*
template<class T>
void Tree<T>::update_pre_suc(shared_ptr<TreeNode<T>> root ,shared_ptr<TreeNode<T>>* pre , shared_ptr<TreeNode<T>>* suc , shared_ptr<TreeNode<T>> key)
{
    if(root->getData)
}
*/
template<class T>
bool Tree<T>::createBalance (shared_ptr<TreeNode<T>> node)
{
    int balance_factor = checkBalance(node);
    if (balance_factor == 2)
    {
        if(checkBalance(node->getLeft()) == -1){
            lrRotate(node);
            return true;
        }
        else if(checkBalance(node->getLeft()) >= 0){
            llRotate(node);
            return true;
        }
    }
    else if (balance_factor == -2)
    {
        if(checkBalance(node->getRight()) <= 0)  {
            rrRotate(node);
            return true;
        }
        else if (checkBalance(node->getRight()) == 1) {
            rlRotate(node);
            return true;
        }
    }
    return false;
}

template<class T>
int Tree<T>::checkBalance (shared_ptr<TreeNode<T>> node)
{
    int left_height = -1, right_height = -1;
    if(node->getLeft() != nullptr){
        left_height = node->getLeft()->getHeight();
    }
    if(node->getRight() != nullptr){
        right_height = node->getRight()->getHeight();
    }
    return left_height - right_height;
}

template<class T>
void Tree<T>::remove(shared_ptr<T> to_remove)
{
    if (primary_root == nullptr)
    {
        return;
    }
    shared_ptr<TreeNode<T>> node_to_remove = find(to_remove);
    if (node_to_remove == nullptr) {
        return;
    }
    shared_ptr<TreeNode<T>> father_node = node_to_remove->getFather();
    //if the node to delete has no children
    if (node_to_remove->getLeft() == nullptr && node_to_remove->getRight() == nullptr)
    {
        if(father_node != nullptr) {
            if (*(father_node->getData()) > *to_remove){
                father_node->changeLeft(nullptr);
            }
            else
            {
                father_node->changeRight(nullptr);
            }
        }
        else
        {
            primary_root = nullptr;
            size--;
            return;
        }
    }

        //if the node to delete has only one child on the right
    else if (node_to_remove->getLeft() == nullptr)
    {
        shared_ptr<TreeNode<T>> temp = node_to_remove->getRight();
        node_to_remove->changeData(temp->getData());
        node_to_remove->changeLeft(temp->getLeft());
        node_to_remove->changeRight(temp->getRight());
        if(temp->getLeft() != nullptr){
            temp->getLeft()->changeFather(node_to_remove);
        }
        if(temp->getRight() != nullptr){
            temp->getRight()->changeFather(node_to_remove);
        }
    }
        //if the node to delete has only one child on the left
    else if (node_to_remove->getRight() == nullptr)
    {
        shared_ptr<TreeNode<T>> temp = node_to_remove->getLeft();
        node_to_remove->changeData(temp->getData());
        node_to_remove->changeRight(temp->getRight());
        node_to_remove->changeLeft(temp->getLeft());
        if(temp->getLeft() != nullptr){
            temp->getLeft()->changeFather(node_to_remove);
        }
        if(temp->getRight() != nullptr){
            temp->getRight()->changeFather(node_to_remove);
        }
    }
        //if reached this point than the node has two children
    else
    {
        shared_ptr<TreeNode<T>> current_minimal = findMinimalNode(node_to_remove->getRight());
        shared_ptr<TreeNode<T>> current_minimal_father = current_minimal->getFather();
        shared_ptr<T>copy_data = current_minimal->getData();
        if (*(current_minimal_father->getData()) == *(node_to_remove->getData()))
        {
            node_to_remove->changeRight(current_minimal->getRight());
            if(current_minimal->getRight() != nullptr){
                current_minimal->getRight()->changeFather(current_minimal->getFather());
            }
        }
        else
        {
            current_minimal_father->changeLeft(current_minimal->getRight());
            if(current_minimal->getRight() != nullptr){
                current_minimal->getRight()->changeFather(current_minimal->getFather());
            }
            current_minimal_father->updateHeight();
        }
        node_to_remove->changeData(copy_data);
        //need to update the start point to create balance and change heights
        father_node = current_minimal_father;
    }
    //update tree size
    size--;
    //touring from the deleated leaf to the root in order to balance the tree
    node_to_remove->updateHeight();
    if (father_node == nullptr)
    {
        createBalance(node_to_remove);
    }
    while(father_node != nullptr)
    {
        father_node->updateHeight();
        createBalance(father_node);
        father_node = father_node->getFather();
    }
}

template <class T>
void Tree<T>::inOrderToArray (shared_ptr<TreeNode<T>> root, shared_ptr<TreeNode<T>>* array, int* index){
    if(root == nullptr){
        return;
    }
    inOrderToArray(root->getLeft(), array, index);
    root->changeLeft(nullptr);
    array[*index] = root;
    (*index)++;
    inOrderToArray(root->getRight(), array, index);
    root->changeRight(nullptr);
    root->changeFather(nullptr);
    root->changeHeight(0);
}

template <class T>
void Tree<T>::inOrderDataToArray (shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index){
    if(root == nullptr){
        return;
    }
    inOrderDataToArray(root->getLeft(), array, index);
    array[*index] = root->getData();
    (*index)++;
    inOrderDataToArray(root->getRight(), array, index);
}



template <class T>
void Tree<T>::LimitedinOrderDataToArray (shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index, int* limit){
    if(root == nullptr || *limit == 0){
        return;
    }
    LimitedinOrderDataToArray(root->getLeft(), array, index, limit);
    if (*limit > 0) {
        array[*index] = root->getData();
        (*index)++;
        (*limit)--;
    }
    LimitedinOrderDataToArray(root->getRight(), array, index, limit);
}

template <class T>
void Tree<T>::minMaxInOrderDataToArray (shared_ptr<TreeNode<T>> root, shared_ptr<T>* array, int* index, shared_ptr<T> min, shared_ptr<T> max)
{
    if(root == nullptr){
        return;
    }

    if(*(root->getData()) >= *min)
    {
        minMaxInOrderDataToArray(root->getLeft(), array, index, min, max);
    }
    if (*(root->getData()) >= *min && *(root->getData()) <= *max)
    {
        array[*index] = root->getData();
        (*index)++;
    }
    if(*(root->getData()) <= *max)
    {
        minMaxInOrderDataToArray(root->getRight(), array, index, min, max);
    }
}

template <class T>
shared_ptr<TreeNode<T>>* Tree<T>::TreeToArray(){
    shared_ptr<TreeNode<T>>* tree_array = new shared_ptr<TreeNode<T>>[this->getSize()];
    try{
        int index = 0;
        inOrderToArray (this->getRoot(), tree_array, &index);
    }
    catch(const std::bad_alloc& ba){
        delete[] tree_array;
        throw ba;
    }
    return tree_array;
}

template<class T>
shared_ptr<TreeNode<T>>* mergeArrays(shared_ptr<TreeNode<T>>* array_a, shared_ptr<TreeNode<T>>* array_b, int size_a, int size_b){
    shared_ptr<TreeNode<T>>* merge_array = new shared_ptr<TreeNode<T>>[size_a + size_b];
    try{
        int index_a = 0, index_b = 0, index_merge = 0;
        while(index_a < size_a && index_b < size_b){
            if(*(array_a[index_a]->getData()) < *(array_b[index_b]->getData())){
                merge_array[index_merge] = array_a[index_a];
                index_a++;
            }
            else{
                merge_array[index_merge] = array_b[index_b];
                index_b++;
            }
            index_merge++;
        }
        while(index_a < size_a){
            merge_array[index_merge] = array_a[index_a];
            index_a++;
            index_merge++;
        }
        while(index_b < size_b){
            merge_array[index_merge] = array_b[index_b];
            index_b++;
            index_merge++;
        }
        delete[] array_a;
        delete[] array_b;
    }
    catch(const std::bad_alloc& ba){
        delete[] array_a;
        delete[] array_b;
        throw ba;
    }
    return merge_array;
}

template <class T>
Tree<T> mergeArrayToTree(shared_ptr<TreeNode<T>>* merge_array, int start, int end){
    Tree<T> new_tree;
    shared_ptr<TreeNode<T>> null_node(nullptr);
    new_tree.changeRoot(mergeArrayToTreeAux(merge_array,null_node, start, end));
    delete[] merge_array;
    return new_tree;
}

template <class T>
shared_ptr<TreeNode<T>> mergeArrayToTreeAux(shared_ptr<TreeNode<T>>* merge_array,shared_ptr<TreeNode<T>> father, int start, int end){
    if(end < start){
        return nullptr;
    }
    int mid = (start+end)/2;
    shared_ptr<TreeNode<T>> new_root = merge_array[mid];
    new_root->changeFather(father);
    new_root->changeLeft(mergeArrayToTreeAux(merge_array, new_root, start, mid-1));
    new_root->changeRight(mergeArrayToTreeAux(merge_array, new_root, mid+1, end));
    new_root->updateHeight();
    return new_root;
}

template<class T>
shared_ptr<Tree<T>> mergeTrees(shared_ptr<Tree<T>> tree_a, shared_ptr<Tree<T>> tree_b){
    //one of the trees is empty
    if(tree_b == nullptr || tree_b->getRoot() == nullptr){
        return tree_a;
    }
    if(tree_a == nullptr || tree_a->getRoot() == nullptr){
        return tree_b;
    }
    shared_ptr<TreeNode<T>>* array_a = tree_a->TreeToArray();
    shared_ptr<TreeNode<T>>* array_b = tree_b->TreeToArray();
    shared_ptr<TreeNode<T>>* merge_array = mergeArrays(array_a, array_b, tree_a->getSize(), tree_b->getSize());
    Tree<T> merge_tree = mergeArrayToTree(merge_array, 0, tree_a->getSize()+tree_b->getSize()-1);
    merge_tree.changeSize(tree_a->getSize()+tree_b->getSize());
    shared_ptr<Tree<T>> tree(new Tree<T>(merge_tree));
    return tree;
}


#endif /*TREE_H*/


