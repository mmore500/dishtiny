#pragma once
#ifndef UITSL_UTILITY_UITSL_IF_WEB_ELSE_HPP_INCLUDE
#define UITSL_UTILITY_UITSL_IF_WEB_ELSE_HPP_INCLUDE

#ifdef __EMSCRIPTEN__
#define UITSL_IF_WEB_ELSE( if_web, else ) (if_web)
#else
#define UITSL_IF_WEB_ELSE( if_web, else ) (else)
#endif // #ifdef __EMSCRIPTEN__

#endif // #ifndef UITSL_UTILITY_UITSL_IF_WEB_ELSE_HPP_INCLUDE
