#include "Particle.h"


// 2-param constructor which takes x and y positions
Particle::Particle(double x_pos, double y_pos){
	x=x_pos;
	y=y_pos;
}

// Moves the particles according to given X and Y inputs
Particle Particle::moveParticle(double moveX , double moveY){
//	std::cout << "X is " << x << " ";
//	std::cout << moveX << "\t "; 
	// Check for X wall collisions
	if(abs(x+moveX) > chamber_width/2.0) {
//		std::cout << "WALL COLLISION x\n";
		if(x>0) {
			setX(chamber_width-moveX-x);
		} else {
			setX(-chamber_width-moveX-x);
		}
	} else {	
		setX(x + moveX);
	}	

	// Check for Y wall collisions
	if(abs(y+moveY) > chamber_height/2.0) {
//		std::cout << "WALL COLLISION y\n";
		if(y>0) {
			setY(chamber_height-moveY-y);
		} else {
			setY(-chamber_height-moveY-y);
		}	
	} else {
		setY(y + moveY);
	}
}

// Output a particle as an ordered pair of its location
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart){
	outgo << "(" << outpart.getX() << " , " << outpart.getY() << ")";
}
