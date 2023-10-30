// ThirdOrderProbsState.cpp
#include <torch/torch.h>
#include "ThirdOrderProbsState.h"

ThirdOrderProbsState::ThirdOrderProbsState(
    torch::Tensor initialProbs, 
    torch::Tensor initialConditionalProbs,
    StateConfig stateConfig
) : SecondOrderProbsState(initialProbs, stateConfig), 
    conditionalProbs(initialConditionalProbs) {
    config = stateConfig;
}

void ThirdOrderProbsState::update(torch::Tensor twiceConditionalProbs) {
    SecondOrderProbsState::update(
        conditionalProbs.index(
            {
                torch::indexing::Slice(), 
                torch::indexing::Slice(config.timeWindowsize-1, config.timeWindowsize)
            }
        )
    );
    auto numberOfTimesteps = conditionalProbs.size(SecondOrderProbsState::TIME_AXIS);
    torch::Tensor nextConditionalProbs;
    if (numberOfTimesteps == 1) {
        nextConditionalProbs = conditionalProbs;
    } else {
        nextConditionalProbs = (
            (config.spaceStepsize / double(numberOfTimesteps)) * torch::tensordot( 
                twiceConditionalProbs.index(
                    {
                        torch::indexing::Slice(),
                        torch::indexing::Slice(),
                        torch::indexing::Slice(), 
                        torch::indexing::Slice(config.timeWindowsize-numberOfTimesteps, config.timeWindowsize),
                        torch::indexing::Slice(),
                        torch::indexing::Slice(config.timeWindowsize-numberOfTimesteps, config.timeWindowsize-1)
                    }
                ),
                conditionalProbs, 
                {SecondOrderProbsState::SPACE_AXIS + 2, SecondOrderProbsState::TIME_AXIS + 2}, 
                {SecondOrderProbsState::SPACE_AXIS, SecondOrderProbsState::TIME_AXIS}
            )
        ).diagonal(0, 0, 2).diagonal(0, 0, 1).transpose(0, 2).transpose(1, 3);
    }
    std::cout << nextConditionalProbs.sizes() << std::endl;
    if (numberOfTimesteps < config.timeWindowsize) {
        conditionalProbs = torch::cat(
            {conditionalProbs, nextConditionalProbs}, 
            SecondOrderProbsState::TIME_AXIS
        );
        std::cout << conditionalProbs.sizes() << std::endl;
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

torch::Tensor ThirdOrderProbsState::getConditionalProbs() const {
    return conditionalProbs;
}