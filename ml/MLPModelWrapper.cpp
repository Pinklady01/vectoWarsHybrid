#include "../headers/ml/MLPModelWrapper.h"

using namespace HybridGGPO;

double* MLPModelWrapper::flatSamples(const m2& samples)
{
    uint sampleCount = (uint)samples.size();
    uint sampleDim = (uint)samples[0].size();
    auto* data = new double[sampleCount * sampleDim];

    for (uint i = 0; i < sampleCount; ++i)
        std::copy(samples[i].begin(), samples[i].end(), data + sampleDim * i);

    return data;
}

MLPModelWrapper::MLPModelWrapper(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();

    file.close();

    this->_model = deserializeModel(buffer.str().c_str());
    this->_inputDim = this->_model->npl[0];
    this->_outputDim = this->_model->npl[this->_model->L];
}

MLPModelWrapper::MLPModelWrapper(std::vector<uint>& npl)
{
    this->_model = createMlpModel(npl.data(), (uint)npl.size());
    this->_inputDim = this->_model->npl[0];
    this->_outputDim = this->_model->npl[this->_model->L];
}

MLPModelWrapper::~MLPModelWrapper()
{
    destroyMlpModel(this->_model);
}

double MLPModelWrapper::evaluate(const m2& inputs, const m2& outputs) const
{
    const double* inputArr = flatSamples(inputs);
    const double* outputArr = flatSamples(outputs);
    const double result = evaluateModelAccuracyClassification(this->_model, inputArr, outputArr, (uint)inputs.size(), this->_inputDim, this->_outputDim) * 100;

    delete[] inputArr;
    delete[] outputArr;

    return result;
}


double MLPModelWrapper::evaluate(const double inputs[], const double outputs[], const uint sampleCount) const
{
    return evaluateModelAccuracyClassification(this->_model, inputs, outputs, sampleCount, this->_inputDim, this->_outputDim) * 100;
}

uint MLPModelWrapper::getInputDim() const
{
    return this->_inputDim;
}

uint MLPModelWrapper::getOutputDim() const
{
    return this->_outputDim;
}

void MLPModelWrapper::predict(const double input[], double prediction[]) const
{
    input;
    prediction;
    // TD: Implement predictMlpModelClassification taking an out parameter array
    // predictMlpModelClassification(this->_model, input, prediction);
}

void MLPModelWrapper::predict(const m1& input, double prediction[]) const
{
    input;
    prediction;
    // predictMlpModelClassification(this->_model, input.data(), prediction);
}

m1 MLPModelWrapper::predict(const m1& input) const
{
    const double* resultArr = predictMlpModelClassification(this->_model, input.data());
    m1 result;

    result.reserve(this->_outputDim);

    std::copy(resultArr, resultArr + this->_outputDim, std::back_inserter(result));

    destroyMlpResult(resultArr);

    return result;
}

m1 MLPModelWrapper::predict(const double input[]) const
{
    const double* resultArr = predictMlpModelClassification(this->_model, input);
    m1 result;

    result.reserve(this->_outputDim);

    std::copy(resultArr, resultArr + this->_outputDim, std::back_inserter(result));

    destroyMlpResult(resultArr);

    return result;
}

void MLPModelWrapper::save(const char* path) const
{
    std::ofstream file(path);

    const char* data = serializeModel(this->_model);

    file << data;

    destroyMlpSerializedData(data);

    file.close();
}

void MLPModelWrapper::train(const m2& inputs, const m2& outputs, const double alpha, const uint epochs) const
{
    const double* inputArr = flatSamples(inputs);
    const double* outputArr = flatSamples(outputs);

    trainMlpModelClassification(this->_model, inputArr, outputArr, (uint)inputs.size(), this->_inputDim, this->_outputDim, alpha, epochs);

    delete[] inputArr;
    delete[] outputArr;
}

void MLPModelWrapper::train(const double inputs[], const double outputs[], const uint sampleCount, const double alpha, const uint epochs) const
{
    trainMlpModelClassification(this->_model, inputs, outputs, sampleCount, this->_inputDim, this->_outputDim, alpha, epochs);
}

void MLPModelWrapper::trainSingle(const m1& input, const m1& output, const double alpha) const
{
    trainMlpModelClassificationSingle(this->_model, input.data(), output.data(), alpha);
}

void MLPModelWrapper::trainSingle(const double input[], const double output[], const double alpha) const
{
    trainMlpModelClassificationSingle(this->_model, input, output, alpha);
}
