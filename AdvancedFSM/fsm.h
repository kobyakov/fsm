#ifndef FSM_H
#define FSM_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace SML
{
    enum class FSMError
    {
        FSM_NO_TRANSITION,
        FSM_NO_ERROR
    };

    enum class FSMBool
    {
        FSM_FALSE,
        FSM_TRUE
    };

    class State;

    using outputBuffer = std::vector<std::string>;
    typedef void (*outputFn)(const State *currentState, const State* nextState, outputBuffer* output);
    typedef bool (*inputMatchFn)(const std::string_view& input, size_t& read);

    using TransitionTableKey = std::string;

    struct transitionTableEntry {
        std::vector<inputMatchFn> inputMatchFunctions;
        outputFn outputFunction;
        State* nextState;    
    };

    using transitionTable = std::vector<transitionTableEntry>;
    
    class State {
    public:
        std::string name;
        FSMBool isFinal;
        transitionTable table;
        State(std::string name, FSMBool isFinal = FSMBool::FSM_FALSE);       
        void insertNewEntry(
            State* nextState,
            std::vector<inputMatchFn> mFns,
            outputFn fn = nullptr
        );
    };

    class FSM
    {
    public:
        std::string name;
        size_t currentCursor;
        outputBuffer output;
        State* initialState;
        State* currentState;
        FSMBool result;
        FSM(std::string name, State* initState);

        const transitionTableEntry* findTransition(const std::string& inputString);
        void applyTransition(const transitionTableEntry* transition);
        FSMError execute(const std::string& buffer);
    };

};
#endif // FSM_H