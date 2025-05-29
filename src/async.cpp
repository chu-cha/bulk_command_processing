#include "async.h"
#include "command_processor.h"
#include <memory>
#include <string>
#include <sstream>
#include <mutex>

namespace async {

    class HandleImpl : public Handle {
    public:
        HandleImpl(size_t bulk_size)
            : processor(std::make_unique<CommandProcessor>(bulk_size)) {
        }

        void receive(const char* data, size_t size) override {
            std::string buffer(data, size);
            std::istringstream stream(buffer);
            std::string command;

            while (std::getline(stream, command)) {
                if (!command.empty()) {
                    processor->process(command);
                }
            }
        }

        ~HandleImpl() override {
            processor->process("EOF");
        }

    private:
        std::unique_ptr<CommandProcessor> processor;
    };

    std::shared_ptr<Handle> connect(std::size_t bulk_size) {
        return std::make_shared<HandleImpl>(bulk_size);
    }

    void disconnect(std::shared_ptr<Handle> handle) {
        // Dtor of HandleImpl calls EOF automatically
    }

} // namespace async