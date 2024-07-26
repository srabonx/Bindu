//
// Created by letsd on 24-Jul-24.
//

#ifndef BINDU_COMMONUTILITY_H
#define BINDU_COMMONUTILITY_H

#include "../../ErrorHandler/EngineException.h"

// Defining a macro to throw a EngineException class
// Uses __FILE__ and __LINE__ constants
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw EngineException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif

#endif //BINDU_COMMONUTILITY_H
