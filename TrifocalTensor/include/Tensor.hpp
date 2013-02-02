#ifndef __TENSOR_HPP__
#define __TENSOR_HPP__

#include <iostream>
#include <Eigen/Dense>

class Tensor {

private:
    Eigen::VectorXd T;

public:

    Tensor();
    ~Tensor();

    void fillWith(Eigen::VectorXd t);
    void print();

    double& operator()(int i, int j, int k);

};


#endif
