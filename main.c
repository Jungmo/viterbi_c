#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 0
#define n_state 2
#define n_observation 11

double max(double a, double b)
{
	double ret = 0;

	if(a>=b)
	{
		ret = a;
	}
	else if(a<b)
	{
		ret = b;
	}

}
int* viterbi(int *observation, int observation_length, double start[n_state], double emission[n_state][n_observation], double transmat[n_state][n_state]){
	int i = 0;
	int j = 0;
	int k = 0;
	int t = 0;
	double **X;//[n_observation][n_state];
	double **T;//[n_observation][n_state];
	int *Q;

	X = (double**)malloc(sizeof(double*) * observation_length);
	T = (double**)malloc(sizeof(double*) * observation_length);
	Q = (int*)malloc(sizeof(int) * observation_length);
	
	for(i=0; i< observation_length; i++)
	{
		X[i] = (double*)malloc(sizeof(double) * n_state);
		T[i] = (double*)malloc(sizeof(double) * n_state);
	} 
	for(i=0; i<n_state;i++)
	{
		X[0][i] = log10(start[i]) + log10(emission[i][observation[0]]);	
	}
	for(t = 1; t<observation_length; t++)
	{
		for(i=0;i<n_state;i++)
		{
			if(X[t-1][0] +log10(transmat[0][i]) >= X[t-1][1] + log10(transmat[1][i]))
			{
				k = 0;
			}
			else
			{
				k = 1;
			}
			X[t][i] = X[t-1][k] +log10(transmat[k][i]) + log10(emission[i][observation[t]]);
			printf("%lf = %lf * %lf * %lf\n", X[t][i],  X[t-1][k], transmat[k][i], emission[i][observation[t]]);
			T[t][i] = k;
		}
	}
	
	if(X[observation_length-1][0] >= X[observation_length-1][1])
	{
		Q[observation_length-1] = 0; 
	}
	else
	{
		Q[observation_length-1] = 1; 
	}

	for(i=observation_length-2; i>=0; i--)
	{
		Q[i] = T[i+1][Q[i+1]];
	}
	
	return Q;
}


int main()
{
	double transmat[n_state][n_state] = {{0.983,0.017}, {0.032, 0.967}};
	
	//double transmat[n_state][n_state] = {{0.98324557,0.01675443}, {0.03277154, 0.96722846}};
	
	double emission[n_state][n_observation] = {{0.87, 0.053 , 0.011, 0.008, 0.006,0.017, 0.02, 0.005, 0.009, 0,0.001},{0.105, 0.164, 0.228, 0.135, 0.111, 0.07, 0.089, 0.054, 0.037, 0.005, 0.002}};
		
	//double emission[n_state][n_observation] = {{0.86956522, 0.0531401 , 0.01111111, 0.00772947, 0.00628019,0.0173913, 0.01980676, 0.00483092, 0.00869565, 0,0.00144928},{0.10486891, 0.16385768, 0.22846442, 0.13483146, 0.11142322, 0.07022472, 0.08895131, 0.05430712, 0.03651685, 0.00468165, 0.00187266}};
	double start[n_state] = {0.5, 0.5};

	FILE *fp = fopen("data/train.csv","r");
	FILE *fp_label = fopen("data/train_label.csv", "r");	
	int ob_count  = 0;	
	int *Q;
	int *observations;
	int *label;
	int i = 0;	
	char strTemp[20];
	if(fp != NULL)
	{
		while(fgets(strTemp, sizeof(strTemp), fp) != NULL)
		{
			ob_count++;
		}
	}
	else
	{
		printf("file loading err");
	}
	if (DEBUG) printf("count:%d", ob_count);	
	
	observations = (int*)malloc(sizeof(int)*ob_count);
	label = (int*)malloc(sizeof(int)*ob_count);
	fseek(fp, 0, SEEK_SET);
	ob_count = 0;
	
	while(fgets(strTemp, sizeof(strTemp), fp) != NULL)
	{
		observations[ob_count] = atoi(strTemp);
		ob_count++;
	}
	ob_count = 0;
	while(fgets(strTemp, sizeof(strTemp), fp_label) != NULL)
	{
		label[ob_count] = atoi(strTemp);
	//	printf("%d", label[ob_count]);
		ob_count++;
	}


	Q = viterbi(observations, ob_count, start, emission, transmat);
	
	double correct = 0;
	double incorrect = 0;
	
	for(i =0; i<ob_count; i++)
	{
		if(label[i] == Q[i])
			correct++;
		else
			incorrect++;
	}
	
	printf("accuracy : %lf", correct/(correct+incorrect));
}
