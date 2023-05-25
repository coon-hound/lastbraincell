#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define train_count 4
#define eps 1e-3;

typedef struct 
{
	double or_w1;
	double or_w2;
	double or_b;
	double nand_w1;
	double nand_w2;
	double nand_b;
	double and_w1;
	double and_w2;
	double and_b;

} neuron;

typedef int temp[3];

temp or[] = 
{
	{0, 0, 0},
	{1, 0, 1},
	{0, 1, 1},
	{1, 1, 1},
};

temp nand[] = 
{
	{0, 0, 1},
	{1, 0, 1},
	{0, 1, 1},
	{1, 1, 0},
	
};

temp xor[] = 
{
	{0, 0, 0},
	{1, 0, 1},
	{0, 1, 1},
	{1, 1, 0},
};

temp *train = xor;

double sigmoid(double x)
{
	return (1.f/(1.f+exp(-x)));
}

double rand_double()
{
	return (double) rand() / (double) RAND_MAX;
}

double forward(neuron n, double x1, double x2)
{
	double a = sigmoid(n.or_w1 * x1 + n.or_w2 * x2 + n.or_b);
	double b = sigmoid(n.nand_w1 * x1 + n.nand_w2 * x2 + n.nand_b);
	return 1.f * sigmoid(n.and_w1 * a + n.and_w2 * b + n.and_b);
}

double rmse(neuron n)
{
	double res = 0.f;

	for(int i = 0; i < train_count; i++){
		double x1 = train[i][0];
		double x2 = train[i][1];

		double y = forward(n, x1, x2);

		double d = y - train[i][2];
		res += d * d;
	}

	res /= train_count;

	return res;
}

neuron rand_neuron()
{
	neuron m;

	m.or_w1 = rand_double();
	m.or_w2 = rand_double();
	m.or_b = rand_double();

	m.nand_w1 = rand_double();
	m.nand_w2 = rand_double();
	m.nand_b = rand_double();

	m.and_w1 = rand_double();
	m.and_w2 = rand_double();
	m.and_b = rand_double();
	return m;
}

void print_neuron(neuron n)
{
	printf("or_w1 = %f\n", n.or_w1);
	printf("or_w2 = %f\n", n.or_w2);
	printf("or_b = %f\n", n.or_b);
	printf("nand_w1 = %f\n", n.nand_w1);
	printf("nand_w2 = %f\n", n.nand_w2);
	printf("nand_b = %f\n", n.nand_b);
	printf("and_w1 = %f\n", n.and_w1);
	printf("and_w2 = %f\n", n.and_w2);
	printf("and_b = %f\n", n.and_b);
}

neuron finite_diff(neuron n)
{
	neuron ret;
	double c = rmse(n);

	double temp;

	temp = n.or_w1;
	n.or_w1 += eps;
	ret.or_w1 = (rmse(n) - c)/eps;
	n.or_w1 = temp;	

	temp = n.or_w2;
	n.or_w2 += eps;
	ret.or_w2 = (rmse(n) - c)/eps;
	n.or_w2 = temp;	

	temp = n.or_b;
	n.or_b += eps;
	ret.or_b = (rmse(n) - c)/eps;
	n.or_b = temp;	

	temp = n.nand_w1;
	n.nand_w1 += eps;
	ret.nand_w1 = (rmse(n) - c)/eps;
	n.nand_w1 = temp;	

	temp = n.nand_w2;
	n.nand_w2 += eps;
	ret.nand_w2 = (rmse(n) - c)/eps;
	n.nand_w2 = temp;	

	temp = n.nand_b;
	n.nand_b += eps;
	ret.nand_b = (rmse(n) - c)/eps;
	n.nand_b = temp;	

	temp = n.and_w1;
	n.and_w1 += eps;
	ret.and_w1 = (rmse(n) - c)/eps;
	n.and_w1 = temp;	

	temp = n.and_w2;
	n.and_w2 += eps;
	ret.and_w2 = (rmse(n) - c)/eps;
	n.and_w2 = temp;	

	temp = n.and_b;
	n.and_b += eps;
	ret.and_b = (rmse(n) - c)/eps;
	n.and_b = temp;	

	return ret;
}

neuron learn(neuron m, neuron d)
{
	double rate = 1e-3;
	m.or_w1 -= rate * d.or_w1;
	m.or_w2 -= rate * d.or_w2;
	m.or_b -= rate * d.or_b;
	m.nand_w1 -= rate * d.nand_w1;
	m.nand_w2 -= rate * d.nand_w2;
	m.nand_b -= rate * d.nand_b;
	m.and_w1 -= rate * d.and_w1;
	m.and_w2 -= rate * d.and_w2;
	m.and_b -= rate * d.and_b;
	return m;
}


int main()
{
	srand(time(0));
	neuron m = rand_neuron();
	print_neuron(m);
	neuron og = m;

	printf("training...\n");

	for(int i = 0; i < 2000*1000; i++){
		m = learn(m, finite_diff(m));
		// printf("cost = %f\n", rmse(m));
	}
	
	printf("---------finished training---------\n");

	printf("cost = %f\n", rmse(m));
	printf("og cost = %f\n", rmse(og));

	for(int i = 0; i < 4; i++){
		printf("%d ^ %d = %f\n", train[i][0], train[i][1], forward(m, train[i][0], train[i][1]));
	}

	printf("---------neuron 1---------\n");
	for(int i = 0; i < 4; i++){
		int x1 = train[i][0];
		int x2 = train[i][1];
		double y = sigmoid(x1 * m.or_w1 + x2 * m.or_w2 + m.or_b);
		printf("%d %d %f\n", x1, x2, y);
	}
	printf("---------neuron 2---------\n");
	for(int i = 0; i < 4; i++){
		int x1 = train[i][0];
		int x2 = train[i][1];
		double y = sigmoid(x1 * m.nand_w1 + x2 * m.nand_w2 + m.nand_b);
		printf("%d %d %f\n", x1, x2, y);
	}
	printf("---------neuron 3---------\n");
	for(int i = 0; i < 4; i++){
		int x1 = train[i][0];
		int x2 = train[i][1];
		double y = sigmoid(x1 * m.and_w1 + x2 * m.and_w2 + m.and_b);
		printf("%d %d %f\n", x1, x2, y);
	}


	return 0;
}
