/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author ??? (TODO: your name)
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_priority_rr.h"

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
#include <algorithm>

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum)
{
     waitAvg=0.0;
     turnAvg=0.0;
     count=0.0;
     this->time_quantum=time_quantum;
     
}

SchedulerPriorityRR::~SchedulerPriorityRR()
{
    while(!rq.empty())
    {
        rq.pop();
    };

}

void SchedulerPriorityRR::init(std::vector<PCB>& process_list)
{

    

    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time;
        processes p;
        p.id=i->id;
        p.burst=burst;
        p.wt=0;
        p.tat=0;
        p.pr=i->priority;
        p.rt=burst;
        rq.push(p);
        count++;
      }
}

void SchedulerPriorityRR::print_results()
{
    
    
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting = " << waitAvg << "\n";  
      
}

void SchedulerPriorityRR::simulate() {
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int current_time = 0;

    while (!rq.empty()) {
        // Sort the processes by priority
        std::vector<processes> sorted_processes;
        while (!rq.empty()) {
            sorted_processes.push_back(rq.front());
            rq.pop();
        }
        std::sort(sorted_processes.begin(), sorted_processes.end(), [](const processes& p1, const processes& p2) {
            return p1.pr > p2.pr;
        });
        for (const auto& p : sorted_processes) {
            rq.push(p);
        }

        // Get the next process from the ready queue
        processes p = rq.front();
        rq.pop();

        // Record the start time of the process
        int start_time = current_time;

        // Simulate the execution of the process for the time quantum or until it completes
        int rt = p.rt;
        int quantum_time = std::min(time_quantum, rt);
        current_time += quantum_time;
        rt -= quantum_time;
        p.rt = rt;

        // Print the status of the process
        std::cout << "Running Process T" << p.id << " for " << quantum_time << " time units" << std::endl;

        if (rt > 0) {
            // If the process has not completed, add it back to the ready queue
            rq.push(p);
        } else {
            // If the process has completed, record the end time and calculate the waiting time and turn-around time
            int end_time = current_time;
            int wait_time = end_time - p.burst;
            int turnaround_time = end_time;

            // Update the total turn-around and waiting times
            total_turnaround_time += turnaround_time;
            total_wait_time += wait_time;

            // Save the wait and turn-around times for the process
            p.wt = wait_time;
            p.tat = turnaround_time;

            // Add the process to the finished queue
            fq.push(p);
        }
    }

    // Display the wait and turn-around times for each process
    while (!fq.empty()) {
        processes p = fq.front();
        fq.pop();

        std::cout << "T" << p.id << " turn-around time = " << p.tat << ", waiting time = " << p.wt << std::endl;
    }

    // Calculate the average turn-around and waiting times
    turnAvg = (double)total_turnaround_time / count;
    waitAvg = (double)total_wait_time / count;
}
