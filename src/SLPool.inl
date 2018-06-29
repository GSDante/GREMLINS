#include "../include/SLPool.hpp"
#include <iostream>
#include <utility>
#include <cmath>


SLPool::SLPool( size_t sizeByte ){

	this->m_n_blocks = ( sizeByte/Block::BlockSize ) + 1; // Formulada dado no texto para definir o numero de blocos +1
	// A partir definiremos a "fileira" de blocos como se fosse uma lista encadeada
	this->m_pool = new Block[this->m_n_blocks]; // concatenando os blocos como se fosse uma lista
	this->m_pool->m_next = nullptr; // nullptr por está criando um bloco vazio
	this->m_pool->m_length = this->m_n_blocks - 1; // o tamanho vai ser mutável de acordo com o tamanho necessaŕio
	// A partir daqui, definiremos a sentinela que será uma especie de nó cabeça da lista de áreas livres
	// ele não pode ser manipulado pelo cliente,pois server de auxilio para manipular novos blocos com essas listas vazias
	m_sentinel = m_pool[m_n_blocks - 1];
	this->m_sentinel.m_next = this->m_pool;
	this->m_sentinel.m_length = 0;

}

// Destruidor é trivial,pois apenas apaga o bloco alocado

SLPool::~SLPool(){
	delete [] this->m_pool;
}


void * SLPool::Allocate( size_t sizeByte ){

	size_t alloc_area = std::ceil(static_cast<float>(sizeByte)/Block::BlockSize); // Adquire o tamanho necessário de blocos para suprir a quantidade de memória requisitada.
	Block *m_new_pool_temp = m_sentinel.m_next; // servirar pra iniciar o laço com a àrea vazia
	Block *m_new_sentinel = &m_sentinel;// gurando o valor pra continuar a zona livre

	/*
	while( m_new_pool_temp->m_length < alloc_area && m_new_pool_temp != nullptr ){
		m_new_pool_temp = m_new_pool_temp->m_next;// aqui há a procura de um espaço que caiba o tamanho pedido
		m_new_sentinel = m_new_sentinel-> m_next;// sendo que há sempre a atualização dos valores necessários
	}
	*/
	
	if (m_new_pool_temp == nullptr) // exceção para caso não haja tamanho suficiente
	{
		throw std::bad_alloc();
	}

	while(m_new_pool_temp != nullptr)
	{

		if (alloc_area == m_new_pool_temp->m_length ) // condição de se o tamanho for igual , e o else para caso for maior do que pedido
		{
			m_new_sentinel->m_next = m_new_pool_temp->m_next;// a area livre passa a ser a anterior da zona ocupada pela solicitação

			break;
		}
		else if(alloc_area < m_new_pool_temp->m_length)
		{

			m_new_sentinel->m_next = m_new_pool_temp + alloc_area;
			m_new_sentinel->m_next->m_length = m_new_pool_temp->m_length - alloc_area;
			m_new_sentinel->m_next->m_next = m_new_pool_temp->m_next;
			m_new_pool_temp->m_length = alloc_area;

			break;
			// nesse else, temos o caso da zona pedida ser menor do que a diponivel,para isso: a zona livre passa a ser a anterior
			// pulando a parte alocada ; configura-se o tamanho para ser o tamanho anterios menos a quantidade de blocos;
			// e a nova zona aponta para a próxima sem alteração;
		}

		m_new_pool_temp = m_new_pool_temp->m_next;
		m_new_sentinel = m_new_sentinel->m_next;
	}

	return reinterpret_cast<Header*>( m_new_pool_temp ) + 1U;

}

void * SLPool::Allocate_Bestfit( size_t sizeByte ){

	size_t alloc_area = std::ceil(static_cast<float>(sizeByte)/Block::BlockSize); // Adquire o tamanho necessário de blocos para suprir a quantidade de memória requisitada.
	Block *m_new_pool_temp = m_sentinel.m_next; // servirar pra iniciar o laço com a àrea vazia
	Block *m_new_sentinel = &m_sentinel;// gurando o valor pra continuar a zona livre
	int i = 0;
	size_t menor;
	Block *best_fit_1 = nullptr;
	Block *best_fit_2 = nullptr;

	
	if (m_new_pool_temp == nullptr) // exceção para caso não haja tamanho suficiente
	{
		throw std::bad_alloc();
	}

	while(m_new_pool_temp != nullptr)
	{

		if (alloc_area == m_new_pool_temp->m_length ) // condição de se o tamanho for igual , e o else para caso for maior do que pedido
		{
			m_new_sentinel->m_next = m_new_pool_temp->m_next;// a area livre passa a ser a anterior da zona ocupada pela solicitação

			return reinterpret_cast<Header*>( m_new_pool_temp ) + 1U;
		}
		else if(alloc_area < m_new_pool_temp->m_length)
		{

			if(i == 0)
			{
				menor = m_new_pool_temp->m_length;
				best_fit_1 = m_new_pool_temp;
				best_fit_2 = m_new_sentinel;

				i++;
			}
			else if(menor > m_new_pool_temp->m_length)
			{
				menor = m_new_pool_temp->m_length;
				best_fit_1 = m_new_pool_temp;
				best_fit_2 = m_new_sentinel;
			}
			// nesse else, temos o caso da zona pedida ser menor do que a diponivel,para isso: a zona livre passa a ser a anterior
			// pulando a parte alocada ; configura-se o tamanho para ser o tamanho anterios menos a quantidade de blocos;
			// e a nova zona aponta para a próxima sem alteração;
		}

		m_new_pool_temp = m_new_pool_temp->m_next;
		m_new_sentinel = m_new_sentinel->m_next;
	}

	best_fit_2->m_next = best_fit_1 + alloc_area;
	best_fit_2->m_next->m_length = best_fit_1->m_length - alloc_area;
	best_fit_2->m_next->m_next = best_fit_1->m_next;
	best_fit_1->m_length = alloc_area;

	return reinterpret_cast<Header*>( best_fit_1 ) + 1U;

}

void SLPool::Free(void * al_mem)
{
    Block *m_new_pool_temp = reinterpret_cast<Block*>(reinterpret_cast<int*>(al_mem)-1U); // Voltando a posiçao do head para percorrer o bloco todo.
    Block *m_free_s = (&m_sentinel)->m_next; //Iniciando um ponteiro para a 1° área livre (fast)
    Block *m_new_sentinel = &m_sentinel; //Iniciando um ponteiro para o sentinela (slow)
    
    // Percore a lista de áreas livres
    while (m_free_s!=nullptr)
    {
    
    	// Percorre as áreas livres até que a área indicada por m_new_pool_temp esteja entre m_free_s e m_new_sentinel.
        if (m_free_s > m_new_pool_temp)
        {
            if ((m_new_sentinel + m_new_sentinel->m_length == m_new_pool_temp) and (m_new_pool_temp + m_new_pool_temp->m_length == m_free_s))
            {
            	// Caso a memória alocada esteja diretamente entre duas áreas livres (1).
                m_new_sentinel->m_length = m_new_sentinel->m_length + m_free_s->m_length + m_new_pool_temp->m_length;
                m_free_s->m_length = 0;
                m_new_pool_temp->m_length = 0;
                m_new_sentinel->m_next = m_free_s->m_next;
            } 
            else if (m_new_pool_temp + m_new_pool_temp->m_length == m_free_s)
            {
            	// Caso a memória alocada esteja diretamente atrás de uma área livre (2).
                m_new_pool_temp->m_length = m_new_pool_temp->m_length + m_free_s->m_length;
                m_free_s->m_length = 0;
                m_new_pool_temp->m_next = m_free_s->m_next;

                if(m_new_pool_temp == m_pool)
                {
                	m_sentinel.m_next = m_new_pool_temp;
                }

            }
            else if (m_new_sentinel+m_new_sentinel->m_length == m_new_pool_temp)
            {
            	// Caso a memória alocara esteja diretamente na frente de uma área livre mas entre ela e a proxima área livre tenha outro espaço alocado (3).
                m_new_sentinel->m_length = m_new_sentinel->m_length + m_new_pool_temp->m_length;
                m_new_pool_temp->m_length = 0;
            } 
            else
            {
            	// Similar ao caso anterior, mas agora a memória se encontra entre a área livre anterior (4).
                m_new_sentinel->m_next = m_new_pool_temp;
                m_new_pool_temp->m_next = m_free_s;
            }
        }
        else
        {
        	// Avançando o sentinel caso m_new_pool_temp ainda não esteja entre as duas áreas livres de m_new_sentinel e m_free_s
        	m_new_sentinel = m_new_sentinel->m_next;
        }

       	// Avançando m_free_s. 
        m_free_s = m_free_s->m_next;
    
    }

    // Caso o m_free_s chegue fora das áreas livres, é por que toda a memória foi consumida, então basta ligar a referência do sentinel para o bloco que se deseja deletar.
    if (m_free_s == nullptr)
    {
        m_new_sentinel->m_next = m_new_pool_temp; // Atualização da referência.
    }
}

void SLPool::MemoryDemonstration( void ){
	Block *aux = this->m_sentinel.m_next;
	int count = 0;

	std::cout << "\n##############  THIS IS A MemoryDemonstration  ##############\n ";

	while(aux != nullptr){
		std::cout << "Àreas livres: " << aux << ": que tem [ " << aux->m_length << " ] espaços\n";
		aux = aux->m_next;
		count++;
	}

	std::cout << "O número de àreas livres foram : " << count << std::endl;
	std::cout << "######### THIS IS THE END#########\n";
	
}