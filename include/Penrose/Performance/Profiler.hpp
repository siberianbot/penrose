#ifndef PENROSE_PERFORMANCE_PROFILER_HPP
#define PENROSE_PERFORMANCE_PROFILER_HPP

#include <chrono>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <string_view>
#include <thread>

#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class Profiler : public Resource<Profiler, ResourceGroup::Performance> {
    public:
        class FunctionCall {
        public:
            FunctionCall(Profiler *profiler, std::string_view &&tag);
            ~FunctionCall();

            FunctionCall(const FunctionCall &) = delete;
            FunctionCall(FunctionCall &&) = delete;
            FunctionCall &operator=(const FunctionCall &) = delete;
            FunctionCall &operator=(FunctionCall &&) = delete;

        private:
            Profiler *_profiler;
            std::thread::id _threadId;
            std::string _tag;
        };

        ~Profiler() override = default;

        [[nodiscard]] FunctionCall begin(std::string_view &&tag);

    private:
        struct RecordedCall {
            std::string tag;
            std::chrono::high_resolution_clock::duration duration;
            std::list<RecordedCall> nestedCalls;
        };

        struct CurrentCall {
            std::string tag;
            std::chrono::high_resolution_clock::time_point begin;
            RecordedCall *recording;
        };

        std::map<std::thread::id, std::stack<CurrentCall>> _currentCallstacks;
        std::map<std::thread::id, RecordedCall> _recordedCallstacks;

        void pushFunctionCall(const std::thread::id &threadId, const std::string_view &tag);
        void popFunctionCall(const std::thread::id &threadId);
    };
}

#endif // PENROSE_PERFORMANCE_PROFILER_HPP
