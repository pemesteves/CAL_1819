/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
#include <algorithm>
#include <cmath>
#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0,0);
	this->p2 = Point(0,0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
			[](Point p, Point q){ return p.x < q.x || (p.x == q.x && p.y < q.y); });
}

static void sortByY(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
			[](Point p, Point q){ return p.y < q.y || (p.y == q.y && p.x < q.x); });
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
	Result res;
	double dist;

	for(unsigned i = 0; i < vp.size(); i++){
		for(unsigned j = i + 1; j < vp.size(); j++){
			if((dist = vp[i].distance(vp[j])) < res.dmin){
				res.p1 = vp[i];
				res.p2 = vp[j];
				res.dmin = dist;
			}
		}
	}

	return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
	Result res;
	sortByX(vp, 0, vp.size()-1);
	res = nearestPoints_BF(vp);
	return res;
}


/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res)
{
	if(right - left == 1 && vp[left].distance(vp[right]) < res.dmin){
		res.p1 = vp[left];
		res.p2 = vp[right];
		res.dmin = vp[left].distance(vp[right]);
		return;
	}

	if(right <= left){
		return;
	}

	npByY(vp, left, (left + right)/2, res);
	npByY(vp, (left + right)/2 + 1, right, res);
}

/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {
	Result res;

	// Base case of two points
	if(right - left == 1){
		res.p1 = vp[left];
		res.p2 = vp[right];
		res.dmin = vp[left].distance(vp[right]);
		return res;
	}

	// Base case of a single point: no solution, so distance is MAX_DOUBLE
	if(right <= left){
		res.dmin = MAX_DOUBLE;
		return res;
	}

	// Divide in halves (left and right) and solve them recursively,
	// possibly in parallel (in case numThreads > 1)
	Result res2;
	res = np_DC(vp, left, (left + right)/2, numThreads);
	res2 = np_DC(vp, (left + right)/2 + 1, right, numThreads);

	// Select the best solution from left and right
	if(res2.dmin < res.dmin)
		res = res2;

	// Determine the strip area around middle point
	int middle = (right + left)/2;
	int strip_left = middle;
	int strip_right = middle + 1;
	for(; strip_left >= left; strip_left--){
		if(vp[middle].distance(vp[strip_left]) > res.dmin){
			strip_left++;
			break;
		}
	}
	for(; strip_right <= right; strip_right++){
		if(vp[middle].distance(vp[strip_right]) > res.dmin){
			strip_right--;
			break;
		}
	}

	// Order points in strip area by Y coordinate
	sortByY(vp, strip_left, strip_right);

	// Calculate nearest points in strip area (using npByY function)
	npByY(vp, strip_left, strip_right, res);

	// Reorder points in strip area back by X coordinate
	sortByX(vp, strip_left, strip_right);

	return res;
}


/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num)
{
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}


/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
