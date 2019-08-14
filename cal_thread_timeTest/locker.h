#ifndef __LOCKER_H
#define __LOCKER_H

#include "mutex.h"

class  locker
{
	mutex &m_mutex;
public:
	 locker(mutex &mutex): m_mutex(mutex){
	 	pthread_mutex_lock(m_mutex);
	 }
	~ locker(){
		pthread_mutex_unlock(m_mutex);
	}
};

#endif