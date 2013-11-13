#include "Particle.h"


// 2-param constructor which takes x and y positions
Particle::Particle( double x_new, double y_new, double x_old, double y_old, double rad){
	setnewX(x_new);
	setnewY(y_new);
	setoldX(x_old);
	setoldY(y_old);
	setR(rad);
}

// Moves the particles according to given X and Y inputs
Particle Particle::moveParticle(double moveX , double moveY){
	// Check for X wall collisions
	setoldX(tox);
	setoldY(toy);
	if(fabs(fromx+moveX) > CHAMBER_WIDTH/2.0) {
//		std::cout << "WALL COLLISION x\n";
		if(fromx>0) {
			setnewX(CHAMBER_WIDTH-moveX-fromx);
		} else {
			setnewX(-CHAMBER_WIDTH-moveX-fromx);
		}
	} else {	
		setnewX(fromx + moveX);
	}	

	// Check for Y wall collisions
	if(fabs(fromy+moveY) > CHAMBER_HEIGHT/2.0) {
//		std::cout << "WALL COLLISION y\n";
		if(fromy>0) {
			setnewY(CHAMBER_HEIGHT-moveY-fromy);
		} else {
			setnewY(-CHAMBER_HEIGHT-moveY-fromy);
		}	
	} else {
		setnewY(fromy + moveY);
	}
}

// Output a particle as an ordered pair of its location
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart){
	outgo << "(" << outpart.getnewX() << " , " << outpart.getnewY() << ")";
}
