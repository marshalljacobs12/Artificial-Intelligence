#include <iostream>
#include <unistd.h>

#include "PartiallyKnownGrid.h"
#include "GridPathPlanner.h"

using namespace std;

void Simulate(PartiallyKnownGrid* grid)
{
	// Use "GridPathPlanner planner(grid, true)" to test your Adaptive A* implementation.
	//GridPathPlanner planner(grid);
	GridPathPlanner planner(grid, true);

	// Start simulation
	int steps = 0;
    int waitCounter = 1000; // amount to wait between steps (milliseconds)
	grid->Reset();
    grid->DrawGrid();

    while (!grid->GoalReached()) {  // loop until your robot find the target or dies
        xyLoc move_to = planner.GetNextMove(grid);

        // Call the simulator to move your robot and count the steps
        bool valid_move = grid->MoveTo(move_to);
        if (!valid_move) {
        	cout<<"Stopping simulation due to invalid move..."<<endl;
        	return;
        }
		steps++;
        grid->DrawGrid();

        #if defined(_WIN32) || defined(_WIN64)
        Sleep(waitCounter);
        #else
        //*
        struct timespec req, rem;
        req.tv_nsec = 1000000*waitCounter;
        req.tv_sec = 0;
        nanosleep(&req, &rem);

		usleep(500*waitCounter);

        #endif
    }
	cout<<"Target found in "<<steps<<" steps !!!"<<endl;
}

int main() {
	PartiallyKnownGrid grid("map");
	Simulate(&grid);

	return 0;
}
