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
    typedef void (*outputFn)(State *currentState, State* nextState, outputBuffer* output);

    using TransitionTableKey = std::string;
    using TransitionTable = std::unordered_map<TransitionTableKey, State*>;
    
    class State {
    public:
        std::string name;
        FSMBool isFinal;
        TransitionTable table;
        outputFn outputFunction;
        State(std::string name, FSMBool isFinal = FSMBool::FSM_FALSE, outputFn output=nullptr);       
        void insertNewEntry(std::string key, State* nextState);
    };

    class FSM
    {
    public:
        std::string name;
        outputBuffer output;
        State* initialState;
        State* currentState;
        FSMBool result;
        FSM(std::string name, State* initState);
        State* applyTransition(const char& key);
        FSMError execute(const std::string& buffer);
    };

};
#endif // FSM_H