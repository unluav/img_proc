#include <stdio.h>
#include <iostream>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "filters.hpp"
#include "videoPlayer.hpp"

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("Usage: %s VIDEO_FILE\n", argv[0]);
		return 1;
	}	


	TestFilter *filter = new TestFilter();

	std::string *fileName = new std::string(argv[1]);

	VideoPlayer *player = new VideoPlayer(filter, fileName, fileName);

	player->play(); 	

	delete filter;
	delete player;

	return 0;
}
