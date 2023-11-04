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

}

#endif
