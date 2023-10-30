// SecondOrderProbsState.cpp
#include "SecondOrderProbsState.h"

SecondOrderProbsState::SecondOrderProbsState(
    torch::Tensor initialProbs, 
    StateConfig stateConfig
) {
    probs = torch::ones({initialProbs.size(0), 0});
    probs = torch::cat({probs, initialProbs}, TIME_AXIS); 
    config = stateConfig;
    currentTimestep = 0;
}

void SecondOrderProbsState::update(torch::Tensor conditionalProbs) {
    auto numberOfTimesteps = probs.size(TIME_AXIS);
    torch::Tensor nextProbs = (
        (config.spaceStepsize / double(numberOfTimesteps)) * torch::tensordot(
            probs, 
            conditionalProbs.index(
                {
                    torch::indexing::Slice(),
                    torch::indexing::Slice(),
                    torch::indexing::Slice(), 
                    torch::indexing::Slice(config.timeWindowsize-numberOfTimesteps, config.timeWindowsize)
                }
            ), 
            {SPACE_AXIS, TIME_AXIS}, 
            {SPACE_AXIS + 2, TIME_AXIS + 2}
        )
    );
    if (numberOfTimesteps < config.timeWindowsize) {
        probs = torch::cat({probs, nextProbs}, TIME_AXIS);
    } else {
        probs = torch::cat(
            {
                probs.index({torch::indexing::Slice(), torch::indexing::Slice(1, config.timeWindowsize)}), 
                nextProbs
            }, 
            TIME_AXIS
        );
    }
    currentTimestep++;
}

torch::Tensor SecondOrderProbsState::getProbs() const {
    return probs;
}

int SecondOrderProbsState::getCurrentTimestep() const {
    return currentTimestep;
}