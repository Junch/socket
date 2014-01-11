#include <gtest/gtest.h>
#include <vector>
using namespace std;

TEST(A, B) {
    vector<int> a{1,2,3,4,5};
    ASSERT_EQ(5, a.size());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
