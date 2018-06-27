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
	
	Block *m_pool_temp = this->m_pool;


	for( auto i = 0; i < m_pool_temp.m_n_blocks ; ++i){
		if( sizeByte == m_pool_temp->m_sentinel){
			return (void*)m_pool_temp;

		}if else( sizeByte < this->m_pool_temp->m_length){

		}

	}

}