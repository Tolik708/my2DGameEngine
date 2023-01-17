#pragma once

namespace myMain
{
	float toRadians = 0.0174532925;
	
	//smooth functions
	float lerp(float a, float b, float t)
	{
		return a * (1.0 - t) + (b * t);
	}
	float smoothstep(float a, float b, float t)
	{
		float t1 = t*t;
		float t2 = 1 - (1-t)*(1-t);
		return lerp(a, b, lerp(t1, t2, t));
	}

	//round
	int myRound(float n)
	{
		return floor(n+0.5);
	}
	float myRound(float n, int u)
	{
		return floor((n+0.5)*(10*u))/(10*u);
	}
	float myRound(float n, int u, bool i)
	{
		if (i == false)
			return floor(n*(10*u))/(10*u);
		return floor((n+0.5)*(10*u))/(10*u);
	}
	//vector2
	struct vec2 {
		float x;
		float y;
		vec2(float x1, float y1)
		{
			x = x1;
			y = y1;
		}
		vec2()
		{
			x = 0;
			y = 0;
		}
		
		vec2 operator + (vec2 const &secondVec) const
		{
			return vec2(x + secondVec.x, y + secondVec.y);
		}
		vec2 operator - (vec2 const &secondVec) const
		{
			return vec2(x - secondVec.x, y - secondVec.y);
		}
		vec2 operator / (vec2 const &secondVec) const
		{
			return vec2(x / secondVec.x, y / secondVec.y);
		}
		vec2 operator * (vec2 const &secondVec) const
		{
			return vec2(x * secondVec.x, y * secondVec.y);
		}
	};

	vec2 vec2Lerp(vec2 num1, vec2 num2, float t)
	{
		vec2 result = {lerp(num1.x, num2.x, t), lerp(num1.y, num2.y, t)};
		return result;
	}

	//math
	int randomRange(int min, int max) //range : [min, max]
	{
	   return min + rand() % (( max + 1 ) - min);
	}
	template<typename T>
	float clampInt(T num, float min, float max)
	{
		if (num > max)
			return max;
		else if (num < min)
			return min;
		
		return num;
	}
	int** bilinearResize2DArray(int sX, int sY, int** arrayFrom, int sizeOfArrayFromX, int sizeOfArrayFromY)
	{
		float ratioX = static_cast<float>(sizeOfArrayFromY-1)/static_cast<float>(sX-1);
		float ratioY = static_cast<float>(sizeOfArrayFromX-1)/static_cast<float>(sY-1);
		
		int** arrayTo = new int*[sY];
		for (int h = 0; h < sY; h++)
			arrayTo[h] = new int[sX];
		
		for (int y = 0; y < sY; y++)
		{
			for (int x = 0; x < sX; x++)
			{
				float tempX = x*ratioX;
				float tempY = y*ratioY;
				
				float weightX = tempX-floor(tempX);
				float weightY = tempY-floor(tempY);

				int x1y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(floor(tempX))];
				int x2y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(ceil(tempX))];
				int x1y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(floor(tempX))];
				int x2y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(ceil(tempX))];
				
				arrayTo[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
			}
		}
		return arrayTo;
	}
	bool YSortVec2 (vec2 num1, vec2 num2)
	{
		return (num1.y < num2.y);
	}
}