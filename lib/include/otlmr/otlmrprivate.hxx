
#ifndef OTLMR_PRIVATE_HXX
#define OTLMR_PRIVATE_HXX

/* From http://gcc.gnu.org/wiki/Visibility */
/* Generic helper definitions for shared library support */
#if defined _WIN32 || defined __CYGWIN__
#define OTLMR_HELPER_DLL_IMPORT __declspec(dllimport)
#define OTLMR_HELPER_DLL_EXPORT __declspec(dllexport)
#define OTLMR_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define OTLMR_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define OTLMR_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define OTLMR_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define OTLMR_HELPER_DLL_IMPORT
#define OTLMR_HELPER_DLL_EXPORT
#define OTLMR_HELPER_DLL_LOCAL
#endif
#endif

/* Now we use the generic helper definitions above to define OTLMR_API and OTLMR_LOCAL.
 * OTLMR_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
 * OTLMR_LOCAL is used for non-api symbols. */

#ifndef OTLMR_STATIC /* defined if OT is compiled as a DLL */
#ifdef OTLMR_DLL_EXPORTS /* defined if we are building the OT DLL (instead of using it) */
#define OTLMR_API OTLMR_HELPER_DLL_EXPORT
#else
#define OTLMR_API OTLMR_HELPER_DLL_IMPORT
#endif /* OTLMR_DLL_EXPORTS */
#define OTLMR_LOCAL OTLMR_HELPER_DLL_LOCAL
#else /* OTLMR_STATIC is defined: this means OT is a static lib. */
#define OTLMR_API
#define OTLMR_LOCAL
#endif /* !OTLMR_STATIC */


#endif // OTLMR_PRIVATE_HXX

