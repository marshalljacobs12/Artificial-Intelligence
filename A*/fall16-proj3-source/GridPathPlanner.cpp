#include "GridPathPlanner.h"

using namespace std;

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star) {
	adaptive_a_star = use_adaptive_a_star;
	//search_num = 0; just used for answering "Adaptive A* Question"

	gridWidth = grid->GetWidth();
	gridHeight = grid->GetHeight();

	hArray = new int*[grid->GetHeight()];
	for(int i=0; i < grid->GetHeight(); i++) {
			hArray[i] = new int[grid->GetWidth()];
	}

	for(int i = 0; i < grid->GetHeight(); i++) {
			for(int j = 0; j < grid->GetWidth(); j++) {
					hArray[i][j] = ManhattanDistance(j, i, grid->GetGoalLocation().x, grid->GetGoalLocation().y);
					cout << "[" << hArray[i][j] << "]";
			}
			cout << endl;
	}
}

GridPathPlanner::~GridPathPlanner(){
		for(int i=0; i < gridHeight; i++) {
				delete [] hArray[i];
		}
		delete [] hArray;
}

xyLoc GridPathPlanner::GetNextMove(PartiallyKnownGrid* grid) {

		/*
			IMPORTANT: curr.x is the row of curr, curr.y is the column of y
			but coordinates are (y, x) or (column, row) because of lines
			44 and 45 of PartiallyKnownGrid.cpp
		*/
		int g = 0;
		int h = hArray[grid->GetCurrentLocation().y][grid->GetCurrentLocation().x];
		int f = g + h;

		xyLoc startLoc = grid->GetCurrentLocation();
		//cout << "startLoc: " << startLoc << " f: " << f << " g: " << g << endl;

		set<xyLoc> explored;
		map<xyLoc, int> f_vals;
		map<xyLoc, int> g_vals;
		map<xyLoc, xyLoc> parents; //child = key then parent = value

		/*
			frontier can have duplicates (ie the same location can appear multiple times
			however, the optimal path to the location is always expanded first because of
			the overloaded < operator. After the optimal path to the location is placed
			in the explored set, the subsequent paths to that location are ignored
		*/
		priority_queue<pqItem, vector<pqItem>, greater<pqItem> > frontier;
		frontier.push(pqItem(f, g, grid->GetCurrentLocation()));

		int curr_expansions = 0;

		while(true) {
				if(frontier.empty()) {
						cout << "FRONTIER IS EMPTY SO FAILURE!" << endl;
						return kInvalidXYLoc;
				}

				// node expansion
				pqItem nodeToExpand = frontier.top();
				frontier.pop();

				/* 
					any node being expanded shouldn't ever be in the expanded set but I check anyways
					because my priority_queue can have duplicates
				*/
				if(explored.count(nodeToExpand.loc)) {
						continue; //this isn't part of an optimal solution so skip it
				}

				/* 
					found the goal, so retrace the path to find the immediate move from where
					the search began
				*/
				if(nodeToExpand.loc == grid->GetGoalLocation()) {
						/*
							trace back to starting node as the parent for the location of the next step
							if adaptive a star, update the h-values for all expanded nodes
						*/
						if(adaptive_a_star) {
								int g_goal = nodeToExpand.g;
								cout << "g_goal: " << g_goal << endl;
								for(set<xyLoc>::iterator it = explored.begin(); it != explored.end(); ++it) {
										int g_loc = g_vals[*it];
										hArray[(*it).y][(*it).x] = g_goal - g_loc;
								}
						}

						// iterate over the map of g-values
						xyLoc currChild = nodeToExpand.loc;
						xyLoc currParent = parents[currChild];

						while (currParent != grid->GetCurrentLocation()) {
								currChild = currParent;
								currParent = parents[currParent];
						}

						//cout << "expansions during search " << search_num << ": " << GetNumExpansions() << endl;
						total_expansions += curr_expansions;
						//cout << "total_expansions: " << GetTotalExpansions() << endl;
						//cout << "Next Move: " << currParent << "--->" << currChild << endl;
						return currChild;
				}

				curr_expansions++;

				explored.insert(nodeToExpand.loc);
				f_vals[nodeToExpand.loc] = nodeToExpand.f;
				g_vals[nodeToExpand.loc] = nodeToExpand.g;

			// expand the node (get its children)
			vector<xyLoc> neighbors;
			neighbors.push_back(xyLoc(nodeToExpand.loc.x+1, nodeToExpand.loc.y));
			neighbors.push_back(xyLoc(nodeToExpand.loc.x-1, nodeToExpand.loc.y));
			neighbors.push_back(xyLoc(nodeToExpand.loc.x, nodeToExpand.loc.y+1));
			neighbors.push_back(xyLoc(nodeToExpand.loc.x, nodeToExpand.loc.y-1));

			// don't add any nodes to the frontier which aren't valid locations or are blocked
			for (int i = 0; i < neighbors.size(); i++) {
				xyLoc n = neighbors[i];
				if (!grid->IsValidLocation(n) || grid->IsBlocked(n)) {
					neighbors[i] = neighbors.back();
					neighbors.pop_back();
					i--;
				}
			}

			// now we have the vector of children nodes that are valid potential next moves
			for(int j = 0; j < neighbors.size(); j++) {
					xyLoc m = neighbors[j];
					// if a m is already in the explored set, then we have already found an optimal path to m
					if(explored.count(m)) {
						continue;
					}

					// calculate child's f and g values
					int gval = nodeToExpand.g + 1; //g value is parent's g value + action cost 1
					int fval = gval + hArray[m.y][m.x]; //childs f val is childs g val + h val
					pqItem child = pqItem(fval, gval, m);

					/*
						if you can't find a f value for the child, that means it's the first time we've encountered that location
						so add it to the frontier
					*/
					if(!f_vals.count(child.loc)) {
							f_vals[child.loc] = fval;
							g_vals[child.loc] = gval;
							parents[child.loc] = nodeToExpand.loc;
							frontier.push(pqItem(fval, gval, m));
					}


					/*
						if you find an f value for the child's location, then either the f value must be smaller or the g
						value must be larger to add it to the frontier (the locations are equal so those are the only
						relevant parts of the tie-breaker criteria)
					*/
					else if (fval <= f_vals[child.loc]) {
							//smaller fval means child won the tiebreaker
							if(fval < f_vals[child.loc]) {
									f_vals[child.loc] = fval;
									g_vals[child.loc] = gval;
									parents[child.loc] = nodeToExpand.loc;
									frontier.push(child);
							} else if (gval >= g_vals[child.loc]) {
									//if f values are equivalent and child's gval is larger, then child wins the tiebreaker
									if(gval > g_vals[child.loc]) {
											f_vals[child.loc] = fval;
											g_vals[child.loc] = gval;
											parents[child.loc] = nodeToExpand.loc;
											frontier.push(child);
									}
							} //end inner else if
					} //end outer else if
				} //end for loop
		}//end while

		//if program gets here something went wrong
		return kInvalidXYLoc;
}

//total number of expansions for current A* or Adaptive A* search
int GridPathPlanner::GetNumExpansions() {
		return curr_expansions;
}

//total number of expansions for all A* or Adaptive A* searches from start state to goal state
int GridPathPlanner::GetTotalExpansions() {
		return total_expansions;
}

int GridPathPlanner::ManhattanDistance(int x1, int y1, int x2, int y2) {
		int md = abs(x1 - x2) + abs(y1 - y2);
		return md;
}
