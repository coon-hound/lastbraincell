#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define train_count (sizeof(train) / sizeof(train[0]))

int train[][2] = 
{
	{0, 0},
	{1, 2},
	{2, 4},
	{3, 6},
	{4, 8},
	{5, 10},
	{6, 12},
	{7, 14},
	{8, 16},
	{9, 18},
	{10, 20}
};

double rand_double()
{
	return (double) rand() / (double) RAND_MAX;
}

double rmse(double w)
{
	double res = 0;
	for(int i = 0; i < train_count; i++)
	{
		double x = train[i][0];
		double y = x * w;
		double d = train[i][1] - y;
		res += d * d;
	}
	res /= train_count;

	return res;

}

int main()
{
	srand(time(0));
	double w = rand_double();

	double x = 1e-3;
	double rate = 1e-2;

	for(int i = 0; i < 200; i++){
		double d = (rmse(w+x) - rmse(w))/x;
		w -= rate * d;
		printf("%f\n", rmse(w));
		printf("%f\n", w);
	}
	printf("-----------\n");

	printf("w = %f\n", w);
/*
	while(rmse(w) > 0.000005){
		if(rmse(w) > rmse(w + x)){
			w = w + x;
		}
		else{
			w = w - x;
		}

	}	
*/
}
