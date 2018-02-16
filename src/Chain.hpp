//-------------------------------------------------------------------
#ifndef __ACCSChain_H__
#define __ACCSChain_H__
//-------------------------------------------------------------------

/** this is an item inside a ACCSCHAINEDLIST.*/
template<class T> class ACCSCHAINEDLISTITEM
{
public:
	/** Item address.*/
	T *Obj;
	/** Address of the next item.*/
	ACCSCHAINEDLISTITEM *pNext;

	/** Default constructor.*/
	inline ACCSCHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

/** This class describes a chained list of items (https://en.wikipedia.org/wiki/Linked_list).
It can be called also a linked list. This is an elegant way to build a list without any big allocation, bypassing
the needs to estimate a minimum size and to increase or decrease this size during execution. The disadvantage is a
slower and more complex way to access to an item on the list, and a more complex way to add a new item.

To start, a chained list needs a starting pointer referencing the first item of the list. After that, each item
have a pointer to the next item. If this pointer is NULL, this is the end of the list !

The 'T' type can be of any type, including base types like int or char. But keep in mind that each item must save
a pointer to the next item. If the size of the pointer is greater than the size of the item itself, consider using
a true array...

A notion of 'current item' is used, to let the user of the list to move through the list without doing a loop itself...
*/
template<class T> class ACCSCHAINEDLIST
{
public:							
	/** Address of next element in chain, or NULL for none. In this case the list is empty.*/
	ACCSCHAINEDLISTITEM<T> *pFirst;
	/** Address of current item in the chain, or NULL for none.*/
	ACCSCHAINEDLISTITEM<T> *pCurrentItem;
	/**If true, the current pointer is null when all items has been used.*/
	bool HasCurrentNull;
	
	/** Default constructor.*/
	inline ACCSCHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	/** Add a new item to the list.
	@param t Address of the item to add.
	*/
	void AddItem(T *t);
	/** Move the current pointer to the next one in the list. If the current was the last one, the current pointer
	will be set to the start of the list, its first item.*/
	void NextCurrent();
	/**Reset the current item to the first of the list, or NULL if HasCurrentNull is true.*/
	inline void ResetCurrent() { this->pCurrentItem = (this->HasCurrentNull ? NULL : this->pFirst); }
	/**Reset the current item to the first of the list.*/
	inline void StartCurrent() { this->pCurrentItem = this->pFirst; }
	/**Checks if the current is NULL.
	@return True if the current item is NULL.
	*/
	inline bool HasCurrent() { return this->pCurrentItem != NULL; }
};

/** Macro to call a given function on all items in a list.
@param T type of the item
@param list	Chained/linked list to scan.
@param function Function to call. Must be of type
void function(T *pItem);
*/
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