#include "command/CommandHandlerCollection.h"
#include "command/CommandHandlerInterface.h"

using UKControllerPlugin::Command::CommandHandlerCollection;

namespace UKControllerPluginTest {
    namespace Command {

        class TestCommandHandler : public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
            TestCommandHandler(int id, bool returnVal)
            {
                this->id = id;
                this->returnVal = returnVal;
            }

            bool ProcessCommand(std::string command)
            {
                this->id = -1;
                return this->returnVal;
            }

            int id;
            bool returnVal;
        };

        TEST(CommandHandlerCollection, ProcessCommandReturnsTrueIfCommandRecognised)
        {
            CommandHandlerCollection collection;
            std::shared_ptr<TestCommandHandler> handler = std::make_shared<TestCommandHandler>(1, true);

            collection.RegisterHandler(handler);
            EXPECT_TRUE(collection.ProcessCommand("test"));
        }

        TEST(CommandHandlerCollection, ProcessCommandReturnsFalseIfCommandNotRecognised)
        {
            CommandHandlerCollection collection;
            std::shared_ptr<TestCommandHandler> handler = std::make_shared<TestCommandHandler>(1, false);

            collection.RegisterHandler(handler);
            EXPECT_FALSE(collection.ProcessCommand("test"));
        }

        TEST(CommandHandlerCollection, ProcessCommandStopsAfterFirstHandlerReturnsTrue)
        {
            CommandHandlerCollection collection;
            std::shared_ptr<TestCommandHandler> handler1 = std::make_shared<TestCommandHandler>(1, true);
            std::shared_ptr<TestCommandHandler> handler2 = std::make_shared<TestCommandHandler>(2, true);

            collection.RegisterHandler(handler1);
            collection.RegisterHandler(handler2);
            EXPECT_TRUE(collection.ProcessCommand("test"));
            EXPECT_EQ(-1, handler1->id);
            EXPECT_EQ(2, handler2->id);
        }
    } // namespace Command
} // namespace UKControllerPluginTest
