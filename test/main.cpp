#include "main.h"
#include "uripp.h"
#include "futures.h"

TEST(DummyCpp, testcase) {
    ASSERT_FALSE(false);
}

TEST(ThreadPool, basic) {
    using target = uripp::runtime::scheduler::ThreadPool;
    target pool(3);
    auto result = pool.post([] { return 56*3; });
    auto result2 = pool.post([] () {});
    auto result3 = pool.post([] (auto&& input) { return input*56; }, 3);
    ASSERT_EQ(result.get(), result3.get());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
