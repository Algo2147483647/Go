#ifndef GO_AI_CNN_H
#define GO_AI_CNN_H

#include <vector>
#include "convolution.h"

using namespace std;

namespace GoAI {
/*
 *  Convolutional Neural Network
 */
class ConvolutionalNeuralNetworkWeights {
public:
	vector<vector<float>> 
		weights_conv,      biases_conv,
		weights_policy,    biases_policy,
		weights_valuation, biases_valuation,
		means_batchnorm,   stddevs_batchnorm;

};

template <size_t spatial_size>
void batchnorm(size_t channels,
               vector<float>& data,
               float* means,
               float* stddevs,
               float* eltwise = nullptr) {

    for (auto c = size_t{ 0 }; c < channels; ++c) {
        auto mean = means[c];
        auto scale_stddev = stddevs[c];
        auto arr = &data[c * spatial_size];

        if (eltwise == nullptr) {
            // Classical BN
            for (auto b = size_t{ 0 }; b < spatial_size; b++) 
                arr[b] = max(0.0f, scale_stddev * (arr[b] - mean));
        }
        else {
            // BN + residual add
            auto res = &eltwise[c * spatial_size];
            for (auto b = size_t{ 0 }; b < spatial_size; b++) 
                arr[b] = max(0.0f, (scale_stddev * (arr[b] - mean)) + res[b]);
        }
    }
}

void forward(vector<float>& in,
             vector<float>& out_policy,
             vector<float>& out_valuation, 
             ConvolutionalNeuralNetworkWeights& weights) {

    int in_channels;
    auto P = WINOGRAD_P;
    auto output_channels = in_channels;
    auto in_channels =
        max(static_cast<size_t>(output_channels),
            static_cast<size_t>(Network::INPUT_CHANNELS));
    auto conv_out = vector<float>(output_channels * BOARD_COLOR_NUM);
    auto V = vector<float>(WINOGRAD_TILE * in_channels * P);
    auto M = vector<float>(WINOGRAD_TILE * output_channels * P);

    convolve_3_winograd(
        output_channels, in, 
        weights.weights_conv[0], V, M, conv_out);

    batchnorm<BOARD_COLOR_NUM>(output_channels, conv_out,
        weights.means_batchnorm[0].data(),
        weights.stddevs_batchnorm[0].data());

    // Residual tower
    auto conv_in = vector<float>(output_channels * BOARD_COLOR_NUM);
    auto res = vector<float>(output_channels * BOARD_COLOR_NUM);

    for (auto i = size_t{ 1 }; i < weights.weights_conv.size(); i += 2) {
        auto output_channels = in_channels;
        swap(conv_out, conv_in);
        convolve_3_winograd(output_channels, conv_in, weights.weights_conv[i], V, M, conv_out);
        batchnorm<BOARD_COLOR_NUM>(output_channels, conv_out,
                            weights.means_batchnorm[i].data(),
                            weights.stddevs_batchnorm[i].data());

        swap(conv_in, res);
        swap(conv_out, conv_in);
        convolve_3_winograd(output_channels, conv_in,
                           weights.weights_conv[i + 1], V, M, conv_out);
        batchnorm<BOARD_COLOR_NUM>(output_channels, conv_out,
                            weights.means_batchnorm[i + 1].data(),
                            weights.stddevs_batchnorm[i + 1].data(), res.data());
    }
    convolve_img2col(1, Network::OUTPUTS_POLICY, conv_out, conv_pol_w, conv_pol_b, output_pol);
    convolve_img2col(1, Network::OUTPUTS_VALUE,  conv_out, conv_val_w, conv_val_b, output_val);
}

}
#endif
