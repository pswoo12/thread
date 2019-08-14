#ifndef TIMECHECKER_H
#define TIMECHECKER_H

#include <sys/time.h>
#include <stdio.h>

class timeChecker {
	std::string m_name;
	struct timeval 	m_timeStart;
public:
	timeChecker(std::string name) : m_name(name)  {
		gettimeofday( &m_timeStart, NULL );
	} 
	~timeChecker() {
		struct timeval timeEnd;
		gettimeofday( &timeEnd, NULL );
		struct timeval timeDiff;

		timeDiff.tv_sec  = timeEnd.tv_sec  - m_timeStart.tv_sec;
	    timeDiff.tv_usec = timeEnd.tv_usec - m_timeStart.tv_usec;

		if( timeDiff.tv_usec < 0 )  {
	        timeDiff.tv_sec  = timeDiff.tv_sec  - 1;
	        timeDiff.tv_usec = timeDiff.tv_usec + 1000000;
	    }

	    printf("[%s] time sec : [%ld.%06ld]\n", m_name.c_str(), timeDiff.tv_sec, (timeDiff.tv_usec));
	}
};

#endif