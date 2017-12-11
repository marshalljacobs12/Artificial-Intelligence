#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include <iostream> //for debugging
#include <set> //for explored (expanded) locations
#include <map> //for explored (expanded) locations' f and g values and parents
#include <queue> //for frontier locations
#include <functional>

#include "PartiallyKnownGrid.h"

/*
	pqItem is a struct containing an xyLoc with its f and g values
	pqItem is used so I can use a stl priority_queue
*/
struct pqItem {
public:
		pqItem(): f(-1), g(-1) {
			loc = xyLoc(-1, -1);
		}
		pqItem(int _f, int _g, xyLoc _loc): f(_f), g(_g) {
			loc = xyLoc(_loc.x, _loc.y);
		}
		int f;
		int g;
		xyLoc loc;
};

static bool operator>(const pqItem &pq1, const pqItem &pq2) {
		if(pq1.f == pq2.f) {
				if(pq1.g == pq2.g) {
						return pq2.loc < pq1.loc;
				} else {
						return pq1.g > pq2.g;
				}
		} else {
				return pq1.f > pq2.f;
		}
}
class GridPathPlanner{
public:
	GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star = false);
	~GridPathPlanner();

	xyLoc GetNextMove(PartiallyKnownGrid* grid);
	int GetNumExpansions();
	int GetTotalExpansions();
	int ManhattanDistance(int x1, int y1, int x2, int y2);

private:
	bool adaptive_a_star;

	int gridWidth;
	int gridHeight;
	int** hArray; //stores 2D array of H-values for every position in grid

	//int search_num; just used this variable to answer question about A* vs Adaptive A*
	int curr_expansions;
	int total_expansions;
};

#endif
