/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Eric Hernandez & Chris Mead (TODO: your name)
 * @brief A class implementing the LIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lifo_replacement.h"

// TODO: Add your implementation here
LIFOReplacement::LIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
    // TODO: Add additional implementation code
}

// TODO: Add your implementations for desctructor, load_page, replace_page here
LIFOReplacement::~LIFOReplacement() {
    // TODO: Add necessary code here
}

// Access an invalid page, but free frames are available
void LIFOReplacement::load_page(int page_num) {
    // TODO: Add necessary code here
    page_table[page_num].frame_num = max_frames - num_frames; // Assigning it
    page_table[page_num].valid = true; // if the page number is valid
    page_stack.push(page_num);//push the page number onto the stack
    num_frames--;
}

// Access an invalid page and no free frames are available
int LIFOReplacement::replace_page(int page_num) {
    // TODO: Add necessary code here
    int front_num = page_stack.top(); // Get the page number top of the queue
    page_stack.pop();
    page_stack.push(page_num);
    // page_table[page_num].frame_num = page_table[front_num].frame_num; // Set the frame number
    page_table[page_num].valid = true;
    page_table[front_num].valid = false; // Set to false
    return 0;
}