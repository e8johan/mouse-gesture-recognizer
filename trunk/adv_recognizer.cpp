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

const std::string dirs[8] =
{
	"Down",
	"Up",
	"Right",
	"Left",
	"Down Right",
	"Down Left",
	"Up Left",
	"Up Right"
};

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
			//hge->System_Log("->");
			recognizeGesture();
			//hge->System_Log("<-");
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
std::string RealTimeMouseGestureRecognizer::DirectionToName(Direction d)
{
	if(d == Up)
		return std::string("Up");
	if(d == Down)
		return std::string("Down");
	if(d == Right)
		return std::string("Right");
	if(d == Left)
		return std::string("Left");
	if(d == DownRight)
		return std::string("DownRight");
	if(d == DownLeft)
		return std::string("DownLeft");
	if(d == UpRight)
		return std::string("UpRight");
	if(d == UpLeft)
		return std::string("UpLeft");
	
	return std::string("Unknow");
	
}
void RealTimeMouseGestureRecognizer::recognizeGesture()
{
	name = "No match";
	
	for( GestureList::const_iterator gi = gestures.begin(); gi != gestures.end(); ++gi )
	{
		//hge->System_Log("trying gesture %s",gi->name.c_str());
		int readIndex = directions.getReadPointer();
		bool match = true;
    
		try
		{
			for( DirectionList::const_iterator di = gi->directions.begin(); di != gi->directions.end() && match; ++di)
			{
				Direction d = directions.pop();
				//hge->System_Log("\t%d comparing %s with %s",directions.getReadPointer(),DirectionToName(*di).c_str(),DirectionToName(d).c_str());
				if(*di != d)
				{
					match = false;
				}
			}

			if(match)
			{
				//hge->System_Log("gesture is \"%s\"",gi->name.c_str());
				name = gi->name;	
				return;
			}
			else
			{
				directions.setReadPointerTo(readIndex);
			}
		}
		catch(std::exception)
		{
			name = "No match";
			//hge->System_Log("exception");
			directions.setReadPointerTo(readIndex);
		}
	}

	directions.pop();
}