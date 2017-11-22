#define _GNU_SOURCE
#include "myHeader.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>
#include <iostream>
#include <conio.h>

const int NUM_OF_CLUSTERS = 2;
int main(int argc,char *argv[])
{
	int NUM_OF_DIMENSIONS, NUM_OF_PRODUCTS, MAX_NUM_OF_CLUSTERS, MAX_NUM_OF_ITERATION;
	float QM;
	Cluster* clusters = (Cluster*)calloc(NUM_OF_CLUSTERS, sizeof(Cluster));
	//read points from file
	Point* points = readDataFromFile(&FIRST_NUM_OF_CLUSTERS, &NUM_OF_DIMENSIONS, &NUM_OF_PRODUCTS, &MAX_NUM_OF_CLUSTERS, &MAX_NUM_OF_ITERATION, &QM, clusters, NUM_OF_CLUSTERS);	

	//add points to clusters
	addPointsToClusters(points, clusters, NUM_OF_PRODUCTS, NUM_OF_CLUSTERS, NUM_OF_DIMENSIONS);
	//calculate cluster center
	for(int i = 0 ; i < FIRST_NUM_OF_CLUSTERS ; i++)
	{
		//bool isNeedtoCaluculateAgain = calculateClusterCenters(&clusters[i], NUM_OF_PRODUCTS, NUM_OF_CLUSTERS, NUM_OF_DIMENSIONS);
	}
	
}

Point *readDataFromFile(int* FIRST_NUM_OF_CLUSTERS, int* NUM_OF_DIMENSIONS, int* NUM_OF_PRODUCTS, int* MAX_NUM_OF_CLUSTERS, int* MAX_NUM_OF_ITERATION, float* QM, Cluster* clusters, const int NUM_OF_CLUSTERS)
{
	FILE* file = fopen(pathToFile, "r");
	int rc = fscanf(file, "%d,%d,%d,%d,%f\n", NUM_OF_PRODUCTS, NUM_OF_DIMENSIONS, MAX_NUM_OF_CLUSTERS, MAX_NUM_OF_ITERATION, QM);
	/*printf("%d\n", *NUM_OF_PRODUCTS);
	printf("%d\n", *NUM_OF_DIMENSIONS);
	printf("%d\n", *MAX_NUM_OF_CLUSTERS);
	printf("%d\n", *MAX_NUM_OF_ITERATION);
	printf("%f\n", *QM);*/
	Point* p = (Point*)calloc(*NUM_OF_PRODUCTS, sizeof(Point));

	for(int i = 0 ; i < *NUM_OF_PRODUCTS ; i++)
	{
		p[i].coordinates = (float*)calloc(*NUM_OF_DIMENSIONS, sizeof(float));
		float temp = 0;
		for(int j = 0 ; j < *NUM_OF_DIMENSIONS ; j++)
		{
			if (j == *NUM_OF_DIMENSIONS-1) 
			{
				int rc = fscanf(file, "%f\n", &temp);
			}
			else 
			{
				int rc = fscanf(file, "%f,", &temp);
			}
			p[i].coordinates[j] = temp;
			//printf("%f\n", p[i].coordinates[j] );
		}

	}
	// set first 2 k
	if (NUM_OF_CLUSTERS == 2)
	{
		clusters[0].center.coordinates = (float*)calloc(*NUM_OF_DIMENSIONS, sizeof(float));
		clusters[1].center.coordinates = (float*)calloc(*NUM_OF_DIMENSIONS, sizeof(float));
		clusters[0].center = p[0];
		clusters[1].center = p[1];
		clusters[0].numOfPoints = 0;
		clusters[1].numOfPoints = 0;
	}
	return p;
}

void addPointsToClusters(Point* points, Cluster* clusters, const int NUM_OF_PRODUCTS, const int NUM_OF_CLUSTERS, const int NUM_OF_DIMENSIONS)
{
	for (int i = 0 ; i < NUM_OF_PRODUCTS ; i++)
	{
		int clusterIndex = 0;
		double smallestValue = 0;
		for (int j = 0; j < NUM_OF_CLUSTERS ; j++)
		{
			double tempRangValue = getDistance(points[i], clusters[j], NUM_OF_DIMENSIONS);
			if(smallestValue >= tempRangValue)
			{
				smallestValue = tempRangValue;
				clusterIndex = j;
			}
		}
		clusters[clusterIndex].numOfPoints++;
	}
	for(int i = 0 ; i < NUM_OF_CLUSTERS ; i++)
	{
		clusters[i].points = (Point *) calloc(clusters[i].numOfPoints, sizeof(Point));
		clusters[i].numOfPoints = 0;
	}
	for (int i = 0 ; i < NUM_OF_PRODUCTS ; i++)
	{
		int clusterIndex = 0;
		double smallestValue = 0;
		for (int j = 0; j < NUM_OF_CLUSTERS ; j++)
		{
			double tempRangValue = getDistance(points[i], clusters[j], NUM_OF_DIMENSIONS);
			if(smallestValue >= tempRangValue)
			{
				smallestValue = tempRangValue;
				clusterIndex = j;
			}
		}
		int index = clusters[clusterIndex].numOfPoints++;
		clusters[clusterIndex].points[index] = points[i];
	}
}

double getDistance(Point point, Cluster cluster, const int NUM_OF_DIMENSIONS)
{
	double sum = 0;
	for(int i = 0 ; i < NUM_OF_DIMENSIONS ; i++)
	{
		sum += pow((double) (point.coordinates[i] - cluster.center.coordinates[i]),2);

	}
	return sqrt(sum);
}

void removePointFromCluster(Cluster* cluster, Point* point, const int NUM_OF_DIMENSIONS)
{
	free(cluster->points);
}

bool calculateClusterCenters(Cluster* cluster, const int NUM_OF_PRODUCTS, const int NUM_OF_CLUSTERS, const int NUM_OF_DIMENSIONS)
{
	bool returnValue = true;
	Point tempCenter;
	//printf("%d\n", cluster->numOfPoints);
	tempCenter.coordinates = (float*)calloc(NUM_OF_DIMENSIONS, sizeof(float));
	for(int i = 0 ; i < cluster->numOfPoints ; i++)
	{
		for (int j = 0 ; j < NUM_OF_DIMENSIONS ; j++)
		{
			tempCenter.coordinates[j] += 1/*cluster->points[i].coordinates[j]*/;
		}
		//printf("%f\n", tempCenter.coordinates[51]);
		//printf("%f\n", cluster->points[0].coordinates);
	}
	return returnValue;
}