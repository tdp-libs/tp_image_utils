#ifndef tp_image_utils_Globals_h
#define tp_image_utils_Globals_h

#include "tp_utils/Globals.h" // IWYU pragma: keep

#if defined(OS3_IO_LIBRARY)
#  define TP_IMAGE_UTILS_EXPORT TP_EXPORT
#else
#  define TP_IMAGE_UTILS_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! A module of image manipulation types.
namespace tp_image_utils
{

//##################################################################################################
//! This tries to guess the image type from the image data
/*!
This tries to guess the image type from the image data, if that fails it will try to use the name.

\param data - The image data.
\param name - The file name of the image where available.
\return The guessed type.
*/
std::string guessImageFormat(const std::string& data, const std::string& name);

}

#endif
