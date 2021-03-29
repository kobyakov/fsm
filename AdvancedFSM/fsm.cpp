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
    State::State(std::string name, FSMBool isFinal, outputFn output) 
        : name(name), 
          isFinal(isFinal),
          table(),
          outputFunction(output)
    {}

    void State::insertNewEntry(std::string key, State* nextState)
    {
        debug_printf("state: %s, insert transition %s - %s\n", this->name.c_str(), key.c_str(), nextState->name.c_str());
        this->table.push_back({key, nextState});
    }

    FSM::FSM(std::string FSMName, State* initState)
    {
        this->name = FSMName;
        this->initialState = initState;
        this->currentState = initialState;
        this->result = FSMBool::FSM_FALSE;
    }

    State* FSM::findNextState(const std::string& inputString)
    {
        State* nextState = nullptr;
        for (const auto& row : this->currentState->table)
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
                nextState = row.nextState;
                debug_printf("Found next state: %s\n", nextState->name.c_str());
                break;
            }
        }
        return nextState;
    }

    void FSM::applyTransition(State* nextState)
    {
        debug_printf("Apply transition: current: %s, new: %s\n", 
            this->currentState->name.c_str(), 
            nextState->name.c_str());
        if (this->currentState->outputFunction)
            (this->currentState->outputFunction)(this->currentState, nextState, &this->output);
        this->currentState = nextState;

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
            State* nextState = this->findNextState(inputString);
            if (!nextState)
                return FSMError::FSM_NO_TRANSITION;
            this->applyTransition(nextState);
        }

        this->result = currentState->isFinal;

        return FSMError::FSM_NO_ERROR;

    }


};