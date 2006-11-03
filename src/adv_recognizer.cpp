#include "adv_recognizer.h"
#include <algorithm>

RealTimeMouseGestureRecognizer::RealTimeMouseGestureRecognizer( int minimumMovement, double minimumMatch, bool allowDiagonals )
: minimumMatch(minimumMatch), allowDiagonals(true)
{
    minimumMovement2 = minimumMovement*minimumMovement;

	directions.resize(64);	
	lastX = 0;
	lastY = 0;
}

RealTimeMouseGestureRecognizer::~RealTimeMouseGestureRecognizer()
{
    
}


const Direction dirsD[8] = 
{
	Down,
	Up,
	Right,
	Left,
	DownRight,
	DownLeft,
	UpLeft,
	UpRight
};

void RealTimeMouseGestureRecognizer::addPoint(int x, int y)
{
    int dx, dy;

    dx = x-lastX;
    dy = y-lastY;

    if( dx*dx+dy*dy < minimumMovement2 )
          return;
    
	Direction direction;

	const int _directions[8][2] = 
	{
		{0, 15}, //down 0
		{0, -15}, //up 1
		{15, 0}, //right 2
		{-15, 0}, //left 3
		{10, 10}, //down right 4
		{-10, 10},//down left 5
		{-10, -10},//up left 6
			{10, -10}//up right 7
	};
    int maxValue = 0;
    int maxIndex = -1;
            
        for( int i=0; i<(allowDiagonals?8:4); i++ )
        {
			int value = dx * _directions[i][0] + dy * _directions[i][1];
            if( value > maxValue )
            {
				maxValue = value;
                maxIndex = i;
            }
		}     
            
		direction = dirsD[maxIndex];

		if(direction != lastDirection)
		{
			directions.push_back(direction);
			recognizeGesture();
		}
	

	lastX = x;
	lastY = y;
	lastDirection = direction;
}

struct DirectionSort
{
     bool operator()( GestureDefinition a, GestureDefinition b )
     {
          return a.directions.size() > b.directions.size();
     }
};

void RealTimeMouseGestureRecognizer::addGestureDefinition( const GestureDefinition &gesture )
{
	gestures.push_back( gesture );

	std::sort( gestures.begin(), gestures.end(), DirectionSort() );
}

void RealTimeMouseGestureRecognizer::clearGestureDefinitions()
{
    gestures.clear();
}

void RealTimeMouseGestureRecognizer::recognizeGesture()
{
	int first = gestures.size();	
	
	for( GestureList::const_iterator gi = gestures.begin(); gi != gestures.end(); ++gi )
	{

		int readIndex = directions.getReadPointer();
		bool match = true;
    
		try
		{
			for( DirectionList::const_iterator di = gi->directions.begin(); di != gi->directions.end() && match; ++di)
			{
				Direction d = directions.pop();
				if(*di != d)
				{
					match = false;
				}
			}

			if(match)
			{
				if(gi->callbackClass)
				{
					gi->callbackClass->callback();
				}
				return;
			}
			else
			{
				first--;	
				directions.setReadPointerTo(readIndex);
			}
		}
		catch(std::exception e)
		{
			directions.setReadPointerTo(readIndex);
		}
	}

	if(first == 0)
	{
		directions.pop();
	}  
}