/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Eric Hernandez and Christopher Mead (TODO: your name)
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_sjf.h"
#include <algorithm> //algorithm for sort function

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here
SchedulerSJF::SchedulerSJF()
{
     waitAvg=0.0; //setting defauled values
     turnAvg=0.0;
     count=0.0;
}

SchedulerSJF::~SchedulerSJF()
{
    while(!rq.empty())
    {
        rq.pop(); //empty queue if not finished
    };

}

void SchedulerSJF::init(std::vector<PCB>& process_list)
{

    std::sort(process_list.begin(), process_list.end(), [](const PCB& a, const PCB& b) { return a.burst_time < b.burst_time; });
//sort the processes by burst time
    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time; //get the burst time
        processes p; //make a process p
        p.id=i->id+1; //get the id from process lsit
        p.burst=burst; //set burst time to process
        p.wt=0; //set wait time to 0
        p.tat=0; //set turn around time to 0
        rq.push(p); //push object to ready queue
        count++;
      }
}

void SchedulerSJF::print_results()
{
    
    //print results
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting time = " << waitAvg << "\n";  
      
}

void SchedulerSJF::simulate()
{
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int current_time = 0;
    std::vector<processes> sorted_processes; //make a vector of processes
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