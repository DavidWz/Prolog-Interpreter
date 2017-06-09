#pragma once

#include "../Common.h"
#include <set>
#include <sstream>

/**
 * Tracks all variable names.
 */
class VariablePool {
private:
    std::set<std::string> mNames;

    int mMaxVarIndex;
    std::string mFreshName;

    int mLockedMaxVarIndex;
    std::string mLockedFreshName;

    VariablePool() :
        mNames(),
        mMaxVarIndex(0),
        mFreshName("X0"),
        mLockedMaxVarIndex(0),
        mLockedFreshName("X0")
    {
    }

    static VariablePool& getInstance() {
        static VariablePool instance;
        return instance;
    }

public:
    VariablePool(VariablePool const&) = delete;
    void operator=(VariablePool const&) = delete;


    static bool contains(const std::string& name) {
        return (getInstance().mNames.count(name) > 0);
    }

    static void add(const std::string& name) {
        getInstance().mNames.insert(name);

        // update the fresh name
        if (name[0] == 'X') {
            try {
                int index = std::stoi(name.substr(1));
                if (index >= getInstance().mMaxVarIndex) {
                    // update required
                    getInstance().mMaxVarIndex = index + 1;
                    std::stringstream ss;
                    ss << "X" << getInstance().mMaxVarIndex;
                    getInstance().mFreshName = ss.str();
                }
            }
            catch (std::exception const & e) {
                // not a variable of the form X[number], so nothing needs to be updated
            }
        }
    }

    static std::string getFreshName() {
        return getInstance().mFreshName;
    }

    static void rememberState() {
        getInstance().mLockedMaxVarIndex = getInstance().mMaxVarIndex;
        getInstance().mLockedFreshName = getInstance().mFreshName;
    }

    static void resetToRememberedState() {
        getInstance().mMaxVarIndex = getInstance().mLockedMaxVarIndex;
        getInstance().mFreshName = getInstance().mLockedFreshName;
    }

    static void reset() {
        getInstance().mMaxVarIndex = 0;
        getInstance().mLockedMaxVarIndex = 0;
        getInstance().mFreshName = "X0";
        getInstance().mLockedFreshName = "X0";
    }
};