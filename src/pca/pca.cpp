#include "pca.h"
#include <cassert>
#include <iostream>

using namespace Eigen;
using namespace std;

bool PCA::performPCA(std::vector<Eigen::VectorXd>& points, MatrixXd& rotationMatrix){
    return performPCA(points,0,points.size(),rotationMatrix);
}

bool PCA::performPCA(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, MatrixXd& rotationMatrix){
    int numPoints = endIndex - beginIndex;
    if(numPoints == 0)
        return false;

    VectorXd& firstPoints = points.at(beginIndex);
    int numDimensions = firstPoints.size();

    MatrixXd mat(numPoints,numDimensions);

    for(int i = beginIndex ; i < endIndex ; ++i){
        VectorXd& point = points.at(i);
        mat.row(i - beginIndex) = point;
//        for(int j = 0 ; j < numDimensions ; ++j){
//            mat(i,j) = point[j];
//        }
    }

    return performPCA(mat,rotationMatrix);
}


bool PCA::performPCA(const Eigen::MatrixXd& mat, MatrixXd& rotationMatrix){
    //
    MatrixXd centered = mat.rowwise() - mat.colwise().mean();
    MatrixXd cov = (centered.adjoint() * centered) / double(mat.rows() - 1);
    SelfAdjointEigenSolver<MatrixXd> eigensolver(cov);
    if (eigensolver.info() != Success){
//        cout << "MAT" << endl << mat;
//        cout << "COV" << endl << cov;
        //abort();
        return false;
    }
//    cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
//    cout << "Here's a matrix whose columns are eigenvectors of A \n"
//         << "corresponding to these eigenvalues:\n"
//         << eigensolver.eigenvectors() << endl;
    rotationMatrix = eigensolver.eigenvectors().rowwise().reverse(); // make coordinates sorted in decreasing order of variance
    return true;
}


void testPCA(){
    //srand(time(NULL));
    MatrixXd mat(5,3);// = MatrixXd::Random(3,3);
    mat(0,0) = 4.0;
    mat(0,1) = 2.0;
    mat(0,2) = 0.60;
    mat(1,0) = 4.2;
    mat(1,1) = 2.1;
    mat(1,2) = 0.59;
    mat(2,0) = 3.9;
    mat(2,1) = 2.0;
    mat(2,2) = 0.58;
    mat(3,0) =  4.3;
    mat(3,1) = 2.1;
    mat(3,2) = 0.62;
    mat(4,0) = 4.1;
    mat(4,1) = 2.2;
    mat(4,2) = 0.63;


    cout << "Matrix A:" << endl;
    cout << mat << endl;
    cout << "-------   " << mat.colwise().mean() << endl;

    MatrixXd centered = mat.rowwise() - mat.colwise().mean();
    cout << "Centered A:" << endl;
    cout << centered << endl;
    cout << endl;

    //
    MatrixXd cov = (centered.adjoint() * centered) / double(mat.rows() - 1);

    cout << "Covariance Matrix of A:" << endl;
    cout << cov << endl;
    cout << endl;

    //
    SelfAdjointEigenSolver<MatrixXd> eigensolver(cov);
    if (eigensolver.info() != Success) abort();
    VectorXd eigenValues = eigensolver.eigenvalues();
    cout << "The eigenvalues of A are:\n" << eigenValues << endl;
    cout << "Here's a matrix whose columns are eigenvectors of A \n"
         << "corresponding to these eigenvalues:\n"
         << eigensolver.eigenvectors() << endl;

    int numValues = eigenValues.size();
    std::vector<pair<double,int> > indices;
    for(int i = 0 ; i < numValues ; ++i){
        indices.push_back(make_pair(eigenValues[i],i));
    }

    MatrixXd rotationMatrix;
    PCA::performPCA(mat,rotationMatrix);

    cout << "Rotation Matrix" << endl;
    cout << rotationMatrix << endl;
    cout << endl;
}
