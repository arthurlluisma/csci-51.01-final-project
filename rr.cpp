using namespace std;
#include <iostream>
#include <fstream>
#include <queue>

struct Process
{
    int arrivalTime; // in nanoseconds
    int burstTime;
    int priority; // in nanoseconds
    int processIndex;
    int waitingTime;
};

int main()
{
    int currentTime = 0;
    int processes;
    float totalBusyTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime;
    float turnaroundTime;
    queue<Process> processQueue;
    ifstream inputfile("input.txt");
    inputfile >> processes;
    for (int x = 0; x < processes; x++)
    {
        Process processInput;
        processInput.processIndex = x+1;
        inputfile >> processInput.arrivalTime >> processInput.burstTime >> processInput.priority;
        processQueue.push(processInput);
    }

    while (!processQueue.empty())
    {
        Process p = processQueue.front();
        if (currentTime < p.arrivalTime) 
        {
            currentTime = p.arrivalTime;
        }

        p.waitingTime = currentTime - p.arrivalTime;

        cout << currentTime << " " << p.processIndex << " "  << p.burstTime << "X" << endl;
        currentTime += p.burstTime;
        totalBusyTime += p.burstTime;
        totalWaitingTime += p.waitingTime;
        turnaroundTime = p.waitingTime + p.burstTime;
        totalTurnaroundTime += turnaroundTime;
        processQueue.pop();
    }

    float cpuUtilization = totalBusyTime / currentTime * 100;
    printf("CPU Utilization: (%) %f\n", cpuUtilization);
    float cpuThroughput = float(processes) / float(currentTime);
    printf("Throughtput: (# of processes) completed per unit of time %f\n", cpuThroughput);
    float cpuWaitingTime = float(totalWaitingTime) / float(processes); 
    printf("Waiting time: (time) cumulative %f\n", cpuWaitingTime);
    float cpuTurnaroundTime = totalTurnaroundTime / float(processes);
    printf("Turnaround time: (time) %f\n", cpuTurnaroundTime);
    printf("Response time: (time) %f\n", cpuWaitingTime);
    return 0;
}