/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Eric Hernandez and Christopher Mead (TODO: your name)
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
//

#include "scheduler_rr.h"
#include <algorithm>

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
SchedulerRR::SchedulerRR(int time_quantum)
{
     waitAvg=0.0; //set the default values
     turnAvg=0.0;
     count=0.0;
     this->time_quantum=time_quantum; //make the time quantum equal to the time set when calling
     
}

SchedulerRR::~SchedulerRR()
{
    while(!rq.empty()) //empties queue if not empty
    {
        rq.pop();
    };

}

void SchedulerRR::init(std::vector<PCB>& process_list)
{

    

    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time; //get burst time
        processes p; //make a process object
        p.id=i->id+1; //get the id
        p.burst=burst; //set the burst time to captured burst time
        p.wt=0; //set wt to 0
        p.tat=0; //set turn around time to 0
        p.pr=i->priority; //get the priority and set it to object
        p.rt=burst; //set the remaing time to burst
        rq.push(p); //push the object
        count++; //incriment count
      }
}

void SchedulerRR::print_results()
{
    
    //prints out turnavg and wait avg results
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting time = " << waitAvg << "\n";  
      
}

void SchedulerRR::simulate() {
    int total_wait_time = 0; //initilize total wait time 
    int total_turnaround_time = 0; //initilize turn around time
    int current_time = 0; //initilize the current time

 std::vector<processes> sorted_processes; //make a vector of processes

  
    while (!rq.empty()) {
        // Get the next process from the ready queue
        processes p = rq.front(); //get the first process
        rq.pop(); //pop it out of ready queue

        // Record the start time of the process
        int start_time = current_time; 

        // Simulate the execution of the process for the time quantum or until it completes
        int rt = p.rt;
        int quantum_time = std::min(time_quantum, rt);//used to get the time if queue finishes before the time quantum
        current_time += quantum_time; 
        rt -= quantum_time;//subtract remainging time
        p.rt = rt; //update remaing time

        // Print the status of the process
        std::cout << "Running Process T" << p.id << " for " << quantum_time << " time units" << std::endl;

        if (rt > 0) {
            // If the process has not completed, add it back to the ready queue
            rq.push(p);
        } else {
            // If the process has completed, record the end time and calculate the waiting time and turn-around time
            int end_time = current_time;
            int wait_time = end_time-p.burst;
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
while (!fq.empty()) {
            sorted_processes.push_back(fq.front()); //push the proceesses into the vector
            fq.pop();
        }
        std::sort(sorted_processes.begin(), sorted_processes.end(), [](const processes& p1, const processes& p2) { 
            return p1.id < p2.id; //sorts the vector based on priority
        });
        for (const auto& p : sorted_processes) { //push the sorted processes back into the ready queue
            fq.push(p);
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




// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here