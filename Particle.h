#include <iostream>
#include <stdlib.h>
#include <math.h>

// Global vars
static const double CHAMBER_HEIGHT = 800;
static const double CHAMBER_WIDTH = 800;
// Escape if on x-positive wall AND y is within this value of the origin
static const double HALF_ESCAPE_WALL_WIDTH = 5;
//static const double escapeHeight = 0.2;
//static const double escapeWidth = 0.2;


class Particle{
	private:
		double fromx, fromy;
		double tox, toy;
		double radius;
	public: 
		Particle(double x_new=0, double y_new=0, double x_old=0, double y_old=0, double rad=1);
		Particle moveParticle(double moveX, double moveY);
		// Getters & Setters 
		double getoldX() const { return fromx; }
		double getoldY() const { return fromy; }
		void setoldX(double x_new) { fromx = x_new; }
		void setoldY(double y_new) { fromy = y_new; }
		
		double getnewX() const { return tox; }
		double getnewY() const { return toy; }
		void setnewX(double x_new) { tox = x_new; }
		void setnewY(double y_new) { toy = y_new; }
		
		double getR() const { return radius; }
		void setR(double rad) { radius = rad; }
		
};
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart);
