using namespace std;
#include <iostream>
#include <fstream>
#include <queue>

struct Process
{
    int arrivalTime; 
    int burstTime;
    int priority; 
    int processIndex;
    int waitingTime;
    int turnaroundTime;  
};

int main()
{
    int currentTime = 0;
    int processes;
    float totalBusyTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;  
    queue<Process> processQueue;
    ifstream inputfile("input.txt");
    inputfile >> processes;
    
    for (int x = 0; x < processes; x++)
    {
        Process processInput;
        processInput.processIndex = x + 1;
        inputfile >> processInput.arrivalTime >> processInput.burstTime >> processInput.priority;
        processInput.waitingTime = 0; 
        processInput.turnaroundTime = 0; 
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
        

        p.turnaroundTime = p.waitingTime + p.burstTime;


        cout << currentTime << " " << p.processIndex << " " << p.burstTime << "X" << endl;

        currentTime += p.burstTime;
        totalBusyTime += p.burstTime;
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        cout << p.waitingTime << endl;
        processQueue.pop();
    }

    cout << endl;

    float cpuUtilization = (totalBusyTime / currentTime) * 100;
    printf("CPU Utilization: (%) %.6f\n", cpuUtilization);
    float cpuThroughput = float(processes) / float(currentTime);
    printf("Throughput: (# of processes) completed per unit of time %.6f\n", cpuThroughput);
    float avgWaitingTime = float(totalWaitingTime) / float(processes); 
    printf("Waiting time: (time) cumulative %.6f\n", avgWaitingTime);
    float avgTurnaroundTime = float(totalTurnaroundTime) / float(processes);
    printf("Turnaround time: (time) %.6f\n", avgTurnaroundTime);
    printf("Response time: (time) %.6f\n", avgWaitingTime);

    return 0;
}
