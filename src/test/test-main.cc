#include <QCoreApplication>
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv); //Needed to set up environment
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

