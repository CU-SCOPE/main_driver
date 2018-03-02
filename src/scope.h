#ifndef SCOPE_H
#define SCOPE_H

#include <semaphore.h>
#include <string>

union char_to_float {
    float f;
    char buff[sizeof(float)];
};

class SCOPE {
public:
	SCOPE();
	~SCOPE();
	void run();
	int test1();
	int test2();
	int acquire();
	int track();
	int orientation();
private:
	int create_process(std::string exeName, std::string options, sem_t **sem, int *filedes);
	void read_position(int file, char *buff);
	float initialPosition;
	sem_t *acquireSem;
	sem_t *trackSem;
};

#endif