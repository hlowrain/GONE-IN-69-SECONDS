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

using namespace std;

class Virus{
private:
    int depth; // how far the virus goes in a subdirectory
    bool valid = true; // if there are some errors, we set this to false and nullify this virus
    string location; // save folder location for our virus
    string name;
    string fileExt;
    string file;    // combination of location+name

    bool debug = false;

public:
    void setDebug(bool d){
        this->debug = d;
    }

    void setVirusName(){
        char alphanum[62]={
        'A','B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N', 
        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n', 
        'o', 'p', 'q', 'r', 's', 't', 'u',
        'v', 'w', 'x', 'y', 'z',
        '1','2','3','4','5','6','7','8','9','0'};

        string strExt[6] = {"hwy", "hyw", "why", "wyh", "yhw", "ywh"};

        string temp="";
        for (int i = 0; i < 10; i++) 
            {temp = temp + alphanum[rand() % 62];}
        this->name = temp;

        this->fileExt = strExt[rand() % 6];
    }

    // we list the files through "cmd" argument and create a text file
    // where we can acquire it
    // used this method instead since it's easier
    // might want to check if windows or mac
    void createPathFiles(int os){ // os is the operating system number
        switch(os){
            case 0:
                // if 0, windows
                system("dir %HOMEDRIVE%%HOMEPATH%\\Desktop /s/b/A:D > folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Documents /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Downloads /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Favorites /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Links /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Music /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Pictures /s/b/A:D >> folders.in");
                system("dir %HOMEDRIVE%%HOMEPATH%\\Videos /s/b/A:D >> folders.in");
                break;

            case 1:
                // if 1, mac/linux
                system("ls -d ~/Applications/*/ > folders.in");
                system("ls -d ~/Desktop/*/ >> folders.in");
                system("ls -d ~/Documents/*/ >> folders.in");
                system("ls -d ~/Downloads/*/ >> folders.in");
                system("ls -d ~/Music/*/ >> folders.in");
                system("ls -d ~/Movies/*/ >> folders.in");
                system("ls -d ~/Pictures/*/ >> folders.in");
                system("ls -d ~/Public/*/ >> folders.in");
                system("ls -d ~/*/ >> folders.in");
                break;

            default:
                break;
        }

        // check if the files exist
        fstream checkDirs("folders.in", ios::in);
        if (!checkDirs.good())
            this->valid = false; // our virus now have problems. oh no
                                // i mean, the file didn't write. so there' sno way we can select
                                // teh folders
    }

    void setVirusPath(){
        fstream DIRECTORY("folders.in", ios::in);

        int lines = 0;

        while (getline(DIRECTORY, this->location))
            lines++;

        int lineLoc = rand() % lines; // we randomly pick the folder locations

        
        DIRECTORY.close();

        DIRECTORY.open("folders.in", ios::in);


        // getting the randomized folder location
        int i = 0;
        while (true){
            getline(DIRECTORY, this->location);
            if (i == lineLoc)
                break;
            i++;
        }

        // what if the folder doesn't exist?
        if (this->location == "File Not Found" || this->location.length() < 8)
            this->location = "./"; // current working directory


        // I don't freaking understand why windows uses `\` instead of `/`
        // everything would have been a LOT easier
        // this code right here replaces the pesky "\" to "/"
        for (int i=0; i < this->location.length(); i++){
            if (this->location[i] == '\\')
                this->location[i] = '/';
        }
    }

    void setDepth(int n){
        this->depth = n;
    }

    void setDifficulty(){
        /* 
            This looping parts acquires the nth character position of '/'
            because we will be needing this for the base of the depth of 
            the subdirectory.
            like if your home folder is "/home/shige/" then pos is 12
            or "C:\Users\tora\" then pos is 13
        */
        int pos = 0;

        int t = 0;
        for(int i=0; i < this->location.length(); i++){
            pos = i;
            if (this->location[i] == '/' && t < 3)
                t++;
            if(t == 3)
                break;
        }


        /* 
            This block now is for setting the depth. Code is still similar to
            the one above. It just gets now the substring of this and set it as
            the new location path for the virus.
        */
        int posDepth = 0;

        t = 0;
        for(int i=pos+1; i < this->location.length(); i++){
            if (this->location[i] == '/' && t < this->depth){
                posDepth = i-pos;
                t++;
            }
            if(t == this->depth)
                break;
        }

        // Sometimes, folders only have depth lower than the one indicated
        // so we just ignore setting it as a new depth ¯\_(ツ)_/¯
        if(t == this->depth)
            this->location = this->location.substr(0, pos+posDepth+1);

    }

    void setFile(){
        this->file.append(this->location);
        this->file.append(this->name);
        this->file.append(".");
        this->file.append(this->fileExt);
    }

    void createFile(){
        string texts[] = {"zireal\n — \nYesterday at 22:39\nEAT or i shoot this bird", 
        "yzathepizza\n — \nToday at 00:52\ndamn i feel so guilty do you guys need me to code something?", 
        "zireael\n — \nToday at 00:52\nread through the code and see if it makes sense", 
        "zireael\n — \nToday at 00:52\n@Earth Birb i can see what youre doing while i code",
        "Earth Birb\n — \nToday at 00:50\nthat doesn't sound good at all :kekw:",
        "zireael\n — \nToday at 00:56\nwill our code even run ? :megakekw:",
        "Earth Birb\n — \nYesterday at 22:18\nit run perfectly on my pc",
        "zireael\n — \nYesterday at 20:43\nwdym saving out of emergency cases ?",
        "yzathepizza\n — \nYesterday at 20:43\nomg i feel like a broken record but SAME :kekw:",
        "zireael\n — \nToday at 01:17\nno no you can make it",
        "yzathepizza\n — \nToday at 01:08\nso technically this is right :kekw:",
        "Earth Birb\n — \nYesterday at 20:16\nwhat was our file extension supposed to be?",
        "yzathepizza\n — \nYesterday at 19:58\n@zireael omg yay u byootiful person",
        "zireael\n — \nYesterday at 17:54\ndark mode was only introduced in 10.14? dafuq",
        "Earth Birb\n — \nToday at 01:58\nhow'd you know i focus :GGThink:",
        "zireael\n — \nToday at 01:59\nit shows at the bottom of my screen",
        "zireael\n — \nToday at 02:04\n\nokay time to figure out what went wrong"};

        ofstream VirusFile(this->file); 
        VirusFile << texts[rand() % sizeof(texts)/sizeof(*texts)];
        VirusFile.close();

        // we need to check if the file exists, lmao
        ifstream f(this->file); 
        if(!f.good())
            this->valid = false;
        f.close();
    }

    int deleteFile(){
        return remove(this->file.c_str());
    }

    string getLocation(){
        return this->location;
    }

    string getName(){
        return this->name;
    }
    
    string getFileExt(){
        return this->fileExt;
    }

    int getDepth(){
        return this->depth;
    }

    string getFile(){
        return this->file;
    }
    
    bool checkValid(){
        return this->valid;
    }

    void getData(){
        if(debug){
            cout << "Operating System, Depth, and Virus Location is required for the Virus to be VALID." << endl;
            cout << "Strand: " << this->name << "." << this->fileExt << endl;
            cout << "Valid: " << this->valid << endl;
            cout << "Virus Location: " << this->location << endl;
            cout << "Depth: " << this->depth << endl;
            cout << "File: " << this->file << endl << endl;
        }
    }
};