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
    int timeStart = -1;
};

int main(int argc, char* argv[])
{
    int numOfTestCases;
    int numOfProcesses;
    char scheduleType;
    int currentTime = 0;
    int lastRunningProcess = -1;
    int lastSwitchTime = 0;
    process* lastProcessPtr = nullptr;

    process p;
    list<process*> processList;
    list<process> allProcesses;

    ifstream inputFile(argv[1]); //reference: https://devdocs.io/cpp/io/c
    inputFile >> numOfTestCases >> numOfProcesses >> scheduleType;
    
    int i = 0;
    int k = 0;
    while (i < numOfTestCases)
    {
        cout << i+1 << endl;
        for (int j = 0; j < numOfProcesses; ++j) //stores all processes in a list
        {
            p.processNum = j + 1;
            inputFile >> p.arrivalTime >> p.burstTime >> p.priority;
            // cout << "Process Number: " << p.processNum << ", Priority: " << p.priority << endl;
            allProcesses.push_back(p);
        }
        while (true)
        {   
            for (process& q : allProcesses)
            {
                if(q.arrivalTime == currentTime) //adds process into queue if correct time
                {   
                    q.timeAdded = currentTime;
                    processList.push_back(&q);
                }
            }

            processList.sort([](const process* a, const process* b) 
            { //sorts the list in decreasing priority
                return a->priority > b->priority; //reference: https://cplusplus.com/reference/list/list/sort/
            });

            auto currentProcess = find_if(processList.begin(), processList.end(), [](const process* p) 
            { //looks for highest priority process not yet done
                return p->timeSpent < p->burstTime; //reference: https://cplusplus.com/reference/algorithm/find_if/
            });
            
            if(currentProcess != processList.end())
            {
                if ((*currentProcess)->timeStart == -1) {
                    (*currentProcess)->timeStart = currentTime;
                }
                (*currentProcess)->timeSpent += 1;
                
                if((*currentProcess)->timeSpent == (*currentProcess)->burstTime)
                {
                    (*currentProcess)->timeFinished = currentTime;
                }

                int currentRunningProcess = (*currentProcess)->processNum;

                if (currentRunningProcess != lastRunningProcess) {
                    if (lastProcessPtr != nullptr) {
                        int duration = currentTime - lastSwitchTime;
                        bool justFinished = (lastProcessPtr->timeSpent == lastProcessPtr->burstTime);
                        cout << lastSwitchTime << " " << lastProcessPtr->processNum << " " << duration
                            << (justFinished ? "X" : "") << endl;
                    }
                
                    lastSwitchTime = currentTime;
                    lastRunningProcess = currentRunningProcess;
                    lastProcessPtr = *currentProcess;
                }
            }         

            currentTime++;

            bool allFinished = all_of(allProcesses.begin(), allProcesses.end(), [](const process& p) { //Check if all processes are finished
                return p.timeSpent >= p.burstTime; //reference: https://cplusplus.com/reference/algorithm/all_of/
            });
        
            if (allFinished)
                break;
        }

        if (lastProcessPtr != nullptr) { //handles last process, which does not trigger a context switch
            int duration = currentTime - lastSwitchTime;
            cout << lastSwitchTime << " " << lastProcessPtr->processNum << " " << duration << "X" << endl;
        }

        int totalBurstTime = 0;
        int totalTurnaroundTime = 0;
        int totalWaitingTime = 0;
        int totalResponseTime = 0;
        int totalTime = currentTime;

        for (const process& p : allProcesses) {
            int turnaround = p.timeFinished + 1 - p.arrivalTime;
            int waiting = turnaround - p.burstTime;
            int response = p.timeStart - p.arrivalTime;
        
            totalBurstTime += p.burstTime;
            totalTurnaroundTime += turnaround;
            totalWaitingTime += waiting;
            totalResponseTime += response;
        }

        int processCount = allProcesses.size();

        double cpuUtil = (double)totalBurstTime / totalTime * 100.0;
        double throughput = (double)processCount / totalTime;
        double avgWaiting = (double)totalWaitingTime / processCount;
        double avgTurnaround = (double)totalTurnaroundTime / processCount;
        double avgResponse = (double)totalResponseTime / processCount;

        printf("CPU Utilization: %f\n", cpuUtil);
        printf("Throughput: %f\n", throughput);
        for (const process& p : allProcesses) {
            int turnaround = p.timeFinished + 1 - p.arrivalTime;
            int waiting = turnaround - p.burstTime;
            cout << "Process " << p.processNum << " Waiting time" << ": " << waiting << "ns" << endl;
        }
        printf("Average Waiting Time: %fns\n", avgWaiting);
        for (const process& p : allProcesses) {
            int turnaround = p.timeFinished + 1 - p.arrivalTime;
            cout << "Process " << p.processNum << " Turnaround time" << ": " << turnaround << "ns" << endl;
        }
        printf("Average Turnaround Time: %fns\n", avgTurnaround);

        for (const process& p : allProcesses) {
            int response = p.timeStart - p.arrivalTime;
            cout << "Process " << p.processNum << " Response time" << ": " << response << "ns" << endl;
        }
        printf("Response Time: %fns\n", avgResponse);

        currentTime = 0; //resets everything for next test case
        i++;
        inputFile >> numOfProcesses >> scheduleType;
        lastSwitchTime = 0;
        lastRunningProcess = -1;
        lastProcessPtr = nullptr;
        allProcesses.clear();
        processList.clear();
    }

    inputFile.close();
    return 0;
}