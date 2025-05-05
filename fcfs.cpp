using namespace std;
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>

struct Process
{
    int arrivalTime; 
    int burstTime;
    int priority; 
    int processIndex;
    int waitingTime;
    int turnaroundTime;  
    
};

bool compareArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

int main()
{
    int currentTime = 0;
    int processes;
    float totalBusyTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;  
    string label;
    queue<Process> processQueue;
    ifstream inputfile("input.txt");
    inputfile >> processes >> label;
    vector<int> waitingTimeList, turnaroundTimeList;
    vector<Process> allProcesses;

    
    
    
    for (int x = 0; x < processes; x++)
    {
        Process processInput;
        processInput.processIndex = x + 1;
        inputfile >> processInput.arrivalTime >> processInput.burstTime >> processInput.priority;
        processInput.waitingTime = 0; 
        processInput.turnaroundTime = 0; 
        allProcesses.push_back(processInput);
    }

    sort(allProcesses.begin(), allProcesses.end(), compareArrival);

    for (int x = 0; x < processes; x++)
    {
        processQueue.push(allProcesses[x]);
    }

    while (!processQueue.empty())
    {
        Process p = processQueue.front();
        

        if (currentTime < p.arrivalTime) 
        {
            currentTime = p.arrivalTime;
        }

        p.waitingTime = currentTime - p.arrivalTime;
        

        p.turnaroundTime = p.waitingTime + p.burstTime;


        cout << currentTime << " " << p.processIndex << " " << p.burstTime << "X" << endl;

        currentTime += p.burstTime;
        totalBusyTime += p.burstTime;
        totalWaitingTime += p.waitingTime;
        waitingTimeList.push_back(p.waitingTime);
        totalTurnaroundTime += p.turnaroundTime;
        turnaroundTimeList.push_back(p.turnaroundTime);
        processQueue.pop();
    }

    cout << endl;

    float cpuUtilization = (totalBusyTime / currentTime) * 100;
    printf("CPU Utilization: %.2f%\n\n", cpuUtilization);
    float cpuThroughput = float(processes) / float(currentTime);
    printf("Throughput: %.2f processes/nanosecond\n\n", cpuThroughput);
    float avgWaitingTime = float(totalWaitingTime) / float(processes); 
    cout << "Waiting time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average waiting time %.2fns\n\n", avgWaitingTime);
    float avgTurnaroundTime = float(totalTurnaroundTime) / float(processes);
    cout << "Turnaround time:" << endl;
    for (int x = 0; x < turnaroundTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, turnaroundTimeList[x]);
    }
    printf("Average turnaround time %.2fns\n\n", avgTurnaroundTime);
    cout << "Response time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average response time %.2fns\n\n", avgWaitingTime);

    return 0;
}
