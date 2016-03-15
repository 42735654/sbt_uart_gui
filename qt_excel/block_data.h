#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

template <typename T> class block_data
{
public:
    T **data;
    int x, y;
public:
    block_data();
    block_data(int __x, int __y);
    ~block_data();
    T *const operator[](const int i);
};
template <typename T>
block_data<T>::block_data()
{

}

template <typename T>
block_data<T>::block_data(int __x, int __y){
    x = __x; y = __y;
    data = new T* [__y];
    data[0] = new T[__y * __x];
    for (int i = 1; i < __y; i++){
        data[i] = data[i - 1] +  __x;
    }
}

template <typename T>
block_data<T>::~block_data(){
    delete [] data[0];
    delete [] data;
}

template <typename T>
T * const block_data<T>::operator[](const int i){
    return data[i];
}
#endif // BLOCK_DATA_H
