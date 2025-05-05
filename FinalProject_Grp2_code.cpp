
/*
I hereby attest to the truth of the following facts:

I have not discussed the C++ code in my program with anyone
other than my instructor or the teaching assistants assigned to this course.

I have not used C++ code obtained from another student, or
any other unauthorized source, whether modified or unmodified.

If any C++ code or documentation used in my program was
obtained from another source, it has been clearly noted with citations in the
comments of my program.

References:
https://devdocs.io/cpp/io/c
https://cplusplus.com/reference/list/list/sort/
https://cplusplus.com/reference/algorithm/find_if/
https://cplusplus.com/reference/algorithm/all_of/
*/
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <list>
#include <queue>
#include <string>
using namespace std;

struct Process {
    int arrivalTime;
    int burstTime;
    int priority; 
    int processIndex;
    int waitingTime;
    int turnaroundTime;
    int remainingTime;
    int timeSpent = 0;
    int timeStart = -1;
    int timeAdded;
    int timeFinished;
};

bool compareArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

void fcfs(int numProcesses, char* input[]) {
    ifstream inputFile(input[1]);
    int currentTime = 0;
    float totalBusyTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;  
    queue<Process> processQueue;
    vector<int> waitingTimeList, turnaroundTimeList;
    vector<Process> allProcesses;
    
    for (int x = 0; x < numProcesses; x++)
    {
        Process processInput;
        processInput.processIndex = x + 1;
        inputFile >> processInput.arrivalTime >> processInput.burstTime >> processInput.priority;
        processInput.waitingTime = 0; 
        processInput.turnaroundTime = 0; 
        allProcesses.push_back(processInput);
    }

    sort(allProcesses.begin(), allProcesses.end(), compareArrival);

    for (int i = 0; i < numProcesses; i++) {
        processQueue.push(allProcesses[i]);
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

    float cpuUtilization = (totalBusyTime / currentTime) * 100;
    printf("CPU Utilization: %.2f\n", cpuUtilization);
    float cpuThroughput = float(numProcesses) / float(currentTime);
    printf("\nThroughput: %.2f numProcesses/nanosecond\n", cpuThroughput);
    float avgWaitingTime = float(totalWaitingTime) / float(numProcesses); 
    cout << "\n" << "Waiting time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average waiting time %.2fns\n\n", avgWaitingTime);
    float avgTurnaroundTime = float(totalTurnaroundTime) / float(numProcesses);
    cout << "Turnaround time:" << endl;
    for (int x = 0; x < turnaroundTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, turnaroundTimeList[x]);
    }
    printf("Average turnaround time %.2fns\n", avgTurnaroundTime);
    cout << "\n" << "Response time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average response time %.2fns\n", avgWaitingTime);
}

void roundRobin(int numProcesses, int timeQuantum, char* input[]) {
    ifstream inputFile(input[1]);
    vector<int> waitingTimeList, turnaroundTimeList, responseTimeList;

    vector<Process> allProcesses;
    for (int i = 0; i < numProcesses; i++) {
        Process p;
        inputFile >> p.arrivalTime >> p.burstTime >> p.priority;
        p.processIndex = i + 1;
        p.remainingTime = p.burstTime;
        p.waitingTime = 0;
        p.timeStart = -1;
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
        while (index < numProcesses && allProcesses[index].arrivalTime <= currentTime) {
            q.push(allProcesses[index]);
            index++;
        }

        if (q.empty()) {
            currentTime = allProcesses[index].arrivalTime;
            continue;
        }

        Process current = q.front();
        q.pop();

        if (current.timeStart == -1) {
            current.timeStart = currentTime;
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
            turnaroundTimeList.push_back(turnaround);
            int waiting = turnaround - current.burstTime;
            waitingTimeList.push_back(waiting);
            int response = current.timeStart - current.arrivalTime;
            responseTimeList.push_back(response);
            totalWaitingTime += waiting;
            totalTurnaroundTime += turnaround;
            totalResponseTime += response;
        } else {
            cout << endl;
            while (index < numProcesses && allProcesses[index].arrivalTime <= currentTime) {
                q.push(allProcesses[index]);
                index++;
            }
            q.push(current);
        }
    }

    float cpuUtilization = (totalBusyTime / currentTime) * 100;
    printf("CPU Utilization: %.2f\n", cpuUtilization);
    float cpuThroughput = float(numProcesses) / float(currentTime);
    printf("\nThroughput: %.2f numProcesses/nanosecond\n", cpuThroughput);
    float avgWaitingTime = float(totalWaitingTime) / float(numProcesses); 
    cout << "\n" << "Waiting time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average waiting time %.2fns\n\n", avgWaitingTime);
    float avgTurnaroundTime = float(totalTurnaroundTime) / float(numProcesses);
    cout << "Turnaround time:" << endl;
    for (int x = 0; x < turnaroundTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, turnaroundTimeList[x]);
    }
    printf("Average turnaround time %.2fns\n", avgTurnaroundTime);
    cout << "\n" << "Response time:" << endl;
    for (int x = 0; x < waitingTimeList.size(); x++)
    {
        printf("Process %i: %ins\n", x+1, waitingTimeList[x]);
    }
    printf("Average response time %.2fns\n", avgWaitingTime);
}

void priority(int numProcesses, char* input[]) {
    ifstream inputFile(input[1]);
    int currentTime = 0;
    int lastRunningProcess = -1;
    int lastSwitchTime = 0;
    Process* lastProcessPtr = nullptr;

    Process p;
    list<Process*> processList;
    list<Process> allProcesses;
    
    for (int j = 0; j < numProcesses; ++j)
    {
        p.processIndex = j + 1;
        inputFile >> p.arrivalTime >> p.burstTime >> p.priority;
        allProcesses.push_back(p);
    }
    while (true)
    {   
        for (Process& q : allProcesses)
        {
            if(q.arrivalTime == currentTime)
            {   
                q.timeAdded = currentTime;
                processList.push_back(&q);
            }
        }

        processList.sort([](const Process* a, const Process* b) 
        {
            return a->priority > b->priority;
        });

        auto currentProcess = find_if(processList.begin(), processList.end(), [](const Process* p) 
        {
            return p->timeSpent < p->burstTime;
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

            int currentRunningProcess = (*currentProcess)->processIndex;

            if (currentRunningProcess != lastRunningProcess) {
                if (lastProcessPtr != nullptr) {
                    int duration = currentTime - lastSwitchTime;
                    bool justFinished = (lastProcessPtr->timeSpent == lastProcessPtr->burstTime);
                    cout << lastSwitchTime << " " << lastProcessPtr->processIndex << " " << duration
                        << (justFinished ? "X" : "") << endl;
                }
            
                lastSwitchTime = currentTime;
                lastRunningProcess = currentRunningProcess;
                lastProcessPtr = *currentProcess;
            }
        }         

        currentTime++;

        bool allFinished = all_of(allProcesses.begin(), allProcesses.end(), [](const Process& p) {
            return p.timeSpent >= p.burstTime;
        });
    
        if (allFinished)
            break;
    }

    if (lastProcessPtr != nullptr) {
        int duration = currentTime - lastSwitchTime;
        cout << lastSwitchTime << " " << lastProcessPtr->processIndex << " " << duration << "X" << endl;
    }

    int totalBurstTime = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int totalResponseTime = 0;
    int totalTime = currentTime;

    for (const Process& p : allProcesses) {
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

    printf("CPU Utilization: %.2f\n", cpuUtil);
    printf("\nThroughput: %.2f\n", throughput);
    cout << "\n" << "Waiting time:" << endl;
    for (const Process& p : allProcesses) {
        int turnaround = p.timeFinished + 1 - p.arrivalTime;
        int waiting = turnaround - p.burstTime;
        cout << "Process " << p.processIndex << ": " << waiting << "ns" << endl;
    }
    printf("Average waiting time: %.2fns\n", avgWaiting);

    cout << "\n" << "Turnaround time:" << endl;
    for (const Process& p : allProcesses) {
        int turnaround = p.timeFinished + 1 - p.arrivalTime;
        cout << "Process " << p.processIndex << ": " << turnaround << "ns" << endl;
    }
    printf("Average turnaround time: %.2fns\n", avgTurnaround);

    cout << "\n" << "Response time:" << endl;
    for (const Process& p : allProcesses) {
        int response = p.timeStart - p.arrivalTime;
        cout << "Process " << p.processIndex << ": " << response << "ns" << endl;
    }
    printf("Average response time: %.2fns\n", avgResponse);
}

void sjf(int numProcesses, char* input[]) {
    ifstream inputFile(input[1]);
    multiset<vector<int>> processes;
    vector<int> waitingTimes, turnaroundTimes, responseTimes;

    waitingTimes.resize(numProcesses+1);
    turnaroundTimes.resize(numProcesses+1);
    responseTimes.resize(numProcesses+1);
    for (int i = 0; i < numProcesses; i++) {
        int arrivalTime, burstTime, priority;
        inputFile >> arrivalTime >> burstTime >> priority;
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
        
        idleTime += processToExecute[0]-time;
        time += (processToExecute[0]-time);

        finalOutput.push_back({time, processToExecute[3], processToExecute[1]});
        responseTimes[processToExecute[3]] = time-processToExecute[0];
        time += processToExecute[1];
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
}

void srtf(int numProcesses, char* input[]) {
    ifstream inputFile(input[1]);
    vector<vector<int>> originalProcessValues;
    multiset<vector<int>> processes;
    vector<bool> hasResponded;
    vector<int> waitingTimes, turnaroundTimes, responseTimes;

    originalProcessValues.resize(numProcesses+1);
    hasResponded.resize(numProcesses+1);
    waitingTimes.resize(numProcesses+1);
    turnaroundTimes.resize(numProcesses+1);
    responseTimes.resize(numProcesses+1);
    for (int i = 0; i < numProcesses; i++) {
        int arrivalTime, burstTime, priority;
        inputFile >> arrivalTime >> burstTime >> priority;
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
}

void solveTestCases(int testCase, char* input[]) {
    int numProcess;
    int timeQuantum;
    string schedulingAlgorithm;
    ifstream inputFile(input[1]);
    inputFile >> numProcess >> schedulingAlgorithm;

    cout << testCase << endl;
    if (schedulingAlgorithm == "FCFS") {
        fcfs(numProcess, input);
    } else if (schedulingAlgorithm == "RR") {
        inputFile >> timeQuantum;
        roundRobin(numProcess, timeQuantum, input);
    } else if (schedulingAlgorithm == "P") {
        priority(numProcess, input);
    } else if (schedulingAlgorithm == "SJF") {
        sjf(numProcess, input);
    } else {
        srtf(numProcess, input);
    }
}

int main(int argc, char* argv[]) {
    ifstream inputFile(argv[1]);
    int t; inputFile >> t;
    int i = 1;
    while (t--) {
        solveTestCases(i, argv);
        i++;
        if (t > 0)
            cout << endl;
    }
}