#pragma once

#include "../../Atlus/src/headers/core.h"

#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace HybridGGPO
{
    /// <summary>
    /// Class wrapping the Atlus library's MLP model
    /// </summary>
    class MLPModelWrapper
    {
    private:
        uint            _inputDim = 0;
        MLPData*        _model = NULL;
        uint            _outputDim = 0;

    public:
        /// <summary>
        /// Flattens a sample collection
        /// </summary>
        /// <param name="samples">Matrix 2xN of samples</param>
        /// <returns>A flattened array of samples</returns>
        static double*  flatSamples(const m2& samples);

    public:
        explicit MLPModelWrapper() = default;

        /// <summary>
        /// Explicit class constructor using model's weight file
        /// </summary>
        /// <param name="path">Model's data file path</param>
        explicit MLPModelWrapper(const char* path);

        /// <summary>
        /// Explicit class constructor using neurons per layer description
        /// </summary>
        /// <param name="npl">Neurons per layer</param>
        explicit MLPModelWrapper(std::vector<uint>& npl);

        ~MLPModelWrapper();

        /// <summary>
        /// Evaluates model's accuracy
        /// </summary>
        /// <param name="inputs">Sample inputs</param>
        /// <param name="outputs">Sample expected outputs</param>
        /// <returns>The accuracy rate between 0 and 100</returns>
        double          evaluate(const m2& inputs, const m2& outputs) const;

        /// <summary>
        /// Gets the current input dimension
        /// </summary>
        /// <returns>Size of the input layer</returns>
        uint            getInputDim() const;

        /// <summary>
        /// Gets the current output dimension
        /// </summary>
        /// <returns>Size of the output layer</returns>
        uint            getOutputDim() const;

        /// <summary>
        /// Evaluates model's accuracy
        /// </summary>
        /// <param name="inputs">Sample inputs in a flattened array</param>
        /// <param name="outputs">Sample expected outputs in a flattened array</param>
        /// <param name="sampleCount">Number of samples in the flattened arrays</param>
        /// <returns>The accuracy rate between 0 and 100</returns>
        double          evaluate(const double inputs[], const double outputs[], const uint sampleCount) const;

        /// <summary>
        /// Predicts an output based on the given input and fills the prediction array
        /// </summary>
        /// <param name="input">Input vector to predict an output from</param>
        /// <param name="prediction">Out parameter for the prediction prediction</param>
        void            predict(const m1& input, double prediction[]) const;

        /// <summary>
        /// Predicts an output based on the given input and fills the prediction array
        /// </summary>
        /// <param name="input">Input array to predict an output from, the array's size should be equal or greater than the output layer's dimension of the model</param>
        /// <param name="prediction">Out parameter for the prediction</param>
        void            predict(const double input[], double prediction[]) const;

        /// <summary>
        /// Predicts an output based on the given input
        /// </summary>
        /// <param name="input">Input vector to predict an output from</param>
        /// <returns>The prediction</returns>
        m1              predict(const m1& input) const;

        /// <summary>
        /// Predicts an output based on the given input
        /// </summary>
        /// <param name="input">Input array to predict an output from</param>
        /// <returns>The prediction</returns>
        m1              predict(const double input[]) const;

        /// <summary>
        /// Saves the model at the given path
        /// </summary>
        /// <param name="path">File path</param>
        void            save(const char* path) const;

        /// <summary>
        /// Trains the model on multiple samples
        /// </summary>
        /// <param name="inputs">Sample inputs</param>
        /// <param name="outputs">Sample expected outputs</param>
        /// <param name="alpha">Learning rate</param>
        /// <param name="epochs">Training iterations</param>
        void            train(const m2& inputs, const m2& outputs, const double alpha, const uint epochs) const;

        /// <summary>
        /// Trains the model on multiple samples
        /// </summary>
        /// <param name="inputs">Sample inputs in a flattened array</param>
        /// <param name="outputs">Sample expected outputs in a flattened array</param>
        /// <param name="sampleCount">Number of samples in the flattened arrays</param>
        /// <param name="alpha">Learning rate</param>
        /// <param name="epochs">Training iterations</param>
        void            train(const double inputs[], const double outputs[], const uint sampleCount, const double alpha, const uint epochs) const;

        /// <summary>
        /// Trains the model on a unique sample
        /// </summary>
        /// <param name="input">Sample input</param>
        /// <param name="output">Sample expected output</param>
        /// <param name="alpha">Learning rate</param>
        void            trainSingle(const m1& input, const m1& output, const double alpha) const;

        /// <summary>
        /// Trains the model on a unique sample
        /// </summary>
        /// <param name="input">Sample input</param>
        /// <param name="output">Sample expected output</param>
        /// <param name="alpha">Learning rate</param>
        void            trainSingle(const double input[], const double output[], const double alpha) const;
    };
}