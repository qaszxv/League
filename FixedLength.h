//
//  FixedLength.h
//  TestProject
//
//  Created by quangle on 5/29/18.
//  Copyright © 2018 quangle. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string modifyString(string str) {
    if(str == "-1") return str;
    if((str[0] == '1' || str[0] == '0'))
        str = str.substr(2);
    return str;
}
// Node to store record
struct node {
    string data;
    node* next;
    
    node() {
        this->data = "";
        this->next = NULL;
    }
    node(string str) {
        this->data = modifyString(str);
        this->next = NULL;
    }
};
// Fixed length list to store records
class FixLengthList {
public:
    node* head=NULL;
public:
    FixLengthList(){
        head = new node("-1");
    }
    // Add new record to list's tail
    void addTail(node* newNode){
        node* curr= head;
        while (curr && curr->next)
            curr = curr->next;
        curr->next = newNode;
    }
    // Return context of the list
    string listContext() {
        string context;
        node* curr= head;
        while(curr != NULL) {
            context += curr->data+" ";
            curr = curr->next;
        }
        return context;
    }
    // Display context of list
    void display() {
        cout<< listContext() << endl;
    }
    // Return index that a node point to
    string getIndex(string str)
    {
        string res="";
        if(str.at(1)=='-' && str.at(2)=='1'){
            res+=str.at(1);
            res+=str.at(2);
        }
        if(isdigit(str.at(1))){
            res+=str.at(1);
            if(isdigit(str.at(2)))
                res+=str.at(2);
        }
        return res;
    }
    // Add new record to the list
    void addRecord(string str) {
        node* newNode = new node(str);
        
        if (head == NULL) {
            head = newNode;
        } else {
            if(head->data == "-1")
                addTail(newNode);
            else {
                node* curr = head;
                int pos = 0;
                for(int i=0; i<stoi(head->data); i++) {
                    curr=curr->next;
                    pos = i;
                }
                head->data= getIndex(curr->data);
                curr->data=modifyString(str);
            }
        }
    }
    // Delete a record
    void deleteRecord(string str) {
        str = modifyString(str);
        node* curr = head;
        if (!curr)
            cout<< "Empty"<< endl;
        else
        {
            int deletedPos = 0;
            while(curr) {
                if(curr->data == (str)) {
                    curr->data = str.replace(0, head->data.size() + 1,
                                             "*" +head->data);
                    head->data = to_string(deletedPos);
                }
                curr = curr->next;
                deletedPos++;
            }
            
        }
        
    }
    // Defragmentation
    void defragment() {
        if (head->data=="-1")
            return;
        else {
            node* curr= head->next;
            node* prev= head;
            while (curr) {
                if (curr->data.find("*") != std::string::npos) {
                    if (curr->next)
                        prev->next = curr->next;
                    else
                        prev->next = NULL;
                    delete(curr);
                    curr = prev->next;
                } else {
                    curr = curr->next;
                    prev = prev->next;
                }
            }
            head->data = "-1";
        }
    }
    
};
    
