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
     auto it = std::find(page_list.begin(), page_list.end(), page_num);
   
        // If the page is in the list, move it to the front (most recently used)
        page_list.erase(it);
        page_list.push_front(page_num);
    
        // If the page is not in the list, it has not been loaded into memory yet
        // So, we need to call load_page to add it to memory
    // TODO: Update your data structure LRU replacement
}

// Access an invalid page, but free frames are available
void LRUReplacement::load_page(int page_num) {
      page_table[page_num].frame_num = max_frames - num_frames; // Assigning it
    page_table[page_num].valid = true; // 
    page_list.push_front(page_num);
    num_frames--;// TODO: Update your data structure LRU replacement and pagetable
}

// Access an invalid page and no free frames are available
int LRUReplacement::replace_page(int page_num) {
       int lru_page = page_list.back();
    
    // Remove the LRU page from the list and the page table
    page_list.pop_back();
    // Load the new page into the frame that was occupied by the LRU page

    
    // Add the new page to the front of the list
    page_list.push_front(page_num);
    page_table[page_num].valid = true;
    page_table[lru_page].valid = false;

    // Return the frame number that was freed up // TODO: Update your data structure LRU replacement and pagetable
    return 0;
}