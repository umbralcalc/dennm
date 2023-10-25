#include <torch/torch.h>
#include "SecondOrderProbsState.h"
#include "ThirdOrderProbsState.h"

int main() {
    torch::Tensor tensor = torch::rand({2, 2});
    StateConfig config;
    config.spaceStepsize = 1.0;
    config.timeWindowsize = 20;
    ThirdOrderProbsState state(tensor, tensor, config);
    for (int i = 0; i <= 10; i++) {
        state.update(tensor);
    }
    std::cout << state.getProbs() << std::endl;
    return 0;
}