// ThirdOrderProbsState.h
#pragma once
#include "SecondOrderProbsState.h"

class ThirdOrderProbsState : public SecondOrderProbsState {
public:
    ThirdOrderProbsState(
        torch::Tensor initialProbs,
        torch::Tensor initialConditionalProbs,
        StateConfig stateConfig
    );

    void update(torch::Tensor twiceConditionalProbs);

    torch::Tensor getConditionalProbs() const;

private:
    torch::Tensor conditionalProbs;
    StateConfig config;
};
