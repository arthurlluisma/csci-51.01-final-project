#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int arrivalTime;
    int burstTime;
    int priority;
    int processIndex;
    int waitingTime;
    int remainingTime;
    int startTime;
};

bool compareArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

int main() {
    ifstream inputfile("input.txt");
    int processes;
    string label;
    int timeQuantum;

    inputfile >> processes >> label >> timeQuantum;

    vector<Process> allProcesses;
    for (int i = 0; i < processes; i++) {
        Process p;
        inputfile >> p.arrivalTime >> p.burstTime >> p.priority;
        p.processIndex = i + 1;
        p.remainingTime = p.burstTime;
        p.waitingTime = 0;
        p.startTime = -1;
        allProcesses.push_back(p);
    }

    sort(allProcesses.begin(), allProcesses.end(), compareArrival);

    queue<Process> q;
    int index = 0;
    int currentTime = 0;
    float totalBusyTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    float totalResponseTime = 0;

    while (!q.empty() || index < allProcesses.size()) {
        while (index < processes && allProcesses[index].arrivalTime <= currentTime) {
            q.push(allProcesses[index]);
            index++;
        }

        if (q.empty()) {
            currentTime = allProcesses[index].arrivalTime;
            continue;
        }

        Process current = q.front();
        q.pop();

        if (current.startTime == -1) {
            current.startTime = currentTime;
        }

        int runFor = timeQuantum;
        if (current.remainingTime < timeQuantum) {
            runFor = current.remainingTime;
        }

        cout << currentTime << " " << current.processIndex << " " << runFor;
        currentTime += runFor;
        current.remainingTime -= runFor;
        totalBusyTime += runFor;

        if (current.remainingTime == 0) {
            cout << "X" << endl;
            int turnaround = currentTime - current.arrivalTime;
            int waiting = turnaround - current.burstTime;
            int response = current.startTime - current.arrivalTime;
            totalWaitingTime += waiting;
            totalTurnaroundTime += turnaround;
            totalResponseTime += response;
        } else {
            cout << endl;
            while (index < processes && allProcesses[index].arrivalTime <= currentTime) {
                q.push(allProcesses[index]);
                index++;
            }
            q.push(current);
        }
    }

    float cpuUtilization = (totalBusyTime / currentTime) * 100.0f;
    float throughput = float(processes) / currentTime;
    float avgWait = totalWaitingTime / processes;
    float avgTurnaround = totalTurnaroundTime / processes;
    float avgResponse = totalResponseTime / processes;

    cout << "CPU Utilization: (%) " << cpuUtilization << endl;
    cout << "Throughput: (# of processes) completed per unit of time " << throughput << endl;
    cout << "Waiting time: (time) cumulative " << avgWait << endl;
    cout << "Turnaround time: (time) " << avgTurnaround << endl;
    cout << "Response time: (time) " << avgResponse << endl;

    return 0;
}
