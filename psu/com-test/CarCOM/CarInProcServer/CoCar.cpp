#include "CoCar.h"
#include <string.h>
#include <stdlib.h>

extern DWORD g_objCount;

CoCar::CoCar() : m_refCount(0), m_currSpeed(0), m_maxSpeed(0) {
	m_petName = SysAllocString(OLESTR("MERS"));
	g_objCount++;
}

CoCar::~CoCar() {
	g_objCount--;
	if (m_petName) SysFreeString(m_petName);
	MessageBox(NULL, L"CoCar is dead", L"Destructor", MB_OK | MB_SETFOREGROUND);
}

//IUnknown
STDMETHODIMP_(DWORD) CoCar::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(DWORD) CoCar::Release() {
	return (--m_refCount == 0) ? (delete this, 0) : m_refCount;
}

STDMETHODIMP CoCar::QueryInterface(REFIID riid, void **pIFace) {
	if (riid == IID_IUnknown) {
		MessageBox(NULL, L"Handed out IUnknown", L"QI", MB_OK | MB_SETFOREGROUND);
		*pIFace = (IUnknown*)(IEngine*) this;
	} else if (riid == IID_IEngine) {
		MessageBox(NULL, L"Handed out IEngine", L"QI", MB_OK | MB_SETFOREGROUND);
     	*pIFace = (IEngine*) this;
    } else if (riid == IID_IStats) {
		MessageBox(NULL, L"Handed out IStats", L"QI", MB_OK | MB_SETFOREGROUND);
		*pIFace = (IStats*) this;
	} else if (riid == IID_ICreateCar) {
		MessageBox(NULL, L"Handed out ICreateCar", L"QI", MB_OK | MB_SETFOREGROUND);
		*pIFace = (ICreateCar*) this;
	} else {
		*pIFace = NULL;
		return E_NOINTERFACE;
    }

	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

//IEngine
STDMETHODIMP CoCar::SpeedUp() {
	m_currSpeed += 10;
	return S_OK;
}

STDMETHODIMP CoCar::GetMaxSpeed(int *maxSpeed) {
	*maxSpeed = m_maxSpeed;
	return S_OK;
}

STDMETHODIMP CoCar::GetCurSpeed(int *curSpeed) {
	*curSpeed = m_currSpeed;
	return S_OK;
}

//IStats
STDMETHODIMP CoCar::GetPetName(BSTR * petName) {
	*petName = SysAllocString(m_petName);
	return S_OK;
}

STDMETHODIMP CoCar::DisplayStats() {
	MessageBox(NULL, (LPCWSTR) m_petName, L"Pet Name", MB_OK | MB_SETFOREGROUND);
	char buf[10];
	itoa(m_maxSpeed, buf, 10);
	MessageBoxA(NULL, buf, "MAX Speed", MB_OK | MB_SETFOREGROUND);
	return S_OK;
}

//ICreateCar
STDMETHODIMP CoCar::SetPetName(BSTR petName) {
	SysReAllocString(&m_petName, petName);
	return S_OK;
}

STDMETHODIMP CoCar::SetMaxSpeed(int maxSpeed) {
	if (maxSpeed < MAX_SPEED) m_maxSpeed = maxSpeed;
	return S_OK;
}