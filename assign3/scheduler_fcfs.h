/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.h
 * @author Chris Mead and Eric Hernandez (TODO: your name)
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#ifndef ASSIGN3_SCHEDULER_FCFS_H
#define ASSIGN3_SCHEDULER_FCFS_H
#include "scheduler.h"
#include <queue>
/**
 * @brief This Scheduler class implements the FCFS scheduling algorithm.
 */
class SchedulerFCFS : public Scheduler {
private:
    // TODO: add necessary member variables here for your implementation
    
    double waitAvg;//find the ending wait time 
    double turnAvg; //finding the ending turn around time
    struct processes //make a struct called processes that has the id burst wt and turn around time
    {
        int id;
        int burst;
        int wt; //wait time
        int tat; //turn around time
    };
    queue<processes> rq; //ready queue of processes
    queue<processes> fq; //final queue of processes
    int count;//number of processes

public:
    /**
     * @brief Construct a new SchedulerFCFS object
     */
    SchedulerFCFS();
    /**
     * @brief Destroy the SchedulerFCFS object
     */
    ~SchedulerFCFS() override;

    /**
     * @brief This function is called once before the simulation starts.
     *        It is used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init(std::vector<PCB>& process_list) override;

    /**
     * @brief This function is called once after the simulation ends.
     *        It is used to print out the results of the simulation.
     */
    void print_results() override;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override;
};
#endif //ASSIGN3_SCHEDULER_FCFS_H