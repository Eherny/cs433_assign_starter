/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Chris Mead and Eric Hernandez (TODO: your name)
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here

SchedulerFCFS::SchedulerFCFS()
{
     waitAvg=0.0; //setting the default values of the stored variables in the constructor
     turnAvg=0.0;
     count=0.0;
}

SchedulerFCFS::~SchedulerFCFS()
{
    while(!rq.empty()) //destructor that destroys the queues if they are not empty
    {
        rq.pop();
    };

}

void SchedulerFCFS::init(std::vector<PCB>& process_list)
{
    int id=1; //the starting id number
    for(auto i=process_list.begin();i!=process_list.end(); i++)
      {
        int burst = i->burst_time; //gets the burst time of the process list and stores it into burst
        processes p; //makes a process p
        p.id=id; //sets the id of the process 
        p.burst=burst; //settings the burst of the process
        p.wt=0; //sets the default wait time to 0
        p.tat=0; //sets the default turn around time to 0
        rq.push(p); // pushes the object into p
        id++; //incriments id
        count++; //incriments count
      }
}

void SchedulerFCFS::print_results() //this function princes the avg turn around time and avg wait time o
{
    
    
    std::cout << "Average turn-around time = " << turnAvg << ", Average waiting = " << waitAvg << "\n";  
      
}

void SchedulerFCFS::simulate()
{
    int total_wait_time = 0;//storing the total wait time
    int total_turnaround_time = 0; //storing the turn around time
    int current_time = 0; //keeping track of the time
    
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


