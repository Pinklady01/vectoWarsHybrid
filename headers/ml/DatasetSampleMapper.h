#pragma once

#include <ggponet.h>

namespace HybridGGPO
{
    /// <summary>
    /// Fixed-sized sample data mapper for dataset construction
    /// </summary>
    class DatasetSampleMapper
    {
    protected:
        double*					_data;
        size_t					_maxSize;
        int                     _offset;
        bool					_selfManagedData;

    protected:
        /// <summary>
        /// Adds a value to the sample
        /// </summary>
        /// <typeparam name="T">The type of the value</typeparam>
        /// <param name="value">The value to add</param>
        /// <returns>The current instance pointer</returns>
        template<class T>
        DatasetSampleMapper*	baseAdd(const T value);

        /// <summary>
        /// Adds a categorical value to the sample
        /// </summary>
        /// <typeparam name="T">The type of the value</typeparam>
        /// <param name="value">The categorical value, contained in the possible values array</param>
        /// <param name="possibleValues">All the possible values, must be size-consistent throughout the training</param>
        /// <param name="possibleValuesSize">Size of the array of possible values</param>
        /// <returns>The current instance pointer</returns>
        template<class T>
        DatasetSampleMapper*	baseAddCategorical(const T value, const T* possibleValues, size_t possibleValuesSize);

    public:
        /// <summary>
        /// Class constructor
        /// </summary>
        /// <param name="sampleSize">Sample max size</param>
        DatasetSampleMapper(const size_t sampleSize);

        /// <summary>
        /// Class constructor
        /// </summary>
        /// <param name="data">Array managed by the user, instanciated with the right size</param>
        /// <param name="sampleSize">Sample max size</param>
        DatasetSampleMapper(double* data, const size_t sampleSize);

        /// <summary>
        /// Copy constructor
        /// </summary>
        /// <param name="original">Base instance</param>
        DatasetSampleMapper(const DatasetSampleMapper& original);

        /// <summary>
        /// Copy assignment operator
        /// </summary>
        /// <param name="original">Base instance</param>
        DatasetSampleMapper& operator=(const DatasetSampleMapper& original);
        
        ~DatasetSampleMapper();

        DatasetSampleMapper*	add(const int value);
        DatasetSampleMapper*	add(const float value);
        DatasetSampleMapper*	add(const double value);
        DatasetSampleMapper*	add(const bool value);
        DatasetSampleMapper*	add(const long value);
        DatasetSampleMapper*	add(const char value);

        DatasetSampleMapper*	addCategorical(const int value, const int* possibleValues, size_t possibleValuesSize);
        DatasetSampleMapper*	addCategorical(const float value, const float* possibleValues, size_t possibleValuesSize);
        DatasetSampleMapper*	addCategorical(const double value, const double* possibleValues, size_t possibleValuesSize);
        DatasetSampleMapper*	addCategorical(const bool value, const bool* possibleValues, size_t possibleValuesSize);
        DatasetSampleMapper*	addCategorical(const long value, const long* possibleValues, size_t possibleValuesSize);
        DatasetSampleMapper*	addCategorical(const char value, const char* possibleValues, size_t possibleValuesSize);

        void					copy(const DatasetSampleMapper& original);

        /// <summary>
        /// Gets the content of the sample
        /// </summary>
        /// <param name="data">Array to fill with the sample's data</param>
        void                    getData(double* data) const;

        /// <summary>
        /// Gets the current size of the sample
        /// </summary>
        /// <returns>Size of the sample</returns>
        size_t                  getSize() const;

        /// <summary>
        /// Reinitializes the state of the mapper
        /// </summary>
        void                    reset();
    };
}