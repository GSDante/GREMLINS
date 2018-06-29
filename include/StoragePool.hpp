#ifndef _STORAGEPOOL_
#define _STORAGEPOOL_

#include <iostream>

class StoragePool {

	public :

		virtual ~StoragePool (){};
		virtual void * Allocate ( size_t ) = 0;
		virtual void Free (void *) = 0;

 };
 #endif 