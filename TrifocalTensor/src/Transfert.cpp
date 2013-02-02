#include "Transfert.hpp"


Eigen::VectorXd transfertTo1(Eigen::Vector3d point2, Eigen::Vector3d point3, Tensor T){

    /* Objective : solve Ax = 0 */

    Eigen::MatrixXd A = MatrixXd::Zero(4, 3);
    Eigen::VectorXd b = VectorXd::Zero(4);

    /* Filling Matrix A and Vector b */
    for(int i=0; i<2; ++i){
        for(int l=0; l<2 ; ++l){
            for(int k=0; k<3; ++k){
                A(2*i+l, k) = point2(i)*T(2,l,k)-T(i,l,k)-point2(i)*point3(l)*T(2,2,k)+point3(l)*T(i,2,k);
            }
        }
    }

    /* Using SVD method */

    Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
    Eigen::MatrixXd V = svd.matrixV();
    Eigen::VectorXd newPoint = V.col(2);
    /* Normalize */
    for (int i=0; i<3; ++i){
        newPoint(i) = newPoint(i)/newPoint(2);
    }

    return newPoint;
}

Eigen::VectorXd transfertTo2(Eigen::Vector3d point1, Eigen::Vector3d point3, Tensor T){

    /* Objective : solve Ax = b */

    Eigen::MatrixXd A = MatrixXd::Zero(4, 2);
    Eigen::VectorXd b = VectorXd::Zero(4);

    /* Filling Matrix A and Vector b */

    for(int i=0; i<2; ++i){
        for(int l=0; l<2 ; ++l){
            for(int k=0; k<3; ++k){
                A(2*i+l, i) += point1(k)*T(2,l,k) - point1(k)*point3(l)*T(2,2,k);
                b(2*i+l) += point1(k)*T(i,l,k) - point1(k)*point3(l)*T(i,2,k);
            }
        }
    }

    /* Using SVD method */

    Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
    Eigen::VectorXd newPoint = svd.solve(b);

    return newPoint;
}

Eigen::VectorXd transfertTo3(Eigen::Vector3d point1, Eigen::Vector3d point2, Tensor T){

    /* Objective : solve Ax = b */

    Eigen::MatrixXd A = MatrixXd::Zero(4, 2);
    Eigen::VectorXd b = VectorXd::Zero(4);

    /* Filling Matrix A and Vector b */
    for(int i=0; i<2; ++i){
        for(int l=0; l<2 ; ++l){
            for(int k=0; k<3; ++k){
                A(2*i+l, l) += point1(k)*T(i,2,k) - point1(k)*point2(i)*T(2,2,k);
                b(2*i+l) += point1(k)*T(i,l,k) - point1(k)*point2(i)*T(2,l,k);
            }
        }
    }

    /* Using SVD method */

    Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
    Eigen::VectorXd newPoint = svd.solve(b);

    return newPoint;
}