//
// Created by Armando Herrera III on 10/24/18.
// Uses Momentum Stochastic Gradient Descent (MSGD) w/ back propagation to train a Neural Network.
//

#ifndef CPPNNET_MSGD_NEURAL_TRAINER_H
#define CPPNNET_MSGD_NEURAL_TRAINER_H

#include "Neural_Trainer.h"

namespace CppNNet {

  struct learning_momentum {
    float momentum = 0.8;
    float learning_rate = 0.01;
  };

  class MSGD_Neural_Trainer : public Neural_Trainer {
  protected:
    std::vector<Ematrix> _past_weights;
    std::vector<Evector> _past_biases;
    float _momentum_constant = 0.8;
    bool _isinit = false;

    void _init();

  public:
    explicit MSGD_Neural_Trainer(std::vector<std::shared_ptr<Neural_Layer>> neural_ptr);

    MSGD_Neural_Trainer(std::vector<std::shared_ptr<Neural_Layer>> neural_ptr, learning_momentum lrm);

    explicit MSGD_Neural_Trainer(std::shared_ptr<Neural_Layer> end_neural_ptr);

    MSGD_Neural_Trainer(std::shared_ptr<Neural_Layer> end_neural_ptr, learning_momentum lrm);

    ~MSGD_Neural_Trainer() = default;

    void train_sample(const Evector &s, const Evector &t) override;

    void train_batch(const std::vector<Evector> &s, const std::vector<Evector> &t, bool shuffle) override;

    using Neural_Trainer::train_batch;
  };

}

#endif //CPPNNET_MSGD_NEURAL_TRAINER_H
