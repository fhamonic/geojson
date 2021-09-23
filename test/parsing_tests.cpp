#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TEST(KnapstackBNB, OptTest) {
//     Knapstack::BranchAndBound<int, int> solver;
//     for(const auto & [instance, opt] : instances) {
//         Knapstack::Solution<int, int> solution = solver.solve(instance);
//         EXPECT_EQ(solution.getValue(), opt);
//     }
// }

