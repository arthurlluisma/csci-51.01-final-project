#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <list>
#include <string>

using namespace std;

//General references used: https://cplusplus.com/reference/list/list/

struct process {
    int processNum;
    int arrivalTime;
    int burstTime;
    int priority;
    int timeAdded; //for debugging purposes
    int timeSpent = 0;
    int timeFinished; //for debugging purposes
};

int main(int argc, char* argv[])
{
    int numOfTestCases;
    int numOfProcesses;
    char scheduleType;
    int currentTime = 0;

    process p;
    list<process> processList;
    list<process> allProcesses;

    ifstream inputFile("textinput.txt"); //reference: https://devdocs.io/cpp/io/c
    inputFile >> numOfTestCases >> numOfProcesses >> scheduleType;
    
    int i = 0;
    int k = 0;
    while (i < numOfTestCases)
    {
        for (int j = 0; j < numOfProcesses; ++j) //stores all processes in a list
        {
            inputFile >> p.processNum >> p.arrivalTime >> p.burstTime >> p.priority;
            allProcesses.push_back(p);
        }
        while (currentTime < 20)
        {   
            for (process q : allProcesses)
            {
                if(q.arrivalTime == currentTime) //adds process into queue if correct time
                {   
                    q.timeAdded = currentTime;
                    processList.push_back(q);
                }
            }

            processList.sort([](const process& a, const process& b) { //sorts the list in decreasing priority
                return a.priority > b.priority; //reference: https://cplusplus.com/reference/list/list/sort/
            });
            
            printf("Current Time %ins\n",currentTime);

            for(process q : processList) //prints out every process in queue
            {
                cout << "Process " << q.processNum
                << ": Arrival=" << q.arrivalTime
                << ", Burst=" << q.burstTime
                << ", Priority=" << q.priority
                << ", Time Added=" << q.timeAdded 
                << ", Time Finished=" << q.timeFinished;
                if(q.timeSpent == q.burstTime)
                {
                   cout << ", Time Spent=" << q.timeSpent << "X" << endl;
                }
                else
                {
                   cout << ", Time Spent=" << q.timeSpent << endl;
                }
                
            }

            auto currentProcess = find_if(processList.begin(), processList.end(), [](const process& p) { //looks for highest priority process not yet done
                return p.timeSpent < p.burstTime; //reference: https://cplusplus.com/reference/algorithm/find_if/
            });
            
            if(currentProcess != processList.end())
            {
                currentProcess->timeSpent += 1;

    
                if(currentProcess->timeSpent == currentProcess->burstTime)
                {
                    currentProcess->timeFinished = currentTime;
                }
            }

            currentTime++;

        }
        currentTime = 0;
        i++;
        inputFile >> numOfProcesses >> scheduleType;
        allProcesses.clear();
        processList.clear();
    }

    inputFile.close();
    return 0;
}