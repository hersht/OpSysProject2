//Operating Systems Project #2 - Memory Management
//Trulee Hersh - hersht
//Mohammed Moftah - moftam
//Alex Seo - seoa

#include <iostream>
#include "process.h"
#include <vector>
#include <iomanip>
#include <cstdio>

using namespace std;

Process::Process(){
    pid = ' ';
    arrivalTime1 = -1;
    runTime1 = -1;
    endTime1 = -1;
    arrivalTime2 = -1;
    runTime2 = -1;
    endTime2 = -1;
    memFrames = -1;
}

/*Process::Process(char id, int arr1, int run1, int arr2, int run2, int mem){
    pid = id
    arrivalTime1 = arr1;
    runTime1 = run1;
    arrivalTime2 = arr2;
    runTime2 = run2;
    memFrames = mem;
}*/

//ACCESSORS
char Process::getPid() const{
	return pid;
}

int Process::getArrivalTime1() const{
    return arrivalTime1;
}

int Process::getRunTime1() const{
    return runTime1;
}

int Process::getEndTime1() const{
    return endTime1;
}

int Process::getArrivalTime2() const{
    return arrivalTime2;
}

int Process::getRunTime2() const{
    return runTime2;
}

int Process::getEndTime2() const{
    return endTime2;
}

int Process::getMemFrames() const{
    return memFrames;
}

//MODIFIERS
void Process::setPid(char p){
    pid = p;
}

void Process::setArrivalTime1(int a){
    arrivalTime1 = a;
}

void Process::setRunTime1(int a){
    runTime1 = a;
}

void Process::setEndTime1(int a){
    endTime1 = a;
}

void Process::setArrivalTime2(int a){
    arrivalTime2 = a;
}

void Process::setRunTime2(int a){
    runTime2 = a;
}

void Process::setEndTime2(int a){
    endTime2 = a;
}

void Process::setMemFrames(int a){
    memFrames = a;
}