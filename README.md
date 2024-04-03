# COP4520 Assignment 3
## Description
This program was written for the Concepts of Parallel and Distributed Processing course at the
    University of Central Florida.

### Problem 1: Birthday Presents Party
The Minotaur’s birthday party was a success. The Minotaur received a lot of presents 
    from his guests. The next day he decided to sort all of his presents and start writing
    “Thank you” cards. Every present had a tag with a unique number that was associated
    with the guest who gave it. Initially all of the presents were thrown into a large bag with
    no particular order. The Minotaur wanted to take the presents from this unordered bag
    and create a chain of presents hooked to each other with special links (similar to storing
    elements in a linked-list). In this chain (linked-list) all of the presents had to be ordered
    according to their tag numbers in increasing order. 
The Minotaur asked 4 of his servants
    to help him with creating the chain of presents and writing the cards to his guests. Each
    servant would do one of three actions in no particular order: 
1. Take a present from the
    unordered bag and add it to the chain in the correct location by hooking it to the
    predecessor’s link. The servant also had to make sure that the newly added present is
    also linked with the next present in the chain. 
2. Write a “Thank you” card to a guest and
    remove the present from the chain. To do so, a servant had to unlink the gift from its
    predecessor and make sure to connect the predecessor’s link with the next gift in the
    chain. 
3. Per the Minotaur’s request, check whether a gift with a particular tag was
    present in the chain or not; without adding or removing a new gift, a servant would scan
    through the chain and check whether a gift with a particular tag is already added to the
    ordered chain of gifts or not. 
As the Minotaur was impatient to get this task done
    quickly, he instructed his servants not to wait until all of the presents from the
    unordered bag are placed in the chain of linked and ordered presents. Instead, every
    servant was asked to alternate adding gifts to the ordered chain and writing “Thank you”
    cards. The servants were asked not to stop or even take a break until the task of writing
    cards to all of the Minotaur’s guests was complete.

#### Approach and Reasoning
The simulation sees an unordered list of presents that the servants, represented by four threads,
    have to go through. When s servant picks the first gift out of the list representing the bag,
    they stick it into an ordered linked list. The linked list make use of coarse-grained locking to
    protect the critical sections of the program. The non-atomic methods use mutex at the beginning 
    and end of their execution to ensure mutual exclusion. When writing thank you cards, a servant
    removes the head of the linked list to determine the guest to write the card for. This is
    represented by putting the id into a new set. And the linked list's `search` function is
    sufficient to represent the check for a guest's gift in the chain.



### Problem 2: Atmospheric Temperature Reading Module
You are tasked with the design of the module responsible for measuring the atmospheric
    temperature of the next generation Mars Rover, equipped with a multicore CPU and 8
    temperature sensors. The sensors are responsible for collecting temperature readings at
    regular intervals and storing them in shared memory space. 
The atmospheric temperature module has to compile a report at the end of every hour, 
    comprising the top 5 highest temperatures recorded for that hour, the top 5 lowest 
    temperatures recorded for that hour, and the 10-minute interval of time when the 
    largest temperature difference was observed. The data storage and retrieval of the 
    shared memory region must be carefully handled, as we do not want to delay a sensor and
    miss the interval of time when it is supposed to conduct temperature reading. 
Design and implement a solution using 8 threads that will offer a solution for this task.
    Assume that the temperature readings are taken every minute.

#### Approach and Reasoning
In this solution, all critical sections are protected by mutexes and lock guards. Additionally, all 
    sensors are required to be ready and available before continuing due to the `sensorStatus` 
    vector.
To represent the hourly report we have a function that print out the interval of the hour in which
    the highest difference was observed. We also have two fucntions that use minheaps and maxheaps 
    to find the highest and lowest temperatures of the hour.
All functions have been proven to execute quickly as a run of 240 hours, or 10 days, executes within 0.2 seconds.


## Installation
1. Download and unzip the source files into a Unix system or WSL filepath.

## Usage
1. Open a terminal or command prompt.
2. Navigate to the project directory.
3. Run the folling commands:
    ```bash
    make p1
    make run-p1
    ```
    You can replace `p1` and `run-p1` with `p2` and `run-p2`, repectively, to run the solution
    for problem 2.
