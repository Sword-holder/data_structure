//
//  table.cpp
//  myTable
//
//  Created by 肖子洋 on 2019/2/3.
//  Copyright © 2019年 肖子洋. All rights reserved.
//

#include <iostream>
#include "table.h"


template<typename T>
Table<T>::Table(unsigned rows, unsigned cols, const T& initial_value):rows(rows), cols(cols) {
    values = new T*[rows];
    for (int i = 0; i < rows; i++) {
        values[i] = new T[cols];
        for (int j = 0; j < cols; j++) {
            values[i][j] = initial_value;
        }
    }
}

template<typename T>
T& Table<T>::get(unsigned row, unsigned col) const {
    if (row >= this->rows || col >= this->cols) {
        std::cerr << "Bad indices!" << std::endl;
        exit(1);
    }
    return values[row][col];
}

template<typename T>
void Table<T>::set(unsigned row, unsigned col, const T& value) {
    if (row >= this->rows || col >= this->cols) {
        std::cerr << "Bad indices!" << std::endl;
        exit(1);
    }
    values[row][col] = value;
}

template<typename T>
unsigned Table<T>::numRows() const {
    return rows;
}

template<typename T>
unsigned Table<T>::numColumns() const {
    return cols;
}

template<typename T>
void Table<T>::push_back_row(const std::vector<T>& row) {
    T** new_values = new T*[rows + 1];
    for (int i = 0; i < rows; i++) {
        new_values[i] = values[i];
    }
    for (int j = 0; j < cols; j++) {
        new_values[rows][j] = rows[j];
    }
    delete []values;
    values = new_values;
    
    rows++;
}

template<typename T>
void Table<T>::push_back_column(const std::vector<T>& col) {
    for (int i = 0; i < rows; i++) {
        T* new_row = new T[cols + 1];
        for (int j = 0; j < cols; j++) {
            new_row[j] = values[i][j];
        }
        new_row[cols] = col[i];
        delete []values[i];
        values[i] = new_row;
    }
    
    cols++;
}

template<typename T>
void Table<T>::pop_back_row() {
    if (rows == 0) {
        std::cerr << "Cannot pop an empty table!" << std::endl;
        exit(1);
    }
    
    T** new_values = new T*[rows - 1];
    for (int i = 0; i < rows - 1; i++) {
        new_values[i] = values[i];
    }
    delete []values[rows];
    delete []values;
    values = new_values;
    
    rows--;
}

template<typename T>
void Table<T>::pop_back_column() {
    if (cols == 0) {
        std::cerr << "Cannot pop an empty table!" << std::endl;
        exit(1);
    }
    
    for (int i = 0; i < rows; i++) {
        T* new_row = new T[cols - 1];
        for (int j = 0; j < cols - 1; j++) {
            new_row[j] = values[i][j];
        }
        delete []values[i];
        values[i] = new_row;
    }
    
    cols--;
}

template<typename T>
void Table<T>::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << values[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void Table<T>::deallocate() {
    for (int i = 0; i < rows; i++) {
        delete []values[i];
    }
    delete []values;
}

template<typename T>
Table<T>::~Table() {
    deallocate();
}
