//-------------------------------------------------------------------
#ifndef __ACCSChain_H__
#define __ACCSChain_H__
//-------------------------------------------------------------------

template<class T> class ACCSCHAINEDLISTITEM
{
public:
	T *Obj;					// Element carried by the link.
	ACCSCHAINEDLISTITEM *pNext;

	inline ACCSCHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class ACCSCHAINEDLIST
{
public:							
	ACCSCHAINEDLISTITEM<T> *pFirst;			// Address of next element in chain, or NULL for none.
	ACCSCHAINEDLISTITEM<T> *pCurrentItem;	// Address of the current item.
	bool HasCurrentNull;		// if true, the current pointer is null when all items has been used.
	
	inline ACCSCHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(T *t);
	void NextCurrent();
	inline void ResetCurrent() { this->pCurrentItem = (HasCurrentNull ? NULL : this->pFirst); }
	inline void StartCurrent() { this->pCurrentItem = this->pFirst; }
	inline bool HasCurrent() { return this->pCurrentItem != NULL; }
};

#define CHAINED_ENUMERATE(T, list, func) ACCSCHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void ACCSCHAINEDLIST<T>::AddItem(T *t)
{
	ACCSCHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new ACCSCHAINEDLISTITEM<T>();
		if (!HasCurrentNull)
			this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
		while (pCurr != NULL)
		{
			if (pCurr->pNext == NULL)
			{
				pCurr->pNext = new ACCSCHAINEDLISTITEM<T>();
				pCurr = pCurr->pNext;
				break;
			}
			pCurr = pCurr->pNext;
		}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void ACCSCHAINEDLIST<T>::NextCurrent()
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