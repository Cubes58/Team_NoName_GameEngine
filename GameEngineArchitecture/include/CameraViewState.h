/**
@file CameraViewState.h
@enum CameraViewState
@brief The different CameraViewStates available.
*/
#pragma once

enum class CameraViewState : unsigned int {
	FIRST_PERSON_CAMERA = 0,	/**< Is coded as unsigned int of value 0 */
	THIRD_PERSON_CAMERA,	/**< Is coded as unsigned int of value 1 */

	END	/**< Is coded as unsigned int of value 3 */
};