#include <QProcess>
#include <gtest/gtest.h>

TEST(BasicAlgoTest, importModule)
{
    QProcess process;
    process.setProgram("python3");
    process.setArguments(QStringList() << "-c"
                                       << "import sys; sys.path.insert(0, './src/pybind/'); import cvmatbindpy");
    process.start();
    process.waitForFinished();

    int status  = process.exitStatus();
    QString err = process.readAllStandardError();

    EXPECT_EQ(status, 0) << "Process not exited with 0";
    EXPECT_TRUE(err.isEmpty()) << err.toStdString();
}

