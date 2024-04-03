#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

#define THREAD_COUNT 8
#define NUM_GUESTS 500000
#define PRINT_STUFF false

// Represents on present in the chain
class Node
{
public:
    int data;
    std::shared_ptr<Node> next;
    Node(int data) : data(data), next(nullptr) {}
};

// Represents the chain of presents
class LinkedList
{
private:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    std::mutex mutex;
    size_t size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    /// @brief Inserts a present into the chain in ascending order
    /// @param data the id of the guest who gave the present
    void insert(int data)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto newNode = std::make_shared<Node>(data);

        // If the chain is empty, set the head and tail to the new node
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        // If the data is less than the head's data, insert at the head
        else if (data <= head->data)
        {
            newNode->next = head;
            head = newNode;
        }
        // Otherwise, insert the present in ascending order
        else
        {
            auto current = head;
            while (current->next != nullptr && current->next->data < data)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            if (newNode->next == nullptr)
            {
                tail = newNode;
            }
        }
        size++;
    }

    /// @brief Searches for a present in the chain
    /// @param data the id of the guest who gave the present
    /// @return true if the present is found, false otherwise
    bool search(int data)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto temp = head;
        while (temp != nullptr)
        {
            if (temp->data == data)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    /// @brief Removes the present at the head of the chain
    /// @return the id of the guest who gave the present
    int removeHead()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (head == nullptr)
        {
            return -1;
        }
        int data = head->data;
        head = head->next;
        size--;

        return data;
    }

    /// @brief returns the size of the chain
    /// @return the size of the chain
    size_t getSize()
    {
        return size;
    }

    /// @brief checks if the chain is empty
    /// @return true if the chain is empty, false otherwise
    bool isEmpty()
    {
        return size == 0;
    }
};

std::mutex mutex;

std::unordered_set<int> generateShuffledSet(int size)
{
    // Fill the vector with numbers from 1 to size
    std::vector<int> numbers(size);
    std::iota(numbers.begin(), numbers.end(), 1);

    // Shuffle the vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);

    // Insert the shuffled numbers into a set
    std::unordered_set<int> shuffledSet(numbers.begin(), numbers.end());

    return shuffledSet;
}

void performTask(LinkedList *list, std::unordered_set<int> *giftBag, std::unordered_set<int> *cards)
{
    while (giftBag->size() > 0 || list->getSize() > 0)
    {
        // Servant randomly chooses a task to perform.
        int task = rand() % 3;

        switch (task)
        {
        // Take a gift from the gift bag and add it to the list.
        case 0:
        {
            mutex.lock();

            // If the gift bag is empty, skip this task.
            if (giftBag->empty() || giftBag->begin() == giftBag->end())
            {
                mutex.unlock();
                continue;
            }

            // Take a gift from the gift bag and add it to the list.
            auto it = giftBag->begin();
            int num = *(giftBag->begin());
            giftBag->erase(it);
            mutex.unlock();

            list->insert(num);
            break;
        }
        // Write a thank you card.
        case 1:
        {
            // If no gifts are in the list, skip this task.
            if (list->isEmpty())
            {
                continue;
            }

            int guest = list->removeHead();

            if (guest == -1)
            {
                continue;
            }

            mutex.lock();

            if (PRINT_STUFF)
            {
                std::cout << "Servant wrote a thank you card for guest " << guest << std::endl;
            }

            cards->insert(guest);
            mutex.unlock();

            break;
        }
        // Search for a gift.
        case 2:
        {
            int randGuest = (rand() % NUM_GUESTS) + 1;
            bool found = list->search(randGuest);
            if (PRINT_STUFF)
            {
                std::cout << "Guest " << randGuest << " found: " << (found ? "true" : "false") << std::endl;
            }
            break;
        }
        }
    }
}

int main()
{
    LinkedList list;
    std::unordered_set<int> giftBag = generateShuffledSet(NUM_GUESTS);
    std::unordered_set<int> cards;
    std::array<std::thread, THREAD_COUNT> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Create thread for each servant
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = std::thread(performTask, &list, &giftBag, &cards);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Cards written: " << cards.size() << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
