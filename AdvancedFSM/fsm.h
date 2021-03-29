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

    using TransitionTableKey = std::string;

    struct transitionTableEntry {
        std::string key;
        State* nextState;
    };

    using transitionTable = std::vector<transitionTableEntry>;
    
    class State {
    public:
        std::string name;
        FSMBool isFinal;
        transitionTable table;
        outputFn outputFunction;
        State(std::string name, FSMBool isFinal = FSMBool::FSM_FALSE, outputFn output=nullptr);       
        void insertNewEntry(std::string key, State* nextState);
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

        State* findNextState(const std::string& inputString);
        void applyTransition(State* nextState);
        FSMError execute(const std::string& buffer);
    };

};
#endif // FSM_H