#ifndef ASYNC_H
#define ASYNC_H

#include <memory>
//#include <cstddef>

namespace async {

    class Handle {
    public:
        virtual ~Handle() = default;
        virtual void receive(const char* data, size_t size) = 0;
    };

    std::shared_ptr<Handle> connect(std::size_t bulk_size);
    void disconnect(std::shared_ptr<Handle> handle);

} // namespace async

#endif // ASYNC_H

