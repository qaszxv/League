#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//Store a player info
struct playerInfo {
    string name;
    int index;              //Beginning index of the player in the list
    int lastDeletedPlayer;  //Store the last deleted player's index
    playerInfo* next;
    
    playerInfo() {
        this->index = 0;
        this->name = "";
        this->next = NULL;
    }
    
    playerInfo(string str, int index, int lastDeletedPlayerIndex) {
        this->index = index;
        this->name = (str);
        this->next = NULL;
        this->lastDeletedPlayer = lastDeletedPlayerIndex;
    }
};

//Store player list of a team
class PlayerList {
public:
    playerInfo* head = NULL;
public:
    //Init head with index & last deleted = -1
    PlayerList(){
        head = new playerInfo("", -1, -1);
    }
    
    void addTail(playerInfo* newPlayer){
        playerInfo* curr= head;
        while (curr && curr->next) {
            newPlayer->index += curr->next->name.length();
            curr = curr->next;
        }
        curr->next = newPlayer;
    }
    
    //Store context of the list
    string listContext() {
        string context;
        playerInfo* curr= head;
        while(curr != NULL) {
            if(curr->lastDeletedPlayer!=0)
                context += to_string(curr->lastDeletedPlayer) + curr->name + "|";
            else context += curr->name+ "|";
            curr = curr->next;
        }
        return context;
    }
    
    void display() {
        cout<< listContext() << endl;
    }
    
    //Return the last deleted player rite b4 curr
    playerInfo* toLastDeletedPlayer(playerInfo* curr) {
        int lastDeletedIndex = curr->lastDeletedPlayer;
        curr = head;
        
        while (curr->index < lastDeletedIndex){
            curr = curr->next;
        }
        return curr;
    }
    
    //Update the pointers which point to deleted nodes
    void updateDeletedPointers(playerInfo* curr) {
        int lastDeletedIndex = curr->lastDeletedPlayer;
        playerInfo* prev = head;
        while (prev->lastDeletedPlayer != curr->index) {
            prev = prev->next;
        }
        prev->lastDeletedPlayer = lastDeletedIndex;
    }
    
    //Insert a new player into the list
    void recursiveInsert(string newName, playerInfo* curr) {
        if(curr->name.length() >= newName.length()){
            for(int i=(int)newName.size(); i<curr->name.length(); i++)
                newName.append(".");
            curr->name = newName;
            updateDeletedPointers(curr);
            curr->lastDeletedPlayer = 0;
            return;
        }
        recursiveInsert(newName, toLastDeletedPlayer(curr));
    }
    
    //Check if new player have a long name -> add tail
    bool isNewPlayerReplaceDeletedPos(string name, playerInfo* curr) {
        while (curr != NULL) {
            if (curr->name.find('*') != std::string::npos && curr->name.length() >= name.length())
                return true;
            curr = curr->next;
        }
        return false;
    }
    
    //Add a new record First Fit
    void addRecordFF(string str) {
        playerInfo* newNode = new playerInfo(str, 1, 0);
        
        if (head == NULL) {
            head = newNode;
        } else {
            if(head->lastDeletedPlayer == -1)
                addTail(newNode);
            else {
                playerInfo* curr = head;
                if(isNewPlayerReplaceDeletedPos(newNode->name, curr) == false){
                    addTail(newNode);
                    return;
                }
                else recursiveInsert(newNode->name, curr);
            }
        }
    }

    vector<pair<int, int>> listDeletedRecord(string str) {
        int posCurr = 0;
        playerInfo*curr = head;
        int ptrDelete = curr->lastDeletedPlayer;
        vector<pair<int, int>> deletedRecord;
        while (ptrDelete != -1) {
            //Move to the pointerDele position
            while (posCurr < ptrDelete) {
                curr = curr->next;
                posCurr = curr->index;
            }
            //Add data of the record to vector. Then continue
            deletedRecord.push_back(pair<int, int>(curr->index, curr->name.length() - str.length()));
            ptrDelete = curr->lastDeletedPlayer;
            posCurr = 0;
            curr = head;
        }
        return deletedRecord;
    }
    
    vector<int> minRemainSizeRecord(vector<pair<int, int>> listRe) {
        //Store a record of smallest remaing size -> minDeleted{index, remaining size}
        int minDeleted[] = { 0,totalFileSize() }; 
        int prevRecordIndex = 0;
        for (int i = 0; i < listRe.size(); i++) {
            if (listRe[i].second >= 0 && listRe[i].second < minDeleted[1])
            {
                minDeleted[0] = listRe[i].first;
                minDeleted[1] = listRe[i].second;
                if (i > 0)
                    prevRecordIndex = listRe[i - 1].first;
            }
        }
        vector <int> result;
        result.push_back(minDeleted[0]);
        result.push_back(minDeleted[1]);
        result.push_back(prevRecordIndex);
        return result;
    }
    
    int totalFileSize() {
        playerInfo*curr = head;
        int size = 0;
        while (curr) {
            size += curr->index;
            curr = curr->next;
        }
        return size;
    }
    
    //add a new record Best Fit
    void addRecordBF(string str) {
        playerInfo* newNode = new playerInfo(str, 1, 0);
        playerInfo*curr = head;
        
        if (head == NULL) {
            head = newNode;
        }
        else {
            if (curr->lastDeletedPlayer == -1)
                addTail(newNode);
            else {
                int prevRecordIndex = 0;//store a index of prev-replacing record
                int posCurr = 0;
                //find record with smallest "remaining size" //if available, store it.
                vector<int> minRecord = minRemainSizeRecord(listDeletedRecord(str));
                
                if (minRecord[1] == totalFileSize())//if there is no available place for the string
                    addTail(newNode);
                else {
                    //Add Record
                    while (posCurr < minRecord[0]) {
                        curr = curr->next;
                        posCurr = curr->index;
                    }
                    for (int i = 0; i < curr->name.length(); i++)
                        curr->name[i] = (i < str.length()) ? str[i] : '.';
                    
                    //Update Deleted Pointer
                    updateDeletedPointers(curr);
                    curr->lastDeletedPlayer = NULL;
                }
            }
        }
    }

    //Delete a record by player name
    void deleteRecord(string str) {
        playerInfo* curr = head;
        if (!curr)
            cout<< "Empty"<< endl;
        else {
            while(curr) {
                if(curr->name == (str)) {
                    curr->name = str.replace(0, head->name.size() + 1, head->name + "*");
                    int temp = head->lastDeletedPlayer;
                    head->lastDeletedPlayer = curr->index;
                    curr->lastDeletedPlayer = temp;
                }
                curr = curr->next;
            }
        }
    }
    
    void defragment() {
        if (head->next == NULL)
            return;
        playerInfo* prev= head;
        playerInfo* curr= head->next;
        while(curr != NULL) {
            curr->lastDeletedPlayer = 0;
            //check if a node was deleted
            if (curr->name.find("*") != std::string::npos) {
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
        head->index = -1;
        head->lastDeletedPlayer = -1;
    }
    

    
    void writeFile(string fileName) {
        string line, context;
        ofstream outFile;
        outFile.open(fileName);
        outFile << listContext() << endl;
        outFile.close();
    }
    
    //Clone a file by season
    void clone(string fileName, int season) {\
        ifstream src;
        ofstream dst;
        
        src.open(fileName, ios::in | ios::binary);
        dst.open(fileName.substr(0, fileName.length()-4) + to_string(season) + ".txt", ios::out | ios::binary);
        dst << src.rdbuf();
    }
    
    //Testing purpose
    void readFile(string fileName)
    {
        ifstream infile;
        infile.open(fileName);
        string line;
        if (infile.is_open()) {
            while (getline(infile, line)) {
                if(line[0] == '+') {
                    addRecordFF(line.substr(1));
                    display();
                }
                else if (line[0] == '-') {
                    deleteRecord(line.substr(1));
                    display();
                }
                else if(line=="DEFRAGMENT") {
                    cout<< "Defragmenting..."<< endl;
                    defragment();
                    display();
                }
            }
            infile.close();
        } else {
            cout << "Unable to open file!!!" << endl;
        }
    }
};
