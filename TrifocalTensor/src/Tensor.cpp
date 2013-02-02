#include "Tensor.hpp"

Tensor::Tensor(){

    T.resize(27);

    /* Initialize values with 0 */

    for (int i=0; i<27; i++){
        this->T(i) = 0;
    }
}


void Tensor::fillWith(Eigen::VectorXd t){
    this->T = t;
}


void Tensor::print(){

int cpt=0;

    std::cout << "*----- PRINT TENSOR -----*" << std::endl;

    std::cout << "Matrix 1" << std::endl;
    for (int i=0; i<9; i++){
        if(cpt<3){
            cpt++;
        }
        else {
            std::cout << std::endl;
            cpt = 0;
        }
        std::cout << this->T(i) << " ";
    }

    std::cout << "Matrix 2" << std::endl;
    for (int i=9; i<18; i++){
        if(cpt<3){
            cpt++;
        }
        else {
            std::cout << std::endl;
            cpt = 0;
        }
        std::cout << this->T(i) << " ";
    }

    std::cout << "Matrix 3" << std::endl;
    for (int i=18; i<27; i++){
        if(cpt<3){
            cpt++;
        }
        else {
            std::cout << std::endl;
            cpt = 0;
        }
        std::cout << this->T(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "*----- END PRINT -----*" << std::endl;

}


double& Tensor::operator()(int i, int j, int k){
    return this->T(9*k + 3*i + j);
}


Tensor::~Tensor(){

}