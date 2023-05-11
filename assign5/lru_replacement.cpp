/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author Eric Hernandez & Chris Mead (TODO: your name)
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lru_replacement.h"
#include "algorithm"//used algorithm in order iterate through

// TODO: Add your implementation here
LRUReplacement::LRUReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
    // TODO: Complete this constructor
}

// TODO: Add your implementations for desctructor, touch_page, load_page, replace_page here
LRUReplacement::~LRUReplacement()
{
    // TODO: Add necessary code here
}

// Accesss a page alreay in physical memory
void LRUReplacement::touch_page(int page_num)
{
  auto it = page_map.find(page_num);//iterate through the page map
    if (it != page_map.end())//if it hasnt reached the end
    {
        page_queue.erase(it->second);//delete the value after the iterator
        page_queue.push_front(page_num);//push the page number at that point
        page_map[page_num] = page_queue.begin();//set the page number in the map to the beginning of the list
    }
    // TODO: Update your data structure LRU replacement
}

// Access an invalid page, but free frames are available
void LRUReplacement::load_page(int page_num) {
  
    if (num_frames > 0)//if there are more frames available
    {
        int frame_num = max_frames - num_frames;//calculate the frames avaible
        page_table[page_num].frame_num = frame_num;//fine the last point used in the page table and set the frame number equal to it
        page_table[page_num].valid = true;//find out if its valid
        page_queue.push_front(page_num);//push it onto the queue
        page_map[page_num] = page_queue.begin();//add the page number onto the unordered map
        num_frames--;//decrement the number of frames
    }
}

// Access an invalid page and no free frames are available
int LRUReplacement::replace_page(int page_num) {
    int victim = page_queue.back();//get the back of the list
    int frame_num = page_table[victim].frame_num;//find the frame number using the queue's frame number on the list

    page_queue.pop_back();//pop out the back
    page_map.erase(victim);//erase the point
    page_queue.push_front(page_num);//replace the frame number and push it to the front of the list
    page_map[page_num] = page_queue.begin(); //get the location of the page on the map

    page_table[page_num].frame_num = frame_num;//change the frame number
    page_table[page_num].valid = true;//check that its valid
    page_table[victim].valid = false;//check that its not valid

    // Return the frame number that was freed up // TODO: Update your data structure LRU replacement and pagetable
    return 0;
}