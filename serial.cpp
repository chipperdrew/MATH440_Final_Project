// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13

#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Class for particles to store x and y position of each
class Particle {
	private:
		int x, y;
	public:
		Particle();
		Particle(int x_pos, int y_pos) {x=x_pos; y=y_pos;};

		// Getters 
		// TODO: Remove after testing (unless need arises)
		int getX() { return x; }
		int getY() { return y; }
};

int main ()
{
	// Declaration of vars (assume 2D problem to start)
	int chamber_height = 800;
	int chamber_width = 800;
	srand(time(NULL));

	// Initializing particles
	Particle test_particle(rand() % chamber_width, 
		rand() % chamber_height);
	Particle test_particle2(rand() % chamber_width, 
		rand() % chamber_height);

	// Output
	cout << "X position is " << test_particle.getX() << 
	" and Y position is " << test_particle.getY() << endl;
	cout << "X position2 is " << test_particle2.getX() <<
	" and Y position is " << test_particle2.getY() << endl;
	
	return 0;
}
