/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author Eric Hernandez and Christopher Mead (TODO: your name)
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_priority_rr.h"

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
#include <algorithm> //include libary to use sort


// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum)
{
     waitAvg=0.0; //initlize waitavg to 0
     turnAvg=0.0; //initilize turn avg to 0
     count=0.0; //initilize count to 0
     this->time_quantum=time_quantum; //get the time quantum entered when running the program
     
}

SchedulerPriorityRR::~SchedulerPriorityRR()
{
    while(!rq.empty()) //empties the queues if not empty
    {
        rq.pop();
    };

}

void SchedulerPriorityRR::init(std::vector<PCB>& process_list)
{

    

    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time; //gets the burst time of each process in list
        processes p; //makes a processes object
        p.id=i->id+1; //gets id of processes
        p.burst=burst; //puts burst into process
        p.wt=0; //sets process wt to 0
        p.tat=0; //sets process turn around time to 0
        p.pr=i->priority;// gets the priority of the object 
        p.rt=burst; //sets the remaining time to the burst
        rq.push(p); //push the object into the ready queue
        count++;//incriment the count 
      }
}

void SchedulerPriorityRR::print_results()
{
    
    //prints turnavg and waitavg
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting time = " << waitAvg << "\n";  
      
}

void SchedulerPriorityRR::simulate() {
    int total_wait_time = 0; //store the total wait time
    int total_turnaround_time = 0; //store turn around time
    int current_time = 0; //store current time
    int rt=0;
    int quantum_time;
 std::vector<processes> sorted_processes; //make a vector of processes
    while (!rq.empty()) {
        // Sort the processes by priority
        std::vector<processes> sorted_processes; //make a vector of processes
        while (!rq.empty()) {
            sorted_processes.push_back(rq.front()); //push the proceesses into the vector
            rq.pop();
        }
        std::sort(sorted_processes.begin(), sorted_processes.end(), [](const processes& p1, const processes& p2) { 
            return p1.pr > p2.pr; //sorts the vector based on priority
        });
        for (const auto& p : sorted_processes) { //push the sorted processes back into the ready queue
            rq.push(p);
        }

        // Get the next process from the ready queue
        processes p = rq.front(); //make the processes p the front
        rq.pop(); //pop it out

        // Record the start time of the process
        int start_time = current_time;
    if(p.pr!=rq.front().pr||(rq.size()==0))
    {
        rt=p.rt;
        current_time += rt;
        quantum_time= rt;
        rt = 0;
        p.rt = rt;
    }
    else 
    {
        // Simulate the execution of the process for the time quantum or until it completes
         rt = p.rt;//get the remaining time
         quantum_time = std::min(time_quantum, rt); //sets the quantium time to the minimum time if it minimum time is less
        current_time += quantum_time;//incriment by the quantum time
        rt -= quantum_time; //subtract the remaining time from the quantum time
        p.rt = rt; //store the value into the remaining time
    }
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
