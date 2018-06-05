#include "GeneticAlgorithm.h"


//-------------------------------------
// Default Constructor
//-------------------------------------
GeneticAlgorithm::GeneticAlgorithm()
{
}

//-------------------------------------
// Default Destructor
//-------------------------------------
GeneticAlgorithm::~GeneticAlgorithm()
{
}

//-------------------------------------
// Creates the starting population
// Populates the inital starting population with members
// Sets the directions for the members
// Sets fitness of members
//-------------------------------------
void GeneticAlgorithm::CreateStartPopulation()
{
	srand(time(NULL)); // Random based on time
	for (int i = 0; i < Pop.Members.size(); i++) // For loop of member size
	{
		Pop.Members.at(i).directions.resize((h - 1) * (w - 1)); // Sets size of directions vector based on size of maze
		for (int j = 0; j < (h - 1) * (w - 1); j++) // for loop for size of directions
		{
			Pop.Members.at(i).directions.at(j) = (int)rand() % 4; // Sets directions to value between 0-3
		}
		Pop.Members.at(i).fitness = 0; // Sets fitness for all members to 0
	}
}

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
void GeneticAlgorithm::CreateMaze(int width, int height, Vector2 StartPos, Vector2 EndPos)
{
	w = width; // Sets Width
	h = height; // Sets Height
	maze = new int*[w] {0}; // initalises maze
	for (size_t i = 0; i < w; i++) // Creates 2d array of ints
	{
		maze[i] = new int[h] {0};
	}
	for (size_t i = 0; i < w; i++) // Sets all outside numbers of maze to 1
	{
		maze[i][0] = 1;
		maze[0][i] = 1;
		maze[w - 1][i] = 1;
		maze[i][h - 1] = 1;
	}
	for (size_t i = 0; i < h - 2; i++) // creates a wall in the maze
	{
		maze[i][2] = 1;
	}
	
	startPos = StartPos; // Sets start position
	endPos = EndPos; // Sets end position

	maze[startPos.x][startPos.y] = 2; // Sets start position to 2
	maze[endPos.x][endPos.y] = 3; // Sets end position to 3

	for (size_t i = 0; i < w; i++) // Prints maze
	{
		for (size_t j = 0; j < h; j++)
		{
			std::cout << maze[i][j];
			std::cout << " ";
		}
		std::cout << "" << std::endl;
	}

}

//-------------------------------------
// Tests the route of each member
// Moves the members through maze depending on their directions
// Sets final position for each member
// Parameters:
//		std::vector<Member> pop: population that you want to test the routes for
//-------------------------------------
void GeneticAlgorithm::TestRoute(std::vector<Member> pop)
{
	Vector2 position = startPos; // Sets starting position to members current position
	for (int i = 0; i < Pop.Members.size(); i++) // For all members
	{
		for (size_t j = 0; j <  Pop.Members.at(i).directions.size(); j++) // for all directions in each member
		{
			switch (Pop.Members.at(i).directions.at(j)) // Switch statement for directions
			{
			case 0: // North 
				if (maze[position.x][position.y - 1] == 1) // Checks if the position above is accessable
					break; // if not break
				else 
					position.y -= 1; // If moveable shifts position to 1 up
				break; 
			case 1: // South
				if (maze[position.x][position.y + 1] == 1) // Checks if positon below is available
					break; // if not break
				else
					position.y += 1; // if available move to one positon below
				break;
			case 2: // East
				if (maze[position.x + 1][position.y] == 1) // Checks if position to right is accessable
					break; // if not break
				else
					position.x += 1; // If accessable move 1 spot to the right
				break;
			case 3: // West
				if (maze[position.x - 1][position.y] == 1) // Checks if position to left is accessable
					break; // if not break
				else
					position.x -= 1; // If accessable move one spot to the left
				break;
			}
		}
		Pop.Members.at(i).finalPos = position; // After going through all directions set the final position of each member
		//std::cout << "Position: " << position.x << ", " << position.y << std::endl;
	}	
}

//-------------------------------------
// Calculates the fitness for each member in a population
// Checks final position against end position for each member
// Gets distance between the 2 points
// Calculates distance, the closer to the end the higher the fitness
// Parameters:
//		vector<Member> pop: population to calculate fitness for
//-------------------------------------
void GeneticAlgorithm::CalculateFitness(std::vector<Member> pop)
{
	for (int i = 0; i < Pop.Members.size(); i++) // for all members in population
	{
		// Find distance between the final position of member and end position of the maze
		float Distance = sqrt(((endPos.x - Pop.Members.at(i).finalPos.x) * (endPos.x - Pop.Members.at(i).finalPos.x)) + ((endPos.y - Pop.Members.at(i).finalPos.y) * (endPos.y - Pop.Members.at(i).finalPos.y)));

		Pop.Members.at(i).fitness = 100 - (Distance * 10); // Sets fitness based on distance between the final position and the end position of maze, 100 = solved
		//std::cout << Pop.Members.at(i).fitness << std::endl;
	}
}

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
void GeneticAlgorithm::Breed(std::vector<Member> pop)
{
	// Sort population from Highest to Lowest fitness
	std::sort(Pop.Members.begin(), Pop.Members.end(), [](Member const &a, Member &b) {return a.fitness > b.fitness; });
	
	// Check if solved
	for (int i = 0; i < Pop.Members.size(); i++)
	{
		if (Pop.Members.at(i).fitness == 100) // checks if solved off fitness
		{
			Solved = true;
			std::cout << "Solved!" << std::endl; // if solved print to console
			PrintBestOfGenerations(Pop.Members.at(i));
			break;
		}
	}
	
	// Prints best fitness of each generation
	std::cout << "Best Fitness of Generation " << Generation << " is: " << Pop.Members.at(0).fitness << std::endl;

	// Selecting best of the population to breed
	std::vector<Member> Parents{ Pop.Members.at(0), Pop.Members.at(1) };

	// Mutation and breeding
	for (int i = 0; i < Pop.Members.size(); i++)
	{
		for (int j = 0; j < Pop.Members.at(i).directions.size(); j++)
		{
			int TempSelection = rand() % Parents.size();
			Pop.Members.at(i).directions.at(j) = Parents.at(TempSelection).directions.at(j); // Selects randomly from each parents genes

			// Random mutation based on value above
			if (rand() % 1000 < mutationRate) { Pop.Members.at(i).directions.at(j) = (int)rand() % 4; }

		}
	}

}

//-------------------------------------
// Runs all functions required for the genetic algorithm to solve the maze
// Runs untill solved
// Updates generation each run
// Prints current generation number to console
//-------------------------------------
void GeneticAlgorithm::Run()
{
	while (!Solved) // runs untill solved
	{
		Generation++; // Updates generation number
		std::cout << "Generation: " << Generation << std::endl; // Prints current generation number to console
		TestRoute(Pop.Members); // Runs test route with current generation
		CalculateFitness(Pop.Members); // Runs Calculate fitness with current generation
		Breed(Pop.Members); // Runs breed with current generation
	}
}

//-------------------------------------
// Prints the route of the best member each generation
// Checks the route of the member and stores its positions each movement
// Prints all the position stored to console
// parameters:
//		Member bestOf: Best member of each generation to get directions from.
//-------------------------------------
void GeneticAlgorithm::PrintBestOfGenerations(Member bestOf)
{
	Vector2 position = startPos; // Sets starting position to members current position
	std::vector<Vector2> positions;
	for (int i = 0; i < bestOf.directions.size(); i++) // For all members
	{
		switch (bestOf.directions.at(i)) // Switch statement for directions
		{
		case 0: // North 
			if (maze[position.x][position.y - 1] == 1) // Checks if the position above is accessable
				break; // if not break
			else
			{
				position.y -= 1; // If moveable shifts position to 1 up
				positions.push_back(position);
			}
			break;
		case 1: // South
			if (maze[position.x][position.y + 1] == 1) // Checks if positon below is available
				break; // if not break
			else
			{
				position.y += 1; // if available move to one positon below
				positions.push_back(position);
			}
			break;
		case 2: // East
			if (maze[position.x + 1][position.y] == 1) // Checks if position to right is accessable
				break; // if not break
			else
			{
				position.x += 1; // If accessable move 1 spot to the right
				positions.push_back(position);
			}
			break;
		case 3: // West
			if (maze[position.x - 1][position.y] == 1) // Checks if position to left is accessable
				break; // if not break
			else
			{
				position.x -= 1; // If accessable move one spot to the left
				positions.push_back(position);
			}
			break;
		}
	}
		std::cout << "Best of generation route: " << std::endl;
		for (int i = 0; i < positions.size(); i++)
		{
			std::cout << "(" << positions.at(i).x << "," << positions.at(i).y << "), ";
		}
}