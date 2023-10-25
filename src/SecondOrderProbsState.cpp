// SecondOrderProbsState.cpp
#include "SecondOrderProbsState.h"

class SecondOrderProbsState {
public:
    static const int SPACE_AXIS = 0;
    static const int TIME_AXIS = 1;

    SecondOrderProbsState(
        torch::Tensor initialProbs, 
        StateConfig stateConfig
    ) {
        probs = torch::ones({0, 1});
        probs = torch::cat({probs, initialProbs}, TIME_AXIS); 
        config = stateConfig;
        currentTimestep = 0;
    }

    void update(torch::Tensor conditionalProbs) {
        auto numberOfTimesteps = double(probs.size(TIME_AXIS));
        torch::Tensor nextProbs = (
            (config.spaceStepsize / numberOfTimesteps) * torch::tensordot(
                probs, 
                conditionalProbs, 
                {SPACE_AXIS, TIME_AXIS}, 
                {SPACE_AXIS + 2, TIME_AXIS + 2}
            ) - probs
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

    torch::Tensor getProbs() const {
        return probs;
    }

    int getCurrentTimestep() const {
        return currentTimestep;
    }

private:
    int currentTimestep;
    torch::Tensor probs;
    StateConfig config;
};