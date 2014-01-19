#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
using namespace std;

char Filozof[6]={'O','O','O','O','O',0};
int Vilica[5]={1,1,1,1,1};

pthread_cond_t Red[5];
pthread_mutex_t Monitor;

void Jesti(int n)
{
   	pthread_mutex_lock(&Monitor);

	Filozof[n] = 'o';
	while(Vilica[n] == 0 || Vilica[(n + 1) % 5] == 0) 
		pthread_cond_wait(&Red[n],&Monitor);
	Vilica[n] = Vilica[(n + 1) % 5] = 0;
	Filozof[n] = 'X';
	cout << Filozof << "(" <<  n+1 << ")" << endl;
	fflush(stdout);

	pthread_mutex_unlock(&Monitor);

	sleep(2); 

	pthread_mutex_lock(&Monitor); 

	Filozof[n] = 'O';
	Vilica[n] = Vilica[(n + 1) % 5] = 1;
	pthread_cond_signal(&Red[(n-1)%5]);
        pthread_cond_signal(&Red[(n+1)%5]);
	cout << Filozof << "(" <<  n+1 << ")" << endl;
	fflush(stdout);

   	pthread_mutex_unlock(&Monitor); 
}	

void Misliti()
{
	sleep(3);
}

void *Filozofi(void *i)
{
	while(1)
    {
     Misliti();
	 Jesti((int)i);
	}
}
int main(int argc, char *argv[])
{
	pthread_t Dretve[5];
	pthread_mutex_init(&Monitor, NULL);
	pthread_cond_init(Red, NULL);
	for(int i=0;i<5;i++)
		pthread_create(&Dretve[i], NULL, &Filozofi, (void *)i);
	for(int i=0; i<5; ++i) 
		pthread_join(Dretve[i], NULL);
	return 0;
}
