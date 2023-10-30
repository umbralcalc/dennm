#include <torch/torch.h>
#include "SecondOrderProbsState.h"
#include "ThirdOrderProbsState.h"

int main() {
    StateConfig config;
    config.spaceStepsize = 1.0;
    config.timeWindowsize = 20;
    torch::Tensor initialProbs = torch::rand({5, 1});
    torch::Tensor initialConditionalProbs = torch::rand({5, 1, 5, config.timeWindowsize});
    SecondOrderProbsState state(initialProbs, config);
    for (int i = 0; i <= 40; i++) {
        state.update(initialConditionalProbs);
    }
    std::cout << state.getProbs() << std::endl;
    torch::Tensor initialTwiceConditionalProbs = torch::rand({5, 1, 5, config.timeWindowsize, 5, config.timeWindowsize-1});
    ThirdOrderProbsState higherState(initialProbs, initialConditionalProbs, config);
    for (int i = 0; i <= 40; i++) {
        higherState.update(initialTwiceConditionalProbs);
    }
    std::cout << higherState.getProbs() << std::endl;
    return 0;
}