#ifndef SCOPE_H
#define SCOPE_H

#include <semaphore.h>
#include <string>

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
	int create_process(std::string exeName, std::string semName, sem_t **sem);
	sem_t *acquireSem;
	sem_t *trackSem;
};

#endif