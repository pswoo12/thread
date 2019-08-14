#ifndef __CAL_HPP_
#define __CAL_HPP_

#include <list>
#include "3rdParty/pevents.h"
#include "locker.h"

using namespace neosmart;

class cal;
class THREAD_PARAM
{
public:
	THREAD_PARAM();
	~THREAD_PARAM();

	int 				id;
	neosmart_event_t	event;
	pthread_t			thread_t;
	cal*				pCal;
};

class cal {

	std::list<int> 		m_numList;
	unsigned int 		m_threadCnt;
	THREAD_PARAM*		m_pThreadParam;
	int 				m_nTotal;

	mutex	 			m_sumMutex;
	mutex 				m_getMutex;

	int 			getNumber(int id);
	void 			sumNumber(int nNum);

	static void* 	workProcess(void* pData); 
public:
	cal();
	virtual ~cal();
	bool run();
};

#endif 

