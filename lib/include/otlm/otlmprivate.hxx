
#ifndef OTLM_PRIVATE_HXX
#define OTLM_PRIVATE_HXX

/* From http://gcc.gnu.org/wiki/Visibility */
/* Generic helper definitions for shared library support */
#if defined _WIN32 || defined __CYGWIN__
#define OTLM_HELPER_DLL_IMPORT __declspec(dllimport)
#define OTLM_HELPER_DLL_EXPORT __declspec(dllexport)
#define OTLM_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define OTLM_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define OTLM_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define OTLM_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define OTLM_HELPER_DLL_IMPORT
#define OTLM_HELPER_DLL_EXPORT
#define OTLM_HELPER_DLL_LOCAL
#endif
#endif

/* Now we use the generic helper definitions above to define OTLM_API and OTLM_LOCAL.
 * OTLM_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
 * OTLM_LOCAL is used for non-api symbols. */

#ifndef OTLM_STATIC /* defined if OT is compiled as a DLL */
#ifdef OTLM_DLL_EXPORTS /* defined if we are building the OT DLL (instead of using it) */
#define OTLM_API OTLM_HELPER_DLL_EXPORT
#else
#define OTLM_API OTLM_HELPER_DLL_IMPORT
#endif /* OTLM_DLL_EXPORTS */
#define OTLM_LOCAL OTLM_HELPER_DLL_LOCAL
#else /* OTLM_STATIC is defined: this means OT is a static lib. */
#define OTLM_API
#define OTLM_LOCAL
#endif /* !OTLM_STATIC */


#endif // OTLM_PRIVATE_HXX

