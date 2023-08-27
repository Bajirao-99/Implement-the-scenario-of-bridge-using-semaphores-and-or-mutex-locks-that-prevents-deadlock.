This is a C++ program that simulates the crossing of a bridge by northbound and southbound persons. The program
uses semaphores and mutex locks to control access to shared resources and ensure that only one person can cross
the bridge at a time.

Steps to execute the program:
To execute the program type the command 'make run'

A brief description of the code:
The program uses three semaphores: northboundSemaphore, southboundSemaphore, and bridgeSemaphore. The
northboundSemaphore and southboundSemaphore are used to control access to the bridge by northbound and southbound
persons, respectively. The bridgeSemaphore is used to limit the number of persons crossing the bridge at any given
time.

The program also uses a mutex lock to control access to shared variables, such as the number of persons waiting to
cross the bridge and the flags indicating whether a person is currently crossing the bridge.

When a person thread is created, it first sleeps for a random period of time to simulate traveling. The person then
tries to acquire the appropriate semaphore (northboundSemaphore or southboundSemaphore) to enter the waiting area.
Once in the waiting area, the person increments the appropriate waiting counter and waits for the bridge to become
available.

When the bridge becomes available (i.e., the bridgeSemaphore is acquired), the person decrements the waiting
counter and sets the appropriate crossing flag (i.e., northboundCrossing or southboundCrossing) to indicate that
they are crossing the bridge. The person then sleeps for a random period of time to simulate crossing the bridge.

After crossing the bridge, the person releases the bridgeSemaphore and the appropriate semaphore
(northboundSemaphore or southboundSemaphore). The person then checks whether there are any waiting persons in the
opposite direction and, if so, signals the appropriate semaphore to allow them to cross.
