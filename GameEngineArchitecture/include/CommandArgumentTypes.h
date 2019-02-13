/**
@file CommandArgumentTypes.h
@enum CommandArgumentType
@brief The different CommandArgumentTypes available.
*/
#pragma once

enum class CommandArgumentType : unsigned int {
	GAME_OBJECTS = 0,	/**< Is coded as unsigned int of value 0 */
	PLAYER,	/**< Is coded as unsigned int of value 1 */
	INPUT_HANDLER,	/**< Is coded as unsigned int of value 2 */
	GAME,	/**< Is coded as unsigned int of value 3 */

	NONE	/**< Is coded as unsigned int of value 4 */
};