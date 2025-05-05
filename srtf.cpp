#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

int numProcesses;
string algorithm;
vector<vector<int>> originalProcessValues;
multiset<vector<int>> processes;
vector<bool> hasResponded;
vector<int> waitingTimes, turnaroundTimes, responseTimes;

int main() {
    cin >> numProcesses >> algorithm;
    originalProcessValues.resize(numProcesses+1);
    hasResponded.resize(numProcesses+1);
    waitingTimes.resize(numProcesses+1);
    turnaroundTimes.resize(numProcesses+1);
    responseTimes.resize(numProcesses+1);
    for (int i = 0; i < numProcesses; i++) {
        int arrivalTime, burstTime, priority;
        cin >> arrivalTime >> burstTime >> priority;
        originalProcessValues[i+1] = {arrivalTime, burstTime, priority, i+1};
        processes.insert({arrivalTime, burstTime, priority, i+1});
    }

    bool isFirstProcess = true;
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

        if (isFirstProcess)
            time = (*chosenProcess)[0];

        multiset<vector<int>>::iterator nextProcess;
        bool preemptProcess = false;
        int nextArrivalTime = -1;
        for (auto it = processes.begin(); it != processes.end(); it++) {    
            if (it == chosenProcess || (*it)[0] <= time)
                continue;

            vector<int> currentProcess = *it;
            if (currentProcess[1] >= (*chosenProcess)[1]-(currentProcess[0]-time))
                continue;
            
            if (nextArrivalTime == -1) {
                if (currentProcess[1] < (*chosenProcess)[1]-(currentProcess[0]-time)) {
                    nextProcess = it;
                    preemptProcess = true;
                    nextArrivalTime = (*it)[0];
                }
            } else {
                if (currentProcess[0] <= nextArrivalTime && currentProcess[1] < (*chosenProcess)[1]-(currentProcess[0]-time)) {
                    nextProcess = it;
                    preemptProcess = true;
                    nextArrivalTime = (*it)[0];
                }
            }
        }

        vector<int> processToExecute = *chosenProcess;
        if (preemptProcess) {
            vector<int> preemptedProcess = {processToExecute[0], processToExecute[1]-((*nextProcess)[0]-time), processToExecute[2], processToExecute[3]};
            processToExecute = preemptedProcess;
            processes.erase(chosenProcess);
            processes.insert(preemptedProcess);

            if (isFirstProcess) {
                time = 0;
                isFirstProcess = false;
            }

            if (!hasArrived) {
                idleTime += originalProcessValues[processToExecute[3]][0]-time;
                time = (*chosenProcess)[0];
            }

            finalOutput.push_back({time, processToExecute[3], (*nextProcess)[0]-time, 0});

            if (!hasResponded[originalProcessValues[processToExecute[3]][3]]) {
                responseTimes[originalProcessValues[processToExecute[3]][3]] = time-originalProcessValues[processToExecute[3]][0];
                hasResponded[originalProcessValues[processToExecute[3]][3]] = true;
            }

            time = (*nextProcess)[0];
        } else {
            if (isFirstProcess) {
                time = 0;
                isFirstProcess = false;
            }

            if (hasArrived) {
                finalOutput.push_back({time, processToExecute[3], processToExecute[1], 1});
                
                if (!hasResponded[originalProcessValues[processToExecute[3]][3]]) {
                    responseTimes[originalProcessValues[processToExecute[3]][3]] = time-originalProcessValues[processToExecute[3]][0];
                    hasResponded[originalProcessValues[processToExecute[3]][3]] = true;
                }

                time += processToExecute[1];
                turnaroundTimes[originalProcessValues[processToExecute[3]][3]] = time-originalProcessValues[processToExecute[3]][0];
                waitingTimes[originalProcessValues[processToExecute[3]][3]] = turnaroundTimes[originalProcessValues[processToExecute[3]][3]]-originalProcessValues[processToExecute[3]][1];
                processes.erase(chosenProcess);
                continue;
            }
    
            idleTime += originalProcessValues[processToExecute[3]][0]-time;
            time += originalProcessValues[processToExecute[3]][0]-time;

            finalOutput.push_back({time, processToExecute[3], processToExecute[1], 1});
            
            if (!hasResponded[originalProcessValues[processToExecute[3]][3]]) {
                responseTimes[originalProcessValues[processToExecute[3]][3]] = time-originalProcessValues[processToExecute[3]][0];
                hasResponded[originalProcessValues[processToExecute[3]][3]] = true;
            }

            time += processToExecute[1];
            turnaroundTimes[originalProcessValues[processToExecute[3]][3]] = time-originalProcessValues[processToExecute[3]][0];
            waitingTimes[originalProcessValues[processToExecute[3]][3]] = turnaroundTimes[originalProcessValues[processToExecute[3]][3]]-originalProcessValues[processToExecute[3]][1];
            processes.erase(chosenProcess);
        }
    }

    for (vector<int> process : finalOutput) {
        if (process[3] == 1)
            cout << process[0] << " " << process[1] << " " << process[2] << "X" << endl;
        else
            cout << process[0] << " " << process[1] << " " << process[2] << endl;
    }

    long double cpuUtilization = ((long double)time-(long double)idleTime)/(long double)time*100;
    long double throughput = (long double)numProcesses/(long double)time;
    printf("CPU Utilization: %.2Lf%%\n", cpuUtilization);
    printf("\nThroughput: %.2Lf processes/nanosecond\n", throughput);

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