#include "../include/mempool_common.hpp"
#include "../include/TimeEvent.hpp"
#include "../include/SLPool.hpp"
#include "../include/StoragePool.hpp"
#include <chrono>

int main()
{
	SLPool p(240);
	SLPool q(32);

	p.MemoryDemonstration();
	q.MemoryDemonstration();

	std::cout<<std::endl;

	std::cout<<"Testes: ----------------------------------------------"<<std::endl;

	auto start = std::chrono::steady_clock::now();
	auto mem = new (p) long int[20];
	auto end = std::chrono::steady_clock::now();

	double x = std::chrono::duration <double, std::milli> (end-start).count();

	std::cout<<"Tempo de alocação do SLPool (new long int[22]): "<<x<<" Milissegundos"<<std::endl;

	start = std::chrono::steady_clock::now();
	auto teste = new long int[20];
	end = std::chrono::steady_clock::now();

	x = std::chrono::duration <double, std::milli> (end-start).count();

	std::cout<<"Tempo de alocação do SO (new long int[22]): "<<x<<" Milissegundos"<<std::endl;

	start = std::chrono::steady_clock::now();
	auto mem_2 = new (q) long int;
	end = std::chrono::steady_clock::now();

	x = std::chrono::duration <double, std::milli> (end-start).count();

	std::cout<<"Tempo de alocação do SLPool (new long int): "<<x<<" Milissegundos"<<std::endl;

	start = std::chrono::steady_clock::now();
	auto teste_2 = new long int;
	end = std::chrono::steady_clock::now();

	x = std::chrono::duration <double, std::milli> (end-start).count();

	std::cout<<"Tempo de alocação do SO (new long int): "<<x<<" Milissegundos"<<std::endl;

	std::cout<<std::endl;
	std::cout<<"Testando a memória alocada----------------------------"<<std::endl;

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
	q.MemoryDemonstration();

	std::cout<<"Deletando a memória alocada---------------------------"<<std::endl;

	delete mem;

	delete mem_2;

	delete teste;

	delete teste_2;

	std::cout<<"Verificando a deleção---------------------------------"<<std::endl;

	p.MemoryDemonstration();
	q.MemoryDemonstration();

	return 0;
}