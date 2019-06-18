#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include<vector>
#include<iostream>
#include<algorithm>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {

private:
    Node<T> *root;
    unsigned rows, columns;
    vector<Node<T>*> hrows;

public:

    Matrix(unsigned r,unsigned c):rows(r),columns(c){
      for(int i=0;i<r;i++){
        Node<T>* n=new Node<T>(0);
        hrows.push_back(n);
      }
    }

    unsigned getRows() const{return rows;}
    unsigned getColumns() const{return columns;}
    vector<Node<T>*> getNodes() const{return hrows;}

    void set(unsigned x,unsigned y, T value){
      Node<T> *tmpBefore=hrows[x];
      Node<T> *tmpCurrent=hrows[x]->next;
      while(tmpCurrent && tmpCurrent->y<=y){
        if(tmpCurrent->y==y){
          if(!value){
            tmpBefore->next=tmpCurrent->next;
            hrows[x]->value--;
            delete tmpCurrent;
          }
          else
            tmpCurrent->value=value;

          return;
        }
        hrows[x]->value++;
        tmpBefore=tmpCurrent;
        tmpCurrent=tmpCurrent->next;
      }

      if(!value)
        return;

      Node<T>* nuevo=new Node<T>(value,x,y);
      tmpBefore->next=nuevo;
      nuevo->next=tmpCurrent;
    }

    T operator()(int i,int j) const{
      unsigned int x=(unsigned int)i;
      unsigned int y=(unsigned int)j;

      if(x<0 || x>=rows || y<0 ||  y>=columns)
        throw printf("El indice de fila o columna es mayor al permitido");

      Node<T> *tmpCurrent=hrows[x]->next;
      while(tmpCurrent && tmpCurrent->y<=y){
        if(tmpCurrent->y==y)
          return tmpCurrent->value;
        tmpCurrent=tmpCurrent->next;
      }
      return 0;
    }

    const Matrix<T> operator*(T scalar) const{
      Matrix<T> res(rows,columns);
      Node<T> *tmpCurrent;
      for(int i=0;i<rows;i++){
        tmpCurrent=hrows[i]->next;
        while(tmpCurrent){
          res.set(i,tmpCurrent->y,tmpCurrent->value*scalar);
          tmpCurrent=tmpCurrent->next;
        }
      }
      return res;
    }

    const Matrix<T> operator*(Matrix<T> &other) const{

      if(other.getRows()!=columns)
        throw printf("#Columnas de Matriz Inicial no coincide con el #Filas de Matriz Secundaria\n");

      int cols=other.getColumns();
      Matrix<T> res(rows,cols);
      T value;
      for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
          value=0;
          for(int k=0;k<columns;k++)
            value+=operator()(i,k)*other(k,j);
          res.set(i,j,value);
        }
      }
      return res;
    }

    const Matrix<T> operator+(Matrix<T> &other) const{
      if(other.getRows()!=rows || other.getColumns()!=columns)
        throw printf("numero de Filas o Columnas no coinciden");

      Matrix<T> res(rows,columns);
      Node<T> *tmpCurrent,*tmpCurrent2;
      vector<Node<T>*> hrows2=other.getNodes();
      for(int i=0;i<rows;i++){
        tmpCurrent=hrows[i]->next;
        tmpCurrent2=hrows2[i]->next;

        for(int j=0;j<columns;j++){

          if(tmpCurrent && tmpCurrent2 && (tmpCurrent->y==tmpCurrent2->y)){
            res.set(i,tmpCurrent->y,tmpCurrent->value+tmpCurrent2->value);
            tmpCurrent=tmpCurrent->next;
            tmpCurrent2=tmpCurrent2->next;
          }
          else if(tmpCurrent && (tmpCurrent->y==j)){
            res.set(i,tmpCurrent->y,tmpCurrent->value);
            tmpCurrent=tmpCurrent->next;
          }
          else if(tmpCurrent2 && (tmpCurrent2->y==j)){
            res.set(i,tmpCurrent2->y,tmpCurrent2->value);
            tmpCurrent2=tmpCurrent2->next;
          }

        }

      }
      return res;
    }

    const Matrix<T>& operator=(const Matrix<T> &other){
      vector<Node<T>*> hrowstmp=other.getNodes();
      Node<T>* nodetmp;
      for(int i=0;i<rows;i++){
        nodetmp=hrowstmp[i]->next;
        for(int j=0;j<columns;j++){
          if(nodetmp){
            if(nodetmp->getY()==j){
              set(nodetmp->getX(),nodetmp->getY(),nodetmp->getValue());
              nodetmp=nodetmp->next;
            }
            else
              set(i,j,0);
          }
          else
              set(i,j,0);
        }
      }
      return *this;
    }

    const Matrix<T> operator-(Matrix<T>& other) const{
      if(other.getRows()!=rows || other.getColumns()!=columns)
        throw ("numero de Filas o Columnas no coinciden");

      Matrix<T> res(rows,columns);
      Node<T> *tmpCurrent,*tmpCurrent2;
      vector<Node<T>*> hrows2=other.getNodes();
      for(int i=0;i<rows;i++){
        tmpCurrent=hrows[i]->next;
        tmpCurrent2=hrows2[i]->next;

        for(int j=0;j<columns;j++){

          if(tmpCurrent && tmpCurrent2 && (tmpCurrent->y==tmpCurrent2->y)){
            res.set(i,tmpCurrent->y,tmpCurrent->value - tmpCurrent2->value);
            tmpCurrent=tmpCurrent->next;
            tmpCurrent2=tmpCurrent2->next;
          }
          else if(tmpCurrent && (tmpCurrent->y==j)){
            res.set(i,tmpCurrent->y,tmpCurrent->value);
            tmpCurrent=tmpCurrent->next;
          }
          else if(tmpCurrent2 && (tmpCurrent2->y==j)){
            res.set(i,tmpCurrent2->y,-tmpCurrent2->value);
            tmpCurrent2=tmpCurrent2->next;
          }

        }

      }
      return res;
    }

    const Matrix<T> transpose() const{
      Matrix<T> res(columns,rows);
      Node<T>* tmp;
      int i;
      for(i=0;i<rows;i++){
        tmp=hrows[i]->next;
        while(tmp){
          res.set(tmp->getY(),tmp->getX(),tmp->getValue());
          tmp=tmp->next;
        }
      }
      return res;
    }

    void print() const{
      cout<<"-------------------------------------------------"<<endl;
      cout<<"Numero de Filas y Columnas "<<rows<<" "<<columns<<endl;
      Node<T> *tmpCurrent;
      unsigned lastj;
      for(int i=0;i<rows;i++){
        tmpCurrent=hrows[i]->next;
        lastj=0;
        while(lastj<columns){
          if(tmpCurrent){
            if(tmpCurrent->y==lastj){
              cout<<tmpCurrent->value<<"\t";
              tmpCurrent=tmpCurrent->next;
              lastj++;
              continue;
            }
          }
          lastj++;
          cout<<"0\t";
        }
        cout<<endl;
      }

      cout<<"Termino de imprimir"<<rows<<" "<<columns<<endl;
      cout<<"-------------------------------------------------"<<endl;
    }

    ~Matrix(){
      Node<T> *tmpCurrent;
      Node<T> *tmpAfter;
      for(int i=rows-1;i>=0;i--){
        tmpCurrent=hrows[i];
        hrows.pop_back();
        while(tmpCurrent){
          tmpAfter=tmpCurrent->next;
          delete tmpCurrent;
          tmpCurrent=tmpAfter;
        }
      }
    }
};

#endif //SPARSE_MATRIX_MATRIX_H
