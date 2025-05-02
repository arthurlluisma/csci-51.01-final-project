#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

int numProcesses;
string algorithm;
multiset<vector<int>> processes;
vector<int> waitingTimes, turnaroundTimes, responseTimes;

int main() {
    cin >> numProcesses >> algorithm;
    waitingTimes.resize(numProcesses+1);
    turnaroundTimes.resize(numProcesses+1);
    responseTimes.resize(numProcesses+1);
    for (int i = 0; i < numProcesses; i++) {
        int arrivalTime, burstTime, priority;
        cin >> arrivalTime >> burstTime >> priority;
        processes.insert({arrivalTime, burstTime, priority, i+1});
    }

    int time = 0, idleTime = 0;
    vector<vector<int>> finalOutput;
    while (!processes.empty()) {
        multiset<vector<int>>::iterator chosenProcess;
        int currentArrivalTime = -1, currentBurstTime = -1;
        bool hasArrived = false;
        for (auto it = processes.begin(); it != processes.end(); it++) {
            vector<int> currentProcess = *it;
            if (currentArrivalTime == -1 && currentBurstTime == -1) {
                chosenProcess = it;
                currentArrivalTime = currentProcess[0];
                currentBurstTime = currentProcess[1];

                if (currentArrivalTime <= time) {
                    hasArrived = true;
                }

                continue;
            }

            if (currentProcess[1] == currentBurstTime) {
                if (currentProcess[0] < currentArrivalTime) {
                    chosenProcess = it;
                    currentArrivalTime = currentProcess[0];

                    if (currentArrivalTime <= time) {
                        hasArrived = true;
                    }
                }
            } else if (currentProcess[1] < currentBurstTime) {
                if (currentProcess[0] <= time) {
                    chosenProcess = it;
                    currentArrivalTime = currentProcess[0];
                    currentBurstTime = currentProcess[1];

                    if (currentArrivalTime <= time) {
                        hasArrived = true;
                    }
                }
            }
        }

        vector<int> processToExecute = *chosenProcess;
        if (hasArrived) {
            finalOutput.push_back({time, processToExecute[3], processToExecute[1]});
            responseTimes[processToExecute[3]] = time-processToExecute[0];
            time += processToExecute[1];
            turnaroundTimes[processToExecute[3]] = time-processToExecute[0];
            waitingTimes[processToExecute[3]] = turnaroundTimes[processToExecute[3]]-processToExecute[1];
            processes.erase(chosenProcess);
            continue;
        }

        finalOutput.push_back({time, processToExecute[3], processToExecute[1]});
        responseTimes[processToExecute[3]] = time-processToExecute[0];
        idleTime += processToExecute[0]-time;
        time += (processToExecute[0]-time)+processToExecute[1];
        turnaroundTimes[processToExecute[3]] = time-processToExecute[0];
        waitingTimes[processToExecute[3]] = turnaroundTimes[processToExecute[3]]-processToExecute[1];
        processes.erase(chosenProcess);
    }

    for (vector<int> process : finalOutput) {
        cout << process[0] << " " << process[1] << " " << process[2] << "X" << endl;
    }

    long double cpuUtilization = ((long double)time-(long double)idleTime)/(long double)time*100;
    long double throughput = (long double)numProcesses/(long double)time;
    printf("CPU Utilization: %.2Lf%%\n", cpuUtilization);
    printf("\nThroughput: %.2Lf processes/second\n", throughput);

    long double waitAverage = 0;
    printf("\nWaiting time:\n");
    for (int i = 1; i <= numProcesses; i++) {
        printf("Process %d: %dns\n", i, waitingTimes[i]);
        waitAverage += (long double)waitingTimes[i];
    }
    printf("Average waiting time = %.2Lfns\n", waitAverage/(long double)numProcesses);

    long double turnaroundAverage = 0;
    printf("\nTurnaround time:\n");
    for (int i = 1; i <= numProcesses; i++) {
        printf("Process %d: %dns\n", i, turnaroundTimes[i]);
        turnaroundAverage += (long double)turnaroundTimes[i];
    }
    printf("Average turnaround time = %.2Lfns\n", turnaroundAverage/(long double)numProcesses);

    long double responseAverage = 0;
    printf("\nResponse time:\n");
    for (int i = 1; i <= numProcesses; i++) {
        printf("Process %d: %dns\n", i, responseTimes[i]);
        responseAverage += (long double)responseTimes[i];
    }
    printf("Average response time = %.2Lfns\n", responseAverage/(long double)numProcesses);

    return 0;
}