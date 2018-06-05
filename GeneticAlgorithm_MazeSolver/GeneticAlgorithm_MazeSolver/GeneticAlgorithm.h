#pragma once
#include <vector>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <cmath>

#ifdef	GENETICALGORITHM_LIBRARY_EXPORTS
#define GENETICLIBRARY_API __declspec(dllexport)   
#else  
#define GENETICLIBRARY_API __declspec(dllimport)   
#endif  

// Struct holding 2 ints
struct Vector2
{
	int x; // X position in vector
	int y; // Y position in vector
};

// -----------------------------------------------
// Member struct to input into Population
// Contains list of ints, int for fitness and vector2 for final position
// -----------------------------------------------
struct Member
{
	std::vector<int> directions = std::vector<int>(10); // Vector of int to represent directions
	int fitness; // fitness for member
	Vector2 finalPos; // Final position of member
};

// -----------------------------------------------
// Population for current generation
// Contains a vector of members
// -----------------------------------------------
struct Population
{
	std::vector<Member> Members = std::vector<Member>(10); // vector of members
};

class GeneticAlgorithm
{
public:
	//-------------------------------------
	// Default Constructor
	//-------------------------------------
	GENETICLIBRARY_API GeneticAlgorithm();

	//-------------------------------------
	// Default Destructor
	//-------------------------------------
	GENETICLIBRARY_API ~GeneticAlgorithm();

	//-------------------------------------
	// Creates the starting population
	// Populates the inital starting population with members
	// Sets the directions for the members
	// Sets fitness of members
	//-------------------------------------
	GENETICLIBRARY_API void CreateStartPopulation();

	//-------------------------------------
	// Tests the route of each member
	// Moves the members through maze depending on their directions
	// Sets final position for each member
	// Parameters:
	//		std::vector<Member> pop: population that you want to test the routes for
	//-------------------------------------
	GENETICLIBRARY_API void TestRoute(std::vector<Member> pop);

	//-------------------------------------
	// Creates maze
	// Sets variables of maze to 0, 1, 2, 3
	// Prints maze
	// Parameters:
	//		int width: width of maze
	//		int height: height of maze
	//		Vector2 StartPos: start position of maze
	//		Vector2 EndPos: end position of maze
	//-------------------------------------
	GENETICLIBRARY_API void CreateMaze(int width, int height, Vector2 StartPos, Vector2 EndPos);

	//-------------------------------------
	// Runs all functions required for the genetic algorithm to solve the maze
	// Runs untill solved
	// Updates generation each run
	// Prints current generation number to console
	//-------------------------------------
	GENETICLIBRARY_API void Run();

	//-------------------------------------
	// Calculates the fitness for each member in a population
	// Checks final position against end position for each member
	// Gets distance between the 2 points
	// Calculates distance, the closer to the end the higher the fitness
	// Parameters:
	//		vector<Member> pop: population to calculate fitness for
	//-------------------------------------
	GENETICLIBRARY_API void CalculateFitness(std::vector<Member> pop);

	//-------------------------------------
	// Takes current population and creates a new one
	// Sorts from highest to lowest fitness in current generation
	// Check if the maze has been solved in the current generation
	// Displays the best fitness on current generation
	// Selects the 2 best members from the current generation and stores them in a parent list 
	// Randomly selects directions from both parents and add them to a new member/child
	// Randomly changes directions based on the mutation rate
	// Creates new generation
	// Parameters:
	//		vector<Member> pop: current population to breed from
	//-------------------------------------
	GENETICLIBRARY_API void Breed(std::vector<Member> pop);

	//-------------------------------------
	// Prints the route of the best member each generation
	// Checks the route of the member and stores its positions each movement
	// Prints all the position stored to console
	// parameters:
	//		Member bestOf: Best member of each generation to get directions from.
	//-------------------------------------
	GENETICLIBRARY_API void PrintBestOfGenerations(Member bestOf);


	Population Pop; // Inialising population
	bool Solved; // Bool to check whether genetic algoritm has solved the maze
	int mutationRate = 25; // 1000 = 100%, 1 = 0.1% mutation rate
	Vector2 endPos; // End position of maze
	int** maze; // 2x array representing the maze
	Vector2 startPos; // Start position of maze
	int w; // Width of maze
	int h; // Height of maze
	int Generation = 0; // Current generation
	Vector2 position; // Current position of member
	std::vector<int> successfulGenome; // overall successful genome
	std::vector<int> FittestOfGeneration; // Fittest genome of current generation
};

