#ifdef CHANGED

#include "pageprovider.h"


PageProvider::PageProvider(int n)
{
	bitmap = new BitMap(n);
	//semBitmap = new Semaphore(1);
}

PageProvider::~PageProvider()
{
	delete bitmap;
	//delete semBitmap;
}

// Renvoie l'indice d'une page libre dans la bitmap, s'il n'y en a pas -1
int PageProvider::GetEmptyPage()
{
	// semBitmap->P();
	int avail = bitmap->Find();
	// semBitmap->V();
	return avail;
}

// Libere un page
void PageProvider::ReleasePage(int p)
{
	// semBitmap->P();
	bitmap->Clear(p);
	// semBitmap->V();
}

// Renvoie le nombre de page libre
int PageProvider::NumAvailPage()
{
	// semBitmap->P();
	int available = bitmap->NumClear();
	// semBitmap->V();
	return available;
}

#endif // CHANGED