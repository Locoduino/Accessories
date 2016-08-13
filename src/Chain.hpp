//-------------------------------------------------------------------
#ifndef __UADChain_H__
#define __UADChain_H__
//-------------------------------------------------------------------

template<class T> class UADCHAINEDLISTITEM
{
public:
	T Obj;					// Element carried by the link.
	UADCHAINEDLISTITEM *pNext;

	inline UADCHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class UADCHAINEDLIST
{
public:							
	UADCHAINEDLISTITEM<T> *pFirst;			// Address of next element in chain, or NULL for none.
	UADCHAINEDLISTITEM<T> *pCurrentItem;	// Address of the current item.
	bool HasCurrentNull;		// if true, the current pointer is null when all items has been used.
	
	inline UADCHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(const T &t);
	void NextCurrent();
	inline void ResetCurrent() { this->pCurrentItem = (HasCurrentNull ? NULL : this->pFirst); }
	inline void StartCurrent() { this->pCurrentItem = this->pFirst; }
	inline bool HasCurrent() { return this->pCurrentItem != NULL; }
};

#define CHAINED_ENUMERATE(T, list, func) UADCHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void UADCHAINEDLIST<T>::AddItem(const T &t)
{
	UADCHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new UADCHAINEDLISTITEM<T>();
		if (!HasCurrentNull)
			this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
		while (pCurr != NULL)
		{
			if (pCurr->pNext == NULL)
			{
				pCurr->pNext = new UADCHAINEDLISTITEM<T>();
				pCurr = pCurr->pNext;
				break;
			}
			pCurr = pCurr->pNext;
		}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void UADCHAINEDLIST<T>::NextCurrent()
{
	if (this->pFirst == NULL)
		return;

	if (this->pCurrentItem == NULL)
	{
		this->pCurrentItem = this->pFirst;
		return;
	}

	this->pCurrentItem = this->pCurrentItem->pNext;

	if (this->pCurrentItem == NULL && !HasCurrentNull)
		this->pCurrentItem = this->pFirst;

	return;
}
#endif