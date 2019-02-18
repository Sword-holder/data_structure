//
//  table.h
//  myTable
//
//  Created by 肖子洋 on 2019/2/3.
//  Copyright © 2019年 肖子洋. All rights reserved.
//

#ifndef table_h
#define table_h

#include <vector>

template<typename T>
class Table {
private:
    unsigned rows, cols;
    T** values;
    
public:
    /********************** constructor **********************/
    Table();
    Table(unsigned rows, unsigned cols, const T& initial_value);
    Table(const Table<T>& table);
    ~Table();
    /********************** set & get **********************/
    T& get(unsigned row, unsigned col) const;
    void set(unsigned row, unsigned col, const T& value);
    unsigned numRows() const;
    unsigned numColumns() const;
    /********************** push & pop **********************/
    void push_back_row(const std::vector<T>& row);
    void push_back_column(const std::vector<T>& col);
    void pop_back_row();
    void pop_back_column();
    void push_back_rows(const Table<T>& table);
    void push_back_columns(const Table<T>& table);
    /********************** print **********************/
    void print() const;
    /**************** operator overloading *****************/
    Table<T>& operator =(const Table<T>& table);
    
private:
    /**************** auxiliary function *****************/
    void _deallocate();
    void _copy(const Table<T>& table);
    void _throw_error(std::string message, int code) const;
};


/*************************table.cpp******************************/
#include <iostream>
#include <climits>

template<typename T>
Table<T>::Table() {
    // By default, table is 0 * 0.
    rows = 0;
    cols = 0;
    values = new T*[0];
}

template<typename T>
Table<T>::Table(unsigned rows, unsigned cols, const T& initial_value):rows(rows), cols(cols) {
    // Dynamically allocate a grid
    values = new T*[rows];
    for (int i = 0; i < rows; i++) {
        // Allocate memory space for each row
        *(values + i) = new T[cols];
        for (int j = 0; j < cols; j++) {
            // Initialize elements with initial_value
            *(*(values + i) + j) = initial_value;
        }
    }
}

template <typename T>
Table<T>::Table(const Table<T>& table) {
    _copy(table);
}

template<typename T>
Table<T>::~Table() {
    // Free memory
    _deallocate();
}

template<typename T>
T& Table<T>::get(unsigned row, unsigned col) const {
    // If index out of bounds, throw the bad indices error
    if (row >= this->rows || col >= this->cols) {
        _throw_error("Bad indices!", 1);
    }
    // Else return the value of (row, col)
    return *(*(values + row) + col);
}

template<typename T>
void Table<T>::set(unsigned row, unsigned col, const T& value) {
    if (row >= this->rows || col >= this->cols) {
        _throw_error("Bad indices!", 1);
    }
    // Else write the value of (row, col)
    *(*(values + row) + col) = value;
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
    if (rows == UINT_MAX) {
         _throw_error("Table is full", 1);
    }
    if (row.size() != cols) {
        _throw_error("Size not match", 1);
    }
    
    // Reallocate a gird space, whose size is (rows + 1, cols)
    // The additional row is used to save the pushed row
    T** new_values = new T*[rows + 1];
    // Reuse the original n rows
    for (int i = 0; i < rows; i++) {
        *(new_values + i) = *(values + i);
    }
    // Allocate memory space for the last row, and assign with pushed row
    new_values[rows] = new T[cols];
    for (int j = 0; j < cols; j++) {
        *(*(new_values + rows) + j) = row[j];
    }
    // Deallocate the original values
    delete []values;
    // Assign new_values pointer for values pointer
    values = new_values;
    
    // Number of rows plus one
    rows++;
}

template<typename T>
void Table<T>::push_back_column(const std::vector<T>& col) {
    if (cols == UINT_MAX) {
        _throw_error("Table is full", 1);
    }
    if (col.size() != rows) {
        _throw_error("Size not match", 1);
    }
    
    for (int i = 0; i < rows; i++) {
        T* new_row = new T[cols + 1];
        for (int j = 0; j < cols; j++) {
            *(new_row + j) = *(*(values + i) + j);
        }
        *(new_row + cols) = col[i];
        delete []values[i];
        *(values + i) = new_row;
    }
    
    // Number of columns plus one
    cols++;
}

template<typename T>
void Table<T>::pop_back_row() {
    // If the table is empty, throw the table empty error
    if (rows == 0) {
        _throw_error("Cannot pop an empty table!", 1);
    }
    
    // Reallocate a space of n - 1 rows, and reuse the first n - 2 rows
    T** new_values = new T*[rows - 1];
    for (int i = 0; i < rows - 1; i++) {
        *(new_values + i) = *(values + i);
    }
    // Delete the last rows
    delete []values[rows - 1];
    // Delete the original values and let it point to new_values
    delete []values;
    values = new_values;
    
    // Number of rows minus one
    rows--;
}

template<typename T>
void Table<T>::pop_back_column() {
    if (cols == 0) {
        _throw_error("Cannot pop an empty table!", 1);
    }
    
    // For each row, save the first n - 1 cols, then delete the original array
    for (int i = 0; i < rows; i++) {
        T* new_row = new T[cols - 1];
        for (int j = 0; j < cols - 1; j++) {
            *(new_row + j) = *(*(values + i) + j);
        }
        delete []values[i];
        *(values + i) = new_row;
    }
    
    // Number of columns minus one
    cols--;
}

template<typename T>
void Table<T>::push_back_rows(const Table<T>& table) {
    if (rows + table.numRows() > UINT_MAX) {
        _throw_error("Table is full", 1);
    }
    if (table.numColumns() != cols) {
        _throw_error("Size not match", 1);
    }
    
    // Add multiple rows
    T** new_values = new T*[rows + table.numRows()];
    // Reuse the first n rows
    for (int i = 0; i < rows; i++) {
        *(new_values + i) = *(values + i);
    }
    // Copy the content of input table
    for (int i = 0; i < table.numRows(); i++) {
        *(new_values + rows + i) = new T[cols];
        for (int j = 0; j < cols; j++) {
            *(*(new_values + rows + i) + j) = table.get(i, j);
        }
    }
    // Delete original values
    delete []values;
    // Let it points to new_values
    values = new_values;
    
    // Update the value of rows
    rows += table.numRows();
}

template<typename T>
void Table<T>::push_back_columns(const Table<T>& table) {
    if (cols + table.numColumns() > UINT_MAX) {
        _throw_error("Table is full", 1);
    }
    if (table.numRows() != rows) {
        _throw_error("Size not match", 1);
    }
    
    // For each row, Delete the original row and append content of input table.
    for (int i = 0; i < rows; i++) {
        T* new_row = new T[cols + table.numColumns()];
        for (int j = 0; j < cols; j++) {
            *(new_row + j) = *(*(values + i) + j);
        }
        for (int j = 0; j < table.numColumns(); j++) {
            *(new_row + cols + j) = table.get(i, j);
        }
        delete []values[i];
        *(values + i) = new_row;
    }
    
    // Update the value of cols
    cols += table.numColumns();
}

template<typename T>
void Table<T>::print() const {
    std::cout << "table: " << rows << " rows, " << cols << " cols" << std::endl;
//    std::cout << "Address == " << this <<std::endl;
    // Traverse each row
    for (int i = 0; i < rows; i++) {
//        std::cout << "Address" << values + i << ": ";
        // For each row, print the value of every element
        for (int j = 0; j < cols; j++) {
            std::cout << *(*(values + i) + j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename T>
Table<T>& Table<T>::operator =(const Table<T>& table) {
//    std::cout << "operator = " << std::endl;
    // If this pointer is the same object as table,
    // It will breakdown when deleted and be referenced again
    // So we are supposed to add if statement as follow
    if (this != &table) {
        // Free memory
        _deallocate();
        // Copy the values of table
        _copy(table);
    }
    return *this;
}



template<typename T>
void Table<T>::_deallocate() {
    /*
     * This function will free the memory space of table
     * It will be called in deconstructor
     * The commented statements are used to memory debug
     */
    //    std::cout << "Now deconstruct object :" << this << ": rows == " << rows << ", cols == " << cols << std::endl;
    for (int i = 0; i < rows; i++) {
        //        std::cout << "i == " << i << ", release object: " << values + i << std::endl;
        if (values && values[i]) {
            delete []values[i];
            *(values + i) = NULL;
        }
    }
    if (values) {
        //        std::cout << ", release object: " << values << std::endl;
        delete []values;
        values = NULL;
    }
}

template<typename T>
void Table<T>::_copy(const Table<T>& table) {
    /*
     * This function is used to deep copy
     * Input is a reference of table
     * It will allocate new space for values and clone the value of input table
     * Make sure deallocate original memory before calling this function
     */
    // Copy table size
    rows = table.numRows();
    cols = table.numColumns();
    // Copy contents of table
    // Use deep copy instead of shallow copy(reference)
    values = new T*[rows];
    for (int i = 0; i < rows; i++) {
        *(values + i) = new T[cols];
        for (int j = 0; j < cols; j++) {
            *(*(values + i) + j) = table.get(i, j);
        }
    }
}

template<typename T>
void Table<T>::_throw_error(std::string message, int code) const {
    // Output the error message to standard error device and exit with input code
    std::cerr << message << std::endl;
    exit(code);
}

#endif /* table_h */
