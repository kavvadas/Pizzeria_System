# Pizzeria_System

The concept of the system we have implemented is that we need to succesfully complete a pizza order and deliver it too.
We have 3 files we need to talk about, but before that I need to emphasize that we are working with a multithreaded system in which we have different mutexes running in the "backround".

# 1st File: p3190064-pizza.h (the header file)

In this file we mostly declare all the constants, variables, mutexes, conditions and some functions that we implement in the .c file.

## The constants in simple words are the resources we are providing as a system. For example we have the number of telephones, cooks, ovens, deliverers,cost/per pizza etc. We also have spaces in which a number is chosen randomly by the system. For example Norderlow=1, Norderhigh=5 and we choose a random number between 1 and 5 and that defines the number of pizzas a customer will order as we run the program. We have also commented each line in .h file so you can understand to what constants, variables etc. are refer to. Furthermore the value of the constants is clearly our choice.

## Some variales we can say that they are like an update for our resources. For example we have number_available_tel where we update the value of the available telephones on line. We will see further on when we discuss about the threads mutexes locks and unlocks etc. Other variables are used for timers. We using the timers so we can create a table with some statistics as the order is totally finished. We have variables that updates the incomings for the pizzeria the orders that failed and succeded to take place.

## The interesting and most important part are the mutexes. We have mutex initializers and condition initialiazers. We using mutexes so we can lock and unlock resource. When we say lock and unlock we mean that we bind a resource on a single thread(operation). For example when a customer makes a call we bind one telephone of three that we have. So when we have 0 available telephones we can't accept a call from a customer, it's not possible. Here the condition wait is used for that exact reason, so we can unlock the resource update the variable and continue with other customer. This thought is the same for all mutexes and operations of the system.

## Lastly we have the two functions. 
1. successful_mutex_action(int rs): we check everytime we use a mutex if it's working right or not.
2. unsigned int sleep(unsigned int seconds): it's like taking a break for some time, it depends on what operation is running. For example when a customer wants to pay we sleep for the paying time.
We finished with the .h file. We move on to the .c file where the fun part begins.

# 2nd File: p3190064-pizza.c (the coding file)

In this section we implement all the coding. For starters when we say multithreaded programming in this system we mean that we allow multiple orders to be created, executing independently but concurrently sharing the same resources as we mentioned previously. Now, let's explain how a thread is running.
I will not get into the pointer and reference part but it's a must on this programming language and totally useful working with threds.

## Firstly we need the function "void * order(void * order_id)" which creates an order(order_id is the name of the customer) and makes all the operations of the order.

### A customer is calling: When a customer is calling we lock the available_tel mutex, check if it's running correctly. Then we check if there is available telephone. If it's not we use the cond_wait to wait for the next available telephone in the line. If we accepted the call we unlock the mutex.
### A customer is paying: 'This is happening if only we have accepted a call from the customer. So if we have no available tel. customer keeps on waiting'. We have to use sleep here and we use the constant paying time for definition of the function. We calculate it with a random number between (Tpaymenthigh and Tpaymentlow) we make the sleep and then we again lock the mutex telephone unbind the resource and unlock the mutex. 
#### Keep in mind as a general rule when we want to bind or unbind a resource we HAVE TO lock and unlock the mutex.
### Check the payment: To do that we calculate a random input probability and if its greater than the constant P_fail, we've created, we're getting True(successful). If it's not successful we update the fail_order and if it's not we update the success_order.
