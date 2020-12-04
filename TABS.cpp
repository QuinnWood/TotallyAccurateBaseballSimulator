// TotallyAccurateBaseballSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <cmath>
using namespace std;

struct Player {
    string playerID;
    string name;
    int year = 0;
    string team;
    int atBats = 0;
    int hits = 0;
    double battingAvg = 0.00;
    int gamesPlayed = 0;
    bool isPitcher = false;
    double ERA = 0.00;   //earned run average is a measure of runs allowed per innings pitched, times 9 for the traditional game length (in innings)
    int ER = 0;
    int IPOuts = 0;
    int IP = 0;    // IPOuts divided by 3
};

void openFile(string filename, vector<Player>& vect, vector<Player>& vectYears, unordered_map<string, int>& map) {
    ifstream workFile;
    workFile.open("C:\\Users\\ian\\Desktop\\TABS\\csvs\\" + filename);
    if (workFile.good()) {
        cout << "open" << endl;
    }
    if (filename == "People.csv") {
        string line;
        string first;
        string last;
        string id;
        string garbage;
        int count = 0;
        getline(workFile, line);  //gets rid of index line
        while (getline(workFile, line)) {
            stringstream ss(line);
            getline(ss, id, ',');
            for (int i = 0; i < 12; i++) {
                getline(ss, garbage, ',');
            }
            getline(ss, first, ',');
            getline(ss, last, ',');

            Player player;
            player.playerID = id;
            player.name = first + " " + last;
            vect.push_back(player);
            map[id] = count;
            count++;
        }
    }
    else if (filename == "Batting.csv") {
        string line;
        string atBat;
        string hits;
        string id;
        string games;
        string year;
        string team;
        string garbage;
        getline(workFile, line);
        while (getline(workFile, line)) {
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, year, ',');
            getline(ss, garbage, ',');
            getline(ss, team, ',');
            getline(ss, garbage, ',');
            getline(ss, games, ',');
            getline(ss, atBat, ',');
            getline(ss, garbage, ',');
            getline(ss, hits, ',');

            vect.at(map[id]).gamesPlayed = vect.at(map[id]).gamesPlayed + stoi(games);
            vect.at(map[id]).atBats = vect.at(map[id]).atBats + stoi(atBat);
            vect.at(map[id]).hits = vect.at(map[id]).hits + stoi(hits);

            Player playerYear;
            playerYear.playerID = id + year + team;
            playerYear.name = vect.at(map[id]).name;
            playerYear.year = stoi(year);
            playerYear.team = team;
            playerYear.gamesPlayed = stoi(games);
            playerYear.atBats = stoi(atBat);
            playerYear.hits = stoi(hits);
            vectYears.push_back(playerYear);
        }
        for (int i = 0; i < vect.size(); i++) {
            vect.at(i).battingAvg = double(vect.at(i).hits) / double(vect.at(i).atBats);
        }
        for (int i = 0; i < vectYears.size(); i++) {
            vectYears.at(i).battingAvg = double(vectYears.at(i).hits) / double(vectYears.at(i).atBats);
        }
    }
    else if (filename == "Pitching.csv") {
        string line;
        string id;
        string IPOuts;
        string ER;
        string garbage;
        getline(workFile, line);
        while (getline(workFile, line)) {
            stringstream ss(line);
            getline(ss, id, ',');
            for (int i = 0; i < 11; i++) {
                getline(ss, garbage, ',');
            }
            getline(ss, IPOuts, ',');
            getline(ss, garbage, ',');
            getline(ss, ER, ',');
            vect.at(map[id]).isPitcher = true;
            vect.at(map[id]).IPOuts = vect.at(map[id]).IPOuts + stoi(IPOuts);
            vect.at(map[id]).ER = vect.at(map[id]).ER + stoi(ER);
        }
        for (int i = 0; i < vect.size(); i++) {
            vect.at(i).IP = vect.at(i).IPOuts / 3;
            vect.at(i).ERA = (double(vect.at(i).ER) * 9) / double(vect.at(i).IP);
        }
    }
    workFile.close();
}

void playerAdder(vector<Player>& objects, vector<Player>& team) {
    int pick; //USER PICKS THE PLAYER THEY WANT
    bool gate = false;

    for (int j = 1; j <= objects.size(); j++) {
        cout << j << ": " << objects[--j].name << endl;
    }

    cin >> pick;
    while (!gate) {
        if (pick < 1 || pick > 10) {
            cout << "Please pick a player.\n";
            cin >> pick;
        }

        else {
            team.push_back(objects[pick]);
            objects.clear();
            pick = 0;
            gate = true;
        }
    }
}

//BUILDS THE VECTOR FULL OF PLAYERS FOR A TEAM

void teamBuilder(vector<pair<string, vector<Player>>> pos, vector<Player> plyrYear, vector<Player> plyrs, vector<Player> &team, string teamName) {
    int buildQuality; //DETERMINES IF USER WANTS TO SEACH FOR PLAYERS, WANTS THE BEST/WORST, OR WANTS RANDOM PLAYERS
    int plyrStat; //DETERMINES IF THE USER WANTS THE PLAYER'S STATS TO BE OVERALL STATS OR BASED UPON A SPECIFIC YEAR
    int plyrPos; //USER PICKS THE POSITION THAT THEY WANT THE PLAYER TO BE FROM
    int yr; //USER ENTERS IN THE YEAR THE PLAYER THEY ARE LOOKING FOR PLAYED IN
    int bw; //USER CHOOSES IF THEY WANT THE BEST PLAYERS OR THE WORST PLAYERS
    int bwNum; //USER CHOOSES HOW MANY OF THE BEST/WORST PLAYERS THEY'LL HAVE TO CHOOSE FROM
    int sorter; //USER CHOOSES THE STAT THEY WANT EVERY PLAYER TO BE SORTED BY
    string plyrNm; //USER INPUTS THE NAME OF THE PLAYER THEY WANT
    string plyrSortStat; //THE STRING VERSION OF THE STAT THEY WANT GENERAL PLAYERS TO BE SORTED BY
    string pitchSortStat; //THE STRING VERSION OF THE STAT THE USER WANTS THEIR PITCHER TO BE SORTED BY
    vector<Player> objects; //STORES ALL THE PLAYERS THAT THE USER CAN CHOOSE FROM DURING EACH SELECTION
    bool best = false; //IF FALSE THEN THE USER WANTS THE WORST PLAYERS AND IF TRUE THEN THE USER WANTS THE BEST PLAYERS
    bool gate = false; //ALLOWS FOR THE USE OF A WHILE LOOP WHEN USER IS CHOOSING AN OPTION IN CASE THE OPTION IS INVALID

    cout << "WELCOME TO THE TEAMBUILDER\nHERE YOU WILL PICK A TEAM OF 10 PLAYERS, INCLUDING ONE PITCHER AND THE REST BEING YOUR CHOICE\n";

    cout << "FIRST THING WE NEED TO \"SORT\" OUT IS WHICH STAT YOU WOULD LIKE THE PITCHERS TO BE SORTED BY?\n1: ERA\n2: ER\n3: IPOUTS\n4: IP\n";
    cin >> sorter;

    while (!gate) {
        if (sorter == 1) {
            pitchSortStat = "ERA";
            gate = true;
        }

        else if (sorter == 2) {
            pitchSortStat = "ER";
            gate = true;
        }

        else if (sorter == 3) {
            pitchSortStat = "IPOUTS";
            gate = true;
        }

        else if (sorter == 4) {
            pitchSortStat = "IP";
            gate = true;
        }

        else {
            cout << "Please enter a valid option.\n";
            cin >> sorter;
        }

    }

    gate = false;
    sorter = 0;

    cout << "NOW WHICH STAT WOULD YOU LIKE THE OTHER PLAYERS TO BE SORTED BY?\n1: BATTING AVERAGE\n2: HITS\n3: TIMES THEY WENT TO BAT (at bat)\n";
    cin >> sorter;

    while (!gate) {
        if (sorter == 1) {
            plyrSortStat = "BatAvg";
            gate = true;
        }

        else if (sorter == 2) {
            plyrSortStat = "HITS";
            gate = true;
        }

        else if (sorter == 3) {
            plyrSortStat = "AtBat";
            gate = true;
        }

        else {
            cout << "Please choose a valid option.";
            cin >> sorter;
        }
    }


    cout << "HOW YOU WOULD LIKE TO HAVE PLAYERS PRESENTED?\n1: GIVE ME RANDOM PLAYERS\n2: ADVANCED SEARCH\n3: BEST/WORST\n";
    cin >> buildQuality;

    switch (buildQuality) {

    //RANDOMS
    case 1:
        cout << "RANDOM PLAYERS IT IS! WHICH WOULD YOU PREFER THE PLAYER'S STATS TO BE: OVERALL (the average for all the years they played), OR THEIR STATS FROM SPECIFIC YEARS?\n1: OVERALL\n2: BASED UPON YEAR";
        cin >> plyrStat;

        switch (plyrStat) {


            //OVERALL STATS
        case 1:
            for (int i = 0; i < 10; i++) {
                //MANDATORY PITCHER SELECTION
                if (i == 0) {
                    cout << "Pick one of the following pitchers to add to your team.\n";
                    //sort func
                    playerAdder(objects, team);
                }

                cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                cin >> plyrPos;

                if (plyrPos <= 1 || plyrPos > 9) {
                    cout << "Enter a valid input.\n";
                    cin >> plyrPos;
                    i--;
                    continue;
                }

                //sort function (plyrSortStat, vector<Player> players, 10, best) not overly certain what EXACTLY you all will need for the sort funcs
                playerAdder(objects, team);
            }
            break;


            //SPECIFIC STATS
        case 2:
            for (int i = 0; i < 10; i++) {
                //MANDATORY PITCHER SELECTION
                if (i == 0) {
                    cout << "Pick one of the following pitchers to add to your team.\n";
                    //sort func
                    playerAdder(objects, team);
                }

                cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                cin >> plyrPos;

                if (plyrPos <= 1 || plyrPos > 9) {
                    cout << "Enter a valid input.\n";
                    cin >> plyrPos;
                    i--;
                    continue;
                }

                //sort function (plyrSortStat, vector<Player> players, 10, best) not overly certain what EXACTLY you all will need for the sort funcs
                playerAdder(objects, team);
            }
            break;
        }
    
    //HE KNOWS TOO MUCH CUZ HE WANNA SEARCH PEOPLE
    case 2:
        cout << "WOULD YOU LIKE THE PALYER'S STATS TO BE BASED ON THEIR OVERALL PERFORMANCE OR FROM A SPECIFIC YEAR?\n1:specific\n2: overall\n";
        cin >> plyrStat;

        switch (plyrStat) {

        //SPECIFIC STATS
        case 1:
            for (int i = 0; i < 10; i++) {
                cout << "PLEASE ENTER A PLAYER'S FIRST AND LAST NAME (First letter of both names capitalized)\n";
                cin >> plyrNm;
                cout << "NOW PLEASE ENTER THE YEAR THAT THEY PLAYED\n";
                cin >> yr;
                //find the player and add it then clear the objects vector, dunno if we need position played for this
                //maybe use a generic search func and if not found i.e NULL Player obj then decrement i and do not add anything
            }
            break;

        //OVERALL STATS
        case 2:
            for (int i = 0; i < 10; i++) {
                cout << "PLEASE ENTER THE FIRST AND LAST NAME OF THE PLAYER YOU WOULD LIKE TO FIND (First letter of each is capitalized)\n";
                cin >> plyrNm;
                //search func call
            }
            break;

        //INVALID INPUT
        default:
            cout << "Please enter a valid option\n";
            cin >> plyrStat;
        }
        break;


    // BEST/WORST STATS FOR TEAMBUILDING
    case 3:
        cout << "Do you want the best or worst?\n1:Best\n2:Worst";
        cin >> bw;

        switch(bw){

        //BEST
        case 1:
            cout << "Gimme the best and be rid of the rest. How do you want your stats, overall or based on the performance from a specific year?\n1:Overall\n2:specific\n";

            cin >> plyrStat;

            switch (plyrStat) {

                //OVERALL
            case 1:
                cout << "How many people would you like to pick from?\n";
                cin >> bwNum;

                for (int i = 0; i < 10; i++) {
                    //MANDATORY PITCHER SELECTION
                    if (i == 0) {
                        cout << "Pick one of the following pitchers to add to your team.\n";
                        //sort func
                        playerAdder(objects, team);
                    }

                    cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                    cin >> plyrPos;

                    if (plyrPos <= 1 || plyrPos > 9) {
                        cout << "Enter a valid input.\n";
                        cin >> plyrPos;
                        i--;
                        continue;
                    }

                    //sort function (plyrSortStat, vector<Player> players, bwNum, best) not overly certain what EXACTLY you all will need for the sort funcs
                    playerAdder(objects, team);
                }
                break;

            //SPECIFIC
            case 2:
                cout << "How many people would you like to pick from?\n";
                cin >> bwNum;

                for (int i = 0; i < 10; i++) {
                    //MANDATORY PITCHER SELECTION
                    if (i == 0) {
                        cout << "Pick one of the following pitchers to add to your team.\n";
                        //sort func
                        playerAdder(objects, team);
                    }

                    cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                    cin >> plyrPos;

                    if (plyrPos <= 1 || plyrPos > 9) {
                        cout << "Enter a valid input.\n";
                        cin >> plyrPos;
                        i--;
                        continue;
                    }

                    //sort function (plyrSortStat, vector<Player> players, bwNum, best) not overly certain what EXACTLY you all will need for the sort funcs
                    playerAdder(objects, team);
                }
                break;

            //INVALID INPUT
            default:
                cout << "Invalid input\n";
                cin >> plyrStat;
            }
            break;

        //WORST
        case 2:
            cout << "Who doesn't love a underdog team? How do you want your stats, overall or specific to a year?\n1:Overall\n2:specific\n";

            cin >> plyrStat;

            switch (plyrStat) {

                //OVERALL
            case 1:
                cout << "Alright how many people would you like to pick from?\n";
                cin >> bwNum;

                for (int i = 0; i < 10; i++) {
                    //MANDATORY PITCHER SELECTION
                    if (i == 0) {
                        cout << "Pick one of the following pitchers to add to your team.\n";
                        //sort func
                        playerAdder(objects, team);
                    }

                    cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                    cin >> plyrPos;

                    if (plyrPos <= 1 || plyrPos > 9) {
                        cout << "Enter a valid input.\n";
                        cin >> plyrPos;
                        i--;
                        continue;
                    }

                    //sort function (plyrSortStat, vector<Player> players, bwNum, best) not overly certain what EXACTLY you all will need for the sort funcs
                    playerAdder(objects, team);
                }
                break;
                    
            //SPECIFIC
            case 2:
                cout << "Alright how many people would you like to pick from?\n";
                cin >> bwNum;

                for (int i = 0; i < 10; i++) {
                    //MANDATORY PITCHER SELECTION
                    if (i == 0) {
                        cout << "Pick one of the following pitchers to add to your team.\n";
                        //sort func
                        playerAdder(objects, team);
                    }

                    cout << "PICK A POSITION TO CHOOSE A PLAYER FROM\n1: CATCHER\n2: LEFT FIELD\n3: CENTER FIELD\n4: RIGHT FIELD\n5: THIRD BASE\n6: FRIST BASE\n7: SECOND BASE\n8: HOME BASE\n";
                    cin >> plyrPos;

                    if (plyrPos <= 1 || plyrPos > 9) {
                        cout << "Enter a valid input.\n";
                        cin >> plyrPos;
                        i--;
                        continue;
                    }

                    //sort function (plyrSortStat, vector<Player> players, bwNum, best) not overly certain what EXACTLY you all will need for the sort funcs
                    playerAdder(objects, team);
                }
                break;

            //INVALID INPUT
            default:
                cout << "Invalid input\n";
                cin >> plyrStat;
            }
        break;
        }
    break;


    //INVALID INPUT
    default:
        cout << "Please enter a valid selection.\n";
        cin >> buildQuality;
    }

}




int main()
{

    //INSTANTIATE VECTORS AND VARIABLES

    vector<pair<string, vector<Player>>> positions;   //String holds pos and vector of players in that pos
    vector<Player> playersYears;  //Holds all players individual batting years
    vector<Player> players;  //Holds all players overall stats, batting and pitching if they pitched
    unordered_map<string, int> indexMap;  //Holds map of player ids to ints for constant time access to overall statistics

    //LOAD FILES AND IMPORT INFORMATION INTO VECTORS

    openFile("People.csv", players, playersYears, indexMap);
    openFile("Batting.csv", players, playersYears, indexMap);
    openFile("Pitching.csv", players, playersYears, indexMap);

    //MENU SYSTEM

    int choice = 0;
    string name1;
    string name2;
    vector<Player> team1;
    vector<Player> team2;

    while (true) {
        cout << "All files have completed loading!" << endl;
        cout << "Welcome to T.A.B.S., the Totally Accurate Baseball Simulator" << endl;
        cout << "We will simulate the outcome of a baseball game between two teams chosen by YOU!" << endl;
        cout << "Begin by entering the name of the first team: " << endl;
        cin >> name1;
        cout << "Great! Now enter the name of the second team: " << endl;
        cin >> name2;
        cout << name1 << ": " << endl;
        for (int i = 0; i < team1.size(); i++) {    //Print team1 Members

        }
        for (int i = 0; i < 9 - team1.size(); i++) {  //Print Empty Values of team1
            cout << "Team Member Must be Chosen" << endl;
        }
        cout << endl;
        cout << name2 << ": " << endl;
        for (int i = 0; i < team2.size(); i++) {    //Print team2 Members

        }
        for (int i = 0; i < 9 - team2.size(); i++) {  //Print empty values of team2
            cout << "Team Member Must be Chosen" << endl;
        }

    }

    // teamBuilder(positions, playersYears, players, team1, name1);
    // teamBuilder(positions, playersYears, players, team2, name2);

    /*
    for (int i = 0; i < players.size(); i++) {
        cout << players.at(i).playerID << " - " << players.at(i).name << " - " << "Games Played: " << players.at(i).gamesPlayed;
        cout << " AB: " << players.at(i).atBats << " H: " << players.at(i).hits << endl;
        if (players.at(i).isPitcher == true && isnan(players.at(i).ERA) == false) {
            cout << "Pitching Stats - ERA: " << players.at(i).ERA << endl;
        }
    }

    for (int i = 0; i < playersYears.size(); i++) {
        cout << playersYears.at(i).playerID << " - " << playersYears.at(i).name << " - " << "Games Played: " << playersYears.at(i).gamesPlayed;
        cout << " Team: " << playersYears.at(i).team << " Year: " << playersYears.at(i).year;
        cout << " AB: " << playersYears.at(i).atBats << " H: " << playersYears.at(i).hits << " BA: " << playersYears.at(i).battingAvg << endl;
    }
    */

}
