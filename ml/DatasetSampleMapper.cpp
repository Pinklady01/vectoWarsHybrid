#include "../headers/ml/DatasetSampleMapper.h"

#include <memory>

using namespace HybridGGPO;

DatasetSampleMapper::DatasetSampleMapper(const size_t sampleSize):
    _data(new double[sampleSize]),
    _maxSize(sampleSize),
    _offset(0),
    _selfManagedData(true)
{
}

DatasetSampleMapper::DatasetSampleMapper(double* data, const size_t sampleSize):
    _data(data),
    _maxSize(sampleSize),
    _offset(0),
    _selfManagedData(false)
{
}

DatasetSampleMapper::DatasetSampleMapper(const DatasetSampleMapper& original)
{
    this->copy(original);
}

DatasetSampleMapper& DatasetSampleMapper::operator=(const DatasetSampleMapper& original)
{
    this->copy(original);
    return *this;
}

DatasetSampleMapper::~DatasetSampleMapper()
{
    if (this->_selfManagedData)
        delete[] this->_data;
}

template<class T>
DatasetSampleMapper* DatasetSampleMapper::baseAdd(const T value)
{
    this->_data[this->_offset] = (double)value;
    ++this->_offset;

    return this;
}

template<class T>
DatasetSampleMapper* DatasetSampleMapper::baseAddCategorical(const T value, const T* possibleValues, const size_t possibleValuesSize)
{
    for (int i = 0; i < possibleValuesSize; ++i)
        this->_data[this->_offset + i] = possibleValues[i] == value ? 1 : 0;
    this->_offset += (int)possibleValuesSize;

    return this;
}

DatasetSampleMapper* DatasetSampleMapper::add(const int value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::add(const float value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::add(const double value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::add(const bool value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::add(const long value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::add(const char value)
{
    return this->baseAdd(value);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const int value, const int* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const float value, const float* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const double value, const double* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const bool value, const bool* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const long value, const long* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

DatasetSampleMapper* DatasetSampleMapper::addCategorical(const char value, const char* possibleValues, const size_t possibleValuesSize)
{
    return this->baseAddCategorical(value, possibleValues, possibleValuesSize);
}

void DatasetSampleMapper::copy(const DatasetSampleMapper& original)
{
    if (this->_data && this->_selfManagedData)
        delete[] this->_data;

    this->_data = new double[original._maxSize];
    this->_maxSize = original._maxSize;
    this->_offset = original._offset;
    this->_selfManagedData = true;

    std::copy(original._data, original._data + original._maxSize, this->_data);
}

void DatasetSampleMapper::getData(double* data) const
{
    std::copy(this->_data, this->_data + this->_offset, data);
}

size_t DatasetSampleMapper::getSize() const
{
    return this->_offset;
}

void DatasetSampleMapper::reset()
{
    this->_offset = 0;
}
