#ifndef _SLPool_
#define _SLPool_

#include <iostream>

#include "StoragePool.hpp"

class SLPool: public StoragePool
{
	public:
		struct Header
		{
			unsigned int m_length;
			Header() : m_length(0u){ /* Empty */ }
			
		};

		struct Block : public Header
		{
			enum { BlockSize = 16 };
			union {
				Block *m_next;
				char m_raw [ BlockSize - sizeof(Header)]; // Talvez necessite colocar -sizeof(m_next).
			};

			Block() : Header(), m_next( nullptr ) { /* Empty */ };

		};

		private :
			unsigned int m_n_blocks;
			Block *m_pool;
			Block &m_sentinel;

		public :
			explicit SLPool ( size_t );
			~SLPool();
			void * Allocate( size_t  );
			void Free( void * );
	 
};

#include "SLPool.inl"

#endif