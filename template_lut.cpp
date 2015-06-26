#include <array>
#include <iostream>

static constexpr size_t SIZE = 10;
static constexpr size_t SIZE2 = 5;

template<typename T>
class TypeArgClass {
public:
    TypeArgClass(){};
private:
    T x;
};

template<size_t Size>
class NonTypeArgClass {
public:
    NonTypeArgClass(){
        for (int i = 0; i < Size; ++i) {
            x[i] = 10*i;
        }
        std::cout << " Created" << std::endl;
    };
    int lookup(int i) {
        return x[i];
    }
    std::array<int, Size> x;
};



class Worker{
public:
    Worker()
    :m_table_B(nullptr)
    {};
    void set_table( NonTypeArgClass<SIZE>* table_B)
    {
        m_table_B = table_B;
    }
//private:
    TypeArgClass<int> x;
    NonTypeArgClass<SIZE>* m_table_B;
};


int main()
{
    NonTypeArgClass<SIZE> table_B;
    for ( size_t i =0; i < SIZE; ++i){
        std::cout << table_B.x[i] << " ";
    }
    std::cout << std::endl;


    Worker jim;
    Worker joe;
    joe.set_table(&table_B);

    if (jim.m_table_B != nullptr) {
        std::cout << "Jim: " ;
        for ( size_t i = 0; i < SIZE; ++i){
            std::cout << jim.m_table_B->lookup(i) << " ";
        }
        std::cout << std::endl;
    }

    if (joe.m_table_B != nullptr) {
        std::cout << "Joe: " ;
        for ( size_t i = 0; i < SIZE; ++i){
            std::cout << joe.m_table_B->lookup(i) << " ";
        }
        std::cout << std::endl;
    }
}

