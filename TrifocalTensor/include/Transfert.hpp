#ifndef __TRANSFERT_HPP__
#define __TRANSFERT_HPP__

#include <iostream>
#include <Eigen/SVD>

#include "MathIO.hpp"
#include "Tensor.hpp"

Eigen::VectorXd transfertTo1(Eigen::Vector3d point2, Eigen::Vector3d point3, Tensor T);
Eigen::VectorXd transfertTo2(Eigen::Vector3d point1, Eigen::Vector3d point3, Tensor T);
Eigen::VectorXd transfertTo3(Eigen::Vector3d point1, Eigen::Vector3d point2, Tensor T);

#endif