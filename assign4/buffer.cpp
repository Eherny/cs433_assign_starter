/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author ??? (TODO: your name)
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include <iostream>
#include "buffer.h"
Buffer::Buffer(int size)
{
    count = 0;
    maxSize=size;
}

    /**
     * @brief Destroy the Buffer object
     */
Buffer::~Buffer(){}
 


    /**
     * @brief Insert an item into the buffer
     * @param item the item to insert
     * @return true if successful
     * @return false if not successful
     */
bool Buffer::insert_item(buffer_item item)
{
    if(!is_full())
    {
        items.push(item);
        count++;
        return true;
    }
    return false;
}

    /**
     * @brief Remove an item from the buffer
     * @param item the item to remove
     * @return true if successful
     * @return false if not successful
     */
bool Buffer::remove_item(buffer_item *item)
{
if(!is_empty())
{
    *item=items.front();
    items.pop();
    count--;
    return true;
}
else
return false;
}
    /**
     * @brief Get the size of the buffer
     * @return the size of the buffer
     */
int Buffer::get_size()
{
    return maxSize;
}

    /**
     * @brief Get the number of items in the buffer
     * @return the number of items in the buffer
     */
int Buffer::get_count()
{
    return count;
}

    /**
     * @brief Chceck if the buffer is empty
     * @return true if the buffer is empty, else false
     */
bool Buffer::is_empty()
{
    if(count==0)
    {
    return true;
    }
    else
    return false;
}
    /**
     * @brief Check if the buffer is full
     * @return true if the buffer is full, else false
     */
bool Buffer::is_full()
{
    return count>=maxSize;
  
}

    /**
     * @brief Print the buffer
     */
void Buffer::print_buffer() {

  std::cout << "Buffer: [";
  
  if (!is_empty()) {
    // Iterate through the queue and display
    for (std::queue<buffer_item> copy = items; !copy.empty(); copy.pop()) { // Queue copy
      std::cout << copy.front();
      if (copy.size() != 1) {
        std::cout << ", ";
      }
    } 
  }

  std::cout << "]" << std::endl;
}

// TODO: Add your implementation of the buffer class here