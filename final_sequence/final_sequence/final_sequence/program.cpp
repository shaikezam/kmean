#define _GNU_SOURCE
#include "myHeader.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>
#include <iostream>
#include <conio.h>

int main(int argc,char *argv[])
{
	int NUM_OF_DIMENSIONS, NUM_OF_PRODUCTS, MAX_NUM_OF_CLUSTERS, MAX_NUM_OF_ITERATION;
	float QM;
	Cluster* clusters = (Cluster*)calloc(NUM_OF_CLUSTERS, sizeof(Cluster));
	//read points from file
	Point* points = readDataFromFile(&NUM_OF_DIMENSIONS, &NUM_OF_PRODUCTS, &MAX_NUM_OF_CLUSTERS, &MAX_NUM_OF_ITERATION, &QM, clusters, NUM_OF_CLUSTERS);	

	//calculate cluster center	
	for(int i = 0 ; i < MAX_NUM_OF_ITERATION ; i++)
	{
		printf("Iteration number %d\n", i);
		addPointsToClusters(points, clusters, NUM_OF_PRODUCTS, NUM_OF_CLUSTERS, NUM_OF_DIMENSIONS);
		for(int j = 0 ; j < NUM_OF_CLUSTERS ; j++)
		{
			calculateClusterCenters(&clusters[j], NUM_OF_DIMENSIONS);
		}
		bool flag = isNeedToCalculateClusterCenter(clusters, NUM_OF_DIMENSIONS, NUM_OF_CLUSTERS);

		if(flag == false)
		{
			break;
		}
		removePointFromCluster(clusters, NUM_OF_CLUSTERS);
	}

}

Point *readDataFromFile(int* NUM_OF_DIMENSIONS, int* NUM_OF_PRODUCTS, int* MAX_NUM_OF_CLUSTERS, int* MAX_NUM_OF_ITERATION, float* QM, Cluster* clusters, const int NUM_OF_CLUSTERS)
{
	FILE* file = fopen(pathToFile, "r");
	int rc = fscanf(file, "%d,%d,%d,%d,%f\n", NUM_OF_PRODUCTS, NUM_OF_DIMENSIONS, MAX_NUM_OF_CLUSTERS, MAX_NUM_OF_ITERATION, QM);
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
		}

	}
	for(int i = 0 ; i < NUM_OF_CLUSTERS ; i++)
	{
		clusters[i].center.coordinates = (float*)calloc(*NUM_OF_DIMENSIONS, sizeof(float));
		clusters[i].center = p[i];
		clusters[i].numOfPoints = 0;
		clusters[i].radius = 0;
	}
	return p;
}

void addPointsToClusters(Point* points, Cluster* clusters, const int NUM_OF_PRODUCTS, const int NUM_OF_CLUSTERS, const int NUM_OF_DIMENSIONS)
{
	for (int i = 0 ; i < NUM_OF_PRODUCTS ; i++)
	{
		int clusterIndex = 0;
		double smallestValue = 1000000;
		for (int j = 0; j < NUM_OF_CLUSTERS ; j++)
		{
			double tempRangValue = getDistanceBetweenTwoPoints(points[i], clusters[j].center, NUM_OF_DIMENSIONS);
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
		printf("Num of points %d\n", clusters[i].numOfPoints);
		clusters[i].points = (Point *) calloc(clusters[i].numOfPoints, sizeof(Point));
		clusters[i].numOfPoints = 0;
	}
	for (int i = 0 ; i < NUM_OF_PRODUCTS ; i++)
	{
		int clusterIndex = 0;
		double smallestValue = 10000;
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
		sum += (point.coordinates[i] - cluster.center.coordinates[i]) * (point.coordinates[i] - cluster.center.coordinates[i]);

	}
	return sqrt(sum);
}

double getDistanceBetweenTwoPoints(Point point1, Point point2, const int NUM_OF_DIMENSIONS)
{
	double sum = 0;
	for(int i = 0 ; i < NUM_OF_DIMENSIONS ; i++)
	{
		sum += (point1.coordinates[i] - point2.coordinates[i])*(point1.coordinates[i] - point2.coordinates[i]);

	}
	return sqrt(sum);
}

void removePointFromCluster(Cluster* clusters, const int NUM_OF_CLUSTERS)
{
	for(int i = 0 ; i < NUM_OF_CLUSTERS ; i++)
	{
		free(clusters[i].points);
		clusters[i].points = NULL;
		clusters[i].radius = 0;
		clusters[i].numOfPoints = 0;
	}

}

bool calculateClusterCenters(Cluster* cluster, const int NUM_OF_DIMENSIONS)
{
	bool returnValue = true;
	Point tempCenter;
	tempCenter.coordinates = (float*)calloc(NUM_OF_DIMENSIONS, sizeof(float));
	for(int i = 0 ; i < cluster->numOfPoints ; i++)
	{
		for (int j = 0 ; j < NUM_OF_DIMENSIONS ; j++)
		{
			tempCenter.coordinates[j] += cluster->points[i].coordinates[j];
		}
	}
	for(int i = 0 ; i < NUM_OF_DIMENSIONS ; i++)
	{
		tempCenter.coordinates[i] = tempCenter.coordinates[i] / cluster->numOfPoints;
	}
	cluster->center = tempCenter;
	return returnValue;
}

void calculateClusterRadius(Cluster* cluster, const int NUM_OF_DIMENSIONS)
{
	//printf("Num of points: %d\n", cluster->numOfPoints);
	float tempRadius = 0;
	for(int i = 0 ; i < cluster->numOfPoints ; i++)
	{
		for(int j = 0 ; j < cluster->numOfPoints ; j++)
		{
			if(i != j)
			{
				float calculatedRadius = getDistanceBetweenTwoPoints(cluster->points[i], cluster->points[j], NUM_OF_DIMENSIONS);
				if(calculatedRadius > tempRadius)
				{
					tempRadius = calculatedRadius;
				}
			}
		}
	}
	printf("%f\n", tempRadius);
	cluster->radius = tempRadius;
}

void printPoint(Point* point, const int NUM_OF_DIMENSIONS)
{
	for(int i = 0 ; i < NUM_OF_DIMENSIONS ; i++)
	{
		printf("%f ", point->coordinates[i]);
	}
	printf("\n");
}

bool isNeedToCalculateClusterCenter(Cluster* clusters, const int NUM_OF_DIMENSIONS, const int NUM_OF_CLUSTERS)
{
	int clusterIndex = 0;
	for(int i = 0 ; i < NUM_OF_CLUSTERS ; i++)
	{
		for(int j = 0 ; j< clusters[i].numOfPoints ; j++)
		{
			for(int p = 0 ; p < NUM_OF_CLUSTERS ; p++)
			{
				if(p!=i)
				{
					float distanceToOtherClusterCenter = getDistanceBetweenTwoPoints(clusters[i].points[j], clusters[p].center, NUM_OF_DIMENSIONS);
					float distanceToPointClusterCenter = getDistanceBetweenTwoPoints(clusters[i].points[j], clusters[i].center, NUM_OF_DIMENSIONS);
					if (distanceToOtherClusterCenter < distanceToPointClusterCenter)
					{
						printf("Other = %f, Now = %f\n", distanceToOtherClusterCenter, distanceToPointClusterCenter);
						return true;
					}
				}


			}
		}
	}
	return false;
}