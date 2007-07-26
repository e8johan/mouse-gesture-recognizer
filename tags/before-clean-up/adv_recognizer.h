#ifndef REAL_TIME_RECOGNIZER_H
#define REAL_TIME_RECOGNIZER_H

#include "recognizer.h"
#include "ring_buffer.h"


class RealTimeMouseGestureRecognizer
{
public:
    RealTimeMouseGestureRecognizer( int minimumMovement = 5, double minimumMatch = 0.9, bool allowDiagonals = true  );
    ~RealTimeMouseGestureRecognizer();

    void addGestureDefinition( const GestureDefinition &gesture );
    void clearGestureDefinitions();

    void addPoint( int x, int y );
    void abortGesture();
private:
	
    void recognizeGesture();

	RingBuffer<Direction> directions;
    GestureList gestures;
	

    int minimumMovement2;
    double minimumMatch;
	bool allowDiagonals;

	int lastX, lastY;
	Direction lastDirection;
};

#endif