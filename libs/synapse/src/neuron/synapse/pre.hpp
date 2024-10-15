#ifdef WIN32
# ifdef SYNAPSE_SHARED
#  ifdef SYNAPSE_SHARED_EXPORTS
#   define SYNAPSE_API __declspec(dllexport)
#  else
#   define SYNAPSE_API __declspec(dllimport)
#  endif
# else
#  define SYNAPSE_API
# endif
#else
# define SYNAPSE_API
#endif


#ifdef UNICODE
#undef UNICODE
#endif