typedef struct
{
	float* coordinates;
} Point;

typedef struct
{
	Point center;
	Point* points;
	float* radius;
	int numOfPoints;
} Cluster;

int FIRST_NUM_OF_CLUSTERS = 2;
Point *readDataFromFile(int* FIRST_NUM_OF_CLUSTERS, int* NUM_OF_DIMENSIONS, int* NUM_OF_PRODUCTS, int* MAX_NUM_OF_CLUSTERS, int* MAX_NUM_OF_ITERATION, float* QM, Cluster* clusters, const int NUM_OF_CLUSTERS);
void addPointsToClusters(Point* points, Cluster* clusters, const int NUM_OF_PRODUCTS, const int NUM_OF_CLUSTERS, const int NUM_OF_DIMENSIONS);
double getDistance(Point point, Cluster cluster, const int NUM_OF_DIMENSIONS);
bool calculateClusterCenters(Cluster* cluster, const int NUM_OF_PRODUCTS, const int NUM_OF_CLUSTERS, const int NUM_OF_DIMENSIONS);
void removePointFromCluster(Cluster* cluster, Point* point, const int NUM_OF_DIMENSIONS);
const char* pathToFile = "E:/College/PComputing/Project/Sales_Transactions_Dataset_Weekly.dat";