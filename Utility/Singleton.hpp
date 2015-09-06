// Singleton.h: interface for the Singleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "Utility.h"

namespace A
{
	template <typename T>
	class Singleton
	{
	public:
		~Singleton()
		{
			
		}

		static T * Instance()
		{
			if (0 == ms_pInstance)
			{
				ms_pInstance = new T;
			}

			return ms_pInstance;
		}

		static void DestroyInstance()
		{
			if (ms_pInstance) {		
				delete ms_pInstance;
				ms_pInstance = NULL;
			}
		}

	private:

		static T *		ms_pInstance;
	};

	template< class T > T * Singleton<T>::ms_pInstance = 0;
}

#endif // __SINGLETON_H__
