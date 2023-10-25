#include <Penrose/Performance/Profiler.hpp>

namespace Penrose {

    Profiler::FunctionCall::FunctionCall(Profiler *profiler, std::string_view &&tag)
            : _profiler(profiler),
              _tag(tag) {
        this->_profiler->pushFunctionCall(this->_threadId, this->_tag);
    }

    Profiler::FunctionCall::~FunctionCall() {
        this->_profiler->popFunctionCall(this->_threadId);
    }

    Profiler::FunctionCall Profiler::begin(std::string_view &&tag) {
        return {
                this,
                std::forward<decltype(tag)>(tag)
        };
    }

    void Profiler::pushFunctionCall(const std::thread::id &threadId, const std::string_view &tag) {
        auto now = std::chrono::high_resolution_clock::now();

        auto &callstack = this->_currentCallstacks[threadId];
        auto recording = callstack.empty()
                         ? &this->_recordedCallstacks[threadId]
                         : &callstack.top().recording->nestedCalls.emplace_back();

        recording->tag = tag;
        recording->nestedCalls.clear();

        callstack.push(CurrentCall{
                .tag = std::string(tag),
                .begin = now,
                .recording = recording
        });
    }

    void Profiler::popFunctionCall(const std::thread::id &threadId) {
        auto now = std::chrono::high_resolution_clock::now();

        auto &callstack = this->_currentCallstacks[threadId];

        auto call = callstack.top();
        callstack.pop();

        call.recording->duration = now - call.begin;
    }
}
