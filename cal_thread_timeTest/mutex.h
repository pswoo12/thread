#ifndef __MUTEXT_H
#define __MUTEXT_H

#include <pthread.h>

class  mutex
{
	pthread_mutex_t m_mutex;
public:
	 mutex() {
		pthread_mutex_init(&m_mutex, NULL);
	 }
	~ mutex(){
		pthread_mutex_destroy(&m_mutex);
	}

	operator pthread_mutex_t&() {
		return m_mutex;
	}
	operator pthread_mutex_t*() {
		return &m_mutex;
	}
};

#endif