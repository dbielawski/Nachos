#ifdef CHANGED

#include "pageprovider.h"
#include "system.h"
#include "stdlib.h"

PageProvider::PageProvider(int n)
{
	bitmap = new BitMap(n);
	semBitmap = new Semaphore("semaphore bitmap", 1);
	size = n;
}

PageProvider::~PageProvider()
{
	delete bitmap;
	delete semBitmap;
}

// Renvoie l'indice d'une page libre dans la bitmap, s'il n'y en a pas -1
// et met la le contenu de la page a 0
int PageProvider::GetEmptyPage()
{
	semBitmap->P();
	
	//avail = bitmap->Find();

	int bitmapSize = size;
	int pagesAvail = bitmap->NumClear();

	unsigned int availPagesIndex[pagesAvail];

	// Construit un tableau avec les indices de la bitmap dont les
	// emplacements sont libres
	int k = 0;
	for (int i = 0; i < bitmapSize; ++i)
		if (!bitmap->Test(i))
			availPagesIndex[k++] = i;
	
	int avail;
	if (pagesAvail == 0)
		avail = -1;
	else
	{	// cherche un indice dans availPagesIndex aleatoirement
		unsigned index = ((rand()) % (pagesAvail));
		avail = availPagesIndex[index];
		bitmap->Mark(avail);
		memset(&(machine->mainMemory[avail*PageSize]), 0, PageSize);
		DEBUG('a', "Page dispo %i\n", avail);
	}
	semBitmap->V();

	return avail;
}

// Libere un page
void PageProvider::ReleasePage(int p)
{
	semBitmap->P();
	bitmap->Clear(p);
	semBitmap->V();
}

// Renvoie le nombre de page libre
int PageProvider::NumAvailPage()
{
	semBitmap->P();
	int available = bitmap->NumClear();
	semBitmap->V();
	return available;
}

int PageProvider::Size()
{
	return size;
}


#endif // CHANGED