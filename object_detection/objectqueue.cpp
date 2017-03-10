using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
queue generatePoints(){
	int seed = 8675309;
	int x=30;
	int y=30;
	queue<Point2f> q;
	for(int i = 0; i<999; i++ ){
		int xoff = rand() % 2 - 1;
		int yoff = rand() % 2 - 1;
		x +=xoff;
		y +=yoff;
		printf("(%d,%d)\n",x,y);
		q.push(Point2f(x,y));
		
	}
	return q;
	
}


int main(){
	q = generatePoints();
	Point2f old;
	point2f current;
	point2f next;
	for(int i=0; i<q.size(); i++){
		if(i>0)
		{
			old=current;
		}
		Point2f pt = q.pop();
		current = pt;
		
		
		
	}
	return 1;
}
