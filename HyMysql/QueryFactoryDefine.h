#ifndef __QUERYFACTORYDEFINE_H_INCLUDED__
#define __QUERYFACTORYDEFINE_H_INCLUDED__




#define BEGIN_INITPOOL()							VOID	Init() \
													{
#define I_POOL_ENTRY( classType, bsize, esize )			m_##classType##Pool.Initialize( bsize, esize );
#define END_INITPOOL()								}

#define BEGIN_RELEAESPOOL()							VOID	Release() \
													{
#define R_POOL_ENTITY( classType )						m_##classType##Pool.Release();
#define END_RELEAESPOOL()							}

#define IMPLEMENT_POOL_ENTITY( classType )			public : classType * Alloc##classType() \
													{ \
													return m_##classType##Pool.Alloc(); \
													} \
													VOID Free##classType( classType * pObj ) \
													{ \
													m_##classType##Pool.Free( pObj ); \
													} \
													private:	\
													SafeMemoryPoolFactory<classType> m_##classType##Pool;


#define QUERYALLOC( classType )						QueryPoolFactory::Instance()->Alloc##classType##()
#define QUERYFREE( classType, classObj )			QueryPoolFactory::Instance()->Free##classType##( classObj )



template<class Type>
class QUERY_POOL
{
public:
	enum { eDEFUALT_POOL_SIZE = 100, };
	QUERY_POOL( int size = eDEFUALT_POOL_SIZE )
	{
		m_pool.Initialize( size );
	}
	~QUERY_POOL()
	{
		m_pool.Release();
	}
	Type * Alloc()
	{
		return m_pool.Alloc();
	}
	VOID Free( Type * pType )
	{
		m_pool.Free( pType );
	}
private:
	A::MemoryFactory<Type> m_pool;
};

#define _DECLARE_QUERY_POOL( CLASSNAME )			\
	private:										\
	static QUERY_POOL<CLASSNAME>		m_Pool;		\
	public:											\
	static CLASSNAME * ALLOC()						\
{												\
	return m_Pool.Alloc();						\
}												\
	static VOID FREE( CLASSNAME * pObject )			\
{												\
	m_Pool.Free(pObject);						\
}
#define _IMPL_QUERY_POOL( CLASSNAME )				\
	QUERY_POOL<CLASSNAME> CLASSNAME::m_Pool;







#endif // __QUERY_POOL_FACTORY_DEFINE_H__