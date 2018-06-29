#include "../include/mempool_common.hpp"
#include "../include/TimeEvent.hpp"
#include "../include/SLPool.hpp"
#include "../include/StoragePool.hpp"

int main()
{
	SLPool p(240);

	p.MemoryDemonstration();

	auto mem = new (p) long int[22];

	auto novo = mem;

	novo[0] = 4;
	novo[1] = 50;
	novo[2] = 30;
	novo[3] = 80;
	novo[4] = 1000;
	novo[5] = 10;

	for(auto i{0}; i < 6; i++)
	{
		std::cout<<"Valor de memo: "<<novo[i]<<std::endl;
	}

	p.MemoryDemonstration();

	delete mem;

	p.MemoryDemonstration();

	return 0;
}