// SecondOrderProbsState.h
#pragma once
#include <torch/torch.h>

struct StateConfig {
    double spaceStepsize;
    int timeWindowsize;
};

class SecondOrderProbsState {
public:
    static const int SPACE_AXIS = 0;
    static const int TIME_AXIS = 1;

    SecondOrderProbsState(torch::Tensor initialProbs, StateConfig stateConfig);

    void update(torch::Tensor conditionalProbs);

    torch::Tensor getProbs() const;

    int getCurrentTimestep() const;

private:
    int currentTimestep;
    torch::Tensor probs;
    StateConfig config;
};
