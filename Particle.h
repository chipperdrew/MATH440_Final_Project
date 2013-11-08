#include <iostream>
#include <stdlib.h>

// Global vars
static const double chamber_height = 800;
static const double chamber_width = 800;

class Particle{
	private:
		double x, y;
	public:
		Particle(double x_pos=0, double y_pos=0);
		Particle moveParticle(double moveX, double moveY);
		// Getters & Setters 
		double getX() const { return x; }
		double getY() const { return y; }
		void setX(double x_new) { x = x_new; }
		void setY(double y_new) { y = y_new; }
		
};
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart);
