/*
 Joshua Shewmaker
 CIS 350 Fall 2017
 Raise the pay part 2
 
 Code to instert into tree and to create forest as well as print the forest was
 created by Bruce Elenbogen Fall 2017
 */

/*DESCRIPTION
    This program will calculate the minimum number of employees that need to ask for a raise in order for everyone to get one. A hierarchy will be built for the company. Each company has a single CEO and any number of workers below him/her. The program will take in each worker and each boss. A boss is someone with workers below them. A worker is someone with a person above them. Workers with no one below them count as 1 request. This program will find the number of request needed for each boss to pass along the request to their boss given a threshold.
 */
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <math.h>
using namespace std;

struct Node
{
    int id;
    int minRequests4Raise;
    vector< Node *> underlings;
    
    Node(int n = 0) // constructor
    {
        id = n;
        minRequests4Raise = 0;
    }
    
};

void addUnderling(Node * ptr, int id)
{
    ptr->underlings.push_back(new Node(id));
}
void addUnderling(Node * ptr, Node *underPtr)
{
    ptr->underlings.push_back(underPtr);
}


class Tree
{
private:
    Node * root;
    void print(Node * ptr, int level) // private version of print
    {
        
        
        if (ptr != nullptr)
        {
            cout << setw(5 * level) << ptr->id << "(" << ptr->minRequests4Raise << ")";
            for (int i = 0; i < ptr->underlings.size(); i++)
                print(ptr->underlings[i], level + 1);
        }
    }
    
    Node * find(Node * root, int target)  // private version of find
    {
        Node * temp;
        if (root == nullptr)
            return nullptr;
        else if (root->id == target)
            return root;
        else
            for (int i = 0; i < root->underlings.size(); i++)
            {
                temp = find(root->underlings[i], target);
                if (temp != nullptr)
                    return temp;
            }
        return nullptr;
    }
    
    void setMinRequest(Node * ptr, float threshold) {
       
        if (ptr->underlings.empty()) {
            ptr->minRequests4Raise = 1;
            return;
        }
        
        for (Node* worker : ptr->underlings)
            setMinRequest(worker, threshold);
        
        sort(ptr->underlings.begin(), ptr->underlings.end(), [](Node* l, Node* r) {
            return l->minRequests4Raise <= r->minRequests4Raise;
        });
        for(int i = 0; i < ceil(ptr->underlings.size() * threshold); i++){
            ptr->minRequests4Raise = ptr->minRequests4Raise + ptr->underlings[i]->minRequests4Raise;
        }
    }
  
public:
    Tree(Node * rt = nullptr) { root = rt; }
    void print() { print(root, 0); }
    Tree(int id) { root = new Node(id); }
    Node * getRoot() { return root; }
    void printTree()                   // public print
    {
        print(root, 0);
       // cout << endl << "SIZE " << root->underlings[1]->underlings[0]->underlings.size();
    }
    Node * find(int target)          // public find
    {
        return find(root, target);
    }
    void setMinRequestPublic(float threshold) {
        setMinRequest(root, threshold);
    }
    void printMinNeededRequest(){
        cout << root->minRequests4Raise;
    }
};

class Forest
{
private:
    vector< Tree > trees;
public:
    void setMinRequest(float threashold) {
        for (int i = 0; i < trees.size(); i++) {
            trees[i].setMinRequestPublic(threashold);
        }
    }
    void print()
    {
        for (int i = 0; i < trees.size(); i++)
            trees[i].printTree();
        cout << endl;
    }
    
    Node * find(int target)
    {
        Node * temp;
        for (int i = 0; i < trees.size(); i++)
        {
            temp = trees[i].find(target);
            if (temp != nullptr)
                return temp;
        }
        return nullptr;
    }
    
    void insert(int boss, int underling)
    {
        Node * bossPtr = find(boss);
        Node * underPtr = find(underling);
        if (bossPtr == nullptr && underPtr == nullptr) // need a new tree
        {
            trees.push_back(*(new Tree(boss)));
            addUnderling(trees[trees.size() - 1].getRoot(), underling);
        }
        else if (underPtr == nullptr)
        {
            addUnderling(bossPtr, underling);
        }
        else if (underPtr != nullptr) // the underling is already a boss
        {
            if (bossPtr == nullptr)  // if boss is not in the tree put him in
            {
                trees.push_back(*(new Tree(boss)));
                bossPtr = find(boss);
            }
            // now both are in the tree
            addUnderling(bossPtr, underPtr);
            for (auto itr = trees.begin(); itr != trees.end(); itr++)
                if (itr->getRoot()->id == underling)
                {
                    trees.erase(itr);
                    return;
                }
            
        }
    }
    
    Forest()
    {
        trees.push_back(Tree(0));
    }
    void printMinRequest(){
        trees[0].printMinNeededRequest();
    }
    void clear() {
        trees.clear();
    }
    
};

int main()
{
    int numEmploys, threshold, boss;
    Forest gump;
    float thresholdPercentage = 0;
    cin >> numEmploys >> threshold;
    while (numEmploys > 0 && threshold > 0)
    {
        
        for (int i = 0; i < numEmploys; i++)
        {
            cin >> boss;
            gump.insert(boss, i + 1);
        }
        
        thresholdPercentage = threshold;
        gump.setMinRequest((thresholdPercentage/100));
        gump.clear();
        cin >> numEmploys >> threshold;
    }
    gump.printMinRequest();
    cout << endl;
    
    return 0;
}
