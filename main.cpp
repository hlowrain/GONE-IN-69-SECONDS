#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <thread>
#include <sstream>
#include <chrono>
#include <sys/stat.h>
#include "./tabulate.hpp"
#include "./virusClass.cpp"

using namespace std;
using namespace tabulate;


int systemChecker(){
    #ifdef _WIN32 //windows
        return 0;

    #elif __APPLE__ //mac
        return 1;

    #elif TARGET_OS_MAC //mac
        return 1
    #else //na
        return -1;

    #endif
        return 0;
}

//clear screen
void cleanConsole(){
    int os = systemChecker();
    if(!os)
        system("cls");
    if(os)
        system("clear");
}

void clearScreen(){
    int clear = 10;
    do {
        cout << endl;
        clear -= 1;
    } while (clear !=0);
}

void pressAnyKey(string words){
    cout << "\n\n" << words << endl;
    cin.get();
    clearScreen();
}

struct Info{
    int id;
    int score;
    string name;
    int level;
};
typedef struct Info Info;



// =============================== LEADERBOARD =============================== //


class Leaderboard{
private:
    int id;
    int score;
    string name;
    int level;

public:
    //checks total players in leaderboard
    int length(){
        int count = 0;
        fstream file;
        file.open("leaderboard.dat", ios::in);
        while (!file.eof()){
            file >> id >> score >> name >> level;
            count++;
        }
        file.close();
        return count;
    }

    //checks if userID inputted already exists in leaderboard
    bool check(int number){
        fstream file;
        file.open("leaderboard.dat", ios::in);

        if (!file){
            file.close();
            return false;
        }

        file >> id >> score >> name >> level;
        while (!file.eof()){
            if (id == number){
                file.close();
                return true;
            }
            file >> id >> score >> name >> level;
        }

        file.close();
        return false;
    }

    //checks level of user
    int checkLevel(int userid){
        fstream file;
        file.open("leaderboard.dat",ios::in);

        file >> id >> score >> name >> level;
        while (!file.eof()){
            if (id == userid){
                file.close();
                return level;
            }
            file >> id >> score >> name >> level;
        }

        return 1;
    }

    //checks score of user
    int checkScore(int userid){
        fstream file;
        file.open("leaderboard.dat",ios::in);

        file >> id >> score >> name >> level;
        while (!file.eof()){
            if (id == userid){
                file.close();
                return score;
            }
            file >> id >> score >> name >> level;
        }
        return 0;
    }

    //checks if returning user id exists, then returns inputted userID as current user
    int login(int userid){
        if (check(userid)){
            return userid;
        }
        else{
            Table dne;
            dne.format().hide_border().width(100);
            dne.add_row({"User does not exist.\n\n"});

            cout << dne;
            return 0;
        }
    }

    //new user
    int addUser(){
        bool duplicate;
        fstream file;
        int the_id;
        
        Table enterid;
        enterid
            .format()
            .hide_border()
            .font_color(Color::magenta)
            .width(100);
        enterid.add_row({"Enter User ID:"});

        do{
            cout << endl << enterid << endl;
            cin >> the_id;
            duplicate = check(the_id);
        }while(duplicate == true);

        Table entername;
        entername
            .format()
            .hide_border()
            .font_color(Color::magenta)
            .width(100);
        entername.add_row({"Enter Name (max. 15 char):"});

        do{
            cout << endl << entername << endl;
            cin >> name;            
            int c = 999;
            while (c != '\n' && c != EOF)
                c = getchar();
            
        }while(name.length() > 15); 

        level = 1;
        score = 0;

        file.open("leaderboard.dat", ios::app | ios::out);
        file << " " << the_id << " " << score << " " << name << " " << level << "\n";
        file.close();

        return the_id;
  }


    //prints leaderboard according to score 
    void printsorted(){
        fstream file;
        file.open("leaderboard.dat", ios::in);

        int n = length();
        int x = 0;
        Info* player = (Info*)calloc(n,sizeof(Info));
        Info tmp;
        
        file >> id >> score >> name >> level;
        while (!file.eof()){
            player[x].id = id;
            player[x].score = score;
            player[x].name = name;
            player[x].level = level;
            file >> id >> score >> name >> level;
            x++;
        }

        for (int i=0; i<n; i++){
            for (int j=i+1; j<n; j++){
                if (player[i].score < player[j].score){
                    tmp = player[i];
                    player[i] = player[j];
                    player[j] = tmp;
                }
            }
        }

    //print sorted leaderboard
    cout << "\tUSER ID\t\t\tNAME \t\t\t\tLEVEL \t\t\tSCORE\n" << endl;

    for (int i=0; i<n; i++){
        if(player[i].id == 0){
            cout << endl;
        }
        else{
            cout << "\t" << player[i].id;
            if(player[i].name.length() == 15){
                cout << "\t" << player[i].name;
            }
            else{                       
                cout << "\t\t\t" << player[i].name;
                int length = player[i].name.length(); 
                for (int i=0; i<(15-length); i++){
                    cout << " ";
                }//keeps adding spaces 15-lengthofusername times
            }
        
            cout<<"\t\t\t"<<player[i].level;
            cout<<"\t\t\t"<<player[i].score<<endl<<endl;}
        }

        file.close();
        pressAnyKey("Press <Enter> key to return to the main menu . . .");
    }


    //updates score & level of user in file
    void updateScore(int userid, int tscore){
        fstream file, file1;
        file.open("leaderboard.dat", ios::in);
        file1.open("lbcopy.dat", ios::app | ios::out);

        file >> id >> score >> name >> level;
        while (!file.eof()){
            if (id == userid){
                score += tscore;
                level = (score/69) + 1;
                file1 << " " << id << " " << score << " " << name << " " << level << "\n";
            }
            else{
                file1 << " " << id << " " << score << " " << name << " " << level << "\n";
            }
            file >> id >> score >> name >> level;
        }

        file1.close();
        file.close();
        remove("leaderboard.dat");
        rename("lbcopy.dat", "leaderboard.dat");
    }
    

    void printlead(){
        cleanConsole();
        Table leaderboard;
        leaderboard 
            .format()
            .font_align(FontAlign::center)
            .border_color(Color::magenta)
            .width(100);
        leaderboard.add_row({"L E A D E R B O A R D"});

        cout << leaderboard << endl;
        printsorted();
        cin.ignore();
  }
  
};

// =============================== GAME FUNCTIONS =============================== //

void congrats(){
    cleanConsole();
    clearScreen();
    Table congrats;
    congrats
        .format()
        .font_align(FontAlign::center)
        .border_color(Color::green)
        .width(100);
    congrats.add_row({"\nC O N G R A T U L A T I O N S\n\n"});
    congrats.add_row({"That took a while, but you did it! You beat the game!"
                        " You are a god among men. The devs bow down to your greatness."
                        " Hopefully this taught you how to be vigilant against malwares!"});
    congrats.add_row({"\n"});
    congrats[1].format().hide_border_top();
    congrats[2].format().hide_border_top();

    cout << congrats << endl;

    cout << "Press <Enter> to return to main menu...";
    cin.ignore();    
    cin.ignore();    
    cleanConsole();

}


//display outcome of the game
void outcome(int rem_time, int secsgiven, int id, Leaderboard lb){
    
    //show if the player won or lost
    Table result;
    result
        .format()
        .font_align(FontAlign::center)
        .width(100);
    
    if (rem_time == 0){
        result.format().border_color(Color::red);
        result.add_row({"\n\nY O U   L O S T   T H I S   R O U N D\n\n"});
        result.add_row({"Oh no!! You failed to deleted the virus!\n"
                        "Better luck next time.\n\n"});
    }

    else{
        result.format().border_color(Color::green);

        string timeleft = to_string(rem_time);
        timeleft = "Time Left: " + timeleft + " seconds\n\n";

        result.add_row({"\n\nY O U   W O N   T H I S   R O U N D\n\n"});
        result.add_row({"Congratulations!!! You have successfully "
                        "eradicated the virus.\n\n"});
        result.add_row({timeleft});

        result[2].format().hide_border_top();
    }

    result[1].format().hide_border_top();

    cleanConsole();
    clearScreen();
    cout << result << endl << endl;
    pressAnyKey("Press <Enter> key to continue . . .");


    //show the player's new score
    Table score;
    score
        .format()
        .font_align(FontAlign::center)
        .border_color(Color::green)
        .width(100);

    if (rem_time == 0){
        score.format().border_color(Color::red);
        score.add_row({"\n\nSince you lost, your score is zero :(\n\n"});
    }

    else{
        cleanConsole();
        clearScreen();
        lb.updateScore(id,rem_time);

        score.format().border_color(Color::green);
        score.add_row({"\n\nYour new score is:\n"});

        int newscore = lb.checkScore(id);
        string str_score = to_string(newscore);
        
        score.add_row({str_score});
        score[1]
            .format()
            .font_style({FontStyle::bold})
            .hide_border_top();

        score.add_row({"\n\n"});
        score[2].format().hide_border_top();
    }

    cleanConsole();
    clearScreen();
    cout << score << endl << endl;

    int highscore = lb.checkScore(id);
    if (highscore > 345){
        congrats();
        return;
    }
    else{pressAnyKey("Press <Enter> key to return to the main menu . . .");}
}


//display the main menu
int mainmenu(){
    Table mainmenu;
    mainmenu
        .format()
        .font_align(FontAlign::center)
        .border_color(Color::cyan)
        .width(100);

    mainmenu.add_row({"\nG O N E   I N   6 9   S E C O N D S\n\n"});
    mainmenu[0]
        .format()
        .font_style({FontStyle::bold, FontStyle::italic})
        .font_color(Color::magenta);


    Table m_options;
    m_options.format().font_align(FontAlign::center);
    m_options.add_row({"\n\n"});
    m_options[0].format().hide_border();

    m_options.add_row({"\n1. New Game\n\n"});
    m_options.add_row({"\n2. Continue Game\n\n"});
    m_options.add_row({"\n3. View Leaderboard\n\n"});
    m_options.add_row({"\n4. Help\n\n"});
    m_options.add_row({"\n5. Quit\n\n"});
    
    mainmenu.add_row({m_options});
    mainmenu[1].format().font_color(Color::yellow);
    mainmenu.add_row({"\n\n"});
    mainmenu[2].format().hide_border_top();


    //player chooses from the menu options
    int choice = 0;
    do{
        cout << endl << mainmenu << "\n\n" << endl;
        cin >> choice;
        if (choice < 1 || choice >5 ){
            if(choice == 69){
                return choice;
            }else{
                clearScreen();
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }while (choice < 1 || choice > 5);
    return choice;
}


//display some stuff before a game starts
int pregame(int id, Leaderboard p){ 

    //check player's current level
    int currentlvl = p.checkLevel(id);

    //tell the player what level they are
    Table showlvl;
    showlvl
        .format()
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .border_color(Color::white)
        .width(100);

    showlvl.add_row({"\n\n\n\n\n"});
    showlvl[0].format().hide_border_bottom();
    switch (currentlvl){
        case 1:
            showlvl.add_row({"YOU ARE CURRENTLY AT LEVEL 1"});
            break;
        case 2:
            showlvl.add_row({"YOU ARE CURRENTLY AT LEVEL 2"});
            break;
        case 3:
            showlvl.add_row({"YOU ARE CURRENTLY AT LEVEL 3"});
            break;
        case 4:
            showlvl.add_row({"YOU ARE CURRENTLY AT LEVEL 4"});
            break;
        case 5:
            showlvl.add_row({"YOU ARE CURRENTLY AT LEVEL 5"});
            break;
        default:
            return 6;
    }
    showlvl[1].format().hide_border_top();
    showlvl.add_row({"\n\n\n\n\n"});
    showlvl[2].format().hide_border_top();

    cleanConsole();
    clearScreen();
    cout << endl << showlvl;
    cin.ignore();
    pressAnyKey("Press <Enter> key to continue . . .");
    

    //inform the player about the virus and the fake files
    Table vinfo;
    vinfo 
        .format()
        .font_align(FontAlign::center)
        .border_color(Color::red)
        .width(100);
    
    vinfo.add_row({"\n\n"});
    vinfo.add_row({"Oh no! A virus is hiding within your folders!"});

    if (currentlvl > 1){
        vinfo.add_row({"\n\n"});
    }

    switch (currentlvl){
        case 2:
            vinfo.add_row({"It has created 2 fake files to throw you off."});
            break;
        case 3:
            vinfo.add_row({"It has created 4 fake files to throw you off."});
            break;
        case 4:
            vinfo.add_row({"It has created 6 fake files to throw you off."});
            break;
        case 5:
            vinfo.add_row({"It has created 10 fake files to throw you off."});
            break;
    }
    
    if (currentlvl > 1){
        vinfo.add_row({"\n\n"});
        vinfo.add_row({"The real virus file, as well as the fake files,"
                    " are inside your common directories."});
    }
    vinfo.add_row({"\n\n"});

    switch (currentlvl){
        case 1:
            vinfo.add_row({"You have 99 seconds to find and delete the virus file,"
                        " or else your computer goes KA-BOOM!!!"});
            break;
        case 2:
            vinfo.add_row({"You have 89 seconds to find and delete the virus file,"
                        " or else your computer goes KA-BOOM!!!"});
            break;
        case 3:
            vinfo.add_row({"You have 79 seconds to find and delete the virus file,"
                        " or else your computer goes KA-BOOM!!!"});
            break;
        case 4:
            vinfo.add_row({"You have 69 seconds to find and delete the virus file,"
                        " or else your computer goes KA-BOOM!!!"});
            break;
        default:
            vinfo.add_row({"You have 69 seconds to find and delete the virus file,"
                        " or else your computer goes KA-BOOM!!!"});
    }

    vinfo.add_row({"\n\n"});
    vinfo.add_row({"DISCLAIMER: We are not responsible for any personal or"
                " important files that you may accidentally delete!"
                " (Tip: You can always recover them from your trash bin though)"});
    vinfo.add_row({"\n\n"});

    if (currentlvl > 1){
        vinfo[9].format().font_style({FontStyle::italic});
        vinfo[10].format().hide_border_top();
        for (int i=1; i<10; i++){
            vinfo[i].format().hide_border_top();
        }
    }
    else {
        vinfo[5].format().font_style({FontStyle::italic});
        vinfo[6].format().hide_border_top();
        for (int i=1; i<6; i++){
            vinfo[i].format().hide_border_top();
        }
    }

    cout << endl << vinfo << endl << endl;
    pressAnyKey("Press <Enter> key to start the game . . .");
    return currentlvl;
}


//the actual start of the game
void gameStart(Leaderboard LB, int LEVEL, int ID, bool DEBUG = false){
    cleanConsole();
    int OS = systemChecker();
    Virus REAL;
    bool DELETED = false;  //initially the virus file exists so it is NOT deleted
    int CDOWN;      //used when calculating the current second in the countdown
    int DURATION;   //how long the countdown is
    string S_CDOWN; //used when converting CDOWN to string (so it can be tabulated)

    // REAL VIRUS init
    REAL.setVirusName();
    REAL.createPathFiles(OS);
    REAL.setVirusPath();
    REAL.setDepth(rand() % LEVEL + 1);          
    REAL.setDifficulty();      
    REAL.setFile();            
    REAL.createFile();         
    REAL.setDebug(DEBUG); 
    REAL.getData();  

    if(REAL.checkValid()){
        // fake virus count
        Virus FAKE[10];
        int fv = 0;
        switch(LEVEL){
            case 1:
                DURATION = 99;
                break;

            case 2:
                DURATION = 89;
                fv = 2;
                break;

            case 3:
                DURATION = 79;
                fv = 4;
                break;

            case 4:
                fv = 6;
                DURATION = 69;
                break;

            default:
                fv = 10;
                DURATION = 69;
        }

        for(int i=0; i<fv; i++){
            FAKE[i].setVirusName();
            FAKE[i].createPathFiles(OS);
            FAKE[i].setVirusPath();
            FAKE[i].setDepth(rand() % LEVEL + 1);          
            FAKE[i].setDifficulty();      
            FAKE[i].setFile();            
            FAKE[i].createFile();       
            FAKE[i].setDebug(DEBUG);  
            FAKE[i].getData();
        }


        //current time
        auto start = chrono::high_resolution_clock::now();
        //also current time
        time_t tstart = time(NULL);
        //how long the countdown is
        time_t waitTime = DURATION;

        //this is used for checking if the file is deleted
        struct stat buffer;
        while (time(NULL) < tstart + waitTime && !DELETED){
            for (int i=0; i<DURATION; ++i){
                if (stat(REAL.getFile().c_str(), &buffer) == -1){
                    DELETED = true;
                    //if deleted then exit the for loop
                    break;
                }

                Table countdown;
                countdown
                    .format()
                    .font_align(FontAlign::center)
                    .hide_border()
                    .font_color(Color::red)
                    .font_style({FontStyle::bold})
                    .width(100);

                //calculate the current second in the countdown
                CDOWN = DURATION-i;
                //convert second (int) to string
                S_CDOWN = to_string(CDOWN);
                //tabulate the second
                countdown.add_row({S_CDOWN});
                //print the second
                cout << countdown << endl;
                //sleep for one second
                this_thread::sleep_until(start + (i+1)*chrono::seconds(1));
            }
        }

        //current time
        auto t2 = chrono::high_resolution_clock::now();
        //how long the countdown lasted
        auto s_int = chrono::duration_cast<chrono::seconds>(t2-start);
        //remaining time left in the countdown
        int timeleft = DURATION - s_int.count();

        outcome(timeleft, DURATION, ID, LB); 

        REAL.deleteFile();
        for(int i=0; i<fv; i++)
            FAKE[i].deleteFile();
    }
    else{gameStart(LB, LEVEL, ID, DEBUG);}
}

void help(){
    cleanConsole();
    Table help;
    help
        .format()
        .font_align(FontAlign::center)
        .border_color(Color::red)
        .width(100);
    help.add_row({"\nA B O U T  T H E  G A M E\n\n"});
    help.add_row({"The player's system is infected by a \"virus,\" and the player must get rid of it."
                " The player has to search under time pressure for the virus itself."
                " The virus has an uncommon file extension (.why,.yhw,.hwy,.ywh., etc.) "
                "and is hidden in one of the player's home subdirectories."}); 
    help.add_row({"\n"});
    help.add_row({"When the player thinks they have found the correct file, they must delete the virus." 
                " The game ends when the player has deleted the virus or when the countdown is over."
                " If they deleted the correct file, the remaining seconds will be the player’s score.   "});
    help.add_row({"\n"});
    help.add_row({"As the player levels up, the virus gets worse, meaning there will be more fake files and a shorter countdown. " 
                " The higher the level, the higher the scores the player will have to gain for them to advance to the next level."});
    help.add_row({"\n"});
    help.add_row({"For more information about the game, visit http://bit.ly/GoneIn69S"});

    for(int i=1;i<8;i++){
    help[i].format().hide_border_top();}

    cout << help << endl;
    cout << "Press <Enter> to continue...";
    cin.ignore();
    cin.ignore();
    cleanConsole();
};


int main(){
    cleanConsole();
    srand(time(NULL)); 
    int os = systemChecker();
    int choice,currentid,uid,level,score;
    bool DEBUG = false;

    Leaderboard player;
    do{
        choice = mainmenu();
        switch(choice){
            case 1:{    //new game
                currentid = player.addUser(); 
                level = pregame(currentid,player);
                gameStart(player,level,currentid, DEBUG);
                break;}

            case 2:{    //return game
                do{
                    Table askid;
                    askid
                        .format()
                        .hide_border()
                        .font_color(Color::magenta)
                        .width(100);

                    askid.add_row({"Enter Returning User ID:"});
                    cout << endl << askid << endl;
                    cin >> uid;
                    currentid = player.login(uid);
                }while(currentid == 0);
                
                level = pregame(currentid,player);
                if(level<=5){gameStart(player,level,currentid, DEBUG);}
                else{congrats();}
                break;}
            case 3: {   //print leaderboard
                int registered=player.length();
                if(registered>0){player.printlead();}
                else{cout<<"No registered user."<<endl;}
                break;
                }
            case 4:{ //print instructions
                help();
                break;
            }
            case 69: { //special debug function - shows locations of virus
                cout << "Debug: " << DEBUG << endl;
                cout << "Switch debug? (ON - 1  | OFF - 0)" << endl;
                cin >> DEBUG;
                break;
            }
            
            default:
                break;
        }
    }while(choice != 5); 

    return 0;
}