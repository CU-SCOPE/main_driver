#ifndef SCOPE_H
#define SCOPE_H

class SCOPE {
public:
	SCOPE();
	~SCOPE();
	void run();

private:
	int create_process(char *exeName);
}

#endif