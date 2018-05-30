//
//  main.cpp
//  BestFit
//
//  Created by ngoChu on 5/29/18.
//  Copyright © 2018 Ngoc. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "FixedLength.h"
#include "VariedLength.h"

using namespace std;

//update club participant after each season
static void updateClubInSeason(FixLengthList &club, string line) {
    string clubName = line.substr(2, line.length()-1);
    if(line[0]=='1')
        club.addRecord(clubName);
    else
        club.deleteRecord(clubName);
}

//update players of each team after each season: First Fit
static void updatePlayerFirstFit(std::string &line, map<std::string, PlayerList> &playersInClub) {
    string clubName = line.substr(2, 3);
    string playerName = line.substr(6, line.length()-5);
    if (line[0] == '1')
        playersInClub[clubName].addRecordFF(playerName);
    else playersInClub[clubName].deleteRecord(playerName);
}
//  Best Fit
static void updatePlayerBestFit(std::string &line, map<std::string, PlayerList> &playersInClub) {
    string clubName = line.substr(2, 3);
    string playerName = line.substr(6, line.length()-5);
    if (line[0] == '1')
        playersInClub[clubName].addRecordBF(playerName);
    else playersInClub[clubName].deleteRecord(playerName);
}

//update clubs, players after 1 season
static void updateEndOfSeason(FixLengthList &club, int &currSeason, map<std::string, PlayerList> &playersInClub) {
    node* curr = club.head->next;
    while(curr) {
        if(curr->data.find('*') == std::string::npos) {
            playersInClub[curr->data].defragment();
            playersInClub[curr->data].writeFile(curr->data+".txt");
            playersInClub[curr->data].clone(curr->data+".txt", currSeason);
        }
        curr=curr->next;
    }
    currSeason++;
}

//read the input file -> output
void readFile(string fileName, FixLengthList &club, map<string, PlayerList> &playersInClub) {
    ifstream inFile;
    inFile.open(fileName);
    
    if(inFile.is_open()) {
        string line, firstline;
        //1st line
        getline(inFile, firstline);
        int type = firstline[3] - '0';
        
        //skip the 2nd line
        getline(inFile, line);
        
        int currSeason = 1;
        
        while (getline(inFile, line)) {
            //update club participant
            if (line.length() == 5 && (line[2] < '0' || line[2] > '9')) { //check for club transaction line
                string clubName = line.substr(1, line.length()-1);
                playersInClub[clubName];
                updateClubInSeason(club, line);
            }
            //Handle player transaction
            else if (count(line.begin(), line.end(), ' ') > 1) {
                if(type == '0')
                    updatePlayerFirstFit(line, playersInClub);
                else updatePlayerBestFit(line, playersInClub);
            }
            //end of the season
            else {
                updateEndOfSeason(club, currSeason, playersInClub);
            }
        }
        
        //end of the league
        club.defragment();
        updateEndOfSeason(club, currSeason, playersInClub);
        inFile.close();
    }
}

int main() {
    FixLengthList club;
    map<string, PlayerList> playersInClub;
    readFile("Input.txt", club, playersInClub);
    
    return 1;
}
