#include "vec2.h"
#include "Entity.h"
#include <algorithm>

float getTheta(float, float);

void CircleVSCircle(Circle* firstBall, Circle* secondBall) {
	
	//Calculate new velocities

	float theta1 = getTheta(firstBall->velocity.x, firstBall->velocity.y);
	float theta2 = getTheta(secondBall->velocity.x, secondBall->velocity.y);

	float phi = getTheta(secondBall->position.x - firstBall->position.x, secondBall->position.y - firstBall->position.y);

	double pi = atan(1) * 4;

	float v1 = firstBall->velocity.length();
	float v2 = secondBall->velocity.length();

	float m1 = firstBall->mass;
	float m2 = secondBall->mass;

	float ptotx = (v1*cos(theta1)*m1) + (v2*cos(theta2) * m2);
	float ptoty = (v1*sin(theta1)*m1) + (v2*sin(theta2) * m2);

	float fract1 = (v1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2*v2 *cos(theta2 - phi)) / (m1 + m2);
	
	int newVelX1 = fract1*cos(phi) + v1 * sin(theta1 - phi)*cos(phi+pi/2);
	int newVelY1 = fract1*sin(phi) + v1 * sin(theta1 - phi)*sin(phi + pi / 2);
	
	int newVelX2 = (ptotx - newVelX1 * m1)/m2;
	int newVelY2 = (ptoty - newVelY1 * m1)/m2;

	

	//Apply restitution

	float e = std::min(firstBall->restitution, secondBall->restitution);

	firstBall->velocity.x = newVelX1 * e;
	firstBall->velocity.y = newVelY1 * e;
	secondBall->velocity.x = newVelX2 * e;
	secondBall->velocity.y = newVelY2 * e;

	//Reposition lighter ball

	float distance = sqrt(pow(firstBall->position.x - secondBall->position.x, 2) + pow(firstBall->position.y - secondBall->position.y, 2));
	distance -= firstBall->radius;
	distance -= secondBall->radius;
	distance *= -1;
	distance = ceil(distance);
	
	float t2 = getTheta(secondBall->position.x - firstBall->position.x, secondBall->position.y - firstBall->position.y);
	float t1 = t2 - pi;
	
	if (firstBall->mass < secondBall->mass) {
		if (cos(t1) > 0) {
			firstBall->move(ceil(distance*cos(t1)), 0);
		}
		else {
			firstBall->move(floor(distance*cos(t1)), 0);
		}
		if (sin(t1) > 0) {
			firstBall->move(0, ceil(distance*sin(t1)));
		}
		else {
			firstBall->move(0,floor(distance*sin(t1)));
		}
	}
	else {
		if (cos(t2) > 0) {
			secondBall->move(ceil(distance*cos(t2)), 0);
		}
		else {
			secondBall->move(floor(distance*cos(t2)), 0);
		}
		if (sin(t2) > 0) {
			secondBall->move(0, ceil(distance*sin(t2)));
		}
		else {
			secondBall->move(0,floor(distance*sin(t2)));
		}
	}

}

float getTheta(float x, float y) {
	
	float theta;
	float thetar;

	double pi = atan(1) * 4;

	thetar = atan(y/x);
	
	if (x > 0) {
		if (thetar > 0) {
			theta = thetar;
		}
		else {
			theta = 2 * pi + thetar;
		}
	}
	else {
		if (thetar > 0) {
			theta = pi + thetar;
		}
		else {
			theta = pi + thetar;
		}
	}

	return theta;
}



//Speculative contact notes

/*

Contact con = contacts[i]; Array of contacts, each contact contains 2 objects(?)
Vector2 n = con.normal; Contact normal

// get all of relative normal velocity
double relNv = (con.b.Vel - con.a.Vel).Dot(n); 
               (obj b vel - obj a vel) dotted with the normal
			   gives rel velocity scalar 


// we want to remove only the amount which leaves them touching next frame
double remove = relNv + con.dist / Constants.kTimeStep; 
remove is equal to relative normal velocity + contact distance/timestep
contact distance/timestep represents velocity needed so there is no penetration


// compute impulse
double imp = remove / (con.a.InvMass + con.b.InvMass);



// accumulate
double newImpulse = Math.Min(imp + con.impulse, 0);

// compute change
double change = newImpulse - con.impulse;

// store accumulated impulse
con.impusle = newImpulse;

// apply impulse
con.a.Vel += change * n * con.a.InvMass;
con.b.Vel -= change * n * con.b.InvMass;

*/