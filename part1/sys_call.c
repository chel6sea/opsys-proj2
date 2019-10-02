#include <unistd.h>

int main(){

	int i=0;
	while(i<6){
		usleep(1);
		++i;
	}

	return 0;
}
