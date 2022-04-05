#pragma once
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>


//constants
const float P_fail = 0.05;//chance the payment was failed

const int Ntel = 3;//number of telephones
const int Ncook = 2;//number of cooks
const int Noven = 10;//number of ovens
const int Ndeliverer = 7;//number of deliverers

const unsigned int Torderlow = 1;//random time in which order is happening
const unsigned int Torderhigh = 5;//between (1-5)seconds

const int Norderlow = 1;//random number of pizzas 
const int Norderhigh = 5;//between(1-5)pizzas

const int Tpaymentlow = 1; //random time in which payment is happening
const int Tpaymenthigh = 2;//between(1-2)seconds

const int Cpizza=10;//cost per 1 pizza
 


const unsigned int Tbake = 10;//baking time of pizzas

const int Tprep = 1;//preperation time of 1 pizza
const int Tpack = 2;//packeting time of pizzas

const unsigned int Tlow = 5;//random delivering time 
const unsigned int Thigh = 15;//between(5-15)


//Variables
int number_available_tel;
int number_available_cooks;
int number_available_ovens;
int number_available_packs;
int number_available_deliverers;
int fail_order;
int success_order;
int incomings;
double waiting_time;
double cooling_time;
double packing_time;
double service_time;
double total_waiting_time;
double total_cooling_time;
double max_waiting_time;
double max_cooling_time;
double max_time;
double total_time;

//Mutexes
pthread_mutex_t mutex_number_available_tel	      = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_number_available_cooks      = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_number_available_ovens      = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_number_available_packs	  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_number_available_deliverers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_console                     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total_waiting_time          = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_waiting_time            = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total_cooling_time          = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_cooling_time            = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total_time		   		  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_time			 		  = PTHREAD_MUTEX_INITIALIZER;


//Conditions
pthread_cond_t cond_number_available_tel	     = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_number_available_cooks       = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_number_available_ovens       = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_number_available_packs		 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_number_available_deliverers  = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_console            = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_t_w_t              = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_m_w_t              = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_t_c_t              = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_m_c_t              = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_t_t                = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_m_t                = PTHREAD_COND_INITIALIZER;

//Functions 

void successful_mutex_action(int rs);
unsigned int sleep(unsigned int seconds);