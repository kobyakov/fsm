#include <fsm.h>
#include <iostream>
#include <string>
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
        this->table.insert({key, nextState});
    }

    FSM::FSM(std::string FSMName, State* initState)
    {
        this->name = FSMName;
        this->initialState = initState;
        this->currentState = initialState;
        this->result = FSMBool::FSM_FALSE;
    }

    State* FSM::applyTransition(const char& key)
    {
        debug_printf("Current state: %s, Key: %c, ", this->currentState->name.c_str(), key);       
        std::unordered_map<std::string, State*>::const_iterator it = this->currentState->table.find(std::string(1, key));
        if (it == this->currentState->table.end())
        {
            debug_printf("No transition.\n");
            return nullptr;
        }
        debug_printf("New state: %s.\n", it->second->name.c_str());
        if (this->currentState->outputFunction)
            (this->currentState->outputFunction)(this->currentState, it->second, &this->output);
        return it->second;

    }

    FSMError FSM::execute(const std::string& inputString)
    {
        assert(this->initialState);
        this->output.clear();

        debug_printf("Process string: %s\n", inputString.c_str());
        
        for (const char& c : inputString)
        {
            debug_printf("Process symbol: %c\n", c);
            State* nextState = this->applyTransition(c);
            if (!nextState)
                return FSMError::FSM_NO_TRANSITION;
            this->currentState = nextState;
        }

        this->result = currentState->isFinal;

        return FSMError::FSM_NO_ERROR;

    }


};