/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author ??? (TODO: your name)
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lru_replacement.h"
#include "algorithm"

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
  auto it = page_map.find(page_num);
    if (it != page_map.end())
    {
        page_queue.erase(it->second);
        page_queue.push_front(page_num);
        page_map[page_num] = page_queue.begin();
    }
    // TODO: Update your data structure LRU replacement
}

// Access an invalid page, but free frames are available
void LRUReplacement::load_page(int page_num) {
  
    if (num_frames > 0)
    {
        int frame_num = max_frames - num_frames;
        page_table[page_num].frame_num = frame_num;
        page_table[page_num].valid = true;
        page_queue.push_front(page_num);
        page_map[page_num] = page_queue.begin();
        num_frames--;
    }
}

// Access an invalid page and no free frames are available
int LRUReplacement::replace_page(int page_num) {
    int victim = page_queue.back();
    int frame_num = page_table[victim].frame_num;

    page_queue.pop_back();
    page_map.erase(victim);
    page_queue.push_front(page_num);
    page_map[page_num] = page_queue.begin();

    page_table[page_num].frame_num = frame_num;
    page_table[page_num].valid = true;
    page_table[victim].valid = false;

    // Return the frame number that was freed up // TODO: Update your data structure LRU replacement and pagetable
    return 0;
}