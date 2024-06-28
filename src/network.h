#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>

#include <omp.h>

extern int maxThreads;

std::vector<float> matrixVectorMultiply(const std::vector<std::vector<float>>& mat, const std::vector<float>& vec)
{
    std::vector<float> res;
    res.resize(mat.size());

    size_t vs = vec.size();

    for(unsigned int row = 0; row < mat.size(); ++row)
    {
        if(vs != mat[row].size())
        {
            std::cerr << "Encountered dimension error during matrix multiplication!\n";
            return std::vector<float>();
        }

        res[row] = 0.0f;

        for(unsigned int i = 0; i < vs; ++i)
            res[row] += mat[row][i]*vec[i];
    }

    return res;
}

std::vector<float> transposedMatrixVectorMultiply(const std::vector<std::vector<float>>& mat, const std::vector<float>& vec)
{
    std::vector<float> res;
    res.resize(mat[0].size());

    size_t vs = vec.size();

    for(unsigned int row = 0; row < mat[0].size(); ++row)
    {
        if(vs != mat.size())
        {
            std::cerr << "Encountered dimension error during transposed matrix multiplication!\n";
            return std::vector<float>();
        }

        res[row] = 0.0f;

        for(unsigned int i = 0; i < vs; ++i)
            res[row] += mat[i][row]*vec[i];
    }

    return res;
}

void vectorAddToSelf(std::vector<float>& a, const std::vector<float>& b)
{
    if(a.size() != b.size())
    {
        std::cerr << "Encountered dimension error during vector addition\n";
        return;
    }

    for(unsigned int i = 0; i < a.size(); ++i)
        a[i] += b[i];
}

void vectorElementMultiplyBySelf(std::vector<float>& a, const std::vector<float>& b)
{
    if(a.size() != b.size())
    {
        std::cerr << "Encountered dimension error during vector multiplication\n";
        return;
    }

    for(unsigned int i = 0; i < a.size(); ++i)
        a[i] *= b[i];
}

std::vector<float> activationFunction(const std::vector<float>& a)
{
    std::vector<float> res;
    res.resize(a.size());

    // sigmoid
    for(unsigned int i = 0; i < a.size(); ++i)
        res[i] = 1.0f/(1.0f+std::exp(-a[i]));

    return res;
}

std::vector<float> activationFunctionPrime(const std::vector<float>& z)
{
    // sigmoid
    std::vector<float> actv = activationFunction(z);
    std::vector<float> tmp = actv;
    for(unsigned int i = 0; i < tmp.size(); ++i)
        tmp[i] = 1.0f - tmp[i];

    vectorElementMultiplyBySelf(actv, tmp);
    return actv;

}

#ifdef _WIN32
void cpyDataFromResource(void* dest, const size_t size, const HGLOBAL& fdRp, size_t& offset)
{
    memcpy(dest, reinterpret_cast<unsigned char*>(LockResource(fdRp))+offset, size);
    offset += size;
}
#endif // _WIN32

struct Network
{
    unsigned int numLayers;
    std::vector<unsigned int> sizes;

    std::vector<std::vector<float>> biases; // bias = biases[layer][neuron]  with input layer not counted
    std::vector<std::vector<std::vector<float>>> weights; // weight = weights[layer][neuron in layer][neuron in layer-1]  with input layer not counted

    Network(){}

    Network(const std::vector<unsigned int> Sizes)
    {
        // initialize PRNG with mu = 0 and sigma = 1
        std::default_random_engine generator;
        std::normal_distribution<double> nDist(0.0, 1.0);

        this->biases.resize(Sizes.size()-1);
        this->weights.resize(Sizes.size()-1);

        this->sizes = Sizes;
        this->numLayers = this->sizes.size();

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            this->biases[l].resize(this->sizes[l+1]);
            this->weights[l].resize(this->sizes[l+1]);

            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                this->biases[l][n] = nDist(generator);

                this->weights[l][n].resize(this->sizes[l]);

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                {
                    std::normal_distribution<double> distribution(0.0, 1.0/sqrt((float)this->sizes[l]));
                    this->weights[l][n][pn] = distribution(generator);
                }
            }
        }
    }

    void saveToCompactBinaryFile(const std::string fName)
    {
        std::ofstream f(fName, std::ios::binary);

        if(!f.good())
        {
            std::cerr << "Failed to open " << fName << '\n';
            return;
        }

        f.write(reinterpret_cast<const char*>(&this->numLayers), sizeof(unsigned int));
        f.write(reinterpret_cast<const char*>(this->sizes.data()), sizeof(unsigned int)*this->sizes.size());

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                f.write(reinterpret_cast<const char*>(&this->biases[l][n]), sizeof(float));

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    f.write(reinterpret_cast<const char*>(&this->weights[l][n][pn]), sizeof(float));
            }
        }

        f.close();
    }

    void loadFromCompactBinaryFile(const std::string fName)
    {
        std::ifstream f(fName, std::ios::binary);

        if(!f.good())
        {
            std::cerr << "Failed to open " << fName << '\n';
            return;
        }

        f.read(reinterpret_cast<char*>(&this->numLayers), sizeof(unsigned int));
        this->sizes.resize(this->numLayers);
        f.read(reinterpret_cast<char*>(this->sizes.data()), sizeof(unsigned int)*this->sizes.size());

        this->biases.resize(this->sizes.size()-1);
        this->weights.resize(this->sizes.size()-1);

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            this->biases[l].resize(this->sizes[l+1]);
            this->weights[l].resize(this->sizes[l+1]);

            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                f.read(reinterpret_cast<char*>(&this->biases[l][n]), sizeof(float));

                this->weights[l][n].resize(this->sizes[l]);

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    f.read(reinterpret_cast<char*>(&this->weights[l][n][pn]), sizeof(float));
            }
        }

        f.close();
    }

    #ifdef _WIN32
    void loadFromBNFAsResource(const unsigned int resourceID)
    {
        // find file data resource
        HRSRC fdRes = FindResourceA(NULL, MAKEINTRESOURCE(resourceID), MAKEINTRESOURCE(256));
        // get resource pointer
        HGLOBAL fdRp = LoadResource(NULL, fdRes);

        size_t fileOffset = 0;

        cpyDataFromResource(&this->numLayers, sizeof(unsigned int), fdRp, fileOffset);
        this->sizes.resize(this->numLayers);
        cpyDataFromResource(this->sizes.data(), sizeof(unsigned int)*this->sizes.size(), fdRp, fileOffset);

        this->biases.resize(this->sizes.size()-1);
        this->weights.resize(this->sizes.size()-1);

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            this->biases[l].resize(this->sizes[l+1]);
            this->weights[l].resize(this->sizes[l+1]);

            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                cpyDataFromResource(&this->biases[l][n], sizeof(float), fdRp, fileOffset);

                this->weights[l][n].resize(this->sizes[l]);

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    cpyDataFromResource(&this->weights[l][n][pn], sizeof(float), fdRp, fileOffset);
            }
        }
    }
    #endif // _WIN32

    std::vector<float> feedForward(std::vector<float> activation) const
    {
        for(unsigned int i = 0; i < this->weights.size(); ++i) // iterate over layers
        {
            std::vector<float> z = matrixVectorMultiply(this->weights[i], activation);
            vectorAddToSelf(z, this->biases[i]);

            activation = activationFunction(z);
        }

        return activation;
    }

    std::vector<float> feedForward_softmax(std::vector<float> activation, const float c) const
    {
        std::vector<float> z;
        for(unsigned int i = 0; i < this->weights.size(); ++i) // iterate over layers
        {
            z = matrixVectorMultiply(this->weights[i], activation);
            vectorAddToSelf(z, this->biases[i]);

            activation = activationFunction(z);
        }

        float denom = 0.0f;
        for(unsigned int i = 0; i < z.size(); ++i)
            denom += std::exp(c*z[i]);

        for(unsigned int i = 0; i < z.size(); ++i)
            activation[i] = std::exp(c*z[i])/denom;

        return activation;
    }

    unsigned int evaluate(const std::vector<std::pair<std::vector<float>,std::vector<float>>>& test_data, int numTests=-1) const
    {
        if(numTests < 0)
            numTests = test_data.size();

        unsigned int numCorrect = 0;

        for(int i = 0; i < numTests; ++i)
        {
            std::vector<float> outputs = this->feedForward(test_data[i].first);

            unsigned int lIndex = 0;
            float lVal = outputs[0];
            for(unsigned int j = 0; j < outputs.size(); ++j)
            {
                if(outputs[j] > lVal)
                {
                    lIndex = j;
                    lVal = outputs[j];
                }
            }

            for(unsigned int j = 0; j < 10; ++j)
            {
                if(test_data[i].second[j] > 0.5f)
                {
                    if(lIndex == j)
                        numCorrect++;
                }
            }
        }

        return numCorrect;
    }

    std::vector<float> cost_derivative(const std::vector<float>& activation, const std::vector<float> y) const
    {
        std::vector<float> res;
        res.resize(activation.size());

        for(unsigned int i = 0; i < res.size(); ++i)
        {
            res[i] = activation[i]-y[i];
            //if((int)i == y)
            //    res[i] = activation[i]-1.0f;
            //else res[i] = activation[i]-0.0f;
        }

        return res;
    }

    void backprop(const std::pair<std::vector<float>, std::vector<float>>& training_instance, std::vector<std::vector<float>>& n_b, std::vector<std::vector<std::vector<float>>>& n_w) const
    {
        // create empty gradient delta arrays
        std::vector<std::vector<float>> nabla_b;
        std::vector<std::vector<std::vector<float>>> nabla_w;

        nabla_b.resize(this->sizes.size()-1);
        nabla_w.resize(this->sizes.size()-1);

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            nabla_w[l].resize(this->sizes[l+1]);

            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                nabla_w[l][n].resize(this->sizes[l]);

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    nabla_w[l][n][pn] = 0.0f;
            }
        }

        std::vector<float> activation = training_instance.first;
        std::vector<std::vector<float>> activations = {training_instance.first};
        std::vector<std::vector<float>> zs = {};

        for(unsigned int i = 0; i < this->weights.size(); ++i) // iterate over layers
        {
            std::vector<float> z = matrixVectorMultiply(this->weights[i], activation);
            vectorAddToSelf(z, this->biases[i]);
            zs.push_back(z);

            activation = activationFunction(z);
            activations.push_back(activation);
        }

        std::vector<float> delta = this->cost_derivative(activations[activations.size()-1], training_instance.second);
        //vectorElementMultiplyBySelf(delta, activationFunctionPrime(zs[zs.size()-1])); // comment out for cross-entropy cost

        nabla_b[nabla_b.size()-1] = delta;

        for(unsigned int k = 0; k < activations[activations.size()-2].size(); ++k)
        {
            for(unsigned int j = 0; j < delta.size(); ++j)
                nabla_w[nabla_w.size()-1][j][k] = activations[activations.size()-2][k] * delta[j];
        }

        for(unsigned int l = 2; l < this->numLayers; ++l)
        {
            std::vector<float> z = zs[zs.size()-l];
            std::vector<float> ap = activationFunctionPrime(z);

            delta = transposedMatrixVectorMultiply(this->weights[this->weights.size()-l+1], delta);
            vectorElementMultiplyBySelf(delta, ap);

            nabla_b[nabla_b.size()-l] = delta;

            for(unsigned int k = 0; k < activations[activations.size()-l-1].size(); ++k)
            {
                for(unsigned int j = 0; j < delta.size(); ++j)
                    nabla_w[nabla_w.size()-l][j][k] = activations[activations.size()-l-1][k] * delta[j];
            }
        }

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                n_b[l][n] += nabla_b[l][n];

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    n_w[l][n][pn] += nabla_w[l][n][pn];
            }
        }
    }

    void update_mini_batch(const std::vector<std::pair<std::vector<float>,std::vector<float>>>& mini_batch, const float eta, const float regularization)
    {
        // create empty gradient arrays
        std::vector<std::vector<float>> nabla_b;
        std::vector<std::vector<std::vector<float>>> nabla_w;

        nabla_b.resize(this->sizes.size()-1);
        nabla_w.resize(this->sizes.size()-1);

        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            nabla_b[l].resize(this->sizes[l+1]);
            nabla_w[l].resize(this->sizes[l+1]);

            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                nabla_b[l][n] = 0.0f;

                nabla_w[l][n].resize(this->sizes[l]);

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    nabla_w[l][n][pn] = 0.0f;
            }
        }

        // run back propagation to compute gradient
        #pragma omp parallel for num_threads(maxThreads)
        for(unsigned int i = 0; i < mini_batch.size(); ++i)
            this->backprop(mini_batch[i], nabla_b, nabla_w);

        // apply gradient
        for(unsigned int l = 0; l < this->sizes.size()-1; ++l) // iterate over layers
        {
            for(unsigned int n = 0; n < this->sizes[l+1]; ++n) // iterate over neurons in layer
            {
                this->biases[l][n] -= (eta/(float)mini_batch.size())*nabla_b[l][n];

                for(unsigned int pn = 0; pn < this->sizes[l]; ++pn) // iterate over neurons in previous layer
                    this->weights[l][n][pn] = regularization*this->weights[l][n][pn] - (eta/(float)mini_batch.size())*nabla_w[l][n][pn];
            }
        }
    }

    void SGD(const std::vector<std::pair<std::vector<float>,std::vector<float>>>& training_data, const unsigned int epochs, const unsigned int mini_batch_size, const float eta, const float lambda) // stochastic gradient decent
    {
        size_t n = training_data.size();

        for(unsigned int j = 0; j < epochs; ++j) // iterate over epochs
        {

            std::vector<unsigned int> rnd_indices;
            rnd_indices.resize(training_data.size());
            for(unsigned int i = 0; i < training_data.size(); ++i)
                rnd_indices[i] = i;
            std::random_shuffle(rnd_indices.begin(), rnd_indices.end());

            std::vector<std::vector<std::pair<std::vector<float>,std::vector<float>>>> mini_batches;

            for(unsigned int k = 0; k < n; k+=mini_batch_size)
            {
                mini_batches.push_back({});
                for(unsigned int i = k; i < k+mini_batch_size; ++i)
                    mini_batches.back().push_back(training_data[rnd_indices[i]]);
            }

            float regularization = 1.0f-eta*lambda/(float)n;

            for(unsigned int i = 0; i < mini_batches.size(); ++i)
            {
                if((i&0xFF) == 0) std::cout << "mini-batch " << i << " out of " << mini_batches.size() << " done\n";

                this->update_mini_batch(mini_batches[i], eta, regularization);
            }

            std::cout << "Epoch " << j << " complete\n";
        }
    }
};

#endif // NETWORK_H_INCLUDED
