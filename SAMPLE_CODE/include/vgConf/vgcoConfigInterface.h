


#ifndef __VGCOCONFIGINTERFACE_H__
#define __VGCOCONFIGINTERFACE_H__

#include <vgKernel/vgkForward.h>
#include <vgConf/XMLProfile.h>
#include <vgConf/vgcoConfigUtility.h>

	
namespace vgConf {

	/**
		@date 	2008/12/04  22:14	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT ConfigInterface
	{
	public:
		typedef std::map<String , String> StringMap;
		typedef std::pair<StringMap::iterator ,  bool> StringMapInsertRes;
		typedef std::map<String , StringMap > SectionKeyValueMap;
		typedef std::pair<SectionKeyValueMap::iterator, bool > SectionKeyValueMapInsertRes;

	public:

		ConfigInterface()
		{
			
		}
		virtual ~ConfigInterface()
		{
			
		}
	
	public:

		
		/**
			注册相应的项.
			当存在时,返回为false.
		*/
		bool registerKey( const String& section , const String& key ,
			const String& defaultValue );
		


		/**
			查找相应的项.
			return_default_value为true时,如果xml中没有相应的相,会返回默认的项.
			然后下一次会写入xml中.
			返回为空的String时,表示没有此项.
		*/
		String getConfig( const String& section , 
			const String& key , const bool& ret_default_val = true );

		bool setConfig( const String& section , 
			const String& key , const String& val );

		/**
			得到默认的存储默认值.
			返回为空的话,表示没有此项.
		*/
		String getDefaultValue( const String& section , const String& key  );

		/**
			 从注册过的DefaultMap中查找相应的项是否存在.
		*/
		bool checkExistFromDefaultMap( const String& section , const String& key );


		/**
			从日志中记录所有的默认项.
		*/
		void trace();


		virtual String getConfigName() = 0;

	protected:

		/**
			初始化profile,ExeConfig和ProjectConfig都继承此接口
		*/
		virtual bool initProfile(  const String& profile_abs_path );

		/**
			卸载的时候保存xml文件.
		*/
		virtual bool uninitProfile( const String& profile_abs_path );

		virtual String getValueFromProfile( const String& section , 
			const String& key );

		bool writeValueToProfile( const String& section , 
			const String& key , const String& val );


		void traceDefaultMap();

		void traceProfile();


	private:

		String		_xmlProfileName;
		CXmlProfile		_xmlProfile;

		SectionKeyValueMap _defaultMap;

	};
	
	
}// end of namespace vgConf
	


#endif // end of __VGCOCONFIGINTERFACE_H__