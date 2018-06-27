#include "../include/SLPool.hpp"
#include <iostream>


SLPool::SLPool( sizet_t sizeByte ){

	this->m_n_blocks = ( sizeByte/Block::BlockSize ) + 1; // Formulada dado no texto para definir o numero de blocos +1
	// A partir definiremos a "fileira" de blocos como se fosse uma lista encadeada
	this->m_pool = new Block[this->m_n_blocks]; // concatenando os blocos como se fosse uma lista
	this->m_pool->m_next = nullptr; // nullptr por está criando um bloco vazio
	this->m_pool->m_length = this->m_n_blocks; // o tamanho vai ser mutável de acordo com o tamanho necessaŕio
	// A partir daqui, definiremos a sentinela que será uma especie de nó cabeça da lista de áreas livres
	// ele não pode ser manipulado pelo cliente,pois server de auxilio para manipular novos blocos com essas listas vazias
	this->m_sentinel.m_next = this->m_pool;
	this->m_sentinel.m_length = 0;

}

// Destruidor é trivial,pois apenas apaga o bloco alocado

SLPool::~SLPool(){
	delete [] this->m_pool;
}

void * SLPool::Allocate( size_t sizeByte ){

	Block *m_new_pool_temp = this->m_sentinel.m_next; // servirar pra iniciar o laço com a àrea vazia
	Block *m_new_sentinel = &this->m_sentinel;// gurando o valor pra continuar a zona livre

	while( m_new_pool_temp->m_length < m_n_blocks && m_new_pool_temp != nullptr ){
		m_new_pool_temp = m_new_pool_temp->m_next;// aqui há a procura de um espaço que caiba o tamanho pedido
		m_new_sentinel = m_new_sentinel-> m_next;// sendo que há sempre a atualização dos valores necessários
	}
	
	if (n_new_pool_temp == nullptr) // exceção para caso não haja tamanho suficiente
	{
		throw std::bad_alloc( "Não tem espaço disponível.");
	}

	if (m_n_blocks == m_new_pool_temp->m_length ) // condição de se o tamanho for igual , e o else para caso for maior do que pedido
	{
		m_new_sentinel->m_next = m_new_pool_temp->m_next;// a area livre passa a ser a anterior da zona ocupada pela solicitação

	}else{

		m_new_sentinel->m_next = m_new_pool_temp + m_n_blocks;
		m_new_sentinel->m_next->m_length = m_new_pool_temp->m_length - m_n_blocks;
		m_new_sentinel->m_next->m_next = m_new_pool_temp->m_next;
		m_new_pool_temp->m_length = m_n_blocks;
		// nesse else, temos o caso da zona pedida ser menor do que a diponivel,para isso: a zona livre passa a ser a anterior
		// pulando a parte alocada ; configura-se o tamanho para ser o tamanho anterios menos a quantidade de blocos;
		// e a nova zona aponta para a próxima sem alteração;
	}

	return reinterpret_cast<Header*>( m_new_pool_temp ) + 1U;

	


}