#include "PuzzleGenerator.h"
using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute

	Puzzle bestPuzzleFound = SimulatedAnnealing(4);

	// We could also do as many random walks as we can within the given time limit.
	while (timer.GetElapsedTime() + 4 < maxtime)
	{
		Puzzle p = SimulatedAnnealing(4);
		// Check if p is better than the best puzzle we have found so far.
		if(p.GetValue() > bestPuzzleFound.GetValue()) {
			bestPuzzleFound  = p;
		}
	}

	return bestPuzzleFound;
}

/*
		Simulated Annealing C++ implementation: http://codereview.stackexchange.com/questions/70310/simple-simulated-annealing-template-in-c11

		Acceptance Probability Function: a= e ^ ( (cold−cnew) / T ) //is this to minimize or maximize? THIS MINIMIZES
		so we want a = e ^ ( (cnew-cold) / T)

		http://katrinaeg.com/simulated-annealing.html
*/

Puzzle PuzzleGenerator::SimulatedAnnealing(double timelimit) {

	Puzzle p(nRows, nColumns, minVal, maxVal);

	Puzzle bestPuzzle = p;
	int bestValue = p.GetValue();

	Timer t;
	t.StartTimer();

	double T = 1.0; //temperature
	double T_min = 0.00001;
	double alpha = 0.9;

	while(t.GetElapsedTime() < timelimit-0.1) {
		vector<Puzzle> successors;//not sure if I'm going to use this
		bestPuzzle.GetAllSuccessors(successors);

		for(int i=0; i < successors.size(); i++) {
			p = successors[i];
			int newValue = p.GetValue();
			double ap = AcceptanceProbability(newValue, bestValue, T);
			if (ap > ( (double) rand() / RAND_MAX) ) {
				bestPuzzle = p;
				bestValue = p.GetValue();
			}
		}
		T = T * alpha;
	}
	return bestPuzzle;
}

double PuzzleGenerator::AcceptanceProbability(int newValue, int bestValue, double T) {
	double ap = exp((newValue-bestValue)/T);
	return ap;
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that starts at a random configuration and keeps randomly modifying it
	// until the time limit is reached. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.

	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;
	int bestValue = p.GetValue();

	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();

	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();

		// Update the current best solution.
		if (p.GetValue() > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = p.GetValue();	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}

	return bestPuzzle;

	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	vector<Puzzle> successors;
	bestPuzzle.GetAllSuccessors(successors);
}
