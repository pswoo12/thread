#include "cal.hpp"
#include <iostream>
#include <thread>
#include "timechecker.h"

THREAD_PARAM::THREAD_PARAM():event(NULL), thread_t(0), pCal(NULL) {

}

THREAD_PARAM::~THREAD_PARAM() {

}

cal::cal():m_threadCnt(0), m_pThreadParam(NULL), m_nTotal(0) {

}

cal::~cal() {
	if (m_pThreadParam) {
		for (unsigned int i = 0; i < m_threadCnt; i++) {
			int status=0;

			if (m_pThreadParam[i].thread_t)
				pthread_join(m_pThreadParam[i].thread_t, (void**)&status);
		}

		for (unsigned int i = 0; i < m_threadCnt; ++i) {
			if (m_pThreadParam[i].event)
				DestroyEvent(m_pThreadParam[i].event);
		}

		delete [] m_pThreadParam;
		m_pThreadParam = NULL;

		std::cout << "m_nTotal = " << m_nTotal << std::endl;
	}
}

bool cal::run() {

	int sum = 0;
	
	for (int i = 0; i < 100000; i++) {
		m_numList.push_back(i+1);
		sum += (i+1);
	}

	int nCnt = 0;
	std::cout <<  "input Thread Number : " ;
	std::cin >> nCnt;

	if (nCnt > 0) {
		m_threadCnt = nCnt;
		m_pThreadParam = new THREAD_PARAM[m_threadCnt];

		for (unsigned int i = 0; i < m_threadCnt;  i++) {
			m_pThreadParam[i].event = CreateEvent();
			ResetEvent(m_pThreadParam[i].event);
			m_pThreadParam[i].pCal = this;
			m_pThreadParam[i].id = i;

			int nCode = pthread_create(&m_pThreadParam[i].thread_t, NULL, &cal::workProcess, (void*)&m_pThreadParam[i]);
			
			if (nCode != 0) {
				return false;
			}
		}

		for (unsigned int i = 0;  i < m_threadCnt; i++) {
			SetEvent(m_pThreadParam[i].event);
		}

		return true;
	}
	return false;
}

int cal::getNumber(int id) {
	locker lock(m_getMutex);
	int nNum = 0;
	if (!m_numList.empty()) {
		nNum = m_numList.front();
		m_numList.pop_front();
	}
	
	return nNum;
}

void cal::sumNumber(int nNum) {
	locker lock(m_sumMutex);
	m_nTotal += nNum;
}


void* cal::workProcess(void* pData) {
	timeChecker tc(std::string("workProcess()"));
	THREAD_PARAM *pThis = (THREAD_PARAM*)pData;
	cal *pCal = pThis->pCal;
	WaitForEvent(pThis->event);

	int nNum = 0;
	while ((nNum = pCal->getNumber(pThis->id)) != 0) {
		pCal->sumNumber(nNum);
	}

	pThis->thread_t = 0;
	return NULL;
}



