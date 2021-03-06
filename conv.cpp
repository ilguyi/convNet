#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <boost/random.hpp>
#include <armadillo>
#include "dataframe.h"
#include "conv.h"
using namespace std;
using namespace df;
using namespace conv;



int main (int argc, char** argv)
{
    if ( argc != 4 ) {
        cout << "Usage: conv training_data test_data parameters_data" << endl;
        exit(1);
    }

    //  Reading Parameters file
    NeuralNetworks nn;
    nn.ReadParameters(argv[3]);
    nn.PrintParameters();

    unsigned N = nn.GetN_train() + nn.GetN_valid();
    unsigned dimension = nn.GetDimension();


    //  Reading training data
    DataFrame<unsigned> trainMNIST;
//  Usage: ReadDataFile(filename, N, dimension, header, target)
    trainMNIST.ReadDataFile(argv[1], N, dimension, "False", "True");
    cout << "Read training data complete" << endl;
//  trainMNIST.PrintData();

    //  Set target data
    arma::uvec target_class;
    target_class << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    if ( target_class.size() != nn.GetN_class() ) {
        cout << "Usage: number of class is NOT the same" << endl;
        exit(1);
    }
    trainMNIST.SetTargetMatrix(target_class, df::Binary);
    cout << "Target Matrix complete" << endl;
//  trainMNIST.PrintTarget();
//  trainMNIST.PrintTargetMatrix();


    //  Randomly extract validation data
    DataFrame<unsigned> validMNIST;
    trainMNIST.SplitValidationSet(validMNIST, nn.GetN_valid());
    cout << "Split validation data complete" << endl;


    //  Reading test data
    DataFrame<unsigned> testMNIST;
    unsigned N_test = nn.GetN_test();
    testMNIST.ReadDataFile(argv[2], N_test, dimension, "False", "True");
    cout << "Read test data complete" << endl;
//  testMNIST.PrintData();


    //  Set linear scaling each features
    DataFrame<double> trainMNIST_norm, validMNIST_norm, testMNIST_norm;
//    trainMNIST.LinearScalingEachFeatures(validMNIST, testMNIST, trainMNIST_norm, validMNIST_norm, testMNIST_norm);
    trainMNIST.NormalizationEachFeatures(validMNIST, testMNIST, trainMNIST_norm, validMNIST_norm, testMNIST_norm);
    cout << "Linear scaling all data complete" << endl << endl;
//    trainMNIST_norm.PrintData();
//    validMNIST_norm.PrintData();
//    testMNIST_norm.PrintData();



//  Execute NeuralNetworks
    nn.Initialize();

    for (unsigned iter=0; iter<100; iter++) {

//        nn.Training(trainMNIST_norm, iter);
        nn.Training(trainMNIST_norm, validMNIST_norm, iter);

        arma::uvec predict(N_test);
        predict.zeros();

//        nn.Test(testMNIST_norm, predict, iter);
        nn.TestwithAccuracy(testMNIST_norm, predict, iter);
    }

    return 0;
}

