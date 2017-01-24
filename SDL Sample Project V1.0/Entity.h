#pragma once
#include "vec2.h"
#include <assert.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>


class Entity {

public:

	float restitution;
	float mass;
	float invMass;

	int height;
	int width;

	bool alive;

	SDL_Rect rect;
	SDL_Surface* surface;

	vec2<float> velocity;
	vec2<float> position;


	void draw(SDL_Surface* screen) {
		if (alive) {
			SDL_BlitSurface(this->surface, NULL, screen, &this->rect); //make this blit self
		}

	}


	void update(float frameDelta) {

		position += velocity*frameDelta;

		rect.x = position.x;
		rect.y = position.y;
	}

	void move(float x, float y) {
		position.x += x;
		position.y += y;

		rect.x = position.x;
		rect.y = position.y;
	}


	Entity::Entity() {
		restitution = 1;
		mass = 1;
		invMass = 1;

		position.x = 0;
		position.y = 0;
		velocity.x = 0;
		velocity.y = 0;

		alive = false;

		surface = IMG_Load("Images/x.bmp");

		height = surface->h;
		width = surface->w;

	}


	Entity::Entity(float r, float m, float x, float y, float vx, float vy, bool al, int pic) {

		restitution = r;
		mass = m;
		if (m != 0) {
			invMass = 1 / mass;
		}
		else {
			invMass = 0;
		}

		position.x = x;
		position.y = y;

		velocity.x = vx;
		velocity.y = vy;

		alive = al;

		if (pic == 1) {
			surface = IMG_Load("Images/circle_1.png");
		}
		else if (pic == 2) {
			surface = IMG_Load("Images/circle_2.png");
		}

		if (!surface)
		{
			printf("IMG_Load: %s\n", IMG_GetError());
		}

		height = surface->h;
		width = surface->w;

	}



};

class Circle : public Entity {

public:

	int radius;

	void update(float frameDelta) {

		position += velocity*frameDelta;

		rect.x = position.x - radius;
		rect.y = position.y - radius;
	}

	void move(float x, float y) {
		position.x += x;
		position.y += y;

		rect.x = position.x - radius;
		rect.y = position.y - radius;
	}


	Circle::Circle() :Entity(1, 1, 0, 0, 0, 0, false, 1) {

		radius = 1;

	}

	Circle::Circle(float r, float m, float x, float y, float vx, float vy, bool al, int pic) : Entity(r, m, x, y, vx, vy, al, pic) {

		radius = width / 2;

	}

};

class AABB : public Entity {
public:

	vec2<float> min;
	vec2<float> max;

	AABB::AABB() :Entity(1, 1, 0, 0, 0, 0, false, 1) {

		min.x = position.x;
		min.y = position.y;
		max.x = 1;
		max.y = 1;

	}

	AABB::AABB(float r, float m, float x1, float y1, float x2, float y2, float vx, float vy, bool al, int pic) : Entity(r, m, x1, y1, vx, vy, al, pic) {

		min.x = position.x;
		min.y = position.y;
		max.x = x2;
		max.y = y2;

	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CircleGroup
{
private:
	int m_nLength;
	Circle* m_pnData;

public:
	CircleGroup()
	{
		m_nLength = 0;
		m_pnData = 0;
	}

	CircleGroup(int nLength)
	{
		m_pnData = new Circle[nLength];
		m_nLength = nLength;
	}

	~CircleGroup()
	{
		delete[] m_pnData;
	}

	void Erase()
	{
		delete[] m_pnData;
		// We need to make sure we set m_pnData to 0 here, otherwise it will
		// be left pointing at deallocated memory!
		m_pnData = 0;
		m_nLength = 0;
	}

	Circle& operator[](int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_nLength);
		return m_pnData[nIndex];
	}

	// Reallocate resizes the array.  Any existing elements will be destroyed.
	// This function operates quickly.
	void Reallocate(int nNewLength)
	{
		// First we delete any existing elements
		Erase();

		// If our array is going to be empty now, return here
		if (nNewLength <= 0)
			return;

		// Then we have to allocate new elements
		m_pnData = new Circle[nNewLength];
		m_nLength = nNewLength;
	}

	// Resize resizes the array.  Any existing elements will be kept.
	// This function operates slowly.
	void Resize(int nNewLength)
	{
		// If we are resizing to an empty array, do that and return
		if (nNewLength <= 0)
		{
			Erase();
			return;
		}

		// Now we can assume nNewLength is at least 1 element.  This algorithm
		// works as follows: First we are going to allocate a new array.  Then we
		// are going to copy elements from the existing array to the new array.
		// Once that is done, we can destroy the old array, and make m_pnData
		// point to the new array.

		// First we have to allocate a new array
		Circle *pnData = new Circle[nNewLength];

		// Then we have to figure out how many elements to copy from the existing
		// array to the new array.  We want to copy as many elements as there are
		// in the smaller of the two arrays.
		if (m_nLength > 0)
		{
			int nElementsToCopy = (nNewLength > m_nLength) ? m_nLength : nNewLength;

			// Now copy the elements one by one
			for (int nIndex = 0; nIndex < nElementsToCopy; nIndex++)
				pnData[nIndex] = m_pnData[nIndex];
		}

		// Now we can delete the old array because we don't need it any more
		delete[] m_pnData;

		// And use the new array instead!  Note that this simply makes m_pnData point
		// to the same address as the new array we dynamically allocated.  Because
		// pnData was dynamically allocated, it won't be destroyed when it goes out of scope.
		m_pnData = pnData;
		m_nLength = nNewLength;
	}


	void InsertBefore(Circle nValue, int nIndex)
	{
		// Sanity check our nIndex value
		assert(nIndex >= 0 && nIndex <= m_nLength);

		// First create a new array one element larger than the old array
		Circle *pnData = new Circle[m_nLength + 1];

		// Copy all of the elements up to the index
		for (int nBefore = 0; nBefore < nIndex; nBefore++)
			pnData[nBefore] = m_pnData[nBefore];

		// insert our new element into the new array
		pnData[nIndex] = nValue;

		// Copy all of the values after the inserted element
		for (int nAfter = nIndex; nAfter < m_nLength; nAfter++)
			pnData[nAfter + 1] = m_pnData[nAfter];

		// Finally, delete the old array, and use the new array instead
		delete[] m_pnData;
		m_pnData = pnData;
		m_nLength += 1;
	}

	void Remove(int nIndex)
	{
		// Sanity check our nIndex value
		assert(nIndex >= 0 && nIndex < m_nLength);

		// First create a new array one element smaller than the old array
		Circle *pnData = new Circle[m_nLength - 1];

		// Copy all of the elements up to the index
		for (int nBefore = 0; nBefore < nIndex; nBefore++)
			pnData[nBefore] = m_pnData[nBefore];

		// Copy all of the values after the inserted element
		for (int nAfter = nIndex + 1; nAfter < m_nLength; nAfter++)
			pnData[nAfter - 1] = m_pnData[nAfter];

		// Finally, delete the old array, and use the new array instead
		delete[] m_pnData;
		m_pnData = pnData;
		m_nLength -= 1;
	}

	// A couple of additional functions just for convenience
	void InsertAtBeginning(Circle nValue) { InsertBefore(nValue, 0); }
	void InsertAtEnd(Circle nValue) { InsertBefore(nValue, m_nLength); }

	int GetLength() { return m_nLength; }

	void draw(SDL_Surface* screen) {
		for (int i = 0; i < m_nLength; i++) {
			m_pnData[i].draw(screen);

		}
	}

	void update(float frameDelta) {
		for (int i = 0; i < m_nLength; i++) {
			m_pnData[i].update(frameDelta);
		}
	}

};