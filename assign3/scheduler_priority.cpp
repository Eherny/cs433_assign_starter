/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Eric Hernandez and Christopher Mead (TODO: your name)
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code


#include "scheduler_priority.h"
#include <algorithm> //algorithim library to support

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
SchedulerPriority::SchedulerPriority()
{
     waitAvg=0.0; //inilize the default values
     turnAvg=0.0; 
     count=0.0;
}

SchedulerPriority::~SchedulerPriority()
{
    while(!rq.empty()) //delete ready queue if not empty
    {
        rq.pop();
    };

}

void SchedulerPriority::init(std::vector<PCB>& process_list)
{

    std::sort(process_list.begin(), process_list.end(), [](const PCB& a, const PCB& b) { return a.priority > b.priority; }); //sort process based on priority

    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time; //get the burst time from the process
        processes p; //make a process p
        p.id=i->id; //get the id
        p.burst=burst; //set the burst into burst
        p.wt=0; //set wt to 0
        p.tat=0; //said turn around time to 0
        rq.push(p); //push object into ready queue
        count++; //incriment count
      }
}

void SchedulerPriority::print_results()
{
    
    
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting = " << waitAvg << "\n";   //turnAvg and waitAvg
      
}

void SchedulerPriority::simulate()
{
    int total_wait_time = 0; //initlize wait time
    int total_turnaround_time = 0; //intizlie turnaround time
    int current_time = 0; //set current time to 0
    
    // Run all the processes
    while(!rq.empty())
    {
        // Get the next process from the ready queue
        processes p = rq.front();
        rq.pop();

        // Record the start time of the process
        int start_time = current_time;

        // Simulate the execution of the process by incrementing the current time
        current_time += p.burst;

        // Record the end time of the process
        int end_time = current_time;

        // Calculate the waiting time and turn-around time of the process
        int wait_time = start_time;
        int turnaround_time = end_time;

        // Print the progress of the simulation
        std::cout << "Running process T" << p.id << " for " << p.burst << " time units" << std::endl;

        // Update the total turn-around and waiting times
        total_turnaround_time += turnaround_time;
        total_wait_time += wait_time;

        // Save the wait and turn-around times for the process
        p.wt = wait_time;
        p.tat = turnaround_time;

        // Add the process to the finished queue
        fq.push(p);
   }

   // Display the wait and turn-around times for each process
   while (!fq.empty())
   {
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