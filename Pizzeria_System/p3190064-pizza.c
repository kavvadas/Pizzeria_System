#include "p3190064-pizza.h"


unsigned int seed;


// Handling of mutexe's lock/unlock errors
void successful_mutex_action(int rs)
{
	if (rs != 0)
	{
		printf("Error in mutex! Error Code: %d \n", rs);
		pthread_exit(&rs);
	}
}


void * order(void * order_id)
{	

	int rs;
	int oid = *(int *)order_id;
	struct timespec order_start;

	rs=clock_gettime(CLOCK_REALTIME, &order_start); //starting time
	successful_mutex_action(rs);
	
	
	//Order
	rs = pthread_mutex_lock(&mutex_number_available_tel);
	successful_mutex_action(rs);
	struct timespec telephone_finish;
	while(number_available_tel==0){
		rs=clock_gettime(CLOCK_REALTIME, &telephone_finish);
		successful_mutex_action(rs);
		printf(" No available telephones , please wait ... \n");
		pthread_cond_wait(&cond_number_available_tel, &mutex_number_available_tel);   
	}
	printf("Please tell us your order %d.\n", oid);
	number_available_tel--; 
	    
	pthread_cond_signal(&cond_number_available_tel);
	rs = pthread_mutex_unlock(&mutex_number_available_tel);
	successful_mutex_action(rs);    
	  
	  
	double paying_time= rand_r(&seed) % Tpaymenthigh + Tpaymentlow;
	sleep(paying_time);
	
	rs = pthread_mutex_lock(&mutex_number_available_tel);
	successful_mutex_action(rs);
	
	
	number_available_tel++;
	
	
	pthread_cond_signal(&cond_number_available_tel);
	rs = pthread_mutex_unlock(&mutex_number_available_tel);
	successful_mutex_action(rs);    

	//Paying
	int succ = rand_r(&seed) % 100 / 100.0f > P_fail;
			
	if(!succ)//Paying was unsuccessful
	{
	printf ("I am sorry %d ,your card is not valid\n ",oid);
	fail_order++;
	pthread_exit(NULL);
	}    
	printf ("Customer: %d ,your order is ready to be prepared!\n",oid);
	success_order++;
	
	
	int random_number_pizza = rand_r(&seed) % Norderhigh + Norderlow;
	printf("Customer: %d ,ordered: %d pizzas\n",oid,random_number_pizza);
	incomings= incomings + Cpizza*random_number_pizza;
	

	

  
	//Preparing the pizzas
	rs = pthread_mutex_lock(&mutex_number_available_cooks);
	
	successful_mutex_action(rs);
	
	while(number_available_cooks == 0)
	{
		pthread_cond_wait(&cond_number_available_cooks, &mutex_number_available_cooks);
	}
	
	number_available_cooks--;
	

	pthread_cond_signal(&cond_number_available_cooks);
	rs = pthread_mutex_unlock(&mutex_number_available_cooks);
	successful_mutex_action(rs);

	

	
	int prep_time = Tprep*random_number_pizza;
	sleep(prep_time); //order preparation
	
	
	//Cooking
	rs = pthread_mutex_lock(&mutex_number_available_ovens);
	successful_mutex_action(rs);

	

	while(number_available_ovens <random_number_pizza)
	{	
		printf("Order: %d ,waiting for an available oven...\n", oid);
		pthread_cond_wait(&cond_number_available_ovens,&mutex_number_available_ovens);
	}

	number_available_ovens-=random_number_pizza;
	
	printf("Baking order: %d\n", oid);



	

	pthread_cond_signal(&cond_number_available_ovens);	
	rs = pthread_mutex_unlock(&mutex_number_available_ovens);
	successful_mutex_action(rs);

	rs = pthread_mutex_lock(&mutex_number_available_cooks);
	successful_mutex_action(rs);

	
	
	number_available_cooks++; // release cook
	

	pthread_cond_signal(&cond_number_available_cooks);
	rs = pthread_mutex_unlock(&mutex_number_available_cooks);
	successful_mutex_action(rs);

	sleep(Tbake);//Baking the pizzas
	struct timespec cooling_start;
	rs=clock_gettime(CLOCK_REALTIME, &cooling_start); //Start cooling time
	successful_mutex_action(rs);
	//Packing
	

	rs = pthread_mutex_lock(&mutex_number_available_packs);
	successful_mutex_action(rs);
	
	while(number_available_packs==0){
		printf("Order: %d ,waiting for available packer...\n",oid);
		pthread_cond_wait(&cond_number_available_packs,&mutex_number_available_packs);
	}
	
	
	number_available_packs--;
	printf("Packing order: %d\n", oid);
	
	pthread_cond_signal(&cond_number_available_packs);	
	rs = pthread_mutex_unlock(&mutex_number_available_packs);
	successful_mutex_action(rs);
	
	rs = pthread_mutex_lock(&mutex_number_available_ovens);
	successful_mutex_action(rs);

	
	number_available_ovens+=random_number_pizza; // ovens release
	
	pthread_cond_signal(&cond_number_available_ovens);	
	rs = pthread_mutex_unlock(&mutex_number_available_ovens);
	successful_mutex_action(rs);
	
	sleep(Tpack);//Packing time
	struct timespec packing_finish;
	rs=clock_gettime(CLOCK_REALTIME, &packing_finish); //finish packing time
	successful_mutex_action(rs);
	
	rs = pthread_mutex_lock(&mutex_number_available_packs);
	successful_mutex_action(rs);
	
	number_available_packs++;//release pack
	
	pthread_cond_signal(&cond_number_available_packs);	
	rs = pthread_mutex_unlock(&mutex_number_available_packs);
	successful_mutex_action(rs);
	
	

	//Delivery
	
	rs = pthread_mutex_lock(&mutex_number_available_deliverers);
	successful_mutex_action(rs);
	
	
	
	while(number_available_deliverers == 0)
	{	
		printf("Order: %d ,waiting for an available deliverer...\n", oid);
		pthread_cond_wait(&cond_number_available_deliverers,&mutex_number_available_deliverers);
	}
	

	number_available_deliverers--;
	printf("Delivering order: %d\n", oid);
	


	pthread_cond_signal(&cond_number_available_deliverers);	
	rs = pthread_mutex_unlock(&mutex_number_available_deliverers);
	successful_mutex_action(rs);

	double Tdelivery = rand_r(&seed) % Thigh + Tlow;//delivery time
	
	sleep(Tdelivery); // Delivering pizza
	struct timespec cooling_finish;
	rs = clock_gettime(CLOCK_REALTIME, &cooling_finish); //finish cooling time
	successful_mutex_action(rs);
	
	sleep(Tdelivery); // Returning to pizzeria

	
	rs = pthread_mutex_lock(&mutex_number_available_deliverers);
	successful_mutex_action(rs);
	
	number_available_deliverers++; // deliverer release

	pthread_cond_signal(&cond_number_available_deliverers);
	rs = pthread_mutex_unlock(&mutex_number_available_deliverers);
	successful_mutex_action(rs);
	
	struct timespec order_finish;
	rs=clock_gettime(CLOCK_REALTIME, &order_finish); // finish time
	successful_mutex_action(rs);
	
	//print order info and update timers

	rs = pthread_mutex_lock(&mutex_console);
	successful_mutex_action(rs);

	
	waiting_time = order_finish.tv_sec - order_start.tv_sec;
	cooling_time = cooling_finish.tv_sec - cooling_start.tv_sec;
	packing_time = packing_finish.tv_sec - order_start.tv_sec;
	service_time = telephone_finish.tv_sec;
	printf("Order: %d ,was ready in: %0.2f minutes.\n", oid, packing_time);
	printf("Order: %d (%d pizzas) delivered in: %0.2f minutes.\n", oid, random_number_pizza, waiting_time);
	
	
	pthread_cond_signal(&cond_available_console);	
	rs = pthread_mutex_unlock(&mutex_console);
	successful_mutex_action(rs);

	//update total_time total_time

	rs = pthread_mutex_lock(&mutex_total_time);
	successful_mutex_action(rs);
	
	total_time += service_time;

	pthread_cond_signal(&cond_available_t_t);	
	rs = pthread_mutex_unlock(&mutex_total_time);
	successful_mutex_action(rs);
	
	//update total_waiting_time  
	rs = pthread_mutex_lock(&mutex_total_waiting_time);
	successful_mutex_action(rs);

	total_waiting_time += waiting_time;

	pthread_cond_signal(&cond_available_t_w_t);	
	rs = pthread_mutex_unlock(&mutex_total_waiting_time);
	successful_mutex_action(rs);

//update total_cooling_time
	rs = pthread_mutex_lock(&mutex_total_cooling_time);
	successful_mutex_action(rs);

	total_cooling_time += cooling_time;

	pthread_cond_signal(&cond_available_t_c_t);	
	rs = pthread_mutex_unlock(&mutex_total_cooling_time);
	successful_mutex_action(rs);
	
//update max_time
	rs = pthread_mutex_lock(&mutex_max_time);
	successful_mutex_action(rs);

	if (service_time > max_time)
	{
		max_time = service_time;
	}

	pthread_cond_signal(&cond_available_m_t);	
	rs = pthread_mutex_unlock(&mutex_max_time);
	successful_mutex_action(rs);
//update max_waiting_time
	rs = pthread_mutex_lock(&mutex_max_waiting_time);
	successful_mutex_action(rs);

	if (waiting_time > max_waiting_time)
	{
		max_waiting_time = waiting_time;
	}

	pthread_cond_signal(&cond_available_m_w_t);	
	rs = pthread_mutex_unlock(&mutex_max_waiting_time);
	successful_mutex_action(rs);

		
	

//update max_cooling_time
	rs = pthread_mutex_lock(&mutex_max_cooling_time);
	successful_mutex_action(rs);

	if (cooling_time > max_cooling_time)
	{
		max_cooling_time = cooling_time;
	}

	pthread_cond_signal(&cond_available_m_c_t);	
	rs = pthread_mutex_unlock(&mutex_max_cooling_time);
	successful_mutex_action(rs);
	
	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("ERROR: Please entero only 2 arguments (number of customers and seed- both integers -)\n\n\n");
		exit(-1);
	}
	
	int number_customers = atoi(argv[1]);
	if (number_customers <= 0)
	{
		printf("ERROR: Customers number must be positive integer  \n");
		exit(-1);
	}
	service_time = 0;
	waiting_time = 0;
	cooling_time = 0;
	packing_time = 0;
	total_time = 0;
	total_waiting_time = 0;
	total_cooling_time = 0;
	max_waiting_time = 0;
	max_cooling_time = 0;
	max_time=0;
	number_available_packs=1;
	number_available_tel=Ntel;
	number_available_cooks = Ncook;
	number_available_ovens = Noven;
	number_available_deliverers = Ndeliverer;
	seed = atoi(argv[2]);
	
	printf("\n\nThe number of customers are: %d ,and the seed is: %d\n\n\n", number_customers, seed);
	

	//Initializing orders id

	int order_id[number_customers];

	for(int i=0; i<number_customers; i++)
	{
		order_id[i] = i+1;
	}

	//Creating threads id

	pthread_t tid[number_customers];
	
	for(int i=0; i<number_customers; i++)
	{	

		printf("Hello %d,how can i help you today?\n", i+1);
		pthread_create(&tid[i], NULL, &order, &order_id[i]);
		double random_order_time = rand_r(&seed) % Torderhigh + Torderlow;
		sleep(random_order_time); // waiting for new order
	}

	//Wait all threads to finish

	for(int i=0; i<number_customers; i++)
	{
		pthread_join(tid[i], NULL);
	}

	//Print results

	printf("\n----------Operation Summary----------\n");
	printf("The total incomings are  : %d    \n", incomings);
	printf("The succesfull orders were:%d    \n", success_order);
	printf("The fail orders were     : %d    \n", fail_order);
	printf("Average waiting time was : %0.2f \n", total_waiting_time/number_customers);
	printf("Max waiting time was     : %0.2f \n", max_waiting_time);
	printf("Average service time was : %0.2f \n", total_time/number_customers);
	printf("Max service time was     : %0.2f \n", max_time);
	printf("Average cooling time was : %0.2f \n", total_cooling_time/number_customers);
	printf("Max cooling time was     : %0.2f \n\n", max_cooling_time);
	
	
	//destroying mutexes and conditions
	
	pthread_mutex_destroy(&mutex_number_available_tel);
	pthread_mutex_destroy(&mutex_number_available_cooks);
	pthread_mutex_destroy(&mutex_number_available_ovens);
	pthread_mutex_destroy(&mutex_number_available_packs);
	pthread_mutex_destroy(&mutex_number_available_deliverers);
	pthread_mutex_destroy(&mutex_console);
	pthread_mutex_destroy(&mutex_total_waiting_time);
	pthread_mutex_destroy(&mutex_max_waiting_time);
	pthread_mutex_destroy(&mutex_total_cooling_time);
	pthread_mutex_destroy(&mutex_max_cooling_time);
	pthread_mutex_destroy(&mutex_max_time);
	pthread_mutex_destroy(&mutex_total_time);


	pthread_cond_destroy(&cond_number_available_tel);
	pthread_cond_destroy(&cond_number_available_cooks);
	pthread_cond_destroy(&cond_number_available_ovens);
	pthread_cond_destroy(&cond_number_available_packs);
	pthread_cond_destroy(&cond_number_available_deliverers);
	pthread_cond_destroy(&cond_available_console);
	pthread_cond_destroy(&cond_available_t_w_t);
	pthread_cond_destroy(&cond_available_m_w_t);
	pthread_cond_destroy(&cond_available_t_c_t);
	pthread_cond_destroy(&cond_available_m_c_t);
	pthread_cond_destroy(&cond_available_m_t);
	pthread_cond_destroy(&cond_available_t_t);
	
	return 0;
}
