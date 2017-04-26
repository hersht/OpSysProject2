//Operating Systems Project #2 - Memory Management
//Trulee Hersh - hersht
//Mohammed Moftah - moftam
//Alex Seo - seoa

#include <iostream> // library for reading & writing from the console/keyboard
#include <cstdlib>  // library with the exit function
#include <vector>   //library for vectors
#include <fstream>  //library to stream files
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "process.h"
#include <iomanip>
#include <algorithm> 
#include <set>

using namespace std; 

//global variables
int row = 8; //number of rows in memory block
int col = 32; //number of cols in memory block
int t_memmove = 1; //the time to move one frame of memory,  measured in milliseconds

void printMemory(vector<vector<char> > memory){
    for(int i = 0; i<col; i++){
        cout<<'=';   
    }
    cout<<endl;
    for(unsigned int i=0;i<memory.size();i++){
        for(unsigned int j = 0; j<memory[i].size();j++){
            cout<<memory[i][j];
        }
        cout<<endl;
    }
    
     for(int i = 0; i<col; i++){
        cout<<'=';   
    }
    cout<<endl;
}



int defragment(vector<vector<char> > &memory, vector<Process> &processes, int time){
    //how long to halt processes for
    int count = 0;
    int pd = 0;
    for(unsigned int i = 0; i<memory.size(); i++){
        for(unsigned int j =0; j<memory[i].size(); j++){
            if(memory[i][j] == '.'){
                pd++;
            }
        }
    }
    count = 256-pd;
    //halt everything for count number of ms
    for(unsigned int i = 0; i<processes.size(); i++){
        int arr1 = processes[i].getArrivalTime1();
        int arr2 = processes[i].getArrivalTime2();
        int end1 = processes[i].getEndTime1();
        int end2 = processes[i].getEndTime2();
        if(arr1>=time){
            processes[i].setArrivalTime1(arr1+count);
        }
        if(arr2>=time){
            processes[i].setArrivalTime2(arr2+count);
        }
        if(end1>=time){
            processes[i].setEndTime1(end1+count);
        }
        if(end2>=time){
            processes[i].setEndTime2(end2+count);
        }
    }
    //update time
    time+=count;
    
    //convert 2d to 1d array
    vector<char>temp;
    for(unsigned int i =0; i<memory.size(); i++){
        for(unsigned int j= 0; j<memory[i].size(); j++){
            temp.push_back(memory[i][j]);
        }
    }
    //find processes that were shifted (for printing later)
    char first = ' ';
    bool f = false;
    if(temp[0]!='.'){
        f = true;
        first = temp[0];
    }
    set<char> myset( temp.begin(), temp.end() );
    myset.erase('.');
    if(f){
        myset.erase(first);
    }
    
    //defragment 1d array
    temp.erase(std::remove(temp.begin(), temp.end(), '.'), temp.end());
    
    //convert back into 2d array
    int x =0;
    int size = temp.size();
    for(unsigned int i =0; i<memory.size(); i++){
        for(unsigned int j= 0; j<memory[i].size(); j++){
            memory[i][j] = temp[x];
            x++;
            if(x>size){
                memory[i][j] = '.';
            }
        }
    }
    //print info
    cout<<"time "<<time<<"ms: Defragmentation complete (moved "<<count<<" frames: ";
    set<char>::iterator iter = myset.begin();
    cout<<*iter;
    iter++;
    for(; iter!=myset.end();++iter){
        cout<<", "<<(*iter);
    }
    cout<<")"<<endl;
    
    printMemory(memory);
    
    return time; //return new time (time defrag. began + time it took to defrag.)
}


//For the best-fit algorithm, process Q is placed in the smallest free partition available in which
//process Q fits. If a “tie” occurs, use the free partition closer to the “top” of memory.
void bestFit(vector<Process> processes, vector<vector< char> > memory){
    int terminated = 0;
    int time = 0;
    int n = processes.size();
    int totalProcs = 0;
    for(int i = 0; i<n; i++){
        if(processes[i].getArrivalTime2()!=-1){
            totalProcs++;
        }
    }
    totalProcs+=n; //get total number of procceses that will run (i.e. if a process arrives twice (arrivalTime1, arrivalTime2), each arrival counts seperately)

    cout<<"time "<<time<<"ms: Simulator started (Contiguous -- Best-Fit)"<<endl;
    while(terminated<totalProcs){
        //TIES: processes leave memory before other processes arrive
        for(unsigned int i =0; i<processes.size(); i++){  //REMOVAL LOOP
            char id = processes[i].getPid();
            //int memSize = processes[i].getMemFrames();
            if(time == processes[i].getEndTime1() || time == processes[i].getEndTime2()){ //remove process from memory
                for(unsigned int j = 0; j<memory.size(); j++){
                    for(unsigned int k = 0; k<memory[j].size(); k++){
                        if(memory[j][k]==id){ //replace process ID with "."
                            memory[j][k]='.';
                        }
                    }
                }
                terminated++;
                cout<<"time "<<time<<"ms: Process "<<id<<" removed:"<<endl;
                printMemory(memory);
            }
        }
        for(unsigned int i =0; i<processes.size(); i++){   //ARRIVAL LOOP
            char id = processes[i].getPid();
            int memSize = processes[i].getMemFrames();
            bool arrivalVersion1 = false; //we are using arr2
            if(time==processes[i].getArrivalTime1() || time==processes[i].getArrivalTime2()){
                if(time==processes[i].getArrivalTime1()){ //we are using arr1
                    arrivalVersion1 = true;
                }
                cout<<"time "<<time<<"ms: Process "<<id<<" arrived (requires "<<memSize<<" frames)"<<endl;
                //place in memory
                int startJ = 0;
                int startK = 0;
                int fitJ =0;
                int fitK = 0;
                int fit = 0;
                int pastFit = 0;
                bool extra = true;
                bool found = false;
                int totalEmpty = 0;
                int availSpaces = 0;
                bool firstOpen = false;
                for(unsigned int j = 0; j<memory.size(); j++){ //loop through memory, find best fit
                    for(unsigned int k = 0; k<memory[j].size(); k++){
                        if(memory[j][k]=='.'){ 
                            firstOpen = true;
                            if(fit==0){
                                startJ = j;
                                startK = k;
                            }
                            fit++;
                            totalEmpty++;
                        }
                        if(memory[j][k] != '.' || (memory[j][k]=='.' && j*k==217)){ //if a process in memory is encountered or we reach the end of memory
                            if(firstOpen && extra && (fit>=memSize)){ //if first loop through, set fit and pastFit equal to eachother
                                pastFit = fit;
                                availSpaces++;
                                extra = false;
                            }
                            //if current fit is greater/equal to memSize and smaller than pastFit, or if only one possible location was found, this is what we want!
                            if((fit>=memSize && fit<pastFit) || (fit>=memSize && availSpaces==1 && fit<=pastFit) ){ 
                                pastFit = fit;
                                fitJ=startJ;
                                fitK=startK;
                                found = true;
                                availSpaces++;
                            }
                            fit=0;
                        }
                    }
                }
                if(found == true){ //if we found a spot, place it
                    int x = 0;
                    if(arrivalVersion1){ //set removal times
                        processes[i].setEndTime1(time+processes[i].getRunTime1());
                    }
                    else{
                        processes[i].setEndTime2(time+processes[i].getRunTime2());
                    }
                    
                    bool initial = true;
                    for(unsigned int j = 0; j<memory.size(); j++){ //loop through memory
                        for(unsigned int k = 0; k<memory[j].size(); k++){
                            if(initial){
                                j=fitJ;
                                k=fitK;
                                initial = false;
                            }
                            if(x==memSize) break; //break if placed memSize frames of process
                            memory[j][k] = id; //place process in memory at correct spot
                            x++;
                        }
                    }
                    cout<<"time "<<time<<"ms: Placed process "<<id<<":"<<endl;
                    printMemory(memory);
                }
                else if(totalEmpty>=memSize && found==false){ //cannot place in memory as is, need to defragment
                    cout<<"time "<<time<<"ms: Cannot place process "<<id<<" -- starting defragmentation"<<endl;
                    time = defragment(memory, processes, time);
                    
                    if(arrivalVersion1){ //set removal times
                        processes[i].setEndTime1(time+processes[i].getRunTime1());
                    }
                    else{
                        processes[i].setEndTime2(time+processes[i].getRunTime2());
                    }
                    int j=0;
                    int k=0;
                    int b = false;
                    for( j = 0; (unsigned)j<memory.size(); j++){ //find end of processes, first empty spot
                        for(k=0; (unsigned)k<memory[j].size(); k++){
                            if(memory[j][k] == '.'){
                                b = true;
                                break;
                            }
                        }
                        if(b==true) break;
                    }
                    int z = 0;
                    bool initial = true;
                    for(unsigned int m = 0; m<memory.size(); m++){
                        for(unsigned int n=0; n<memory[m].size(); n++){
                            if(initial){
                                m=j;
                                n=k;
                                initial = false;
                            }
                            if(z==memSize) break; //break if placed memSize frames of process
                            memory[m][n] = id; //place process in memory at correct spot
                            z++;
                        }
                    }
                    cout<<"time "<<time<<"ms: Placed process "<<id<<":"<<endl;
                    printMemory(memory);
                }else{ //cannot place in memory at all--no room
                    cout<<"time "<<time<<"ms: Cannot place process "<<id<<" -- skipped!"<<endl;
                    terminated++;
                }
            }
        }
        time++;
    }
    cout<<"time "<<time<<"ms: Simulator ended (Contiguous -- Best-Fit)"<<endl;
}

// MAIN
int main(int argc, char* argv[]) {
    
    if (argc != (2)) {
		std::cerr << "ERROR: Invalid arguments.\n USAGE: ./a.out <input-file> <output-file>\n";
		return EXIT_FAILURE;
	} 

	//open text file to read and check success
	std::ifstream in_str(argv[1]);
	if (!in_str.good()) {
	    std::cerr << "ERROR: Can't open " << argv[1] << " to read.\n";
	    return EXIT_FAILURE;
	}

/*	//open text file to write and check success
  	std::ofstream out_str(argv[2]);
	  	if (!out_str.good()) {
	    std::cerr << "ERROR: Can't open " << argv[2] << " to write.\n";
	     return EXIT_FAILURE;
  	} */
  	
  	string line;
    vector<Process> processes; //Vector of processes to simulate
    
     //Read the file, gather data
    while (!in_str.eof()) {
        getline(in_str, line);
        string ar1, ar2;
        Process process;
        //parse line
        if(!(line.length()==0 || !isupper(line[0]))){ //line by line (if line length>0 and starts w uppercase)
            bool data2 = false; 
            char id = line[0];
            string mem = line.substr(1,3);
            int j =5;
            int space=0;
            for(unsigned int i = 5; i<line.length(); i++){
                if(!isspace(line[i])){
                    for(j=i; j<(signed)line.length(); j++){
                        if(isspace(line[j])){
                            break;
                        }
                    }
                    j--;
                    int len = j-i;
                    if(space ==0){
                        ar1 = line.substr(i,len+1);
                        i=j+1;
                        space++;
                        continue;
                    }
                    if(space ==1){
                        ar2 = line.substr(i,len+1);
                        data2=true;
                        i=j+1;
                    }
                }
            }
            //set process info
            process.setPid(id);
            process.setMemFrames(atoi(mem.c_str()));
            
            //delimit data further
            int i = ar1.rfind('/');
            string arr = ar1.substr(0, i);
            string run = ar1.substr(i+1, ar1.length()-1);
            char *cstr = new char[arr.length() + 1];
            strcpy(cstr, arr.c_str());
            process.setArrivalTime1(atoi(cstr));
            delete cstr;
            
            cstr = new char[run.length() + 1];
            strcpy(cstr, run.c_str());
            process.setRunTime1(atoi(cstr));
            
            delete cstr;
            
            if(data2){
                i = ar2.rfind('/');
                arr = ar2.substr(0, i);
                run = ar2.substr(i+1, ar1.length()-1);
                cstr = new char[arr.length() + 1];
                strcpy(cstr, arr.c_str());
                process.setArrivalTime2(atoi(cstr));
                delete cstr;
                
                cstr = new char[run.length() + 1];
                strcpy(cstr, run.c_str());
                process.setRunTime2(atoi(cstr));
                delete cstr;
            }

            //cout<<process.getPid()<< " "<<process.getMemFrames()<<" "<<process.getArrivalTime1()<<"/"<<process.getRunTime1()<<" "<<process.getArrivalTime2()<<"/"<<process.getRunTime2()<<endl;
            processes.push_back(process);
        }
    }
    //create memory block
    vector<vector<char > > memory;  //8x32 block of "."
    vector<char> column (col, '.');
    for(int i=0; i<row; i++){
        memory.push_back(column);
    }
    
    bestFit(processes, memory);

    return 0;
}