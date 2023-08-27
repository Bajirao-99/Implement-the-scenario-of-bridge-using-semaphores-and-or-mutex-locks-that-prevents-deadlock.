#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

// Define constants
const int MAX_BRIDGE_CAPACITY = 1; // Maximum capacity of the bridge

// Define semaphores
sem_t northboundSemaphore;
sem_t southboundSemaphore;
sem_t bridgeSemaphore;
mutex mtx; // Mutex lock for shared resources

// Define variables
int northboundWaiting = 0; // Number of northbound persons waiting to cross the bridge
int southboundWaiting = 0; // Number of southbound persons waiting to cross the bridge
bool northboundCrossing = false; // Flag indicating whether a northbound person is currently crossing the bridge
bool southboundCrossing = false; // Flag indicating whether a southbound person is currently crossing the bridge


// Function to simulate northbound person crossing the bridge
void northboundPerson(string id) {
    // Sleep for a random period of time to simulate traveling
    this_thread::sleep_for(chrono::seconds(rand() % 5 + 1));

    // Try to acquire the northboundSemaphore
    sem_wait(&northboundSemaphore);

    // Enter the northbound waiting area (critical section)
    unique_lock<mutex> lock(mtx);
    northboundWaiting ++;
    cout << "Northbound person " << id << " is waiting to cross the bridge." << endl;
    lock.unlock();

    // Try to acquire the bridgeSemaphore
    sem_wait(&bridgeSemaphore);

    // Enter the bridge(critical section)
    lock.lock();
    northboundWaiting--;
    northboundCrossing = true;
    cout << "Northbound person " << id << " is crossing the bridge." << endl;
    lock.unlock();
        
    // Sleep for a random period of time to simulate crossing the bridge
    this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

    // Exit the bridge (critical section)
    lock.lock();
    northboundCrossing = false;
    cout << "Northbound person " << id << " has crossed the bridge." << endl;
    lock.unlock();

    // Release the bridgeSemaphore
    sem_post(&bridgeSemaphore);

    // Release the northboundSemaphore
    sem_post(&northboundSemaphore);

    // Try to allow southbound persons to cross the bridge if no northbound persons are waiting
    if (northboundWaiting == 0 && southboundWaiting > 0 && !southboundCrossing) {
        sem_post(&southboundSemaphore);
    }
}

// Function to simulate southbound person crossing the bridge
void southboundPerson(string id) {
    // Sleep for a random period of time to simulate traveling
    this_thread::sleep_for(chrono::seconds(rand() % 5 + 1));

    // Try to acquire the southboundSemaphore
    sem_wait(&southboundSemaphore);

    // Enter the southbound waiting area (critical section)
    unique_lock<mutex> lock(mtx);
    southboundWaiting ++;
    cout << "Southbound person " << id << " is waiting to cross the bridge." << endl;
    lock.unlock();

    // Try to acquire the bridgeSemaphore
    sem_wait(&bridgeSemaphore);

    // Enter the bridge(critical section)
    lock.lock();
    southboundWaiting--;
    southboundCrossing = true;
    cout << "Southbound person " << id << " is crossing the bridge." << endl;
    lock.unlock();

    // Sleep for a random period of time to simulate crossing the bridge
    this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

    // Exit the bridge (critical section)
    lock.lock();
    southboundCrossing = false;
    cout << "Southbound person " << id << " has crossed the bridge." << endl;
    lock.unlock();

    // Release the bridgeSemaphore
    sem_post(&bridgeSemaphore);

    // Release the southboundSemaphore
    sem_post(&southboundSemaphore);

    // Try to allow northbound persons to cross the bridge if no southbound persons are waiting
    if (southboundWaiting == 0 && northboundWaiting > 0 && !northboundCrossing) {
        sem_post(&northboundSemaphore);
    }
}


int main() {
    // Seed the random number generator
    srand(time(0));

    // Initialize semaphores
    sem_init(&northboundSemaphore, 0, 5);
    sem_init(&southboundSemaphore, 0, 5);
    sem_init(&bridgeSemaphore, 0, MAX_BRIDGE_CAPACITY);

    // Create northbound person threads
    thread northboundThreads[5];
    for (int i = 0; i < 5; i++) {
        northboundThreads[i] = thread(northboundPerson, "N" + to_string(i + 1));
    }

    // Create southbound person threads
    thread southboundThreads[5];
    for (int i = 0; i < 5; i++) {
        southboundThreads[i] = thread(southboundPerson, "S" + to_string(i + 1));
    }

    // Join northbound person threads
    for (int i = 0; i < 5; i++) {
        northboundThreads[i].join();
    }

    // Join southbound person threads
    for (int i = 0; i < 5; i++) {
        southboundThreads[i].join();
    }

    // Destroy semaphores
    sem_destroy(&northboundSemaphore);
    sem_destroy(&southboundSemaphore);
    sem_destroy(&bridgeSemaphore);

    return 0;
}
