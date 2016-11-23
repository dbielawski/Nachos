#ifdef CHANGED 
#include "bitmap.h"


class PageProvider
{

public:

	PageProvider(int nbPage);	// taille de la bitmap
	~PageProvider();

	int GetEmptyPage();			// Renvoie un page libre
	void ReleasePage(int p);			// Libere un page
	int NumAvailPage();			// Renvoie le nombre page libre

private:
	BitMap* bitmap;
	//Semaphore* semBitmap;
};

#endif // CHANGED