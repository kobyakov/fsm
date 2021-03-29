#include <fsm.h>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cassert>

#ifdef DEBUG
#define PRINT_DEBUG 1
#else
#define PRINT_DEBUG 0
#endif

#define debug_printf(fmt, ...)                  \
    do {                                        \
        if (PRINT_DEBUG)                        \
            fprintf(stderr, fmt, ##__VA_ARGS__);  \
    } while (0)


namespace SML
{
    State::State(std::string name, FSMBool isFinal) 
        : name(name), 
          isFinal(isFinal),
          table()
    {}

    void State::insertNewEntry(std::string key, State* nextState, outputFn outputFunction)
    {
        debug_printf("state: %s, insert transition %s - %s\n", this->name.c_str(), key.c_str(), nextState->name.c_str());
        this->table.push_back({key, nextState, outputFunction});
    }

    FSM::FSM(std::string FSMName, State* initState)
    {
        this->name = FSMName;
        this->initialState = initState;
        this->currentState = initialState;
        this->result = FSMBool::FSM_FALSE;
    }

    const transitionTableEntry* FSM::findTransition(const std::string& inputString)
    {
        for (const transitionTableEntry& row : this->currentState->table)
        {
            const std::string& key = row.key;
            size_t keySize = key.length();
            debug_printf("Key now: '%s', keySize: %zu.\n", key.c_str(), keySize);
            if (this->currentCursor + keySize > inputString.length())
            {
                debug_printf("This key too much, continue.\n");
                continue;
            }
            const std::string_view substring = inputString.substr(this->currentCursor, keySize);
            if (substring == key)
            {
                this->currentCursor += keySize;
                debug_printf("Found transition with next state: %s\n", row.nextState->name.c_str());
                return &row;
            }
        }
        return nullptr;
    }

    void FSM::applyTransition(const transitionTableEntry* transition)
    {
        if (transition->outputFunction)
            transition->outputFunction(this->currentState, transition->nextState, &this->output);

        debug_printf("Apply transition: current: %s, new: %s\n", 
            this->currentState->name.c_str(), 
            transition->nextState->name.c_str());
        this->currentState = transition->nextState;

    }

    FSMError FSM::execute(const std::string& inputString)
    {
        assert(this->initialState);
        this->output.clear();
        this->currentCursor = 0;
        
        debug_printf("Process string: %s\n", inputString.c_str());

        while(this->currentCursor < inputString.length())
        {
            debug_printf("Current state: %s Cursor: %zu\n", this->currentState->name.c_str(), this->currentCursor);
            const transitionTableEntry* tr = this->findTransition(inputString);
            if (!tr)
                return FSMError::FSM_NO_TRANSITION;
            this->applyTransition(tr);
        }

        this->result = currentState->isFinal;

        return FSMError::FSM_NO_ERROR;

    }


};