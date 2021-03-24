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
    using TransitionTableKey = std::string;
    using TransitionTable = std::unordered_map<TransitionTableKey, State*>;

    typedef (output_fn*)(State *currentState, State* nextState, )

    class State {
    public:
        std::string name;
        FSMBool isFinal;
        TransitionTable table;
        State(std::string name, FSMBool isFinal = FSMBool::FSM_FALSE);
        void insertNewEntry(std::string key, State* nextState);
    };

    class FSM
    {
    public:
        std::string name;
        State* initialState;
        State* currentState;
        FSMBool result;
        FSM(std::string name, State* initState);
        State* applyTransition(const char& key);
        FSMError execute(const std::string& buffer);
    };

};
#endif // FSM_H