#ifndef NEWTIMER_H
#define NEWTIMER_H
#include <string>

class Timer
{
public:
	Timer(); // Start the timer here
	~Timer(); // Cleanup any memory allocated here

	std::string getTimeText() const; // Return the time in text form, rounded off to 1 d.p.
protected:

private:
	double getTime() const; // Get time elapsed since start

	// Add all variables required below here

};

#endif // !NEWTIMER_H
