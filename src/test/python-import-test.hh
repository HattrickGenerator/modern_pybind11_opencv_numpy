#include <QProcess>
#include <gtest/gtest.h>

///Class that already sets up client server system, that is ready to communicate!
class PythonTests : public testing::Test
{
public:
    static void SetUpTestSuite() { }

    static void TearDownTestSuite() { }

    // You can define per-test set-up logic as usual.
    virtual void SetUp()
    {
        m_worker = new QProcess();
        m_worker->setProgram("python3");
    }

    // You can define per-test tear-down logic as usual.
    virtual void TearDown()
    {
        m_worker->start();
        m_worker->waitForFinished();

        int status  = m_worker->exitStatus();
        QString err = m_worker->readAllStandardError();

        EXPECT_EQ(status, 0) << "Process not exited with 0";
        EXPECT_TRUE(err.isEmpty()) << err.toStdString();

        m_worker->kill();
        m_worker->waitForFinished();
        m_worker->deleteLater();
    }

    QProcess *m_worker;
};

