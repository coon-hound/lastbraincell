#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define train_count (sizeof(train) / sizeof(train[0]))

int train[][3] = 
{
	{0, 0, 0},
	{0, 1, 1},
	{1, 0, 1},
	{1, 1, 0},
};

double rand_double()
{
	return (double) rand() / (double) RAND_MAX;
}

double sigmoid(double x)
{
	return (1.f/(1.f+exp(-x)));
}

double rmse(double w1, double w2, double b)
{
	double res = 0.f;

	for(int i = 0; i < train_count; i++) {
		double x1 = train[i][0];
		double x2 = train[i][1];

		double y = sigmoid(x1 * w1 + x2 * w2 + b);

		double d = y - train[i][2];
		res += d*d;
	}
	res /= train_count;
	return res;
}


int main()
{
	srand(time(0));

	double w1 = rand_double();
	double w2 = rand_double();
	double b = rand_double();

	double x = 1e-3;
	double rate = 1e-3;


	for(int i = 0; i < 1000000; i++) {
		double dw1 = (rmse(w1 + x, w2, b) - rmse(w1, w2, b))/x;
		double dw2 = (rmse(w1, w2 + x, b) - rmse(w1, w2, b))/x;
		double db = (rmse(w1, w2, b + x) - rmse(w1, w2, b))/x;

		w1 -= rate*dw1;
		w2 -= rate*dw2;
		b -= rate*db;
		
		printf("w1 = %f, w2 = %f, b = %f, cost = %f\n", w1, w2, b, rmse(w1, w2, b));
	}

	// print results

	printf("----------------\n");	
	printf("cost = %f\n", rmse(w1, w2, b));
	printf("w1 = %f, w2 = %f, b = %f\n", w1, w2, b);
	printf("----------------\n");	

	for(int i = 0; i < train_count; i++) {
		double x1 = train[i][0];
		double x2 = train[i][1];

		double y = sigmoid(x1 * w1 + x2 * w2 + b);

		printf("x1 = %f, x2 = %f, y = %f\n", x1, x2, y);

	}

}
