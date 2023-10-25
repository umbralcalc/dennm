// ThirdOrderProbsState.cpp
#include <torch/torch.h>
#include "ThirdOrderProbsState.h"

class ThirdOrderProbsState : public SecondOrderProbsState {
public:
    ThirdOrderProbsState(
        torch::Tensor initialProbs, 
        torch::Tensor initialConditionalProbs,
        StateConfig stateConfig
    ) : SecondOrderProbsState(initialProbs, stateConfig), 
        conditionalProbs(initialConditionalProbs) {
        config = stateConfig;
    }

    void update(torch::Tensor twiceConditionalProbs) {
        SecondOrderProbsState::update(conditionalProbs);
        auto numberOfTimesteps = double(conditionalProbs.size(SecondOrderProbsState::TIME_AXIS));
        torch::Tensor nextConditionalProbs;
        if (numberOfTimesteps == 1) {
            nextConditionalProbs = conditionalProbs;
        } else {
            nextConditionalProbs = (
                (config.spaceStepsize / numberOfTimesteps) * torch::tensordot(
                    conditionalProbs, 
                    twiceConditionalProbs.index(
                        {
                            torch::indexing::Slice(), 
                            torch::indexing::Slice(config.timeWindowsize-numberOfTimesteps, config.timeWindowsize),
                            torch::indexing::Slice(),
                            torch::indexing::Slice(config.timeWindowsize-numberOfTimesteps, config.timeWindowsize-1)
                        }
                    ), 
                    {SecondOrderProbsState::SPACE_AXIS, SecondOrderProbsState::TIME_AXIS}, 
                    {SecondOrderProbsState::SPACE_AXIS + 2, SecondOrderProbsState::TIME_AXIS + 2}
                ) - conditionalProbs
            );
        }
        if (numberOfTimesteps < config.timeWindowsize) {
            conditionalProbs = torch::cat(
                {conditionalProbs, nextConditionalProbs}, 
                SecondOrderProbsState::TIME_AXIS
            );
        } else {
            conditionalProbs = torch::cat(
                {
                    conditionalProbs.index(
                        {
                            torch::indexing::Slice(), 
                            torch::indexing::Slice(1, config.timeWindowsize)
                        }
                    ), 
                    nextConditionalProbs
                }, 
                SecondOrderProbsState::TIME_AXIS
            );
        }
    }

    torch::Tensor getConditionalProbs() const {
        return conditionalProbs;
    }

private:
    torch::Tensor conditionalProbs;
    StateConfig config;
};