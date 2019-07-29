
#include <iostream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

using namespace std;

pthread_mutex_t sumlock;
pthread_cond_t qEmpty;
pthread_mutex_t numlock;
pthread_mutex_t consolelock;

int NUMTHREADS = 1;
long double max_iterations = 100000000;
mp_bitcnt_t precision = 2048;
int progress = 0.0;

mpf_t pi;
long double lowest = 0;

void Nila(int);
void thread_func();
thread thandle[20]; //Max 20 threads.
thread::id threadids[20]; //Thread ID's
fstream f;



void thread_func() {
	for (int i = 0; i < max_iterations; i++) {

		pthread_mutex_lock(&numlock);
		int s;
		if(lowest == 0){
			lowest = 30;
			s = 0;
		}
		else{
			s = lowest; 
			lowest = lowest + 20;
			
		}
		progress = i;
		pthread_mutex_unlock(&numlock);

		Nila(s);
		
	}
	return;	
}


void Nila(int n) {
	mpf_t sum;
	mpf_init(sum);
	mpf_t intermed;
	mpf_init(intermed);
	mpf_t quotient;
	mpf_init(quotient);
	mpf_t top;
	mpf_init(top);
	mpf_t bottom;
	mpf_init(bottom);
	

	int ceiling = n + 20;
	if (n == 0){
		n = 2;
		ceiling = 30;
	}

	bool flip = 0;
	mpf_set_d(sum, 0);
	while (n < ceiling)
	{

		if (flip == 0) {
			//intermed += 4.0 / (n * (n + 1) * (n + 2));
			mpf_set_d(bottom, n);
			mpf_mul_ui(bottom, bottom, n + 1);
			mpf_mul_ui(bottom, bottom, n + 2);
			mpf_ui_div(quotient, 4, bottom);
			mpf_set(intermed, sum);
			mpf_add(sum, intermed, quotient);
			n = n + 2;
			flip = 1;
		}
		else{
			//intermed -= 4.0 / (n * (n + 1) * (n + 2));
			mpf_set_d(bottom, n);
			mpf_mul_ui(bottom, bottom, n + 1);
			mpf_mul_ui(bottom, bottom, n + 2);
			mpf_ui_div(quotient, 4, bottom);
			mpf_set(intermed, sum);

			mpf_sub(sum, intermed, quotient);

			n = n + 2;
			flip = 0;
		}

	}
	pthread_mutex_lock(&sumlock);
	mpf_add(pi, pi, sum);
	pthread_mutex_unlock(&sumlock);
	mpf_clear(sum);
	mpf_clear(intermed);
	mpf_clear(quotient);
	mpf_clear(top);
	mpf_clear(bottom);
}

int main(int argc, char **argv)
{
	NUMTHREADS = atoi(argv[1]);
	mpf_set_default_prec(precision);
	mpf_init(pi);
	mpf_set_d(pi, 3);

	pthread_mutex_init(&sumlock, NULL);
	pthread_mutex_init(&numlock, NULL);

	cout << "Starting Nila with " << NUMTHREADS << " Threads" << endl;
	thread thandle[20]; //Max 20 threads.
	for(int j = 0; j < NUMTHREADS; ++j) {
		thandle[j] = thread(&thread_func); //Address of method/function...
		threadids[j] = thandle[j].get_id();
	}

	while (progress < max_iterations - 1) {

    			int barWidth = 70;


				cout << "Iteration: \u001b[31;1m" << progress << "\u001b[0m of \u001b[32;1m" << max_iterations << "\u001b[0m \r";
				cout.flush();
				cout<<endl;
    			cout << "[";
  				int pos = barWidth * progress/max_iterations;
   				for (int i = 0; i < barWidth; ++i) {
        			if (i < pos) cout << "=";
       				else if (i == pos) cout << ">";
        			else cout << " ";
    			}

				// Output loading bar - Thanks StackOverflow (https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf)
    			cout << "] " << int(progress/max_iterations * 100.0) << " %\r";
    			cout.flush();
				cout << endl;
				// Output PI value on line below
				gmp_printf("Pi: %.*Ff \r", 100, pi);
				fflush(stdout);
			
				// Jump back up one line - beware of use on Windows systems!
				// But works in Ubuntu Shell (WSL)
				cout << "\x1b[1A";
				cout << "\x1b[1A";
			}


	cout << "\n\n\n\n";
	for(int j = 0; j < NUMTHREADS; ++j) {
		thandle[j].join();

	}


	mpf_clear(pi);

	pthread_mutex_destroy(&sumlock);
	pthread_mutex_destroy(&numlock);
}