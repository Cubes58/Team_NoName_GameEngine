#pragma once
/**
@file FrameBufferType.h
@enum FrameBufferType
@brief The Different frame buffer types for Depth attachments.
*/
#pragma once

enum class FrameBufferType : unsigned int {
	NONE = 0,	/**< No Depth Attachment */
	DEPTH_TEXTURE,	/**< Is coded as unsigned int of value 1 */
	DEPTH_RENDER_BUFFER,	/**< Is coded as unsigned int of value 2 */
	DIRECTIONAL_SHADOW, /**< Is coded as unsigned int of value 3 */
	POINT_SHADOW,
};